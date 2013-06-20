
extern void PUT32 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );
extern void dummy ( unsigned int );

#define DELAY_COUNT 100000

void backlight_on ( void )
{
    PUT32(0x6000d024+0x800,0x0808);
}
void backlight_off ( void )
{
    PUT32(0x6000d024+0x800,0x0800);
}

int main ( void )
{
    unsigned int ra;
    unsigned int rb;

    for(rb=0;rb<5;rb++)
    {
        backlight_on();
        for(ra=0;ra<DELAY_COUNT;ra++) dummy(ra);
        backlight_off();
        for(ra=0;ra<DELAY_COUNT;ra++) dummy(ra);
    }
    return(0);
}
















