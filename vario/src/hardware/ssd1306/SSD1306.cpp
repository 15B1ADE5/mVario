#include <stdint.h>

#include "SSD1306.h"

#include "../i2cmaster/i2cmaster.h"

#ifdef DEBUG
#include <stdio.h>
#endif

int8_t SSD1306driver::cmd(const uint8_t data)
{
	// Start i2c write
	if (i2c_start(dev_addr << 1 | I2C_WRITE) == I2C_OK)
	{
		// Init CMD write
		if (i2c_write(0x00) == I2C_OK)
		{
			// Write data
			if (i2c_write(data) == I2C_OK)
			{
				i2c_stop();
#ifdef DEBUG
				printf("cmd(0x%X)\n", data);
#endif
				// Data write success
				return SSD1306_OK;
			}
			i2c_stop();
			// Data write fail
			return SSD1306_ERR_WRITE_FAIL;
		}
		i2c_stop();
		// reg_addr setting fail
		return SSD1306_ERR_WRITE_FAIL;
	}
	// i2c start write fail
	return SSD1306_ERR_CONN_FAIL;
}

int8_t SSD1306driver::cmd(const uint8_t *data, const uint8_t data_len)
{
	// Start i2c write
	if (i2c_start(dev_addr << 1 | I2C_WRITE) == I2C_OK)
	{
		// Set reg_addr
		if (i2c_write(0x00) == I2C_OK)
		{
			for(uint8_t i = 0; i < data_len; i++) 
			{
				// Write data
				if (i2c_write(data[i]) != I2C_OK)
				{
					i2c_stop();
					// Data write fail
					return SSD1306_ERR_WRITE_FAIL;
				}
#ifdef DEBUG
				printf("cmd(0x%X)\n", data[i]);
#endif
			}
		}
		else
		{
			i2c_stop();
			// reg_addr setting fail
			return SSD1306_ERR_WRITE_FAIL;
		}
		i2c_stop();
		// Data write success
		return SSD1306_OK;
	}
	// i2c start write fail
	return SSD1306_ERR_CONN_FAIL;
}

#ifdef REDUCE_BINARY_SIZE
	#define SSD1306_CHECK_RET
#else
	#define SSD1306_CHECK_RET    if(res == SSD1306_OK) res = 
#endif

int8_t SSD1306driver::init()
{
	int res = SSD1306_OK;

	// Turn display off
	res = sleep();

	// Set Display Clock Divide Ratio/Oscillator Frequency
	// SSD1306_CHECK_RET setRefreshRate();
	SSD1306_CHECK_RET cmd(SSD1306_CMD_SET_DISPLAY_CLOCK);
	SSD1306_CHECK_RET cmd( (uint8_t) SSD1306_DISPLAY_CLOCK(
		SSD1306_DEFAULT_CLOCK_DIV_RATIO,
		SSD1306_DEFAULT_CLOCK_FREQUENCY)
	);

	// Set Multiplex Ratio
	SSD1306_CHECK_RET cmd(SSD1306_CMD_SET_MULTIPLEX_RATIO);
	SSD1306_CHECK_RET cmd( SSD1306_MULTIPLEX_RATIO(SSD1306_DEFAULT_MULTIPLEX_RATIO) );

	// Set Display Offset
	//SSD1306_CHECK_RET setVerticalOffset();
	SSD1306_CHECK_RET cmd(SSD1306_CMD_SET_DISPLAY_OFFSET);
	SSD1306_CHECK_RET cmd( (uint8_t) SSD1306_DISPLAY_OFFSET(SSD1306_DEFAULT_VERTICAL_OFFSET) );

	// Set Display Start Line
	SSD1306_CHECK_RET setStartLine();
	//cmd( SSD1306_CMD_SET_START_LINE(SSD1306_DEFAULT_START_LINE) );

	// Set Charge Pump
	SSD1306_CHECK_RET cmd(SSD1306_CMD_CHARGEPUMP);
	SSD1306_CHECK_RET cmd(SSD1306_CHARGEPUMP_ON);

	// Set Segment Re-Map (horizontal scan)
	SSD1306_CHECK_RET setHorizontalScan();
	//cmd(SSD1306_CMD_SET_SEGMENT_REMAP_LEFT);

	// Set COM Output Scan Direction (vertical scan)
	SSD1306_CHECK_RET setVerticalScan();
	//cmd(SSD1306_CMD_SET_COM_SCAN_FROM_0);

	// Set COM Pins Hardware Configuration
	SSD1306_CHECK_RET cmd(SSD1306_CMD_SET_COM_PINS);
	SSD1306_CHECK_RET cmd(SSD1306_COM_PINS_A_ALTERNATIVE | SSD1306_COM_PINS_B_REMAP_OFF);

	// Set Contrast Control
	 SSD1306_CHECK_RET setContrast();
	//cmd(SSD1306_CMD_SET_CONTRAST);
	//cmd(SSD1306_DEFAULT_CONTRAST);

	// Set Pre-Charge Period
	SSD1306_CHECK_RET setPreChargePeriod();
	//cmd(SSD1306_CMD_SET_PRECHARGE_PERIOD);
	//cmd(SSD1306_DEFAULT_PRECHARGE_PERIOD);

	// Set VCOMH Deselect Level
	SSD1306_CHECK_RET setVCOMHdeselectLevel();
	//cmd(SSD1306_CMD_SET_V_COM_DESELECT);
	//cmd(SSD1306_DEFAULT_V_COM_DESELECT);

	// Set Normal/Inverse Display
	SSD1306_CHECK_RET setMode();
	//cmd(SSD1306_CMD_MODE_NORMAL);
    
	// Horizontal memory mode
	SSD1306_CHECK_RET setMemoryMode();
	//cmd(SSD1306_CMD_SET_MEM_ADDR_MODE);
	//cmd(SSD1306_MEM_ADDR_MODE_HORIZONTAL);

	SSD1306_CHECK_RET clearBuffer();
	
	SSD1306_CHECK_RET wakeup();

	return res;
}


