
//------------------------------------------------------------------
//
// Based on code from the ipodlinux project - http://ipodlinux.org/
// And RockBox http://www.rockbox.org
//
// Original file: linux/arch/armnommu/mach-ipod/fb.c
// rockbox file : firmware/target/arm/ipod/lcd-gray.c
//
// Copyright (c) 2003-2005 Bernard Leach (leachbj@bouncycastle.org)
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
// KIND, either express or implied.
//
//------------------------------------------------------------------

extern void PUT32 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );

#include "hexfont.h"

void display_image ( void );

unsigned char textdisplay[16][21];

void backlight_on ( void )
{
    PUT32(0x6000d824,0x0808);
}
void backlight_off ( void )
{
    PUT32(0x6000d824,0x0800);
}
int timer_get_current(void)
{
    return GET32(0x60005010);
}
int timer_check(int clock_start, int usecs)
{
    unsigned int clock;
    clock = GET32(0x60005010);

    if ( (clock - clock_start) >= usecs ) {
        return 1;
    } else {
        return 0;
    }
}
int wait_usec ( int usecs )
{
    int start = timer_get_current();
    while ( timer_check(start, usecs) == 0 )
    {
    }
    return 0;
}
void lcd_wait_write(void)
{
    if ((GET32(0x70003000) & 0x8000) != 0) {
        int start = timer_get_current();

        do {
            if ((GET32(0x70003000) & 0x8000) == 0) break;
        } while (timer_check(start, 1000) == 0);
    }
}
void lcd_prepare_cmd(int cmd)
{
    lcd_wait_write();
    PUT32(0x70003000,(GET32(0x70003000) & ~0x1f00000) | 0x1700000);
    PUT32(0x70003008, cmd | 0x740000);
}
void lcd_send_data(int data_lo, int data_hi)
{
    lcd_wait_write();
    PUT32(0x70003000,(GET32(0x70003000) & ~0x1f00000) | 0x1700000);
    PUT32(0x70003008,data_hi | (data_lo << 8) | 0x760000);
}
void lcd_cmd_and_data(int cmd, int data_lo, int data_hi)
{
    lcd_prepare_cmd(cmd);
    lcd_send_data(data_lo, data_hi);
}
void init_lcd(void)
{
    unsigned int ra,rb;

    for(rb=0;rb<16;rb++) for(ra=0;ra<21;ra++) textdisplay[rb][ra]=16;
    lcd_cmd_and_data(0x1, 0x0, 0xd);
    lcd_cmd_and_data(0x5, 0x0, 0x10);
    backlight_on();
    display_image();
}
void display_image ( void )
{
    unsigned int py,y;
    unsigned int ty,tx;

    for(py=0;py<128;py++)
    {
        ty=py>>3;
        y=py&7;
        lcd_cmd_and_data(0x11,(py<<5)>>8,(py<<5)&0xff);
        lcd_prepare_cmd(0x12);
        for(tx=0;tx<20;tx++)
        {
            lcd_send_data(hexfont[textdisplay[ty][tx]][y][0],hexfont[textdisplay[ty][tx]][y][1]);
        }
    }
    //wait_usec(15);
}
void hexstring ( unsigned int d )
{
    unsigned int ra;
    unsigned int rb;
    for(rb=1;rb<16;rb++) for(ra=0;ra<21;ra++) textdisplay[rb-1][ra]=textdisplay[rb][ra];
    for(rb=0;rb<8;rb++)
    {
        textdisplay[12][rb]=(d>>28)&0xF;
        d<<=4;
    }
    display_image();
}
