#include "bme280.h"

#include <util/delay.h>
#include <math.h> 
#include "../i2cmaster/i2cmaster.h"


#ifdef DEBUG
#include <stdio.h>
#endif

#define BME280_CONCAT_BYTES(msb, lsb)	 (((uint16_t)msb << 8) | (uint16_t)lsb)

void delay_ms(uint32_t ms)
{
	while (ms--)
	{
		_delay_ms(1);
	}
}

int8_t BME280driver::read(uint8_t reg_addr, uint8_t *data, uint32_t len)
{
	// Start i2c write
	if (i2c_start(dev_addr << 1 | I2C_WRITE) == I2C_OK)
	{
		// Set reg_addr
		if (i2c_write(reg_addr) == I2C_OK)
		{
			i2c_stop();
			// Start i2c read
			if (i2c_start(dev_addr << 1 | I2C_READ) == I2C_OK)
			{
				// Read data
				for(uint32_t i = 0; i < len; i++) 
				{
					if(i != (len - 1) ) data[i] = i2c_readAck();
					else data[i] = i2c_readNak();
#ifdef DEBUG
					printf("read(0x%X, 0x%X)\n", reg_addr, data[i]);
#endif
				}
				i2c_stop();
				// Data read success
				return BME280_OK;
			}
			// i2c start read fail
			return BME280_ERR_CONN_FAIL;
		}
		i2c_stop();
		// reg_addr setting fail
		return BME280_ERR_WRITE_FAIL;
	}
	// i2c start write fail
	return BME280_ERR_CONN_FAIL;
}

int8_t BME280driver::write(uint8_t reg_addr, uint8_t *data, uint32_t len)
{
	// Start i2c write
	if (i2c_start(dev_addr << 1 | I2C_WRITE) == I2C_OK)
	{
		for(uint32_t i = 0; i < len; i++) 
		{
			// Set reg_addr
			if (i2c_write(reg_addr) == I2C_OK)
			{
				// Write data
				if (i2c_write(data[i]) != I2C_OK)
				{
					i2c_stop();
					// Data write fail
					return BME280_ERR_WRITE_FAIL;
				}
#ifdef DEBUG
				printf("write(0x%X, 0x%X)\n", reg_addr, data[i]);
#endif
			}
			else
			{
				i2c_stop();
				// reg_addr setting fail
				return BME280_ERR_WRITE_FAIL;
			}
		}
		i2c_stop();
		// Data write success
		return BME280_OK;
	}
	// i2c start write fail
	return BME280_ERR_CONN_FAIL;
}

int8_t BME280driver::read(uint8_t reg_addr, uint8_t *data)
{
	// Start i2c write
	if (i2c_start(dev_addr << 1 | I2C_WRITE) == I2C_OK)
	{
		// Set reg_addr
		if (i2c_write(reg_addr) == I2C_OK)
		{
			i2c_stop();
			// Start i2c read
			if (i2c_start(dev_addr << 1 | I2C_READ) == I2C_OK)
			{
				// Read data
				*data = i2c_readNak();
				i2c_stop();
#ifdef DEBUG
				printf("read(0x%X, 0x%X)\n", reg_addr, *data);
#endif
				// Data read success
				return BME280_OK;
			}
			// i2c start read fail
			return BME280_ERR_CONN_FAIL;
		}
		i2c_stop();
		// reg_addr setting fail
		return BME280_ERR_WRITE_FAIL;
	}
	// i2c start write fail
	return BME280_ERR_CONN_FAIL;
}

int8_t BME280driver::write(uint8_t reg_addr, uint8_t data)
{
	// Start i2c write
	if (i2c_start(dev_addr << 1 | I2C_WRITE) == I2C_OK)
	{
		// Set reg_addr
		if (i2c_write(reg_addr) == I2C_OK)
		{
			// Write data
			if (i2c_write(data) == I2C_OK)
			{
				i2c_stop();
#ifdef DEBUG
				printf("write(0x%X, 0x%X)\n", reg_addr, data);
#endif
				// Data write success
				return BME280_OK;
			}
			i2c_stop();
			// Data write fail
			return BME280_ERR_WRITE_FAIL;
		}
		i2c_stop();
		// reg_addr setting fail
		return BME280_ERR_WRITE_FAIL;
	}
	// i2c start write fail
	return BME280_ERR_CONN_FAIL;
}

