
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx


 // SSD1675A Red 2.9" OTP 128x296

//xx Includes xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
#include "msp430x22x4.h"
#include "image1.h"



//xx Private macro xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

#define SDA_H     	(P1OUT |=BIT7)						// P1.7
#define SDA_L     	(P1OUT &=~BIT7)
#define SCLK_H    	(P1OUT |=BIT6)  					// P1.6
#define SCLK_L   	(P1OUT &=~BIT6) 
#define nCS_H     	(P1OUT |=BIT5)						// P1.5
#define nCS_L     	(P1OUT &=~BIT5)
#define nDC_H     	(P1OUT |=BIT4)						// P1.4
#define nDC_L     	(P1OUT &=~BIT4)
#define nRST_H     	(P1OUT |=BIT3)						// P1.3 
#define nRST_L     	(P1OUT &=~BIT3)

#define R_SDA         0x80 	//P1.7

#define DELAY_TIME	2					// 图片显示完停留时间(单位:秒)

#define MODE1  							// panel scan direction



#define PIC_BLACK		252
#define PIC_WHITE		255
#define PIC_A			1
#define PIC_B   	    2
#define PIC_HLINE		3
#define PIC_VLINE	    4
#define PIC_C			5
#define PIC_D   	    6
#define PIC_E		    7
#define PIC_R	        8
#define ed_dot          9
/*
#define PIC_SUNING		3
#define PIC_SHOUSI 		4
#define PIC_BAZHE 		5
#define PIC_CHESSBOARD		6
#define PIC_JIFEN		7
#define PIC_FU			8
#define PIC_GC4			9
#define PIC_SAVE		10
#define PIC_HK                  13
#define PIC_T                   14
#define PIC_P                   15
#define PIC_Z                   16
#define PIC_COOK                17
*/
//xx Private functions xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

unsigned int tempcode;
unsigned char tempvalue;
unsigned char temp1,temp2;	

//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//xx   延时函数    xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
void DELAY_100nS(int delaytime)   						// 30us 
{
	int i,j;
	
	for(i=0;i<delaytime;i++)
		for(j=0;j<1;j++);
}

void DELAY_mS(int delaytime)    						// 1ms
{
	int i,j;
	
	for(i=0;i<delaytime;i++)
		for(j=0;j<2400;j++);
}

void DELAY_S(int delaytime)     						// 1s
{
	int i,j,k;
	
	for(i=0;i<delaytime;i++)
		for(j=0;j<1000;j++)
			for(k=0;k<2400;k++);
}



//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//xx   电子纸驱动操作函数    xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

// 复位 xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
void RESET()
{
	nRST_L;
	DELAY_mS(1);								
 	nRST_H;
  	DELAY_mS(1);
}

// 读忙 xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
void READBUSY()
{
  	while(1)
  	{
   		_NOP();
   	 	 if((P1IN & 0x04)==0)
    		break;
  	}      
}

// 写命令 xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
void SPI4W_WRITECOM(unsigned char INIT_COM)
{
	unsigned char TEMPCOM;
	unsigned char scnt;
   P1DIR |= R_SDA;
	TEMPCOM=INIT_COM;
	nCS_H;
	nCS_L;
	SCLK_L;
	nDC_L;
	for(scnt=0;scnt<8;scnt++)
	{
		if(TEMPCOM&0x80)
			SDA_H;
		else
			SDA_L;
		SCLK_H;  
		SCLK_L;  
		TEMPCOM=TEMPCOM<<1;
	}
}

// 写数据 xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
void SPI4W_WRITEDATA(unsigned char INIT_DATA)
{
	unsigned char TEMPCOM;
	unsigned char scnt;
	P1DIR |= R_SDA;

	TEMPCOM=INIT_DATA;
	SCLK_L;
	nDC_H;
	for(scnt=0;scnt<8;scnt++)
	{
		if(TEMPCOM&0x80)
		SDA_H;
		else
		SDA_L;
		SCLK_H;  
		SCLK_L;  
		TEMPCOM=TEMPCOM<<1;
	}
}