SSD1306driver::SSD1306driver(uint8_t dev_addr)
{
	this->device_ok = false;
	this->dev_addr = dev_addr;

	if (init() == SSD1306_OK) this->device_ok = true;
}

/*
{
	cmd( (const uint8_t []) {
		SSD1306_CMD_CHARGEPUMP,
		SSD1306_CHARGEPUMP_ON
	}, 2);
}
*/

// Display power:
int8_t SSD1306driver::sleep()
{
	return cmd(SSD1306_CMD_DISPLAY_OFF);
}

int8_t SSD1306driver::wakeup()
{
	return cmd(SSD1306_CMD_DISPLAY_ON);
}

int8_t SSD1306driver::off()
{
	return cmd(SSD1306_CMD_ENTIRE_DISPLAY_OFF);
}

int8_t SSD1306driver::on()
{
	return cmd(SSD1306_CMD_ENTIRE_DISPLAY_ON);
}


// Display Settings:
int8_t SSD1306driver::setRefreshRate(const uint8_t divide_ratio, const uint8_t frequency)
{
	const uint8_t cmd_data[] = {
		SSD1306_CMD_SET_DISPLAY_CLOCK,
		(uint8_t) SSD1306_DISPLAY_CLOCK(divide_ratio, frequency)
	};
	return cmd(cmd_data, 2);
}

int8_t SSD1306driver::setVerticalOffset(const uint8_t offset)
{
	const uint8_t cmd_data[] = {
		SSD1306_CMD_SET_DISPLAY_OFFSET,
		(uint8_t) SSD1306_DISPLAY_OFFSET(offset)
	};
	return cmd(cmd_data, 2);
}

int8_t SSD1306driver::setStartLine(const uint8_t line)
{
	return cmd(SSD1306_CMD_SET_START_LINE(line) );
}

int8_t SSD1306driver::setHorizontalScan(const bool right)
{
	if(right) return cmd(SSD1306_CMD_SET_SEGMENT_REMAP_RIGHT);
	return cmd(SSD1306_CMD_SET_SEGMENT_REMAP_LEFT);
}

int8_t SSD1306driver::setVerticalScan(const bool bottom)
{
	if(bottom) return cmd(SSD1306_CMD_SET_COM_SCAN_FROM_N);
	return cmd(SSD1306_CMD_SET_COM_SCAN_FROM_0);
}

int8_t SSD1306driver::setMode(const bool inverse)
{
	if(inverse) return cmd(SSD1306_CMD_MODE_INVERSE);
	return cmd(SSD1306_CMD_MODE_NORMAL);
}

int8_t SSD1306driver::setMemoryMode(const MemoryMode mode)
{
	const uint8_t cmd_data[] = {
		SSD1306_CMD_SET_MEM_ADDR_MODE,
		(uint8_t) mode
	};
	return cmd(cmd_data, 2);
}

// Display Brightness Settings:
int8_t SSD1306driver::setContrast(const uint8_t contrast)
{
	const uint8_t cmd_data[] = {
		SSD1306_CMD_SET_CONTRAST,
		contrast
	};
	return cmd(cmd_data, 2);
}

int8_t SSD1306driver::setPreChargePeriod(const uint8_t period)
{
	const uint8_t cmd_data[] = {
		SSD1306_CMD_SET_PRECHARGE_PERIOD,
		period
	};
	return cmd(cmd_data, 2);
}

int8_t SSD1306driver::setVCOMHdeselectLevel(const uint8_t level)
{
	const uint8_t cmd_data[] = {
		SSD1306_CMD_SET_V_COM_DESELECT,
		(uint8_t) SSD1306_V_COM_DESELECT(level)
	};
	return cmd(cmd_data, 2);
}

