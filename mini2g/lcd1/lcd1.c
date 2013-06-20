
extern void PUT32 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );
extern void dummy ( unsigned int );

void init_lcd(void);
void hexstring ( unsigned int );

int main ( void )
{
    init_lcd();
    hexstring(0x12345678);
    hexstring(0xABCDEF90);
    return(0);
}