// 读数据xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
 unsigned char SPI4W_READDATA1()
 {
	 P1DIR &=~ R_SDA;

	 unsigned char scnt,temp;
         temp=0;
         
	 nDC_H;
	 for(scnt=0;scnt<8;scnt++)
	 {
		 	  
		 SCLK_L;  
		 if(P1IN&R_SDA)
		 temp=(temp<<1)|0x01;
		 else
		 temp=temp<<1;		
		 SCLK_H;	  
		 SCLK_L;  
	 }
         tempvalue= temp;
	 return temp;
 }

// 电子纸驱动初始化 xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

void INIT_SSD1675A()
{       

        SPI4W_WRITECOM(0x74);
        SPI4W_WRITEDATA(0x54);
        SPI4W_WRITECOM(0x7E);
        SPI4W_WRITEDATA(0x3B);
		
        SPI4W_WRITECOM(0x2B);  // Reduce glitch under ACVCOM	
        SPI4W_WRITEDATA(0x04);           
        SPI4W_WRITEDATA(0x63);
        
        SPI4W_WRITECOM(0x0C);  // Soft start setting
        SPI4W_WRITEDATA(0x8B);           
        SPI4W_WRITEDATA(0x9C);
        SPI4W_WRITEDATA(0x96);
        SPI4W_WRITEDATA(0x0F);
        
        
		
		SPI4W_WRITECOM(0x01);  // Set MUX as 296
        SPI4W_WRITEDATA(0x27);           
        SPI4W_WRITEDATA(0x01);
        SPI4W_WRITEDATA(0x00);     
        
        SPI4W_WRITECOM(0x11);  // Data entry mode
        SPI4W_WRITEDATA(0x01);         
        SPI4W_WRITECOM(0x44); 
        SPI4W_WRITEDATA(0x00); // RAM x address start at 0
        SPI4W_WRITEDATA(0x0F); // RAM x address end at 0Fh(15+1)*8->128
        SPI4W_WRITECOM(0x45); 
        SPI4W_WRITEDATA(0x27);   // RAM y address start at 127h     
        SPI4W_WRITEDATA(0x01);
        SPI4W_WRITEDATA(0x00); // RAM y address end at 00h     
        SPI4W_WRITEDATA(0x00);
		
		SPI4W_WRITECOM(0x3C); // board
		SPI4W_WRITEDATA(0x01);		//LUT1

		SPI4W_WRITECOM(0x18);
		SPI4W_WRITEDATA(0X80);
		SPI4W_WRITECOM(0x22);
		SPI4W_WRITEDATA(0XB1);	//Load Temperature and waveform setting.
		SPI4W_WRITECOM(0x20);
		nCS_H;  
		READBUSY();
		SPI4W_WRITECOM(0x1B);
		temp1=SPI4W_READDATA1(); 
		temp2=SPI4W_READDATA1();

		nCS_H;  
         
         
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
			nCS_H;  
		}   
}

// 入深度睡眠 xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

 void enterdeepsleep()
 {
  if ( temp1<10) // for less than 10 degC. new added in 07Sep2017
{
SPI4W_WRITECOM(0x72);
SPI4W_WRITEDATA(0x80);
nCS_H;
}
SPI4W_WRITECOM(0x10);
SPI4W_WRITEDATA(0x01);
nCS_H;
}

