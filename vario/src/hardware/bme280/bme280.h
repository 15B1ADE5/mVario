#ifndef BME280_H
#define BME280_H

//////////////////////////////////////////////////
// BME280 lib:
// Based on 
//   official BoschSensortec BME280_driver:
//   https://github.com/BoschSensortec/BME280_driver
// and Adafruit_BME280_Library:
//   https://github.com/adafruit/Adafruit_BME280_Library/
//////////////////////////////////////////////////

#include <stdint.h>

#define BME280_I2C_ADDR                    0x76
#define BME280_I2C_ADDR_SEC                0x77

//////////////////////////////////////////////////
// BME280 Registers:
//////////////////////////////////////////////////

#define BME280_REG_TEMP_PRESS_CALIB_DATA   0x88
#define BME280_REG_CHIP_ID                 0xD0
#define BME280_REG_RESET                   0xE0
#define BME280_REG_HUMIDITY_CALIB_DATA     0xE1
#define BME280_REG_CTRL_HUM                0xF2
#define BME280_REG_STATUS                  0xF3
#define BME280_REG_CTRL_MEAS               0xF4
#define BME280_REG_CONFIG                  0xF5
#define BME280_REG_DATA                    0xF7

#define BME280_ALLOWED_CHIP_ID             0x60

#define BME280_CMD_SOFT_RESET              0xB6
#define BME280_STATUS_IM_UPDATE            0x01

#define BME280_PWR_CTRL_MODE_MASK          0x3

#define BME280_CTRL_HUM_MASK               0x07
#define BME280_CTRL_HUM_POS                0x00

#define BME280_CTRL_PRESS_MASK             0x1C
#define BME280_CTRL_PRESS_POS              0x02

#define BME280_CTRL_TEMP_MSK               0xE0
#define BME280_CTRL_TEMP_POS               0x05

#define BME280_FILTER_MASK                 0x1C
#define BME280_FILTER_POS                  0x02

#define BME280_STANDBY_MASK                0xE0
#define BME280_STANDBY_POS                 0x05
/*
#define BME280_MODE_SLEEP                  0x0
#define BME280_MODE_FORCED                 0x1
#define BME280_MODE_NORMAL                 0x3
*/

// BME280_REG_CTRL_HUM
typedef union
{
	struct
	{
		uint8_t : 5;

		// pressure oversampling
		// 000 = skipped
		// 001 = x1
		// 010 = x2
		// 011 = x4
		// 100 = x8
		// 101 and above = x16
		uint8_t osrs_h : 3;
		
	} __attribute__((__packed__));
	uint8_t raw;
} BME280_reg_ctrl_hum;


// BME280_REG_STATUS
typedef union
{
	struct
	{
		uint8_t : 4;

		// measure status
		// 0 = results have been transferred to the data registers
		// 1 = conversion is running
		uint8_t measuring : 1;

		uint8_t : 2;

		// NVM data status
		// 0 = NVM data copying is done
		// 1 = NVM data are being copied to image registers
		uint8_t im_update : 1;
		
	} __attribute__((__packed__));
	uint8_t raw;
} BME280_reg_status;

// BME280_REG_CTRL_MEAS
typedef union
{
	struct
	{
		// temperature oversampling
		// 000 = skipped
		// 001 = x1
		// 010 = x2
		// 011 = x4
		// 100 = x8
		// 101 and above = x16
		uint8_t osrs_t : 3;

		// pressure oversampling
		// 000 = skipped
		// 001 = x1
		// 010 = x2
		// 011 = x4
		// 100 = x8
		// 101 and above = x16
		uint8_t osrs_p : 3;

		// device mode
		// 00       = sleep
		// 01 or 10 = forced
		// 11       = normal
		uint8_t mode : 2;
	} __attribute__((__packed__));
	uint8_t raw;
} BME280_reg_ctrl_meas;

// BME280_REG_CONFIG
typedef union
{
	struct
	{
		// standby time duration in normal mode
		// 000 = 0.5 ms
		// 001 = 62.5 ms
		// 010 = 125 ms
		// 011 = 250 ms
		// 100 = 500 ms
		// 101 = 1000 ms
		// 110 = 10 ms
		// 111 = 20 ms
		uint8_t t_sb : 3;
		
		// filter settings
		// 000 = filter off
		// 001 = 2x filter
		// 010 = 4x filter
		// 011 = 8x filter
		// 100 and above = 16x filter
		uint8_t filter : 3;

		uint8_t : 1;

		// Enables 3-wire SPI interface
		// 1 = enable
		uint8_t spi3w_en : 1;
	} __attribute__((__packed__));
	uint8_t raw;
} BME280_reg_config;


