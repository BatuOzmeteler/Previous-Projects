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
#define nfCS_H     	(P4OUT |=BIT3)						// P4.3
#define nfCS_L     	(P4OUT &=~BIT3)
//#define nfSDA_H     	(P4OUT |=BIT2)						// P4.2
//#define nfSDA_L     	(P4OUT &=~BIT2)
#define DELAY_TIME	60									// 图片显示完停留时间(单位:秒)

#define MODE1  											// panel scan direction



#define PIC_BLACK		252
#define PIC_WHITE		255
#define PIC_A			1
#define PIC_B   	    2
#define PIC_HLINE		3
#define PIC_VLINE	    4
#define PIC_C			5
#define PIC_D   	    6
#define PIC_E		    7
#define PIC_F	        8
#define PIC_RED             9
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
unsigned char addrh;
unsigned char addrm;
unsigned char addrl;
unsigned char temph;
unsigned char templ;
unsigned char aa[260];
unsigned char Vcomvalue;

//xx Private functions xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx


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
		for(j=0;j<200;j++);
}

void DELAY_S(int delaytime)     						// 1s
{
	int i,j,k;
	
	for(i=0;i<delaytime;i++)
		for(j=0;j<1000;j++)
			for(k=0;k<200;k++);
}



//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//xx   电子纸驱动操作函数    xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

// 复位 xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
void RESET()
{
	nRST_L;
	DELAY_mS(100);								
 	nRST_H;
  	DELAY_mS(100);
}

// 读忙 xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
void READBUSY()
{
  	while(1)
  	{
   		_NOP();
   	 	if((P1IN & 0x04)==0x04)
    		break;
  	}      
}
// 写命令 flash xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
void FSPI4W_WRITECOM(unsigned char INIT_COM)
{
  	unsigned char TEMPCOM;
  	unsigned char scnt;

  	TEMPCOM=INIT_COM;
    	SCLK_H;  
  	//nfCS_H;
  	nfCS_L;
  	for(scnt=0;scnt<8;scnt++)
  	{
    	if(TEMPCOM&0x80)
     	 	SDA_H;
    	else
      		SDA_L;
    	SCLK_L;  
    	SCLK_H;
    	TEMPCOM=TEMPCOM<<1;
  	}
  	
}

// 写数据 flash xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
void FSPI4W_WRITEDATA(unsigned char INIT_DATA)
{
  	unsigned char TEMPCOM;
  	unsigned char scnt;

  	TEMPCOM=INIT_DATA;
    	SCLK_H;  
  	//nfCS_H;
  	nfCS_L;
  	for(scnt=0;scnt<8;scnt++)
  	{
    	if(TEMPCOM&0x80)
     	 	SDA_H;
    	else
      		SDA_L;
    	SCLK_L;  
    	 SCLK_H; 
    	TEMPCOM=TEMPCOM<<1;
  	}
  	//nCS_H;  
}
// 写数据 flash xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
unsigned char  FSPI4W_READDATA(void)
{
  	unsigned char TEMPCOM;
  	unsigned char scnt;
        TEMPCOM=0X00;
  	//TEMPCOM=INIT_DATA;
    	SCLK_L;  
  	//nfCS_H;
  	nfCS_L;
  	for(scnt=0;scnt<8;scnt++)
  	{DELAY_mS(3);
          TEMPCOM=TEMPCOM<<1;
    	if(P4IN&0x04)
     	 TEMPCOM|=0X01;
    	
    	SCLK_H;  
    	SCLK_L;
    	
  	}
        return (TEMPCOM);
  	//nCS_H;  
}
// 写命令  xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
void SPI4W_WRITECOM(unsigned char INIT_COM)
{
  	unsigned char TEMPCOM;
  	unsigned char scnt;

  	TEMPCOM=INIT_COM;
    	SCLK_H;  
 	nDC_L;
  	nCS_H;
  	nCS_L;
  	for(scnt=0;scnt<8;scnt++)
  	{
    	if(TEMPCOM&0x80)
     	 	SDA_H;
    	else
      		SDA_L;
    	SCLK_L;  
    	SCLK_H;  
    	TEMPCOM=TEMPCOM<<1;
  	}
  	nCS_H;  
}