//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//xx   图片显示函数    xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
void dis_img(unsigned char num)
{
	unsigned int row, col;
	unsigned int pcnt;
/***************************************************bw image****************************************************/
	SPI4W_WRITECOM(0x4E);		// set RAM x address count to 0;      X内存 地址 计数
	SPI4W_WRITEDATA(0x00);
	SPI4W_WRITECOM(0x4F);		// set RAM y address count to 296;	2D9    Y内存 地址 计数
	SPI4W_WRITEDATA(0x27);
	SPI4W_WRITEDATA(0x01);
	SPI4W_WRITECOM(0x24);             //   写入图片数据 B W       
	
	pcnt = 0;											// 复位或保存提示字节序号
	for(col=0; col<296; col++)							
	{
		for(row=0; row<16; row++)					
		{
				switch (num)
				{
				//case PIC_A:
					//SPI4W_WRITEDATA(gImage_YB02[pcnt]);
					//break;	
                                        
                                        
                    //             case PIC_B:
			//		SPI4W_WRITEDATA(gImage_5[pcnt]);
                      //                 break;
				case PIC_C:
					SPI4W_WRITEDATA(gImage_290_bw[pcnt]);         
				break;
					
				case PIC_VLINE:
					if(col<98)
						SPI4W_WRITEDATA(0x00);
					else
						SPI4W_WRITEDATA(0xff);
				break;

				case PIC_HLINE:
					if(row<5)
						SPI4W_WRITEDATA(0x00);
					else
						SPI4W_WRITEDATA(0xff);
				break;  

				case PIC_R:
					SPI4W_WRITEDATA(0xff);
				break;	
				
				case PIC_WHITE:
					SPI4W_WRITEDATA(0xff);
				break;	

				case PIC_BLACK:
					SPI4W_WRITEDATA(0x00);
				break;	

			   	default:
				break;
				}
			pcnt++;
		}
	}
              
/***************************************************red image****************************************************/
        SPI4W_WRITECOM(0x4E);		// set RAM x address count to 0;      X内存 地址 计数
        SPI4W_WRITEDATA(0x00);
        SPI4W_WRITECOM(0x4F);		// set RAM y address count to 296;	2D9    Y内存 地址 计数
         SPI4W_WRITEDATA(0x27);
        SPI4W_WRITEDATA(0x01);
	SPI4W_WRITECOM(0x26);             //   写入图片数据 B W       
	
	pcnt = 0;											// 复位或保存提示字节序号
	for(col=0; col<296; col++)							
	{
		for(row=0; row<16; row++)					
		{
				switch (num)
				{
				//case PIC_A:
					//SPI4W_WRITEDATA(gImage_YB02[pcnt]);
					//break;	
                                        
                                        
                    //             case PIC_B:
			//		SPI4W_WRITEDATA(gImage_5[pcnt]);
                      //                 break;
				case PIC_C:
					SPI4W_WRITEDATA(gImage_290_red[pcnt]);         
				break;
					
				case PIC_VLINE:
					if(col>196)
						SPI4W_WRITEDATA(0xff);
					else
						SPI4W_WRITEDATA(0x00);
				break;

				case PIC_HLINE:
					if(row>9)
						SPI4W_WRITEDATA(0xff);
					else
						SPI4W_WRITEDATA(0x00);
				break;  

				case PIC_R:
					SPI4W_WRITEDATA(0xff);
				break;	
				
				case PIC_WHITE:
					SPI4W_WRITEDATA(0x00);
				break;	

				case PIC_BLACK:
					SPI4W_WRITEDATA(0x00);
				break;	

			   	default:
				break;
				}
			pcnt++;
		}
	}        
              
        
        
        SPI4W_WRITECOM(0x22);
	SPI4W_WRITEDATA(0xC7);		
	SPI4W_WRITECOM(0x20);
	DELAY_mS(1);
	READBUSY();
	
}




//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//xx   主函数    xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
void main( void )
{
	//int i;

	WDTCTL = WDTPW + WDTHOLD;							// Stop watchdog timer to prevent time out reset
	BCSCTL1 = CALBC1_12MHZ; 							// set DCO frequency 1MHZ
	DCOCTL = CALDCO_12MHZ; 

	P1DIR |=0x78;  								// set P1.3~7 output
	P3OUT = 0XFF;
        P3DIR = 0xff;								// p3.4,p3.5 设为输入

		
	
	
//	while(1)
//	{
            
                   
			RESET();								  // 电子纸控制器复位
			SPI4W_WRITECOM(0x12); 		  //SWRESET
			READBUSY(); 
			INIT_SSD1675A();				  
			dis_img(PIC_WHITE);
			enterdeepsleep();
			DELAY_S(DELAY_TIME);
			_NOP();		

			RESET();								  // 电子纸控制器复位
			SPI4W_WRITECOM(0x12); 		  //SWRESET
			READBUSY(); 
			INIT_SSD1675A();
			dis_img(PIC_C);
			enterdeepsleep();
			DELAY_S(DELAY_TIME);
			_NOP();		

			RESET();								  // 电子纸控制器复位
			SPI4W_WRITECOM(0x12); 		  //SWRESET
			READBUSY(); 
			INIT_SSD1675A();
			dis_img(PIC_VLINE);
			DELAY_S(DELAY_TIME); 
			enterdeepsleep();
			DELAY_S(DELAY_TIME);
			_NOP();		
     
//	}

}
