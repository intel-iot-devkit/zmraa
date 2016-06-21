///* sensor_bmc150_magn.c - Driver for Bosch BMC150 magnetometer sensor */

/*
 * Copyright (c) 2016 Intel Corporation
 *
 * This code is based on bmm050.c from:
 * https://github.com/BoschSensortec/BMM050_driver
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define SYS_LOG_DOMAIN "BMC150_MAGN"
#define SYS_LOG_LEVEL CONFIG_BMC150_MAGN_SYS_LOG_LEVEL

#include <nanokernel.h>
#include <errno.h>
#include <misc/byteorder.h>
#include <misc/util.h>
#include <misc/sys_log.h>
#include "bmc150.h"

struct bmc150_magn_trim_regs {
	int8_t x1;
	int8_t y1;
	uint16_t reserved1;
	uint8_t reserved2;
	int16_t z4;
	int8_t x2;
	int8_t y2;
	uint16_t reserved3;
	int16_t z2;
	uint16_t z1;
	uint16_t xyz1;
	int16_t z3;
	int8_t xy2;
	uint8_t xy1;
} __packed;


struct _upm_bmc150_magn {
	mraa_i2c_context i2c;
	struct nano_sem sem;
#if defined(CONFIG_BMC150_MAGN_TRIGGER)
	char __stack fiber_stack[CONFIG_BMC150_MAGN_TRIGGER_FIBER_STACK];
#endif
#if defined(CONFIG_BMC150_MAGN_TRIGGER_DRDY)
	struct device *gpio_drdy;
	struct device *dev;
	struct gpio_callback gpio_cb;
	struct sensor_trigger trigger_drdy;
	sensor_trigger_handler_t handler_drdy;
#endif
	struct bmc150_magn_trim_regs tregs;
	int rep_xy, rep_z, odr, max_odr;
	int sample_x, sample_y, sample_z;
};

static const struct {
	int freq;
	uint8_t reg_val;
} bmc150_magn_samp_freq_table[] = { {2, 0x01},
				    {6, 0x02},
				    {8, 0x03},
				    {10, 0x00},
				    {15, 0x04},
				    {20, 0x05},
				    {25, 0x06},
				    {30, 0x07} };

static const struct bmc150_magn_preset {
	uint8_t rep_xy;
	uint8_t rep_z;
	uint8_t odr;
} bmc150_magn_presets_table[] = {
	[LOW_POWER_PRESET] = {3, 3, 10},
	[REGULAR_PRESET] = {9, 15, 10},
	[ENHANCED_REGULAR_PRESET] = {15, 27, 10},
	[HIGH_ACCURACY_PRESET] = {47, 83, 20}
};

static struct _upm_bmc150_magn upm_bmc150_magn_dev;

static mraa_result_t bmc150_magn_set_power_mode(upm_bmc150_magn dev,
				      enum bmc150_magn_power_modes mode,
				      int state)
{
	mraa_result_t res = MRAA_SUCCESS;

	switch (mode) {
	case BMC150_MAGN_POWER_MODE_SUSPEND:
		res = mraa_i2c_update_byte_data(dev->i2c,
					BMC150_MAGN_MASK_POWER_CTL,
					!state,
					BMC150_MAGN_REG_POWER);
		if (res == MRAA_SUCCESS)
			sys_thread_busy_wait(5 * USEC_PER_MSEC);
		return res;
	case BMC150_MAGN_POWER_MODE_SLEEP:
		return mraa_i2c_update_byte_data(dev->i2c,
					   BMC150_MAGN_MASK_OPMODE,
					   BMC150_MAGN_MODE_SLEEP <<
					   BMC150_MAGN_SHIFT_OPMODE,
					   BMC150_MAGN_REG_OPMODE_ODR);
	case BMC150_MAGN_POWER_MODE_NORMAL:
		return mraa_i2c_update_byte_data(dev->i2c,
					   BMC150_MAGN_MASK_OPMODE,
					   BMC150_MAGN_MODE_NORMAL <<
					   BMC150_MAGN_SHIFT_OPMODE,
					   BMC150_MAGN_REG_OPMODE_ODR);
		break;
	}

	return MRAA_ERROR_FEATURE_NOT_SUPPORTED;
}

static mraa_result_t bmc150_magn_set_odr(upm_bmc150_magn dev, uint8_t val)
{
	uint8_t i;

	for (i = 0; i < ARRAY_SIZE(bmc150_magn_samp_freq_table); ++i) {
		if (val <= bmc150_magn_samp_freq_table[i].freq) {
			return mraa_i2c_update_byte_data(dev->i2c,
						BMC150_MAGN_MASK_ODR,
						bmc150_magn_samp_freq_table[i].
						reg_val << BMC150_MAGN_SHIFT_ODR,
						BMC150_MAGN_REG_OPMODE_ODR);
		}
	}

	return MRAA_ERROR_FEATURE_NOT_SUPPORTED;
}


#if defined(BMC150_MAGN_SET_ATTR)
static int bmc150_magn_read_rep_xy(upm_bmc150_magn dev)
{
	int reg_val = mraa_i2c_read_byte_data(dev->i2c, BMC150_MAGN_REG_REP_XY);
	if (reg_val < 0)
		return -EIO;
	dev->rep_xy = BMC150_MAGN_REGVAL_TO_REPXY(reg_val);
	return 0;
}

static int bmc150_magn_read_rep_z(upm_bmc150_magn dev)
{
	int reg_val = mraa_i2c_read_byte_data(dev->i2c, BMC150_MAGN_REG_REP_Z);
	if (reg_val < 0)
		return -EIO;
	dev->rep_xy = BMC150_MAGN_REGVAL_TO_REPZ(reg_val);
	return 0;
}

static int bmc150_magn_compute_max_odr(upm_bmc150_magn dev, int rep_xy,
				       int rep_z, int *max_odr)
{
	if (rep_xy == 0) {
		if (data->rep_xy <= 0) {
			if (bmc150_magn_read_rep_xy(dev) < 0) {
				return -EIO;
			}
		}
		rep_xy = data->rep_xy;
	}

	if (rep_z == 0) {
		if (data->rep_z <= 0) {
			if (bmc150_magn_read_rep_z(dev) < 0) {
				return -EIO;
			}
		}
		rep_z = data->rep_z;
	}

	*max_odr = 1000000 / (145 * rep_xy + 500 * rep_z + 980);

	return 0;
}
#endif


#if defined(BMC150_MAGN_SET_ATTR_REP)
static int bmc150_magn_read_odr(upm_bmc150_magn dev)
{
	uint8_t i, odr_val, reg_val;

	if (mraa_i2c_read_bytes_data(dev->i2c, BMC150_MAGN_REG_OPMODE_ODR,
		                         reg_val, 1) != 1) {
		return -EIO;
	}

	odr_val = (reg_val & BMC150_MAGN_MASK_ODR) >> BMC150_MAGN_SHIFT_ODR;

	for (i = 0; i < ARRAY_SIZE(bmc150_magn_samp_freq_table); ++i) {
		if (bmc150_magn_samp_freq_table[i].reg_val == odr_val) {
			dev->odr = bmc150_magn_samp_freq_table[i].freq;
			return 0;
		}
	}

	return -ENOTSUP;
}
#endif

#if defined(CONFIG_BMC150_MAGN_SAMPLING_REP_XY)
static int bmc150_magn_write_rep_xy(upm_bmc150_magn dev, int val)
{
	if (mraa_i2c_update_byte_data(dev->i2c,
				BMC150_MAGN_REG_REP_DATAMASK,
				BMC150_MAGN_REPXY_TO_REGVAL(val),
				BMC150_MAGN_REG_REP_XY) != MRAA_SUCCESS) {
		return -EIO;
	}
	dev->rep_xy = val;
	return 0;
}
#endif

#if defined(CONFIG_BMC150_MAGN_SAMPLING_REP_Z)
static int bmc150_magn_write_rep_z(upm_bmc150_magn dev, int val)
{
	if (mraa_i2c_update_byte_data(dev->i2c,
				BMC150_MAGN_REG_REP_DATAMASK,
				BMC150_MAGN_REPZ_TO_REGVAL(val),
				BMC150_MAGN_REG_REP_Z) != MRAA_SUCCESS) {
		return -EIO;
	}
	dev->rep_z = val;
	return 0;
}
#endif

/*
 * Datasheet part 4.3.4, provided by Bosch here:
 * https://github.com/BoschSensortec/BMM050_driver
 */
