#ifndef BME280_H
#define BME280_H

#include <stdint.h>

#define BME280_I2C_ADDR                    0x76 // 0x77 another

#define BME280_ALLOWED_CHIP_ID             0x60

#define BME280_OK                          0

#define BME280_ERR_DEV_NOT_FOUND           -2
#define BME280_ERR_CONN_FAIL               -4
#define BME280_ERR_WRITE_FAIL              -11
#define BME280_ERR_NVM_NOT_COPIED          -12

#define BME280_REG_CHIP_ID                 0xD0
#define BME280_REG_RESET                   0xE0
#define BME280_REG_TEMP_PRESS_CALIB_DATA   0x88
#define BME280_REG_HUMIDITY_CALIB_DATA     0xE1
#define BME280_REG_CTRL_HUM                0xF2
#define BME280_REG_STATUS                  0xF3
#define BME280_REG_PWR_CTRL                0xF4
#define BME280_REG_CTRL_MEAS               0xF4
#define BME280_REG_CONFIG                  0xF5
#define BME280_REG_DATA                    0xF7

#define BME280_CMD_SOFT_RESET              0xB6
#define BME280_STATUS_IM_UPDATE            0x01

#define BME280_RESET_NVM_COPY_WAIT_RETRY   5
#define BME280_RESET_NVM_COPY_WAIT         2000

#define BME280_TEMP_PRESS_CALIB_DATA_LEN   26
#define BME280_HUMIDITY_CALIB_DATA_LEN     7
#define BME280_P_T_H_DATA_LEN              8

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
	uint8_t dev_addr;

	BME280CalibData calib_data;
	
	int8_t read(uint8_t reg_addr, uint8_t *data, uint32_t len);
	int8_t write(uint8_t reg_addr, uint8_t *data, uint32_t len);

	int8_t read(uint8_t reg_addr, uint8_t *data);
	int8_t write(uint8_t reg_addr, uint8_t data);

	// int8_t getRegs(uint8_t reg_addr, uint8_t *reg_data, uint16_t len);
	// int8_t setRegs(uint8_t *reg_addr, const uint8_t *reg_data, uint8_t len);

	int8_t softReset();
	int8_t getCalibData();

	void parseTempPressCalibData(const uint8_t *reg_data);
	void parseHumidCalibData(const uint8_t *reg_data);

	int8_t init();
public:
	BME280driver(uint8_t dev_addr = BME280_I2C_ADDR);
	void reset();
};


#endif // BME280_H