// 写数据 xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
void SPI4W_WRITEDATA(unsigned char INIT_DATA)
{
  	unsigned char TEMPCOM;
  	unsigned char scnt;

  	TEMPCOM=INIT_DATA;
    	SCLK_H;  
 	nDC_H;
  	nCS_H;
  	nCS_L;
  	for(scnt=0;scnt<8;scnt++)
  	{
    	if(TEMPCOM&0x80)
     	 	SDA_H;
    	else
      		SDA_L;
    	SCLK_L;  
    	SCLK_H;  
    	TEMPCOM=TEMPCOM<<1;
  	}
  	nCS_H;  
}
// read flash xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
unsigned char  SPI4W_READDATA(void)
{
  	unsigned char TEMPCOM;
  	unsigned char scnt;
        TEMPCOM=0X00;
  	//TEMPCOM=INIT_DATA;
        nCS_L;
        DELAY_mS(1);
        nDC_H;
        DELAY_mS(1);
        SCLK_L; 
        DELAY_mS(1);
      
  	//nfCS_H;
  	 P1DIR &=0x78;
         DELAY_mS(3);
  	for(scnt=0;scnt<8;scnt++)
  	{
          TEMPCOM=TEMPCOM<<1;
          SCLK_H;  
    	if(P1IN&0x80)
     	 TEMPCOM|=0X01;
    	
    	
    	SCLK_L;
    	  DELAY_mS(1);
  	}
         P1DIR |=0x80;
       
  //      nCS_H; 
        return (TEMPCOM);
  	 
}
// 写波形数据表 xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
/*void WRITE_LUT()                                
{  
    unsigned int count;

    READBUSY();
    SPI4W_WRITECOM(0x20);
    for(count=0;count<220;count++)      
    { SPI4W_WRITEDATA(lut_vcom[count]);  }

    READBUSY();
    SPI4W_WRITECOM(0x21);
    for(count=0;count<260;count++)      
    {SPI4W_WRITEDATA(lut_b[count]);} 

    READBUSY();
    SPI4W_WRITECOM(0x22);
    for(count=0;count<260;count++)      
    {SPI4W_WRITEDATA(lut_w[count]);}  

    READBUSY();
    SPI4W_WRITECOM(0x23);
    for(count=0;count<260;count++)      
    {SPI4W_WRITEDATA(lut_g0[count]);}   


    READBUSY();
    SPI4W_WRITECOM(0x24);
    for(count=0;count<260;count++)      
    {SPI4W_WRITEDATA(lut_g1[count]);}

    READBUSY();
    SPI4W_WRITECOM(0x25);
    for(count=0;count<260;count++)      
    {SPI4W_WRITEDATA(lut_r0[count]);}  

    READBUSY();
    SPI4W_WRITECOM(0x26);
    for(count=0;count<260;count++)     
    {SPI4W_WRITEDATA(lut_r1[count]);} 

    READBUSY();
    SPI4W_WRITECOM(0x27);
    for(count=0;count<260;count++)     
    {SPI4W_WRITEDATA(lut_r2[count]);} 

    READBUSY();
    SPI4W_WRITECOM(0x28);
    for(count=0;count<260;count++)     
    {SPI4W_WRITEDATA(lut_r3[count]);} 

    READBUSY();
    SPI4W_WRITECOM(0x29);
    for(count=0;count<200;count++)     
    {SPI4W_WRITEDATA(lut_xon[count]);}  
}*/