// Scrolling:
int8_t SSD1306driver::setHorizontalScroll(const uint8_t right, const ScrollInterval interval, const uint8_t start_page, const uint8_t end_page)
{
	const uint8_t cmd_data[] = {
		(uint8_t) (right ? SSD1306_CMD_SET_CONT_HOR_SCROLL_R : SSD1306_CMD_SET_CONT_HOR_SCROLL_L),
		SSD1306_CONT_SCR_A_DUMMY,
		(uint8_t) SSD1306_CONT_SCR_B_START_PAGE(start_page),
		(uint8_t) interval,
		(uint8_t) SSD1306_CONT_SCR_D_END_PAGE(end_page),
		SSD1306_CONT_SCR_E_DUMMY,
		SSD1306_CONT_SCR_F_DUMMY
	};
	return cmd(cmd_data, 7);
}

int8_t SSD1306driver::setVerticalHorizontalScroll(const uint8_t right, const ScrollInterval interval, const uint8_t start_page, const uint8_t end_page, const uint8_t vertical_scroll_offset)
{
	const uint8_t cmd_data[] = {
		(uint8_t) (right ? SSD1306_CMD_SET_CONT_VERT_SCROLL_R : SSD1306_CMD_SET_CONT_VERT_SCROLL_L),
		SSD1306_CONT_SCR_A_DUMMY,
		(uint8_t) SSD1306_CONT_SCR_B_START_PAGE(start_page),
		(uint8_t) interval,
		(uint8_t) SSD1306_CONT_SCR_D_END_PAGE(end_page),
		(uint8_t) SSD1306_CONT_SCR_E_VERT_SCR(vertical_scroll_offset)
	};
	return cmd(cmd_data, 6);
}

int8_t SSD1306driver::startScroll()
{
	return cmd(SSD1306_CMD_ACTIVATE_SCROLL);
}

int8_t SSD1306driver::stopScroll()
{
	return cmd(SSD1306_CMD_DEACTIVATE_SCROLL);
}


int8_t SSD1306driver::setColumnRange(const uint8_t start, const uint8_t end)
{
	const uint8_t cmd_data[] = {
		SSD1306_CMD_SET_COL_ADDR,
		(uint8_t) SSD1306_COL_ADDR(start),
		(uint8_t) SSD1306_COL_ADDR(end)
	};
	return cmd(cmd_data, 3);
}

int8_t SSD1306driver::setPagesRange(const uint8_t start, const uint8_t end)
{
	const uint8_t cmd_data[] = {
		SSD1306_CMD_SET_PAGE_ADDR,
		(uint8_t) SSD1306_PAGE_ADDR(start),
		(uint8_t) SSD1306_PAGE_ADDR(end)
	};
	return cmd(cmd_data, 3);
}


int8_t SSD1306driver::sendData(const uint8_t *data, const uint16_t data_len)
{
	// Start i2c write
	if (i2c_start(dev_addr << 1 | I2C_WRITE) == I2C_OK)
	{
		// Set reg_addr
		if (i2c_write(0x40) == I2C_OK)
		{
			for(uint16_t i = 0; i < data_len; i++) 
			{
				// Write data
				if (i2c_write(data[i]) != I2C_OK)
				{
					i2c_stop();
					// Data write fail
					return SSD1306_ERR_WRITE_FAIL;
				}
#ifdef DEBUG
				printf("data(0x%X)\n", data[i]);
#endif
			}
		}
		else
		{
			i2c_stop();
			// reg_addr setting fail
			return SSD1306_ERR_WRITE_FAIL;
		}
		i2c_stop();
		// Data write success
		return SSD1306_OK;
	}
	// i2c start write fail
	return SSD1306_ERR_CONN_FAIL;
}

int8_t SSD1306driver::clearBuffer()
{
	setColumnRange(0, SSD1306_MAX_WIDTH);
	setPagesRange(0, SSD1306_MAX_HEIGHT);
	// Start i2c write
	if (i2c_start(dev_addr << 1 | I2C_WRITE) == I2C_OK)
	{
		// Set reg_addr
		if (i2c_write(0x40) == I2C_OK)
		{
			for(uint16_t i = 0; i < 1024; i++) 
			{
				// Write data
				if (i2c_write(0x00) != I2C_OK)
				{
					i2c_stop();
					// Data write fail
					return SSD1306_ERR_WRITE_FAIL;
				}
			}
		}
		else
		{
			i2c_stop();
			// reg_addr setting fail
			return SSD1306_ERR_WRITE_FAIL;
		}
		i2c_stop();
		// Data write success
		return SSD1306_OK;
	}
	// i2c start write fail
	return SSD1306_ERR_CONN_FAIL;
}