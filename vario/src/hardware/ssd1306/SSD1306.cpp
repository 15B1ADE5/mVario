#include <stdint.h>

#include "SSD1306.h"

#include "../i2cmaster/i2cmaster.h"

#ifdef DEBUG
#include <stdio.h>
#endif

uint8_t SSD1306driver::cmd(const uint8_t data)
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

uint8_t SSD1306driver::cmd(const uint8_t *data, const uint8_t data_len)
{
	// Start i2c write
	if (i2c_start(dev_addr << 1 | I2C_WRITE) == I2C_OK)
	{
		// Set reg_addr
		if (i2c_write(0x00) == I2C_OK)
		{
			for(uint32_t i = 0; i < data_len; i++) 
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

uint8_t SSD1306driver::init()
{
	
	// Turn display off
	cmd(SSD1306_CMD_DISPLAY_OFF);

	// Set Display Clock Divide Ratio/Oscillator Frequency
	cmd(SSD1306_CMD_SET_DISPLAY_CLOCK_DIV);
	cmd(0xF0);//SSD1306_DEFAULT_DISPLAY_CLOCK_DIV);

	// Set Multiplex Ratio
	cmd(SSD1306_CMD_SET_MULTIPLEX_RATIO);
	cmd( SSD1306_MULTIPLEX_RATIO(SSD1306_DEFAULT_MULTIPLEX_RATIO) );

	// Set Display Offset
	cmd(SSD1306_CMD_SET_DISPLAY_OFFSET);
	cmd( SSD1306_DISPLAY_OFFSET(SSD1306_DEFAULT_DISPLAY_OFFSET) );

	// Set Display Start Line
	cmd( SSD1306_CMD_SET_START_LINE(SSD1306_DEFAULT_START_LINE) );

	// Set Charge Pump
	cmd(SSD1306_CMD_CHARGEPUMP);
	cmd(SSD1306_CHARGEPUMP_ON);

	// Set Segment Re-Map (horizontal scan)
	cmd(SSD1306_CMD_SET_SEGMENT_REMAP_LEFT);

	// Set COM Output Scan Direction (vertical scan)
	cmd(SSD1306_CMD_SET_COM_SCAN_FROM_0);

	// Set COM Pins Hardware Configuration
	cmd(SSD1306_CMD_SET_COM_PINS);
	cmd(SSD1306_COM_PINS_A_ALTERNATIVE | SSD1306_COM_PINS_B_REMAP_OFF);

	// Set Contrast Control
	cmd(SSD1306_CMD_SET_CONTRAST);
	cmd(0x80);

	// Set Pre-Charge Period
	cmd(SSD1306_CMD_SET_PRECHARGE_PERIOD);
	cmd(0xF1);

	// Set VCOMH Deselect Level
	cmd(SSD1306_CMD_SET_V_COM_DESELECT);
	cmd(0x20);

	// Set Normal/Inverse Display
	cmd(SSD1306_CMD_MODE_NORMAL);
    
	// Horizontal memory mode
	cmd(SSD1306_CMD_SET_MEM_ADDR_MODE);
	cmd(SSD1306_MEM_ADDR_MODE_HORIZONTAL);
   
	cmd(SSD1306_CMD_DISPLAY_ON);

	return SSD1306_OK;
}


SSD1306driver::SSD1306driver(uint8_t dev_addr)
{
	this->device_ok = false;
	this->dev_addr = dev_addr;

	if (init() == SSD1306_OK) this->device_ok = true;
}

void SSD1306driver::sendFramebuffer(uint8_t *buffer) {
    cmd(0x21);
    cmd(0x00);
    cmd(0x7F);

    cmd(0x22);
    cmd(0x00);
    cmd(0x07);

    // We have to send the buffer as 16 bytes packets
    // Our buffer is 1024 bytes long, 1024/16 = 64
    // We have to send 64 packets
    for (uint8_t packet = 0; packet < 64; packet++) {
        i2c_start(dev_addr << 1 | I2C_WRITE);
        i2c_write(0x40);
        for (uint8_t packet_byte = 0; packet_byte < 16; ++packet_byte) {
            i2c_write(buffer[packet*16+packet_byte]);
        }
        i2c_stop();
    }
}