// 读波形数据表 xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
void READ_LUT(unsigned char faddrh,unsigned char faddrm,unsigned char faddrl,unsigned command)
{unsigned char TEMPCOM;
 unsigned int i;
   TEMPCOM=command;
   nfCS_L;
   FSPI4W_WRITECOM(TEMPCOM);
   DELAY_mS(3);
   TEMPCOM=faddrh;
   // nfCS_L;
   FSPI4W_WRITEDATA(TEMPCOM);
   DELAY_mS(3);
   TEMPCOM=faddrm;
   FSPI4W_WRITEDATA(TEMPCOM);
   DELAY_mS(3);
   TEMPCOM=faddrl;
   FSPI4W_WRITEDATA(TEMPCOM);
 for(i=0;i<28;i++)
   {DELAY_mS(3);
     aa[i]=FSPI4W_READDATA();
   }
  /* for(i=0;i<256;i++)
   {DELAY_mS(3);
     aa[i]=FSPI4W_READDATA();
   }*/
  nfCS_H; 
}
//读温度 xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
void READ_TEMP(unsigned char command)
{unsigned char TEMPCOM;
// unsigned int i;
   TEMPCOM=command;
    nCS_L;
   SPI4W_WRITECOM(TEMPCOM); 
   nCS_H;
   DELAY_mS(30);
   
   READBUSY();
    nCS_L;
  
   temph=SPI4W_READDATA();
   templ=SPI4W_READDATA();

 nCS_H; 
}

void READ_VCOM(void)
 {

 
	SPI4W_WRITECOM(0x01);		  //power setting
	SPI4W_WRITEDATA(0x17);
	SPI4W_WRITEDATA(0x01);   //VGH  00 20V  01 19V
	SPI4W_WRITEDATA(0x05);
	SPI4W_WRITEDATA(0x05);


	SPI4W_WRITECOM(0x00);		  //panel setting
	SPI4W_WRITEDATA(0x8f);
	SPI4W_WRITEDATA(0x08);

	SPI4W_WRITECOM(0x04);
	READBUSY();
	DELAY_S(2);
	SPI4W_WRITECOM(0x80);
	SPI4W_WRITEDATA(0x11);

	READBUSY();	
	SPI4W_WRITECOM(0x81);
	Vcomvalue=SPI4W_READDATA();
	SPI4W_WRITECOM(0x02);
	READBUSY();
	DELAY_S(2);
	SPI4W_WRITECOM(0x82);
	SPI4W_WRITEDATA(Vcomvalue);

 }


// 电子纸驱动初始化 xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

