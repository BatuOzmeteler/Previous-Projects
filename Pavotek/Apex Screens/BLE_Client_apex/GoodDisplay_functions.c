#include <types.h>          /* Commonly used type definitions */
#include "platform_config.h"
#include "gooddisplay_functions.h"

#define APEX4_2
//#define APEX2_9


unsigned int tempcode;
unsigned char tempvalue;
unsigned char temp1,temp2;


#ifdef APEX2_9
	uint8 height = 128;
	uint8 width = 296;
#endif
#ifdef APEX4_2
	uint8 height = 300;
	uint8 width = 400;
#endif

void delay_ms(uint16 nTime) 
{
    
    uint32   _nTime = (uint32) nTime * 1000;

    if ( _nTime > 65535 ){ // TimeDelayUSec uint16 aliyor
        
        uint32 timeCount = _nTime / 10000;
        
        while(timeCount--)
            TimeDelayUSec(10000);
        
    }else{
        
        TimeDelayUSec( (uint16) _nTime);
        
    }

}

void PinModeDir()
{

    //PioSetDir( VPP_PIN, OUTPUT);
    PioSetDir( SPI_MASTER_PIO_SCLK, OUTPUT);
    PioSetDir( SPI_MASTER_PIO_MOSI, OUTPUT );
    PioSetDir( CS_PIN, OUTPUT );
    PioSetDir( DC_PIN, OUTPUT );
    
    //PioSet( CS_PIN, HIGH );
    
    PioSetDir( RESET_PIN, OUTPUT );
    PioSetDir( BUSY_PIN, INPUT );

}

void Reset()
{

    PioSet( RESET_PIN, LOW );
    delay_ms(1);
    PioSet( RESET_PIN, HIGH );
    delay_ms(1);

}

