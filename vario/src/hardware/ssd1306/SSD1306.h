#ifndef SSD1306
#define SSD1306

#include <stdint.h>

#define SSD1306_DEFAULT_ADDRESS                 0x3C

//////////////////////////////////////////////////
// 1. Fundamental Commands:
//////////////////////////////////////////////////
//
#define SSD1306_CMD_SET_CONTRAST                0x81
#define SSD1306_DEFAULT_CONTRAST                0xCF // 0xCF
//
#define SSD1306_CMD_ENTIRE_DISPLAY_OFF          0xA4
#define SSD1306_CMD_ENTIRE_DISPLAY_ON           0xA5
//
#define SSD1306_CMD_MODE_NORMAL                 0xA6
#define SSD1306_CMD_MODE_INVERSE                0xA7
//
#define SSD1306_CMD_DISPLAY_OFF                 0xAE // LOW POWER MODE
#define SSD1306_CMD_DISPLAY_ON                  0xAF


//////////////////////////////////////////////////
// 2. Scrolling Commands:
//////////////////////////////////////////////////

#define SSD1306_CMD_SET_CONT_HOR_SCROLL_R       0x26
#define SSD1306_CMD_SET_CONT_HOR_SCROLL_L       0x27

#define SSD1306_CMD_SET_CONT_VERT_SCROLL_R      0x29
#define SSD1306_CMD_SET_CONT_VERT_SCROLL_L      0x2A

#define SSD1306_CONT_SCR_A_DUMMY                0x00

#define SSD1306_CONT_SCR_B_START_PAGE(addr)     (0x07 & addr)

#define SSD1306_CONT_SCR_C_STEP_FRAMES_2        0x07
#define SSD1306_CONT_SCR_C_STEP_FRAMES_3        0x04
#define SSD1306_CONT_SCR_C_STEP_FRAMES_4        0x05
#define SSD1306_CONT_SCR_C_STEP_FRAMES_5        0x00
#define SSD1306_CONT_SCR_C_STEP_FRAMES_25       0x06
#define SSD1306_CONT_SCR_C_STEP_FRAMES_64       0x01
#define SSD1306_CONT_SCR_C_STEP_FRAMES_128      0x02
#define SSD1306_CONT_SCR_C_STEP_FRAMES_256      0x03

#define SSD1306_CONT_SCR_D_END_PAGE(addr)       (0x07 & addr)

#define SSD1306_CONT_SCR_E_DUMMY                0x00

#define SSD1306_CONT_SCR_E_VERT_SCR(offset)     (0x3F & offset)

#define SSD1306_CONT_SCR_F_DUMMY                0xFF

#define SSD1306_CMD_DEACTIVATE_SCROLL           0x2E
#define SSD1306_CMD_ACTIVATE_SCROLL             0x2F

//////////////////////////////////////////////////
// 3. Addressing Setting Commands:
//////////////////////////////////////////////////

#define SSD1306_CMD_SET_COL_ADDR_PAGE_LB(lab)   (0x0F & lab)
#define SSD1306_CMD_SET_COL_ADDR_PAGE_HB(hab)   (0x10 | (0x1F & hab) )
//
#define SSD1306_CMD_SET_MEM_ADDR_MODE           0x20
#define SSD1306_MEM_ADDR_MODE_HORIZONTAL        0x00
#define SSD1306_MEM_ADDR_MODE_VERTICAL          0x01
#define SSD1306_MEM_ADDR_MODE_PAGE              0x02

// for MEM_ADDR_MODE_HORIZONTAL/VERTICAL only
#define SSD1306_CMD_SET_COL_ADDR                0x21
#define SSD1306_COL_ADDR(addr)                  (0x7F & addr)

// for MEM_ADDR_MODE_HORIZONTAL/VERTICAL only
#define SSD1306_CMD_SET_PAGE_ADDR               0x22
#define SSD1306_PAGE_ADDR(addr)                 (0x07 & addr)

#define SSD1306_CMD_SET_PAGE_ADDR_PAGE(addr)    (0xB0 | (0x07 & addr) )


//////////////////////////////////////////////////
// 4. Hardware Configuration (Panel resolution & layout related) Commands:
//////////////////////////////////////////////////
//
#define SSD1306_CMD_SET_START_LINE(line)        (0x40 | (0x3F & line) )
#define SSD1306_DEFAULT_START_LINE              0x00
//
#define SSD1306_CMD_SET_SEGMENT_REMAP_LEFT      0xA0
#define SSD1306_CMD_SET_SEGMENT_REMAP_RIGHT     0xA1
//leave
#define SSD1306_CMD_SET_MULTIPLEX_RATIO         0xA8
#define SSD1306_MULTIPLEX_RATIO(ratio)          (0x3F & ratio)
#define SSD1306_DEFAULT_MULTIPLEX_RATIO         0x3F
//
#define SSD1306_CMD_SET_COM_SCAN_FROM_0         0xC0
#define SSD1306_CMD_SET_COM_SCAN_FROM_N         0xC8
//
#define SSD1306_CMD_SET_DISPLAY_OFFSET          0xD3
#define SSD1306_DISPLAY_OFFSET(offset)          (0x3F & offset)
#define SSD1306_DEFAULT_DISPLAY_OFFSET          0x00
//leave
#define SSD1306_CMD_SET_COM_PINS                0xDA
#define SSD1306_COM_PINS_A_SEQUENTIAL           0x02
#define SSD1306_COM_PINS_A_ALTERNATIVE          0x12
#define SSD1306_COM_PINS_B_REMAP_OFF            0x02
#define SSD1306_COM_PINS_B_REMAP_ON             0x22