static int32_t bmc150_magn_compensate_xy(struct bmc150_magn_trim_regs *tregs,
					int16_t xy, uint16_t rhall, bool is_x)
{
	int8_t txy1, txy2;
	int16_t val;

	if (xy == BMC150_MAGN_XY_OVERFLOW_VAL) {
		return INT32_MIN;
	}

	if (!rhall) {
		rhall = tregs->xyz1;
	}

	if (is_x) {
		txy1 = tregs->x1;
		txy2 = tregs->x2;
	} else {
		txy1 = tregs->y1;
		txy2 = tregs->y2;
	}

	val = ((int16_t)(((uint16_t)((((int32_t)tregs->xyz1) << 14) / rhall)) -
	      ((uint16_t)0x4000)));
	val = ((int16_t)((((int32_t)xy) * ((((((((int32_t)tregs->xy2) *
	      ((((int32_t)val) * ((int32_t)val)) >> 7)) + (((int32_t)val) *
	      ((int32_t)(((int16_t)tregs->xy1) << 7)))) >> 9) +
	      ((int32_t)0x100000)) * ((int32_t)(((int16_t)txy2) +
	      ((int16_t)0xA0)))) >> 12)) >> 13)) + (((int16_t)txy1) << 3);

	return (int32_t)val;
}