int8_t BME280driver::init()
{
	int8_t res;
	
	// chip id read try count
	uint8_t try_count = 5;
	uint8_t chip_id = 0;

	while (try_count)
	{
		// Read the chip-id of bme280 sensor
		res = read(BME280_REG_CHIP_ID, &chip_id);

		// Check for chip id validity
		if ( (res == BME280_OK) && (chip_id == BME280_ALLOWED_CHIP_ID) )
		{
			break;
		}
		// Wait for 1 ms
		_delay_us(1000);
		--try_count;
	}

	// Chip id check failed
	if (!try_count) return BME280_ERR_DEV_NOT_FOUND;

	// Reset the sensor
	res = reset();

	// Read the calibration data
	if (res == BME280_OK) res = readCalibData();

	// Read config
	if (res == BME280_OK) res = readSettings();

	return res;
}

BME280driver::BME280driver(uint8_t dev_addr)
{
	this->device_ok = false;
	this->dev_addr = dev_addr;

	// settings.ctrl_hum.osrs_h = SAMPLING_X1;
	// settings.ctrl_meas.osrs_t = SAMPLING_X1;
	// settings.ctrl_meas.osrs_p = SAMPLING_X1;
	// settings.config.filter = FILTER_OFF;

	if (init() == BME280_OK) this->device_ok = true;
}

int8_t BME280driver::reset()
{
	int8_t res;
	BME280_reg_status status { .raw = 0 };
	uint8_t try_run = BME280_RESET_NVM_COPY_WAIT_RETRY; // default 5

	// Write the soft reset command in the sensor
	res = write(BME280_REG_RESET, BME280_CMD_SOFT_RESET);

	if (res == BME280_OK)
	{
		// If NVM not copied yet, Wait for NVM to copy
		do
		{
			// As per BME280 data sheet - Table 1, startup time is 2 ms.
			_delay_us(BME280_RESET_NVM_COPY_WAIT); // default 2000
			res = read(BME280_REG_STATUS, &(status.raw) );
		} 
		while ((res == BME280_OK) && (try_run--) && status.im_update);

		if (status.im_update) return BME280_ERR_NVM_NOT_COPIED;
	}
	return res;	
}

#ifdef BME280_ACQ_DELAY_ENABLE
void BME280driver::calcACQdelay()
{
	uint8_t osr_sett_to_act_osr[] = { 0, 1, 2, 4, 8, 16, 16, 16 };

	acq_delay = (uint32_t)( 
		(
			BME280_MEAS_OFFSET + (BME280_MEAS_DURATION * (uint32_t)osr_sett_to_act_osr[settings.ctrl_meas.osrs_t]) +
			BME280_PRES_HUM_MEAS_OFFSET + (BME280_MEAS_DURATION * (uint32_t)osr_sett_to_act_osr[settings.ctrl_meas.osrs_p]) +
			BME280_PRES_HUM_MEAS_OFFSET + (BME280_MEAS_DURATION * (uint32_t)osr_sett_to_act_osr[settings.ctrl_hum.osrs_h])
		) / BME280_MEAS_SCALING_FACTOR
	);
}
#endif


int8_t BME280driver::writeMode(const Mode mode)
{
	Mode current_mode;
	int8_t res = readMode(&current_mode);

	if( (res == BME280_OK) && (current_mode != mode))
	{
		// Setting to sleep procedure as in BoschSensortec driver. Needed? 
		if(mode == MODE_SLEEP) {
			res = reset();
			if(res == BME280_OK) return writeSettings();
		}

		BME280_reg_ctrl_meas ctrl_meas = settings.ctrl_meas;
		ctrl_meas.mode = mode;
		return write(BME280_REG_CTRL_MEAS, ctrl_meas.raw);
	}
	return BME280_OK;
}

int8_t BME280driver::readMode(Mode *mode)
{
	BME280_reg_ctrl_meas ctrl_meas;
	int8_t res = read(BME280_REG_CTRL_MEAS, &(ctrl_meas.raw));
	*mode = static_cast<Mode>(ctrl_meas.mode);
	return res;
}

int8_t BME280driver::readSettings()
{
	int8_t res;
	uint8_t settings_data[BME280_SETTINGS_DATA_LEN] = { 0 };
	res = read(BME280_REG_CTRL_HUM, settings_data, BME280_SETTINGS_DATA_LEN);
	if (res == BME280_OK)
	{
		settings.ctrl_hum.raw = settings_data[0];
		settings.ctrl_meas.raw = settings_data[2];
		settings.config.raw = settings_data[3];
	}
	return res;
}

