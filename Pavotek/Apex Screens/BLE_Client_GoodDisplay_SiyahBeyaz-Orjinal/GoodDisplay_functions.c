
#include <types.h>          /* Commonly used type definitions */



#include "platform_config.h"
#include "gooddisplay_functions.h"


void delay_ms(uint16 nTime) {
    
    uint32   _nTime = (uint32) nTime * 1000;

    if ( _nTime > 65535 ){ // TimeDelayUSec uint16 aliyor
        
        uint32 timeCount = _nTime / 10000;
        
        while(timeCount--)
            TimeDelayUSec(10000);
        
    }else{
        
        TimeDelayUSec( (uint16) _nTime);
        
    }
    
    
}


void PinModeDir(){
    //PioSetDir( VPP_PIN, OUTPUT);
    PioSetDir( SPI_MASTER_PIO_SCLK, OUTPUT);
    PioSetDir( SPI_MASTER_PIO_MOSI, OUTPUT );
    PioSetDir( CS_PIN, OUTPUT );
    PioSetDir( DC_PIN, OUTPUT );
    //    PioSetDir( BS_PIN, OUTPUT );
    PioSetDir( RESET_PIN, INPUT );  //pull_up direnc var
    PioSetDir( BUSY_PIN, INPUT );         //All Pins 0

}

void EPD_W27_Init(){
    
    //    PioSet( BS_PIN, LOW );    //PANEL_ON# = 1
    //delay_ms(10);
    
    PioSetDir( RESET_PIN, OUTPUT );
    PioSet( RESET_PIN, LOW );    
    delay_ms(1000);

    PioSetDir( RESET_PIN, INPUT );  //pull_up direnc var
//    PioSet( RESET_PIN, HIGH );
    delay_ms(1000);
}    


void Display(){
    
        PinModeDir();
//    EPD_W27_Init(); 
    
    EPD_W27_Init(); 
     
    SPI4W_WRITECOM(0x01); 	
    SPI4W_WRITEDATA (0x03);	
    SPI4W_WRITEDATA (0x00);
    SPI4W_WRITEDATA (0x2b);
    SPI4W_WRITEDATA (0x2b);
    SPI4W_WRITEDATA (0x09);
    
    SPI4W_WRITECOM(0x06);   
    SPI4W_WRITEDATA (0x07);
    SPI4W_WRITEDATA (0x07);
    SPI4W_WRITEDATA (0x17); 
    
    SPI4W_WRITECOM(0xF8);   
    SPI4W_WRITEDATA (0x60);
    SPI4W_WRITEDATA (0xA5);
    
    SPI4W_WRITECOM(0xF8);   
    SPI4W_WRITEDATA (0x89);
    SPI4W_WRITEDATA (0xA5);
    
    SPI4W_WRITECOM(0xF8);   
    SPI4W_WRITEDATA (0x90);
    SPI4W_WRITEDATA (0x00);
    
    SPI4W_WRITECOM(0xF8);   
    SPI4W_WRITEDATA (0x93);
    SPI4W_WRITEDATA (0x2A);
    
    SPI4W_WRITECOM(0xF8);   
    SPI4W_WRITEDATA (0x73);
    SPI4W_WRITEDATA (0x41);
    
    SPI4W_WRITECOM(0x16);
    SPI4W_WRITEDATA(0x00);	
    
    SPI4W_WRITECOM(0x04);
    READBUSY();
    
    SPI4W_WRITECOM(0x00);
    SPI4W_WRITEDATA(0xaf);	
    
    SPI4W_WRITECOM(0x30);	
    SPI4W_WRITEDATA (0x3a); 
    
    SPI4W_WRITECOM(0x61);	
    SPI4W_WRITEDATA (0x00);
    SPI4W_WRITEDATA (0xb0); 
    SPI4W_WRITEDATA (0x01);	
    SPI4W_WRITEDATA (0x08);	
    
    SPI4W_WRITECOM(0x82);	
    SPI4W_WRITEDATA (0x12);	
    
    SPI4W_WRITECOM(0X50);	
    SPI4W_WRITEDATA(0x87);	
    lut();					
    pic_display();
    SPI4W_WRITECOM(0x12); 
    READBUSY();
    

    SPI4W_WRITECOM(0X07);   //power off
    SPI4W_WRITEDATA (0xa5);
    
    PioSet( SPI_MASTER_PIO_MOSI, LOW );      //CS Low
    PioSet( SPI_MASTER_PIO_SCLK, LOW );

}