//////////////////////////////////////////////////
// BME280 Return codes:
//////////////////////////////////////////////////
#define BME280_OK                          0

#define BME280_ERR_DEV_NOT_INIT            -1
#define BME280_ERR_DEV_NOT_FOUND           -2
#define BME280_ERR_CONN_FAIL               -4
#define BME280_ERR_WRITE_FAIL              -11
#define BME280_ERR_NVM_NOT_COPIED          -12



//////////////////////////////////////////////////
// BME280 Settings:
//////////////////////////////////////////////////
#define BME280_RESET_NVM_COPY_WAIT_RETRY   5
#define BME280_RESET_NVM_COPY_WAIT         2000

#define BME280_SETTINGS_DATA_LEN           4

#define BME280_TEMP_PRESS_CALIB_DATA_LEN   26
#define BME280_HUMIDITY_CALIB_DATA_LEN     7
#define BME280_P_T_H_DATA_LEN              8


struct BME280Settings
{
	BME280_reg_ctrl_hum ctrl_hum;
	BME280_reg_ctrl_meas ctrl_meas;
	BME280_reg_config config;

	/*
	// pressure oversampling
	uint8_t osr_p;

	// temperature oversampling
	uint8_t osr_t;

	// humidity oversampling
	uint8_t osr_h;

	// filter coefficient
	uint8_t filter;

	// standby time
	uint8_t standby_time;
	*/
};

struct BME280CalibData
{
	// Calibration coefficients for the temperature sensor
	uint16_t dig_t1;
	int16_t dig_t2;
	int16_t dig_t3;
	// Variable to store the intermediate temperature coefficient
	int32_t t_fine;

	// Calibration coefficients for the pressure sensor
	uint16_t dig_p1;
	int16_t dig_p2;
	int16_t dig_p3;
	int16_t dig_p4;
	int16_t dig_p5;
	int16_t dig_p6;
	int16_t dig_p7;
	int16_t dig_p8;
	int16_t dig_p9;

	// Calibration coefficients for the humidity sensor
	uint8_t dig_h1;
	int16_t dig_h2;
	uint8_t dig_h3;
	int16_t dig_h4;
	int16_t dig_h5;
	int8_t dig_h6;

};

class BME280driver
{
public:
	enum Sampling 
	{
		SAMPLING_NONE = 0b000,
		SAMPLING_X1 = 0b001,
		SAMPLING_X2 = 0b010,
		SAMPLING_X4 = 0b011,
		SAMPLING_X8 = 0b100,
		SAMPLING_X16 = 0b101
	};
  
	enum Mode {
		MODE_SLEEP = 0b00,
		MODE_FORCED = 0b01,
		MODE_NORMAL = 0b11
	};

	enum Filter {
		FILTER_OFF = 0b000,
		FILTER_X2 = 0b001,
		FILTER_X4 = 0b010,
		FILTER_X8 = 0b011,
		FILTER_X16 = 0b100
	};

	enum StandbyDuration {
		STANDBY_MS_0_5 = 0b000,
		STANDBY_MS_10 = 0b110,
		STANDBY_MS_20 = 0b111,
		STANDBY_MS_62_5 = 0b001,
		STANDBY_MS_125 = 0b010,
		STANDBY_MS_250 = 0b011,
		STANDBY_MS_500 = 0b100,
		STANDBY_MS_1000 = 0b101
	};

private:
	bool device_ok;
	uint8_t dev_addr;

	BME280Settings settings;
	BME280CalibData calib_data;
	
	int8_t read(uint8_t reg_addr, uint8_t *data, uint32_t len);
	int8_t write(uint8_t reg_addr, uint8_t *data, uint32_t len);

	int8_t read(uint8_t reg_addr, uint8_t *data);
	int8_t write(uint8_t reg_addr, uint8_t data);

	// int8_t getRegs(uint8_t reg_addr, uint8_t *reg_data, uint16_t len);
	// int8_t setRegs(uint8_t *reg_addr, const uint8_t *reg_data, uint8_t len);

	int8_t softReset();
	int8_t putDeviceToSleep();
	int8_t writePowerMode(const uint8_t mode);

	int8_t readSettings();

	int8_t readCalibData();
	void parseTempPressCalibData(const uint8_t *reg_data);
	void parseHumidCalibData(const uint8_t *reg_data);


	int8_t init();
public:
	BME280driver(uint8_t dev_addr = BME280_I2C_ADDR);

	bool deviceOK() const { return device_ok; }
	void reset();

	int8_t getMode(uint8_t *mode);
	int8_t setMode(const uint8_t mode);
	
	int8_t setSettings(uint8_t desired_settings);
	int8_t getSettings(uint8_t desired_settings);
};


#endif // BME280_H 