int8_t BME280driver::writeSettings(const bool ctrl_meas, const bool ctrl_hum, const bool config)
{
	Mode current_mode;
	int8_t res = readMode(&current_mode);
	
	// If device not in sleep mode put in sleep mode by reset
	if( (res == BME280_OK) && (current_mode != MODE_SLEEP) ) res = reset();
	
	// Ensure sleep mode selected & write ctrl_meas
	settings.ctrl_meas.mode = MODE_SLEEP;
	if( (res == BME280_OK) && ctrl_meas)
		res = write(BME280_REG_CTRL_MEAS, settings.ctrl_meas.raw);

	// Write ctrl_hum
	if( (res == BME280_OK) && ctrl_hum)
		res = write(BME280_REG_CTRL_HUM, settings.ctrl_hum.raw);
	
	// Write config
	if( (res == BME280_OK) && config)
		res = write(BME280_REG_CONFIG, settings.config.raw);
	
#ifdef BME280_ACQ_DELAY_ENABLE
	calcACQdelay();
#ifdef DEBUG
	printf("acq_delay = %lu\n", acq_delay);
#endif
#endif
	return res;
}

int8_t BME280driver::readCalibData()
{
	int8_t res;
	uint8_t calib_data[BME280_TEMP_PRESS_CALIB_DATA_LEN] = { 0 };

	// Read the calibration data from the sensor
	res = read(BME280_REG_TEMP_PRESS_CALIB_DATA, calib_data, BME280_TEMP_PRESS_CALIB_DATA_LEN);

	if (res == BME280_OK)
	{
		// Parse temperature and pressure calibration data and store it in device structure
		parseTempPressCalibData(calib_data);

		// Read the humidity calibration data from the sensor
		res = read(BME280_REG_HUMIDITY_CALIB_DATA, calib_data, BME280_HUMIDITY_CALIB_DATA_LEN);
		if (res == BME280_OK)
		{
			// Parse humidity calibration data and store it in device structure
			parseHumidCalibData(calib_data);
		}
	}
	return res;
}

void BME280driver::parseTempPressCalibData(const uint8_t *reg_data)
{
	calib_data.dig_t1 = BME280_CONCAT_BYTES(reg_data[1], reg_data[0]);
	calib_data.dig_t2 = (int16_t)BME280_CONCAT_BYTES(reg_data[3], reg_data[2]);
	calib_data.dig_t3 = (int16_t)BME280_CONCAT_BYTES(reg_data[5], reg_data[4]);
	calib_data.dig_p1 = BME280_CONCAT_BYTES(reg_data[7], reg_data[6]);
	calib_data.dig_p2 = (int16_t)BME280_CONCAT_BYTES(reg_data[9], reg_data[8]);
	calib_data.dig_p3 = (int16_t)BME280_CONCAT_BYTES(reg_data[11], reg_data[10]);
	calib_data.dig_p4 = (int16_t)BME280_CONCAT_BYTES(reg_data[13], reg_data[12]);
	calib_data.dig_p5 = (int16_t)BME280_CONCAT_BYTES(reg_data[15], reg_data[14]);
	calib_data.dig_p6 = (int16_t)BME280_CONCAT_BYTES(reg_data[17], reg_data[16]);
	calib_data.dig_p7 = (int16_t)BME280_CONCAT_BYTES(reg_data[19], reg_data[18]);
	calib_data.dig_p8 = (int16_t)BME280_CONCAT_BYTES(reg_data[21], reg_data[20]);
	calib_data.dig_p9 = (int16_t)BME280_CONCAT_BYTES(reg_data[23], reg_data[22]);
	calib_data.dig_h1 = reg_data[25];
}

void BME280driver::parseHumidCalibData(const uint8_t *reg_data)
{
	int16_t dig_h4_lsb;
	int16_t dig_h4_msb;
	int16_t dig_h5_lsb;
	int16_t dig_h5_msb;

	calib_data.dig_h2 = (int16_t)BME280_CONCAT_BYTES(reg_data[1], reg_data[0]);
	calib_data.dig_h3 = reg_data[2];
	dig_h4_msb = (int16_t)(int8_t)reg_data[3] * 16;
	dig_h4_lsb = (int16_t)(reg_data[4] & 0x0F);
	calib_data.dig_h4 = dig_h4_msb | dig_h4_lsb;
	dig_h5_msb = (int16_t)(int8_t)reg_data[5] * 16;
	dig_h5_lsb = (int16_t)(reg_data[4] >> 4);
	calib_data.dig_h5 = dig_h5_msb | dig_h5_lsb;
	calib_data.dig_h6 = (int8_t)reg_data[6];
}