//////////////////////////////////////////////////
// 5. Timing & Driving Scheme Setting Commands:
//////////////////////////////////////////////////

#define SSD1306_CMD_SET_DISPLAY_CLOCK_DIV       0xD5
#define SSD1306_DEFAULT_DISPLAY_CLOCK_DIV       0x80

#define SSD1306_CMD_SET_PRECHARGE_PERIOD        0xD9

#define SSD1306_CMD_SET_V_COM_DESELECT          0xDB
#define SSD1306_V_COM_DESELECT(level)           (0x70 & (level << 4) )
#define SSD1306_V_COM_DESELECT_0_65_X           0x00
#define SSD1306_V_COM_DESELECT_0_77_X           0x20
#define SSD1306_V_COM_DESELECT_0_83_X           0x30
#define SSD1306_V_COM_DESELECT_0_92_X           0x40

#define SSD1306_CMD_NOP                         0xE3


//////////////////////////////////////////////////
// 6. Charge Pump Command:
//////////////////////////////////////////////////
//
#define SSD1306_CMD_CHARGEPUMP                  0x8D
#define SSD1306_CHARGEPUMP_OFF                  0x10
#define SSD1306_CHARGEPUMP_ON                   0x14


//////////////////////////////////////////////////
// SSD1306 Return codes:
//////////////////////////////////////////////////

#define SSD1306_OK                              0

#define SSD1306_ERR_DEV_NOT_FOUND               -1
#define SSD1306_ERR_DEV_NOT_INIT                -2
#define SSD1306_ERR_CONN_FAIL                   -3
#define SSD1306_ERR_WRITE_FAIL                  -4


//////////////////////////////////////////////////
// ?:
//////////////////////////////////////////////////

#define SSD1306_CMD_SETLOWCOLUMN                0x00
#define SSD1306_CMD_SETHIGHCOLUMN               0x10
#define SSD1306_CMD_SWITCHCAPVCC                0x2

#define SSD1306_WIDTH 128
#define SSD1306_HEIGHT 64
#define SSD1306_BUFFERSIZE (SSD1306_WIDTH*SSD1306_HEIGHT)/8


//////////////////////////////////////////////////
// Driver:
//////////////////////////////////////////////////

class SSD1306driver{
	bool device_ok;
	uint8_t dev_addr;

	//uint8_t cmd(const uint8_t data);
	uint8_t cmd(const uint8_t *data, const uint8_t data_len);

	uint8_t init();
public:
	uint8_t cmd(const uint8_t data);
	SSD1306driver(uint8_t dev_addr = SSD1306_DEFAULT_ADDRESS);
	bool deviceOK() const { return device_ok; }

// Display Settings:
	uint8_t setRefreshRate(const uint8_t rate); // 0x00:0xFF
	uint8_t setVerticalOffset(const uint8_t offset); // 0x00:0x3F
	uint8_t setStartLine(const uint8_t line); // 0x00:0x3F
	uint8_t setHorizontalScan(const bool right); // 0=left, 1=right
	uint8_t setVerticalScan(const bool bottom); // 0=top, 1=bottom
	uint8_t setMode(const bool inverse); // 0=normal, 1=inverse

	enum MemoryMode 
	{
		HORIZONTAL = SSD1306_MEM_ADDR_MODE_HORIZONTAL,
		VERTICAL = SSD1306_MEM_ADDR_MODE_VERTICAL,
		PAGE = SSD1306_MEM_ADDR_MODE_PAGE
	};
	uint8_t setMemoryMode(const MemoryMode mode);

// Display Brightness Settings:
	uint8_t setContrast(const uint8_t contrast); // 0x00:0xFF
	uint8_t setPreChargePeriod(const uint8_t period); // 0x00:0xFF
	uint8_t setVCOMHdeselectLevel(const uint8_t level); // 0x00:0x07



	void sendFramebuffer(uint8_t *buffer);
	void invert(uint8_t inverted);
private:
	void sendCommand(uint8_t command);
	void sendData(uint8_t data);
};

/*
#define SSD1306_DEFAULT_ADDRESS 0x3C
#define SSD1306_SETCONTRAST 0x81
#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_DISPLAYALLON 0xA5
#define SSD1306_NORMALDISPLAY 0xA6
#define SSD1306_INVERTDISPLAY 0xA7
#define SSD1306_DISPLAYON 0xAF
#define SSD1306_SETDISPLAYOFFSET 0xD3
#define SSD1306_SETCOMPINS 0xDA
#define SSD1306_SETVCOMDETECT 0xDB
#define SSD1306_SETDISPLAYCLOCKDIV 0xD5
#define SSD1306_SETPRECHARGE 0xD9
#define SSD1306_SETMULTIPLEX 0xA8
#define SSD1306_SETLOWCOLUMN 0x00
#define SSD1306_SETHIGHCOLUMN 0x10
#define SSD1306_SETSTARTLINE 0x40
#define SSD1306_MEMORYMODE 0x20
#define SSD1306_COLUMNADDR 0x21
#define SSD1306_PAGEADDR   0x22
#define SSD1306_COMSCANINC 0xC0
#define SSD1306_COMSCANDEC 0xC8
#define SSD1306_SEGREMAP 0xA0
#define SSD1306_CHARGEPUMP 0x8D
#define SSD1306_SWITCHCAPVCC 0x2
#define SSD1306_NOP 0xE3

#define SSD1306_WIDTH 128
#define SSD1306_HEIGHT 64
#define SSD1306_BUFFERSIZE (SSD1306_WIDTH*SSD1306_HEIGHT)/8
*/

#endif // SSD1306 