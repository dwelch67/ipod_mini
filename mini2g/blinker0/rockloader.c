
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *fpin;
FILE *fpout;

char mn2g[]=
{
    0x00,0x00,0x00,0x00,0x6d,0x6e,0x32,0x67
};

unsigned int rb;
unsigned int rc;
unsigned char data[4096];
unsigned int checksum;

int main ( int argc, char *argv[] )
{
    if(argc<3)
    {
        printf("rockloader input output\n");
        return(1);
    }
    fpin=fopen(argv[1],"rb");
    if(fpin==NULL)
    {
        printf("Error opening file [%s]\n",argv[1]);
        return(1);
    }
    fpout=fopen(argv[2],"wb");
    if(fpout==NULL)
    {
        printf("Error creating file [%s]\n",argv[2]);
        return(1);
    }
    checksum=0;
    while(1)
    {
        rb=fread(data,1,sizeof(data),fpin);
        if(rb==0) break;
        for(rc=0;rc<rb;rc++) checksum+=data[rc];
    }
    checksum+=11; //mini2g
    printf("0x%04X\n",checksum);
    fseek(fpin,0,0);
    mn2g[0]=(checksum>>24)&0xFF;
    mn2g[1]=(checksum>>16)&0xFF;
    mn2g[2]=(checksum>> 8)&0xFF;
    mn2g[3]=(checksum>> 0)&0xFF;
    fwrite(mn2g,1,sizeof(mn2g),fpout);
    while(1)
    {
        rb=fread(data,1,sizeof(data),fpin);
        if(rb==0) break;
        fwrite(data,1,rb,fpout);
    }
    fclose(fpout);
    fclose(fpin);
    return(0);
}