int8_t BME280driver::forcedACQ(uint32_t *pressure, uint32_t *temperature, uint32_t *humidity)
{
	int8_t res;
	uint8_t reg_data[BME280_P_T_H_DATA_LEN] = { 0 };

	// Start ACQ
	res = writeMode(MODE_FORCED);

	if (res == BME280_OK)
	{
		BME280_reg_status status;
		uint32_t acq_retry = 0;

#ifdef BME280_ACQ_DELAY_ENABLE
		// Wait calculated time until ACQ ends
		// May work faster if disabled
		delay_ms(acq_delay);
#endif

		// Ensure ACQ is not running
		read(BME280_REG_STATUS, &(status.raw));
		while (status.measuring)
		{
#ifdef DEBUG
			printf("acq_retry = %lu, BME280_reg_status: 0x%X\n", acq_retry, status.raw);
#endif
			if(acq_retry > BME280_ACQ_WAIT_RETRY) return BME280_ERR_ACQ_TIMEOUT;
			acq_retry++;
			_delay_us(BME280_ACQ_WAIT_US);
			read(BME280_REG_STATUS, &(status.raw));
		}
	}
	
	// Read data
	if (res == BME280_OK) res = read(BME280_REG_DATA, reg_data, BME280_P_T_H_DATA_LEN);

	// Parse data
	if (res == BME280_OK)
	{
		if(pressure != nullptr)
		{
			*pressure = ( (uint32_t)reg_data[0] << 12) + ( (uint32_t)reg_data[1] << 4) + ( (uint32_t)reg_data[2] >> 4);
		}

		if(temperature != nullptr)
		{
			*temperature = ( (uint32_t)reg_data[3] << 12) + ( (uint32_t)reg_data[4] << 4) + ( (uint32_t)reg_data[5] >> 4);
		}

		if(humidity != nullptr)
		{
			*humidity = ( (uint32_t)reg_data[6] << 8) + ( (uint32_t)reg_data[7]);
		}
	}
	
	return res;
}

BME280::BME280(uint8_t dev_addr) : BME280driver(dev_addr)
{
	// Initial temperature reading to calculate t_fine, t_fine_adjust

	setTemperatureSampling(BME280::SAMPLING_X1);
	float temperature;
	measure(nullptr, &temperature);
}

void BME280::setFilter(const Filter filter)
{
	settings.config.filter = filter;
	changed_settings.config = true;
}

BME280::Filter BME280::getFilter() const
{
	return static_cast<Filter>(settings.config.filter);
}

void BME280::setPressureSampling(const Sampling sampling)
{
	settings.ctrl_meas.osrs_p = sampling;
	changed_settings.ctrl_meas = true;
}

BME280::Sampling BME280::getPressureSampling() const
{
	return static_cast<Sampling>(settings.ctrl_meas.osrs_p);
}

void BME280::setTemperatureSampling(const Sampling sampling)
{
	settings.ctrl_meas.osrs_t = sampling;
	changed_settings.ctrl_meas = true;
}

BME280::Sampling BME280::getTemperatureSampling() const
{
	return static_cast<Sampling>(settings.ctrl_meas.osrs_t);
}

void BME280::setHumiditySampling(const Sampling sampling)
{
	settings.ctrl_hum.osrs_h = sampling;
	changed_settings.ctrl_hum = true;
}

BME280::Sampling BME280::getHumiditySampling() const
{
	return static_cast<Sampling>(settings.ctrl_hum.osrs_h);
}

int8_t BME280::applySettings()
{
	int8_t res = writeSettings(changed_settings.ctrl_meas, changed_settings.ctrl_hum, changed_settings.config);
	changed_settings.raw = 0;
	return res;
}

float BME280::compensateTemperature(uint32_t &uncomp_temperature)
{
	int32_t var1, var2;

	var1 = (int32_t)((uncomp_temperature / 8) - ((int32_t)calib_data.dig_t1 * 2));
	var1 = (var1 * ((int32_t)calib_data.dig_t2)) / 2048;
	var2 = (int32_t)((uncomp_temperature / 16) - ((int32_t)calib_data.dig_t1));
	var2 = (((var2 * var2) / 4096) * ((int32_t)calib_data.dig_t3)) / 16384;

	t_fine = var1 + var2 + t_fine_adjust;

	return (float)((t_fine * 5 + 128) / 256) / 100;
}