void READBUSY(void)
{
    
    while(1)
    {    //=1 BUSY
        if(PioGet( BUSY_PIN ) == 0 ) break;
        delay_ms(2);
    }
    //delay_ms(500);
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

unsigned char SPI4W_READDATA()
{
    PioSetDir( SPI_MASTER_PIO_MOSI, INPUT );
    unsigned char scnt,temp;
    temp=0; 
    PioSet( DC_PIN, HIGH );      //DC HIGH

    for(scnt=0;scnt<8;scnt++)
    {
        PioSet( SPI_MASTER_PIO_SCLK, LOW );      // SCLK LOW

        if(SPI_MASTER_PIO_MOSI)
            temp=(temp<<1)|0x01;
        else
            temp=temp<<1;

        PioSet( SPI_MASTER_PIO_SCLK, HIGH );      // SCLK HIGH
        PioSet( SPI_MASTER_PIO_SCLK, LOW );      // SCLK LOW 
    }

    PioSetDir( SPI_MASTER_PIO_MOSI, OUTPUT );
    tempvalue = temp;
    return temp;
}



void EPD_W27_Init()
{

    SPI4W_WRITECOM(0x74); //Set Analog Block Control
    SPI4W_WRITEDATA(0x54);
    
    SPI4W_WRITECOM(0x7E); // Set Digital Block Control
    SPI4W_WRITEDATA(0x3B);

    
    
    SPI4W_WRITECOM(0x2B);  // Reduce glitch under ACVCOM    
    SPI4W_WRITEDATA(0x04);           
    SPI4W_WRITEDATA(0x63);

    
    SPI4W_WRITECOM(0x0C);  // Soft start setting
    SPI4W_WRITEDATA(0x8B);           
    SPI4W_WRITEDATA(0x9C);
    SPI4W_WRITEDATA(0x96);
    SPI4W_WRITEDATA(0x0F);

    // Driver Output control
    SPI4W_WRITECOM(0x01);  // Set MUX as 296
    SPI4W_WRITEDATA(0x27);           
    SPI4W_WRITEDATA(0x01);
    SPI4W_WRITEDATA(0x00);


    SPI4W_WRITECOM(0x11);  // Data entry mode setting.
    // A[2] = 0b The address counter is updated in the X direction,
    // A[1:0] = 01b Y decrement, X increment
    SPI4W_WRITEDATA(0x01); 
    
    #ifdef APEX2_9
    	// Set RAM X - address Start / End position
	    SPI4W_WRITECOM(0x44); 
	    SPI4W_WRITEDATA(0x00); // RAM x address start at 0
	    SPI4W_WRITEDATA(0x0F); // RAM x address end at 0Fh(15+1)*8->128

	    // Set Ram Y- address Start / End position
	    SPI4W_WRITECOM(0x45); 
	    SPI4W_WRITEDATA(0x27);   // RAM y address start at 127h     
	    SPI4W_WRITEDATA(0x01);  
	    SPI4W_WRITEDATA(0x00); // RAM y address end at 00h     
	    SPI4W_WRITEDATA(0x00);
	#endif
    #ifdef APEX4_2
	    // Set RAM X - address Start / End position
	    SPI4W_WRITECOM(0x44); 
	    SPI4W_WRITEDATA(0x00); // RAM x address start at 0
	    SPI4W_WRITEDATA(0x31); // RAM x address end at 50-1 = 31h

	    // Set Ram Y- address Start / End position
	    SPI4W_WRITECOM(0x45); 
	    SPI4W_WRITEDATA(0x2b);   // RAM y address start at      
	    SPI4W_WRITEDATA(0x01);  
	    SPI4W_WRITEDATA(0x00); // RAM y address end at 00h     
	    SPI4W_WRITEDATA(0x00);
	#endif
    // Border Waveform Control
    SPI4W_WRITECOM(0x3C); // board
    SPI4W_WRITEDATA(0x01);      //LUT1 orj 01h

    // Temperature sensor control
    SPI4W_WRITECOM(0x18);
    SPI4W_WRITEDATA(0X80);


    // Display Update Control 2
    SPI4W_WRITECOM(0x22);
    SPI4W_WRITEDATA(0XB1);  //Load Temperature and waveform setting.

    // Master activation
    SPI4W_WRITECOM(0x20);
    
    READBUSY();

    
    // Sýcaklýðý 10'dan büyük alýp burayý pas geçebiliriz.
    SPI4W_WRITECOM(0x1B);
    temp1=SPI4W_READDATA(); 
    temp2=SPI4W_READDATA();
    
    //temp1=20;
    
    PioSet( CS_PIN, HIGH );      //CS HIGH  
    //nCS_H;  
         
         
		if ((temp1&0x80)!=0) temp1=0; 		//for negative degreeC calculation.
		 //  init code added for E4 R1.2 less than 10degC (0 to 10degC) application
	 
		if ( temp1<10)		// for less than 10 degC. 
		{
			SPI4W_WRITECOM(0x3D);		
			SPI4W_WRITEDATA(0x09);
			SPI4W_WRITEDATA(0x09);
			SPI4W_WRITECOM(0x3E);		
			SPI4W_WRITEDATA(0x01);		
			SPI4W_WRITEDATA(0x11);		
			SPI4W_WRITEDATA(0x0C);		
			SPI4W_WRITECOM(0x3F);		
			SPI4W_WRITEDATA(0x07);		
			PioSet( CS_PIN, HIGH );      //CS HIGH   
		}
}    

void pic_display(void)
{
	#ifdef APEX2_9
    	// Set RAM X - address Start / End position
	    SPI4W_WRITECOM(0x4E);		// set RAM x address count to 0;      XÄÚ´æ µØÖ· ¼ÆÊy
		SPI4W_WRITEDATA(0x00);
		SPI4W_WRITECOM(0x4F);		// set RAM y address count to 296;	2D9    YÄÚ´æ µØÖ· ¼ÆÊy
		SPI4W_WRITEDATA(0x27);
		SPI4W_WRITEDATA(0x01);

		SPI4W_WRITECOM(0x24);
    	//delay_ms(2);
   		Ultrachip0();

		SPI4W_WRITECOM(0x4E);		// set RAM x address count to 0;      XÄÚ´æ µØÖ· ¼ÆÊy
		SPI4W_WRITEDATA(0x00);
		SPI4W_WRITECOM(0x4F);		// set RAM y address count to 296;	2D9    YÄÚ´æ µØÖ· ¼ÆÊy
		SPI4W_WRITEDATA(0x27);
		SPI4W_WRITEDATA(0x01);
	    
		SPI4W_WRITECOM(0x26);
	    Ultrachip1();
	#endif
	    
    #ifdef APEX4_2
		// Set RAM X - address Start / End position
	    SPI4W_WRITECOM(0x4E);		// set RAM x address count to 0;      XÄÚ´æ µØÖ· ¼ÆÊy
		SPI4W_WRITEDATA(0x00);
		SPI4W_WRITECOM(0x4F);		// set RAM y address count to 399
		SPI4W_WRITEDATA(0x2b);
		SPI4W_WRITEDATA(0x01);

		SPI4W_WRITECOM(0x24);
		Ultrachip0();

		SPI4W_WRITECOM(0x4E);		// set RAM x address count to 0;      XÄÚ´æ µØÖ· ¼ÆÊy
		SPI4W_WRITEDATA(0x00);
		SPI4W_WRITECOM(0x4F);		// set RAM y address count to 296;	2D9    YÄÚ´æ µØÖ· ¼ÆÊy
		SPI4W_WRITEDATA(0x2b);
		SPI4W_WRITEDATA(0x01);
	    
		SPI4W_WRITECOM(0x26);
	    Ultrachip1();

	#endif
}

void Ultrachip0(void)
{
    unsigned int i;
    for( i = 0 ; i < height * width / 8 ; i++)	     
    {
        SPI4W_WRITEDATA(final[i]);  
    }  
    //delay_ms(2);	              
}

void Ultrachip1(void)
{
    unsigned int i;
    for(i = height * width / 8 ; i < height * width / 4 ; i++)	     
    {
        SPI4W_WRITEDATA(final[i]);  //renkli kï¿½sï¿½m
    }  
    //delay_ms(2);	              
}


void enterdeepsleep()
{
    if ( temp1<10) // for less than 10 degC. new added in 07Sep2017
    {
        SPI4W_WRITECOM(0x72);
        SPI4W_WRITEDATA(0x80);
        PioSet( CS_PIN, HIGH );
    }

    SPI4W_WRITECOM(0x10);
    SPI4W_WRITEDATA(0x01); //00h = Normal mode, 01h = Deep Sleep
    PioSet( CS_PIN, HIGH );
}

void Display(){
    
    PinModeDir();

    Reset();
    
    READBUSY();
    SPI4W_WRITECOM(0x12); // Software Reset
    READBUSY();
    
    EPD_W27_Init(); 
    
    pic_display();
    
    // Display Refresh
    SPI4W_WRITECOM(0x22);
	SPI4W_WRITEDATA(0xC7);		
	SPI4W_WRITECOM(0x20);
	delay_ms(1);
	READBUSY();
    
    enterdeepsleep();
    
    PioSet( SPI_MASTER_PIO_MOSI, LOW );
    PioSet( SPI_MASTER_PIO_SCLK, LOW );
    
    delay_ms(2000); // 2sn bekleme.
}