void INIT_SSD1673()
{   // unsigned int j;
    SPI4W_WRITECOM(0x65);		// 2bit or 3bit address selection of external flash
	SPI4W_WRITEDATA(0x01);		// 
      //  nfCS_L;
       // FSPI4W_WRITECOM (0XB9);
       // nfCS_H;
      
// FWRITE_LUT();
    
   nfCS_L; 
   FSPI4W_WRITECOM(0x06); //写使能
   nfCS_H;  
   READ_LUT(0X00,0X61,0Xc7,0X03);

   SPI4W_WRITECOM(0x65);		// 2bit or 3bit address selection of external flash
   SPI4W_WRITEDATA(0x00);		// 

       DELAY_mS(100); 
	SPI4W_WRITECOM(0x06);		//booster setting
	SPI4W_WRITEDATA(0xD7);
	SPI4W_WRITEDATA(0xD7);
        SPI4W_WRITEDATA(0x2f);
	SPI4W_WRITECOM(0x01);		//power setting
	SPI4W_WRITEDATA(0x17);
        SPI4W_WRITEDATA(0x01);
        SPI4W_WRITEDATA(0x05);
        SPI4W_WRITEDATA(0x05);
		
      //SPI4W_WRITECOM(0x41);		///pll setting 100hz 
	// SPI4W_WRITEDATA(0x00);		//100HZ
	SPI4W_WRITECOM(0x00);		//panel setting
	SPI4W_WRITEDATA(0x8f);
       // SPI4W_WRITEDATA(0x09);
      
         //SPI4W_WRITECOM(0X40);
        // SPI4W_WRITEDATA(0x00);
        //READ_TEMP(0x40);
	//SPI4W_WRITECOM(0x30);		///pll setting 100hz 
	// SPI4W_WRITEDATA(0x3a);		//100HZ
        //SPI4W_WRITEDATA(0x29);		//pll setting 143hz 
      //  SPI4W_WRITEDATA(0x3C);		//pll setting 50hz 
        //SPI4W_WRITEDATA(0x2A);		//71HZ
     
	SPI4W_WRITECOM(0xe5);		// 2bit or 3bit address selection of external flash
	SPI4W_WRITEDATA(0x03);		// 
        SPI4W_WRITECOM(0x50);           // // VCOM AND DATA INTERVAL
       // SPI4W_WRITEDATA(0x07);		// lutw
	SPI4W_WRITEDATA(0x47);		// gray1
       //	SPI4W_WRITECOM(0x82);           // vcom
        //SPI4W_WRITEDATA(0x1E);          //-1.0V
   //	SPI4W_WRITEDATA(0x22);          //-1.7V
       // SPI4W_WRITEDATA(0x3C);          //3V
	
	SPI4W_WRITECOM(0x60);		// TCON SETTING
	SPI4W_WRITEDATA(0x22);		//GS1-->GS1
        SPI4W_WRITECOM(0x61);		// TCON SETTING
	SPI4W_WRITEDATA(0x02);		//GS1-->GS1
        SPI4W_WRITEDATA(0x80);		// TCON SETTING
	SPI4W_WRITEDATA(0x01);		//GS1-->GS1
        SPI4W_WRITEDATA(0x80);		//GS1-->GS1
	//WRITE_LUT();
}

// 入深度睡眠 xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
void enterdeepsleep()
{	
  
  
  	SPI4W_WRITECOM(0x07);
  	SPI4W_WRITEDATA(0xa5);
	
}



//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//xx   图片显示函数    xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
void load_img(unsigned char dptr)
{
 unsigned int row, col;
 unsigned long pcnt; 
 SPI4W_WRITECOM(0x10);
 pcnt = 0;           // ???????????
 for(col=0; col<384; col++)       // ??172?  // send 128x252bits ram 2D13
 {      for( row=0; row<320; row++)
                    {
    switch (dptr)
    {
          

                case PIC_VLINE:         if(row<106)
                                        SPI4W_WRITEDATA(0x00);
                                        else if(row<213)
                                        SPI4W_WRITEDATA(0x33);
                                        else
                                        SPI4W_WRITEDATA(0x77);
     break;
                                        
                 case PIC_HLINE:       if(col<128)
                                        SPI4W_WRITEDATA(0x77);
                                        else if(col<256)
                                        SPI4W_WRITEDATA(0x00);
                                        else
                                        SPI4W_WRITEDATA(0x33);
     break; 
     
     case PIC_B:
      SPI4W_WRITEDATA(0x0F);
      break; 
    case PIC_RED:
     SPI4W_WRITEDATA(0x77);
     break; 
 
    case PIC_WHITE:
     SPI4W_WRITEDATA(0x00);
     break; 

    case PIC_BLACK:
     SPI4W_WRITEDATA(0x33);
     break; 
    
//     case PIC_A:
//      SPI4W_WRITEDATA(gImage_1[pcnt]);
//      break;
    /* case PIC_D:
      SPI4W_WRITEDATA(gImage_2[pcnt]);
      break;   */  
       default:
     break;
    }
                  pcnt++;
        }
  }
 
   READBUSY();
  SPI4W_WRITECOM(0x04);   // power on
  DELAY_mS(1000);  
 SPI4W_WRITECOM(0x12);
 DELAY_mS(120);
 READBUSY();
  SPI4W_WRITECOM(0x02);
}

