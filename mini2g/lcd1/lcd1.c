
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
    //hexstring(GET32(0x70000000)); //pp5022
    if(((GET32(0x70000000)>>24)&0xFF)==0x32)
    {
        //pp5022
        hexstring(GET32(0x60000000)); //pp5022
        hexstring(GET32(0x4001ff18)); //pp5022
        hexstring(GET32(0x4001ff1c)); //pp5022
    }
    else
    {
        //pp5002
        hexstring(GET32(0xc4000000)); //pp5002
        hexstring(GET32(0x40017f18)); //pp5002
        hexstring(GET32(0x40017f1c)); //pp5002
    }
    return(0);
}