float BME280::compensatePressure(uint32_t &uncomp_pressure)
{
	int64_t var1, var2, var3, var4;

	var1 = ((int64_t)t_fine) - 128000;
	var2 = var1 * var1 * (int64_t)calib_data.dig_p6;
	var2 = var2 + ((var1 * (int64_t)calib_data.dig_p5) * 131072);
	var2 = var2 + (((int64_t)calib_data.dig_p4) * 34359738368);
	var1 = ((var1 * var1 * (int64_t)calib_data.dig_p3) / 256) +
		((var1 * ((int64_t)calib_data.dig_p2) * 4096));
	var3 = ((int64_t)1) * 140737488355328;
	var1 = (var3 + var1) * ((int64_t)calib_data.dig_p1) / 8589934592;

	if (var1 == 0) {
		return NAN; // avoid exception caused by division by zero
	}

	var4 = 1048576 - uncomp_pressure;
	var4 = (((var4 * 2147483648) - var2) * 3125) / var1;
	var1 = (((int64_t)calib_data.dig_p9) * (var4 / 8192) * (var4 / 8192)) /
		33554432;
	var2 = (((int64_t)calib_data.dig_p8) * var4) / 524288;
	var4 = ((var4 + var1 + var2) / 256) + (((int64_t)calib_data.dig_p7) * 16);

	return (float) (var4 / 256.0);
}

float BME280::compensateHumidity(uint32_t &uncomp_humidity)
{
	int32_t var1, var2, var3, var4, var5;

	var1 = t_fine - ((int32_t)76800);
	var2 = (int32_t)(uncomp_humidity * 16384);
	var3 = (int32_t)(((int32_t)calib_data.dig_h4) * 1048576);
	var4 = ((int32_t)calib_data.dig_h5) * var1;
	var5 = (((var2 - var3) - var4) + (int32_t)16384) / 32768;
	var2 = (var1 * ((int32_t)calib_data.dig_h6)) / 1024;
	var3 = (var1 * ((int32_t)calib_data.dig_h3)) / 2048;
	var4 = ((var2 * (var3 + (int32_t)32768)) / 1024) + (int32_t)2097152;
	var2 = ((var4 * ((int32_t)calib_data.dig_h2)) + 8192) / 16384;
	var3 = var5 * var2;
	var4 = ((var3 / 32768) * (var3 / 32768)) / 128;
	var5 = var3 - ((var4 * ((int32_t)calib_data.dig_h1)) / 16);
	var5 = (var5 < 0 ? 0 : var5);
	var5 = (var5 > 419430400 ? 419430400 : var5);

	return (float)( (uint32_t)(var5 / 4096) ) / 1024.0;
}

int8_t BME280::measure(float *pressure, float *temperature, float *humidity)
{
	uint32_t uncomp_pressure=0, uncomp_temperature=0, uncomp_humidity=0;
	int8_t res = BME280_OK;

	if(changed_settings.raw) res = applySettings();
	
	res = forcedACQ(
		(settings.ctrl_meas.osrs_p ? &uncomp_pressure : nullptr),
		(settings.ctrl_meas.osrs_t ? &uncomp_temperature : nullptr),
		(settings.ctrl_hum.osrs_h ? &uncomp_humidity : nullptr)
	);

	if( (temperature != nullptr) && (settings.ctrl_meas.osrs_t) ) *temperature = compensateTemperature(uncomp_temperature);
	if( (pressure != nullptr) && (settings.ctrl_meas.osrs_p) ) *pressure = compensatePressure(uncomp_pressure);
	if( (humidity != nullptr) && (settings.ctrl_hum.osrs_h) ) *humidity = compensateHumidity(uncomp_humidity);

	return res;
}

float BME280calcAltitude(float pressure) {
  return 44330.0 * (1.0 - pow(pressure / BME280_ALT_SEA_LEVEL_PRESSURE, BME280_ALT_HYPSOMETRIC_F_POW));
}

float BME280calcAltitude(float pressure, float temperature) {
  return BME280_ALT_MULT_CONST * (BME280_ALT_T_0_K + temperature) * (1.0 - pow(pressure / BME280_ALT_SEA_LEVEL_PRESSURE, BME280_ALT_HYPSOMETRIC_F_POW));
}
