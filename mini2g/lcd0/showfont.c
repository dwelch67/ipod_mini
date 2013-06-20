
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ifont.h"

int main ( void )
{
    unsigned int ra,rb,rc,rd,re;

    for(ra=0;ra<256;ra++)
    {
        printf("+----- 0x%02X -----+\n",ra);
        for(rb=0;rb<8;rb++)
        {
            printf("|");
            rd=ifont[ra][rb][0];
            rd<<=8;
            rd|=ifont[ra][rb][1];
            //for(re=0x80;re;re>>=1) if(rd&re) printf("#"); else printf(" ");
            for(re=0x01;re<=0x8000;re<<=1) if(rd&re) printf("#"); else printf(" ");
            printf("|\n");
        }
        printf("+----- 0x%02X -----+\n",ra);
    }
    return(0);
}

