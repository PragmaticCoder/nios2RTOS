/*
 * alt_sys_init.c - HAL initialization source
 *
 * Machine generated for CPU 'Nios2' in SOPC Builder design 'Computer_System'
 * SOPC Builder design path: ../../Computer_System.sopcinfo
 *
 * Generated: Fri Feb 28 12:46:01 CST 2020
 */

/*
 * DO NOT MODIFY THIS FILE
 *
 * Changing this file will have subtle consequences
 * which will almost certainly lead to a nonfunctioning
 * system. If you do modify this file, be aware that your
 * changes will be overwritten and lost when this file
 * is generated again.
 *
 * DO NOT MODIFY THIS FILE
 */

/*
 * License Agreement
 *
 * Copyright (c) 2008
 * Altera Corporation, San Jose, California, USA.
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * This agreement shall be governed in all respects by the laws of the State
 * of California and by the laws of the United States of America.
 */

#include "system.h"
#include "sys/alt_irq.h"
#include "sys/alt_sys_init.h"

#include <stddef.h>

/*
 * Device headers
 */

#include "altera_nios2_gen2_irq.h"
#include "Altera_UP_SD_Card_Avalon_Interface.h"
#include "altera_avalon_jtag_uart.h"
#include "altera_avalon_sysid_qsys.h"
#include "altera_avalon_timer.h"
#include "altera_up_avalon_audio.h"
#include "altera_up_avalon_audio_and_video_config.h"
#include "altera_up_avalon_character_lcd.h"
#include "altera_up_avalon_irda.h"
#include "altera_up_avalon_ps2.h"
#include "altera_up_avalon_rs232.h"
#include "altera_up_avalon_usb.h"
#include "altera_up_avalon_video_dma_controller.h"
#include "altera_up_avalon_video_rgb_resampler.h"

/*
 * Allocate the device storage
 */

ALTERA_NIOS2_GEN2_IRQ_INSTANCE ( NIOS2, Nios2);
ALTERA_AVALON_JTAG_UART_INSTANCE ( JTAG_UART, JTAG_UART);
ALTERA_AVALON_SYSID_QSYS_INSTANCE ( SYSID, SysID);
ALTERA_AVALON_TIMER_INSTANCE ( INTERVAL_TIMER, Interval_Timer);
ALTERA_AVALON_TIMER_INSTANCE ( INTERVAL_TIMER_2, Interval_Timer_2);
ALTERA_UP_AVALON_AUDIO_AND_VIDEO_CONFIG_INSTANCE ( AV_CONFIG, AV_Config);
ALTERA_UP_AVALON_AUDIO_INSTANCE ( AUDIO_SUBSYSTEM_AUDIO, Audio_Subsystem_Audio);
ALTERA_UP_AVALON_CHARACTER_LCD_INSTANCE ( CHAR_LCD_16X2, Char_LCD_16x2);
ALTERA_UP_AVALON_IRDA_INSTANCE ( IRDA, IrDA);
ALTERA_UP_AVALON_PS2_INSTANCE ( PS2_PORT, PS2_Port);
ALTERA_UP_AVALON_PS2_INSTANCE ( PS2_PORT_DUAL, PS2_Port_Dual);
ALTERA_UP_AVALON_RS232_INSTANCE ( SERIAL_PORT, Serial_Port);
ALTERA_UP_AVALON_USB_INSTANCE ( USB, USB);
ALTERA_UP_AVALON_VIDEO_DMA_CONTROLLER_INSTANCE ( VGA_SUBSYSTEM_CHAR_BUF_SUBSYSTEM_CHAR_BUF_DMA, VGA_Subsystem_Char_Buf_Subsystem_Char_Buf_DMA);
ALTERA_UP_AVALON_VIDEO_DMA_CONTROLLER_INSTANCE ( VGA_SUBSYSTEM_VGA_PIXEL_DMA, VGA_Subsystem_VGA_Pixel_DMA);
ALTERA_UP_AVALON_VIDEO_DMA_CONTROLLER_INSTANCE ( VIDEO_IN_SUBSYSTEM_VIDEO_IN_DMA, Video_In_Subsystem_Video_In_DMA);
ALTERA_UP_AVALON_VIDEO_RGB_RESAMPLER_INSTANCE ( VGA_SUBSYSTEM_VGA_PIXEL_RGB_RESAMPLER, VGA_Subsystem_VGA_Pixel_RGB_Resampler);
ALTERA_UP_SD_CARD_AVALON_INTERFACE_INSTANCE ( SD_CARD, SD_Card);

/*
 * Initialize the interrupt controller devices
 * and then enable interrupts in the CPU.
 * Called before alt_sys_init().
 * The "base" parameter is ignored and only
 * present for backwards-compatibility.
 */

void alt_irq_init ( const void* base )
{
    ALTERA_NIOS2_GEN2_IRQ_INIT ( NIOS2, Nios2);
    alt_irq_cpu_enable_interrupts();
}

/*
 * Initialize the non-interrupt controller devices.
 * Called after alt_irq_init().
 */

void alt_sys_init( void )
{
    ALTERA_AVALON_TIMER_INIT ( INTERVAL_TIMER, Interval_Timer);
    ALTERA_AVALON_TIMER_INIT ( INTERVAL_TIMER_2, Interval_Timer_2);
    ALTERA_AVALON_JTAG_UART_INIT ( JTAG_UART, JTAG_UART);
    ALTERA_AVALON_SYSID_QSYS_INIT ( SYSID, SysID);
    ALTERA_UP_AVALON_AUDIO_AND_VIDEO_CONFIG_INIT ( AV_CONFIG, AV_Config);
    ALTERA_UP_AVALON_AUDIO_INIT ( AUDIO_SUBSYSTEM_AUDIO, Audio_Subsystem_Audio);
    ALTERA_UP_AVALON_CHARACTER_LCD_INIT ( CHAR_LCD_16X2, Char_LCD_16x2);
    ALTERA_UP_AVALON_IRDA_INIT ( IRDA, IrDA);
    ALTERA_UP_AVALON_PS2_INIT ( PS2_PORT, PS2_Port);
    ALTERA_UP_AVALON_PS2_INIT ( PS2_PORT_DUAL, PS2_Port_Dual);
    ALTERA_UP_AVALON_RS232_INIT ( SERIAL_PORT, Serial_Port);
    ALTERA_UP_AVALON_USB_INIT ( USB, USB);
    ALTERA_UP_AVALON_VIDEO_DMA_CONTROLLER_INIT ( VGA_SUBSYSTEM_CHAR_BUF_SUBSYSTEM_CHAR_BUF_DMA, VGA_Subsystem_Char_Buf_Subsystem_Char_Buf_DMA);
    ALTERA_UP_AVALON_VIDEO_DMA_CONTROLLER_INIT ( VGA_SUBSYSTEM_VGA_PIXEL_DMA, VGA_Subsystem_VGA_Pixel_DMA);
    ALTERA_UP_AVALON_VIDEO_DMA_CONTROLLER_INIT ( VIDEO_IN_SUBSYSTEM_VIDEO_IN_DMA, Video_In_Subsystem_Video_In_DMA);
    ALTERA_UP_AVALON_VIDEO_RGB_RESAMPLER_INIT ( VGA_SUBSYSTEM_VGA_PIXEL_RGB_RESAMPLER, VGA_Subsystem_VGA_Pixel_RGB_Resampler);
    ALTERA_UP_SD_CARD_AVALON_INTERFACE_INIT ( SD_CARD, SD_Card);
}