static int32_t bmc150_magn_compensate_z(struct bmc150_magn_trim_regs *tregs,
					int16_t z, uint16_t rhall)
{
	int32_t val;

	if (z == BMC150_MAGN_Z_OVERFLOW_VAL) {
		return INT32_MIN;
	}

	val = (((((int32_t)(z - tregs->z4)) << 15) - ((((int32_t)tregs->z3) *
	      ((int32_t)(((int16_t)rhall) - ((int16_t)tregs->xyz1)))) >> 2)) /
	      (tregs->z2 + ((int16_t)(((((int32_t)tregs->z1) *
	      ((((int16_t)rhall) << 1))) + (1 << 15)) >> 16))));

	return val;
}


static void bmc150_magn_convert(double *val, int raw_val)
{
	// val->type = SENSOR_VALUE_TYPE_DOUBLE;
	*val = (double)(raw_val) * (1.0/1600.0);
}

mraa_result_t upm_bmc150_magn_get_value(upm_bmc150_magn dev, double *x, double *y, double *z)
{
	uint16_t values[BMC150_MAGN_AXIS_XYZR_MAX];
	int16_t raw_x, raw_y, raw_z;
	uint16_t rhall;

	// __ASSERT(chan == SENSOR_CHAN_ALL || chan == SENSOR_CHAN_MAGN_ANY);

    mraa_i2c_address(dev->i2c, BMC150_MAGN_I2C_ADDR);
	if (mraa_i2c_read_bytes_data(dev->i2c,
			   BMC150_MAGN_REG_X_L, (uint8_t *)values,
			   sizeof(values)) != sizeof(values)) {
		SYS_LOG_DBG("failed to read sample");
		return -EIO;
	}

	raw_x = (int16_t)sys_le16_to_cpu(values[BMC150_MAGN_AXIS_X]) >>
		BMC150_MAGN_SHIFT_XY_L;
	raw_y = (int16_t)sys_le16_to_cpu(values[BMC150_MAGN_AXIS_Y]) >>
		BMC150_MAGN_SHIFT_XY_L;
	raw_z = (int16_t)sys_le16_to_cpu(values[BMC150_MAGN_AXIS_Z]) >>
		BMC150_MAGN_SHIFT_Z_L;
	rhall = sys_le16_to_cpu(values[BMC150_MAGN_RHALL]) >>
		BMC150_MAGN_SHIFT_RHALL_L;

	dev->sample_x = bmc150_magn_compensate_xy(&dev->tregs, raw_x, rhall,
						   true);
	dev->sample_y = bmc150_magn_compensate_xy(&dev->tregs, raw_y, rhall,
						   false);
	dev->sample_z = bmc150_magn_compensate_z(&dev->tregs, raw_z, rhall);

	bmc150_magn_convert(x, dev->sample_x);
	bmc150_magn_convert(y, dev->sample_y);
	bmc150_magn_convert(z, dev->sample_z);


	return 0;
}


