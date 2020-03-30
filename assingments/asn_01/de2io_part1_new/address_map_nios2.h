/*******************************************************************************
 * This file provides address values that exist in the DE2-115 Computer
 ******************************************************************************/

#ifndef __SYSTEM_INFO__
#define __SYSTEM_INFO__

#define BOARD				"DE2-115"

/* Memory */
#define SDRAM_BASE			0x00000000
#define SDRAM_END			0x07ffffff
#define FPGA_SRAM_BASE			0x08000000
#define FPGA_SRAM_END			0x081FFFFF
#define FPGA_PIXEL_BUF_BASE		0x08000000 // for compatibility
#define FPGA_PIXEL_BUF_END		0x081FFFFF // for compitibility
#define FPGA_CHAR_BASE			0x09000000
#define FPGA_CHAR_END			0x09001FFF
#define FPGA_SD_CARD_BASE		0x0B000000
#define FPGA_SD_CARD_END		0x0B0003FF
#define FPGA_FLASH_DATA_BASE		0x0C000000
#define FPGA_FLASH_DATA_END		0x0C7FFFFF
#define FPGA_FLASH_ERASE_BASE		0x0BFF0000
#define FPGA_FLASH_ERASE_END		0x0BFF0003
	
/* Devices */
#define LED_BASE			0xFF200000
#define LEDR_BASE			0xFF200000
#define LEDG_BASE			0xFF200010
#define HEX3_HEX0_BASE			0xFF200020
#define HEX7_HEX4_BASE			0xFF200030
#define HEX5_HEX4_BASE			0xFF200030 // for compatibility
#define SW_BASE				0xFF200040
#define KEY_BASE			0xFF200050
#define JP5_BASE			0xFF200060
#define PS2_BASE			0xFF200100
#define PS2_DUAL_BASE			0xFF200108
#define USB_BASE			0xFF200110
#define JTAG_UART_BASE			0xFF201000
#define SERIAL_BASE			0xFF201010
#define IrDA_BASE			0xFF201020
#define TIMER_BASE			0xFF202000
#define TIMER_2_BASE			0xFF202020
#define AV_CONFIG_BASE			0xFF203000
#define RGB_RESAMPLER_BASE    		0xFF203010
#define PIXEL_BUF_CTRL_BASE		0xFF203020
#define CHAR_BUF_CTRL_BASE		0xFF203030
#define AUDIO_BASE			0xFF203040
#define CHAR_LCD_BASE			0xFF203050
#define VIDEO_IN_BASE			0xFF203060
#define EDGE_DETECT_CTRL_BASE		0xFF203070
#define ADC_BASE			0xFF204000

#endif