void SPI4W_WRITECOM(char INIT_COM)
{
    unsigned char TEMPCOM;
    unsigned char scnt;
    TEMPCOM=INIT_COM;
    
    PioSet( CS_PIN, HIGH );      //CS Low
    PioSet( CS_PIN, LOW );      //CS Low
    PioSet( SPI_MASTER_PIO_SCLK, LOW );      //CS Low
    PioSet( DC_PIN, LOW );      //DC Low
    
    
    
    for(scnt=0;scnt<8;scnt++)
    {
        if(TEMPCOM&0x80)
            PioSet( SPI_MASTER_PIO_MOSI, HIGH );      //CS Low
        else
            PioSet( SPI_MASTER_PIO_MOSI, LOW );      //CS Low
        //delay_ms(10);
        PioSet( SPI_MASTER_PIO_SCLK, HIGH );      //CS Low
        //delay_ms(10);
        PioSet( SPI_MASTER_PIO_SCLK, LOW );      //CS Low
        TEMPCOM=TEMPCOM<<1;
        //delay_ms(10);
    }
    PioSet( CS_PIN, HIGH );      //CS Low
}

void READBUSY(void)
{
    while(1)
    {	 //=0 BUSY
        if(PioGet( BUSY_PIN )==1) break;
    } 
    
    delay_ms(2);
}
void lut(void)
{
    unsigned int count;	 
    {
        SPI4W_WRITECOM(0x20);							//vcom
        for(count=0;count<44;count++)	     
        {SPI4W_WRITEDATA(lut_vcomDC[count]);}
        
        SPI4W_WRITECOM(0x21);							//ww --
        for(count=0;count<42;count++)	     
        {SPI4W_WRITEDATA(lut_ww[count]);}   
        
        SPI4W_WRITECOM(0x22);							//bw r
        for(count=0;count<42;count++)	     
        {SPI4W_WRITEDATA(lut_bw[count]);} 
        
        SPI4W_WRITECOM(0x23);							//wb w
        for(count=0;count<42;count++)	     
        {SPI4W_WRITEDATA(lut_bb[count]);} 
        
        SPI4W_WRITECOM(0x24);							//bb b
        for(count=0;count<42;count++)	     
        {SPI4W_WRITEDATA(lut_wb[count]);} 
    }	         
}
void pic_display(void)
{
    
    SPI4W_WRITECOM(0x10);	  	 
    //delay_ms(2);
    Ultrachip0();
    SPI4W_WRITECOM(0x13);
    Ultrachip1();		
    //delay_ms(2);	
}

void Ultrachip0(void)
{
    unsigned int i;
    for(i=0;i<5808;i++)	     
    {
        SPI4W_WRITEDATA(final[i]);  
    }  
    //delay_ms(2);	              
}

void Ultrachip1(void)
{
    unsigned int i;
    for(i=5808;i<11616;i++)	     
    {
        SPI4W_WRITEDATA(final[i]);  //renkli kýsým
    }  
    //delay_ms(2);	              
}


void SPI4W_WRITEDATA(char INIT_DATA)
{
    unsigned char TEMPCOM;
    unsigned char scnt;
    TEMPCOM=INIT_DATA;
    
    PioSet( CS_PIN, HIGH );      //CS Low
    PioSet( CS_PIN, LOW );      //CS Low
    PioSet( SPI_MASTER_PIO_SCLK, LOW );      //CS Low
    PioSet( DC_PIN, HIGH );      //DC Low
    
    
    
    for(scnt=0;scnt<8;scnt++)
    {
        if(TEMPCOM&0x80)
            PioSet( SPI_MASTER_PIO_MOSI, HIGH );      //CS Low
        else
            PioSet( SPI_MASTER_PIO_MOSI, LOW );      //CS Low
        //delay_ms(10);
        PioSet( SPI_MASTER_PIO_SCLK, HIGH );      //CS Low
        //delay_ms(10);
        PioSet( SPI_MASTER_PIO_SCLK, LOW );      //CS Low
        TEMPCOM=TEMPCOM<<1;
        //delay_ms(10);
    }
    PioSet( CS_PIN, HIGH );      //CS Low
}