/*
static int bmc150_magn_channel_get(upm_bmc150_magn dev,

				   struct sensor_value *val)
{
	switch (chan) {
	case SENSOR_CHAN_MAGN_X:
		bmc150_magn_convert(val, dev->sample_x);
		break;
	case SENSOR_CHAN_MAGN_Y:
		bmc150_magn_convert(val, dev->sample_y);
		break;
	case SENSOR_CHAN_MAGN_Z:
		bmc150_magn_convert(val, dev->sample_z);
		break;
	case SENSOR_CHAN_MAGN_ANY:
		break;
	default:
		return -EINVAL;
	}

	return 0;
}
*/

#if defined(BMC150_MAGN_SET_ATTR_REP)
static inline int bmc150_magn_attr_set_rep(upm_bmc150_magn dev,
					   enum sensor_channel chan,
					   const struct sensor_value *val)
{
	int max_odr;

	switch (chan) {
#if defined(CONFIG_BMC150_MAGN_SAMPLING_REP_XY)
	case SENSOR_CHAN_MAGN_X:
	case SENSOR_CHAN_MAGN_Y:
		if (val->val1 < 1 || val->val1 > 511) {
			return -EINVAL;
		}

		if (bmc150_magn_compute_max_odr(dev, val->val1, 0,
						&max_odr) < 0) {
			return -EIO;
		}

		if (data->odr <= 0b) {
			if (bmc150_magn_read_odr(dev) < 0) {
				return -EIO;
			}
		}

		if (data->odr > max_odr) {
			return -EINVAL;
		}

		if (bmc150_magn_write_rep_xy(dev, val->val1) < 0) {
			return -EIO;
		}
		break;
#endif
#if defined(CONFIG_BMC150_MAGN_SAMPLING_REP_Z)
	case SENSOR_CHAN_MAGN_Z:
		if (val->val1 < 1 || val->val1 > 256) {
			return -EINVAL;
		}

		if (bmc150_magn_compute_max_odr(dev, 0, val->val1,
						&max_odr) < 0) {
			return -EIO;
		}

		if (data->odr <= 0) {
			if (bmc150_magn_read_odr(dev) < 0) {
				return -EIO;
			}
		}

		if (data->odr > max_odr) {
			return -EINVAL;
		}

		if (bmc150_magn_write_rep_z(dev, val->val1) < 0) {
			return -EIO;
		}
		break;
#endif
	default:
		return -EINVAL;
	}

	return 0;
}
#endif

#if defined(BMC150_MAGN_SET_ATTR)
static int bmc150_magn_attr_set(upm_bmc150_magn dev,
				enum sensor_channel chan,
				enum sensor_attribute attr,
				const struct sensor_value *val)
{
	switch (attr) {
#if defined(CONFIG_BMC150_MAGN_SAMPLING_RATE_RUNTIME)
	case SENSOR_ATTR_SAMPLING_FREQUENCY:
		if (val->type != SENSOR_VALUE_TYPE_INT) {
			SYS_LOG_DBG("invalid parameter type");
			return -ENOTSUP;
		}

		if (data->max_odr <= 0) {
			if (bmc150_magn_compute_max_odr(dev, 0, 0,
							&dev->max_odr) < 0) {
				return -EIO;
			}
		}

		if (dev->max_odr < val->val1) {
			SYS_LOG_DBG("not supported with current oversampling");
			return -ENOTSUP;
		}

		if (bmc150_magn_set_odr(dev, (uint8_t)(val->val1)) < 0) {
			return -EIO;
		}
		break;
#endif
#if defined(BMC150_MAGN_SET_ATTR_REP)
	case SENSOR_ATTR_OVERSAMPLING:
		if (val->type != SENSOR_VALUE_TYPE_INT) {
			SYS_LOG_DBG("invalid parameter type");
			return -ENOTSUP;
		}

		bmc150_magn_attr_set_rep(dev, chan, val);

		break;
#endif
	default:
		return -EINVAL;
	}

	return 0;
}b
#endif


