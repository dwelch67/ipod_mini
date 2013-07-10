/*
 * iPod driver based on code from the ipodlinux project - http://ipodlinux.org
 * Modified by dwelch in 2013
 * Original file: linux/arch/armnommu/mach-ipod/keyboard.c
 * Copyright (c) 2003-2005 Bernard Leach (leachbj@bouncycastle.org)
 */

extern void PUT32 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );
extern void PUT8 ( unsigned int, unsigned int );
extern unsigned int GET8 ( unsigned int );
extern void dummy ( unsigned int );

void init_lcd(void);
void hexstring ( unsigned int );
int wait_usec ( int usecs );

#define IPOD_I2C_CTRL   (0x7000c000+0x00)
#define IPOD_I2C_ADDR   (0x7000c000+0x04)
#define IPOD_I2C_DATA0  (0x7000c000+0x0c)
#define IPOD_I2C_DATA1  (0x7000c000+0x10)
#define IPOD_I2C_DATA2  (0x7000c000+0x14)
#define IPOD_I2C_DATA3  (0x7000c000+0x18)
#define IPOD_I2C_STATUS (0x7000c000+0x1c)

/* IPOD_I2C_CTRL bit definitions */
#define IPOD_I2C_SEND   0x80

/* IPOD_I2C_STATUS bit definitions */
#define IPOD_I2C_BUSY   (1<<6)
void outl ( unsigned int d, unsigned int a)
{
    PUT32(a,d);
}
void outb ( unsigned int d, unsigned int a)
{
    PUT8(a,d);
}
void udelay ( int usecs )
{
    wait_usec(usecs);
}


void ipod_i2c_wait_not_busy(void)
{
    while (1)
    {
        if (!(GET8(IPOD_I2C_STATUS) & IPOD_I2C_BUSY))
        {
            break;
        }
    }
}



int ipod_i2c_read_byte(unsigned int addr, unsigned int *data)
{
    ipod_i2c_wait_not_busy();

    // clear top 15 bits, left shift 1, or in 0x1 for a read
    outb(((addr << 17) >> 16) | 0x1, IPOD_I2C_ADDR);

    outb(GET8(IPOD_I2C_CTRL) | 0x20, IPOD_I2C_CTRL);

    outb(GET8(IPOD_I2C_CTRL) | IPOD_I2C_SEND, IPOD_I2C_CTRL);

    ipod_i2c_wait_not_busy();

    if (data) {
        *data = GET8(IPOD_I2C_DATA0);
    }

    return 0;
}

int ipod_i2c_send_bytes(unsigned int addr, unsigned int len, unsigned char *data)
{
    int data_addr;
    int i;

    //if (len < 1 || len > 4) {
        //return -EINVAL;
    //}

    ipod_i2c_wait_not_busy();

    // clear top 15 bits, left shift 1
    outb((addr << 17) >> 16, IPOD_I2C_ADDR);

    outb(GET8(IPOD_I2C_CTRL) & ~0x20, IPOD_I2C_CTRL);

    data_addr = IPOD_I2C_DATA0;
    for ( i = 0; i < len; i++ ) {
        outb(*data++, data_addr);

        data_addr += 4;
    }

    outb((GET8(IPOD_I2C_CTRL) & ~0x26) | ((len-1) << 1), IPOD_I2C_CTRL);

    outb(GET8(IPOD_I2C_CTRL) | IPOD_I2C_SEND, IPOD_I2C_CTRL);

    return 0x0;
}


int ipod_i2c_send_byte(unsigned int addr, unsigned int data0)
{
    unsigned char data[1];

    data[0] = data0;

    return ipod_i2c_send_bytes(addr, 1, data);
}


int i2c_readbyte(unsigned int dev_addr, unsigned int addr)
{
    unsigned int data;

    ipod_i2c_send_byte(dev_addr, addr);
    ipod_i2c_read_byte(dev_addr, &data);

    return data;
}

void ipod_i2c_init(void)
{
    //ipod_i2c_base = 0x7000c000;

    outl(GET32(0x6000600c) | 0x1000, 0x6000600c); /* enable 12 */

    outl(GET32(0x60006004) | 0x1000, 0x60006004); /* start reset 12 */
    outl(GET32(0x60006004) & ~0x1000, 0x60006004);    /* end reset 12 */

    outl(0x0, 0x600060a4);
    outl(0x80 | (0 << 8), 0x600060a4);

    i2c_readbyte(0x8, 0);
}

static void opto_i2c_init(void)
{
    int i, curr_value;

    /* wait for value to settle */
    i = 1000;
    curr_value = (GET32(0x7000c104) << 16) >> 24;
    while (i > 0)
    {
        int new_value = (GET32(0x7000c104) << 16) >> 24;

        if (new_value != curr_value) {
            i = 10000;
            curr_value = new_value;
        }
        else {
            i--;
        }
    }

    outl(GET32(0x6000d024) | 0x10, 0x6000d024);   /* port B bit 4 = 1 */

    outl(GET32(0x6000600c) | 0x10000, 0x6000600c);    /* dev enable */
    outl(GET32(0x60006004) | 0x10000, 0x60006004);    /* dev reset */
    udelay(5);
    outl(GET32(0x60006004) & ~0x10000, 0x60006004);   /* dev reset finish */

    outl(0xffffffff, 0x7000c120);
    outl(0xffffffff, 0x7000c124);
    outl(0xc00a1f00, 0x7000c100);
    outl(0x1000000, 0x7000c104);
}

void button_init_device(void)
{
    ipod_i2c_init();
    opto_i2c_init();

    /* hold button - enable as input */
    outl(GET32(0x6000d000) | 0x20, 0x6000d000);
    outl(GET32(0x6000d010) & ~0x20, 0x6000d010);

    /* hold button - set interrupt levels */
    outl(~(GET32(0x6000d030) & 0x20), 0x6000d060);
    //outl((GET32(0x6000d040) & 0x20), 0x6000d070);

    /* enable interrupts */
    //outl(0x20, 0x6000d050);

    //misc_register (&ikb_misc);
}
unsigned int read_wheel ( void )
{
    unsigned int wheel_value;
    unsigned int reg;

    wheel_value=0;
    udelay(250);
    reg = 0x7000c104;
    if ((GET32(0x7000c104) & 0x04000000) != 0)
    {
        reg = reg + 0x3C;   /* 0x7000c140 */
        wheel_value=GET32(0x7000c140);
    }
    if ((GET32(reg) & 0x8000000) != 0)
    {
        outl(0xffffffff, 0x7000c120);
        outl(0xffffffff, 0x7000c124);
    }
    outl(GET32(0x7000c104) | 0xC000000, 0x7000c104);
    outl(0x400a1f00, 0x7000c100);
    outl(GET32(0x6000d024) | 0x10, 0x6000d024);
    return(wheel_value);
}

int main ( void )
{
    unsigned int ra;

    init_lcd();
    button_init_device();
    while(1)
    {
        ra=read_wheel();
        if(ra) hexstring(ra);
    }
}


