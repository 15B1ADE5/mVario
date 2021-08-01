#include "bme280.h"

#include <stdio.h>
#include <util/delay.h>
#include "../i2cmaster/i2cmaster.h"

#define BME280_CONCAT_BYTES(msb, lsb)	 (((uint16_t)msb << 8) | (uint16_t)lsb)

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
					printf("read(0x%X, 0x%X)\n", reg_addr, data[i]);
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
		// Set reg_addr
		if (i2c_write(reg_addr) == I2C_OK)
		{
			for(uint32_t i = 0; i < len; i++) 
			{
				// Write data
				if (i2c_write(data[i]) == I2C_OK)
				{
					i2c_stop();
					// Data write fail
					return BME280_ERR_WRITE_FAIL;
				}
			}
			i2c_stop();
			// Data write success
			return BME280_OK;
		}
		i2c_stop();
		// reg_addr setting fail
		return BME280_ERR_WRITE_FAIL;
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
	printf("write(0x%x, 0x%x)\n", reg_addr, data);
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
				// Data write fail
				return BME280_ERR_WRITE_FAIL;
			}
			i2c_stop();
			// Data write success
			return BME280_OK;
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

		printf("%d ID: 0x%x\n", res, chip_id);

		// Check for chip id validity
		if ( (res == BME280_OK) && (chip_id == BME280_ALLOWED_CHIP_ID) )
		{
			// Reset the sensor
			res = softReset();

			if (res == BME280_OK)
			{
				// Read the calibration data
				res = readCalibData();
			}
			break;
		}

		// Wait for 1 ms
		_delay_us(1000);
		--try_count;
	}

	// Chip id check failed
	if (!try_count)
	{
		res = BME280_ERR_DEV_NOT_FOUND;
	}

	return res;
}

BME280driver::BME280driver(uint8_t dev_addr)
{
	this->device_ok = false;
	this->dev_addr = dev_addr;
	if (init() == BME280_OK) this->device_ok = true;
}

int8_t BME280driver::softReset()
{
	int8_t res;
	BME280_reg_status status { .raw = 0 };
	uint8_t try_run = BME280_RESET_NVM_COPY_WAIT_RETRY; // default 5

	// Write the soft reset command in the sensor
	res = write(BME280_REG_RESET, BME280_CMD_SOFT_RESET);

	//if (res == BME280_OK) // writing RESET_CMD always returns error on ATmega328p 
	{
		// If NVM not copied yet, Wait for NVM to copy
		do
		{
			// As per BME280 data sheet - Table 1, startup time is 2 ms.
			_delay_us(BME280_RESET_NVM_COPY_WAIT); // default 2000
			res = read(BME280_REG_STATUS, &(status.raw) );
		} 
		while ((res == BME280_OK) && (try_run--) && status.im_update);

		if (status.im_update)
		{
			res = BME280_ERR_NVM_NOT_COPIED;
		}
	}
	return res;	
}

int8_t BME280driver::readSettings()
{
	if(device_ok)
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
	return BME280_ERR_DEV_NOT_INIT;
}

int8_t BME280driver::readCalibData()
{
	if(device_ok)
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
	return BME280_ERR_DEV_NOT_INIT;
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

int8_t BME280driver::putDeviceToSleep()
{
	if(device_ok)
	{
		int8_t res;

		return res;
	}
	return BME280_ERR_DEV_NOT_INIT;
}

int8_t BME280driver::writePowerMode(const uint8_t mode)
{
	if(device_ok)
	{
		int8_t res;

		return res;
	}
	return BME280_ERR_DEV_NOT_INIT;
}

int8_t BME280driver::getMode(uint8_t *mode)
{
	if(device_ok)
	{
		uint8_t mode_reg;
		int8_t res = read(BME280_REG_CTRL_MEAS, &mode_reg);
		*mode = mode_reg & BME280_PWR_CTRL_MODE_MASK;
		return res;
	}
	return BME280_ERR_DEV_NOT_INIT;
}

int8_t BME280driver::setMode(const uint8_t mode)
{
	if(device_ok)
	{
		uint8_t mode_reg;
		int8_t res = getMode(&mode_reg);

		// If the sensor is not in sleep mode put the device to sleep mode
		if ((res == BME280_OK) && (mode_reg != MODE_SLEEP))
		{
			res = putDeviceToSleep();
		}

		// Set the power mode
		if (res == BME280_OK)
		{
			res = writePowerMode(mode);
		}
		return res;
	}
	return BME280_ERR_DEV_NOT_INIT;
}