//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
void load_img1(unsigned char num)
{
	unsigned int row, col;
	unsigned long pcnt;
        unsigned char i;
        unsigned char tempram1;
        unsigned char tempram2;
        unsigned char tempram3;
        unsigned char tempram4;
        unsigned char tempram5;
        unsigned char tempram6;
	 
	SPI4W_WRITECOM(0x10);
        pcnt = 0X00;	
        switch(num)
        { case PIC_A: 
	            while (!((gImage_xx[pcnt+1]==0x0d)&&(gImage_xx[pcnt+2]==0x0a)&&(gImage_xx[pcnt+3]==0x55)&&(gImage_xx[pcnt+4]==0xaa))) 
	                 { if((pcnt%0X02)==0X00)
                           { tempram1= gImage_xx[pcnt];
                             tempram3=0x00;  
                              for (i=0;i<4;i++)
                                  { 
                                    tempram2=tempram1;
                                    tempram2&=0x03;
                                    if (tempram2==0x00) 
                                    {  
                                    } 
                                    else if(tempram2==0x01)
                                    { 
                                       tempram3|=0x30;
                                    } 
                                    else 
                                    { tempram3|=0x70;
                                    }
                                                   
                                  
                                    if(i==0x01)
                                      {tempram4=tempram3;
                                       tempram3=0x00;
                                       }
                                    if(i==0x03)
                                       {tempram5=tempram3;
                                        tempram3=0x00;
                                       }
                                      tempram3=(tempram3>>4);
                                      tempram1=(tempram1>>2);
                                    }
                               pcnt+=1; 
                               }
                           else
                               {tempram6=gImage_xx[pcnt];
                                for(i=0;i<tempram6;i++)
                                   { SPI4W_WRITEDATA(tempram5);
                                     SPI4W_WRITEDATA(tempram4);
                                   }
                                 pcnt+=1; 
                               }
                           if(pcnt==14)
                           {row=0;
                           }
                          }
                            
		          break;	
        default:     break;
        }
pcnt++;
        
//}
 
 
   READBUSY();
  SPI4W_WRITECOM(0x04);   // power on
  DELAY_mS(1000);  
 SPI4W_WRITECOM(0x12);
 DELAY_mS(120);
 READBUSY();
  SPI4W_WRITECOM(0x02);
}



//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//xx   主函数    xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
void main( void )
{
	//int i;
        

	WDTCTL = WDTPW + WDTHOLD;							// Stop watchdog timer to prevent time out reset
	BCSCTL1 = CALBC1_1MHZ; 								// set DCO frequency 1MHZ
	DCOCTL = CALDCO_1MHZ; 

	P1DIR |=0xF8;  										// set P1.3~7 output
	P3OUT = 0XFF;
        P3DIR = 0xff;										// p3.4,p3.5 设为输入
        P4DIR = 0X08; 
		
	RESET();											// 电子纸控制器复位
    	
	
	
	INIT_SSD1673();	
    	

	while(1)
	{
			//dis_img(PIC_BLACK);
			//DELAY_S(3);
			//RESET();								// 电子纸控制器复位
			//READBUSY(); 
			//INIT_SSD1673();
			load_img(PIC_WHITE);
			//enterdeepsleep();
			DELAY_S(3);
			//_NOP(); 	 

			/*load_img(PIC_HLINE);
			DELAY_S(3);
                        
                        load_img(PIC_VLINE);
			DELAY_S(3);*/
			
			load_img1(PIC_A);
			DELAY_S(3);
			//   dis_img(PIC_RED);
			//   DELAY_S(60);
			//load_img(PIC_D);
                       //DELAY_S(5);
			//DELAY_S(7200);
			//DELAY_S(7200);
			//DELAY_S(7200);
                        
                       
                        
                        enterdeepsleep();
                
	}

}