static int bmc150_magn_init_chip(upm_bmc150_magn dev)
{
	uint8_t chip_id;
	struct bmc150_magn_preset preset;

    mraa_i2c_address(dev->i2c, BMC150_MAGN_I2C_ADDR);
	bmc150_magn_set_power_mode(dev, BMC150_MAGN_POWER_MODE_NORMAL, 0);
	bmc150_magn_set_power_mode(dev, BMC150_MAGN_POWER_MODE_SUSPEND, 1);

	if (bmc150_magn_set_power_mode(dev, BMC150_MAGN_POWER_MODE_SUSPEND, 0)
				       != MRAA_SUCCESS) {
		SYS_LOG_DBG("failed to bring up device from suspend mode");
		return -EIO;
	}

	if (mraa_i2c_read_bytes_data(dev->i2c, BMC150_MAGN_REG_CHIP_ID, &chip_id, 1) != 1) {
		SYS_LOG_DBG("failed reading chip id");
		goto err_poweroff;
	}
	if (chip_id != BMC150_MAGN_CHIP_ID_VAL) {
		SYS_LOG_DBG("invalid voidchip id 0x%x", chip_id);
		goto err_poweroff;
	}
	SYS_LOG_DBG("chip id 0x%x", chip_id);

	preset = bmc150_magn_presets_table[BMC150_MAGN_DEFAULT_PRESET];
	if (bmc150_magn_set_odr(dev, preset.odr) < 0) {
		SYS_LOG_DBG("failed to set ODR to %d",
			    preset.odr);
		goto err_poweroff;
	}

	if (mraa_i2c_write_byte_data(dev->i2c,
			       BMC150_MAGN_REPXY_TO_REGVAL(preset.rep_xy),
			       BMC150_MAGN_REG_REP_XY)
			       != MRAA_SUCCESS) {
		SYS_LOG_DBG("failed to set REP XY to %d", preset.rep_xy);
		goto err_poweroff;
	}

	if (mraa_i2c_write_byte_data(dev->i2c,
			       BMC150_MAGN_REPZ_TO_REGVAL(preset.rep_z),
			       BMC150_MAGN_REG_REP_Z) != MRAA_SUCCESS) {
		SYS_LOG_DBG("failed to set REP Z to %d", preset.rep_z);
		goto err_poweroff;
	}

	if (bmc150_magn_set_power_mode(dev, BMC150_MAGN_POWER_MODE_NORMAL, 1)
				       < 0) {
		SYS_LOG_DBG("failed to power on device");
		goto err_poweroff;
	}

	if (mraa_i2c_read_bytes_data(dev->i2c,
			   BMC150_MAGN_REG_TRIM_START, (uint8_t *)&dev->tregs,
			   sizeof(dev->tregs)) != sizeof(dev->tregs)) {
		SYS_LOG_DBG("failed to read trim regs");
		goto err_poweroff;
	}

	dev->rep_xy = 0;
	dev->rep_z = 0;
	dev->odr = 0;
	dev->max_odr = 0;
	dev->sample_x = 0;
	dev->sample_y = 0;
	dev->sample_z = 0;

	dev->tregs.xyz1 = sys_le16_to_cpu(dev->tregs.xyz1);
	dev->tregs.z1 = sys_le16_to_cpu(dev->tregs.z1);
	dev->tregs.z2 = sys_le16_to_cpu(dev->tregs.z2);
	dev->tregs.z3 = sys_le16_to_cpu(dev->tregs.z3);
	dev->tregs.z4 = sys_le16_to_cpu(dev->tregs.z4);

	return 0;

err_poweroff:
	bmc150_magn_set_power_mode(dev, BMC150_MAGN_POWER_MODE_NORMAL, 0);
	bmc150_magn_set_power_mode(dev, BMC150_MAGN_POWER_MODE_SUSPEND, 1);
	return -EIO;
}

upm_bmc150_magn upm_bmc150_magn_init(int bus)
{
	upm_bmc150_magn dev = &upm_bmc150_magn_dev;

    dev->i2c = mraa_i2c_init(bus);
    if (dev->i2c == NULL) {
    	SYS_LOG_DBG("Failed to open i2c bus %d\n", bus);
    	return NULL;
    }
	if (bmc150_magn_init_chip(dev) < 0) {
		SYS_LOG_DBG("failed to initialize chip");
		return NULL;
	}

#if defined(CONFIG_BMC150_MAGN_TRIGGER_DRDY)
	if (bmc150_magn_init_interrupt(dev) < 0) {
		SYS_LOG_DBG("failed to initialize interrupts");
		return NULL;
	}
#endif

	return dev;
}
