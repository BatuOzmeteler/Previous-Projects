#include <types.h>          /* Commonly used type definitions */

#include "barcode_generator.h"
#include "platform_config.h"


/*
 * EAN13 barkod standard�n� checksum hesab� konusunda inceleyiniz.
 */

/*
bool checkChecksum(char *barcode) {
    int i, temp = 0;
    
    for(i = 0; i < 11; i+=2) temp += barcode[i] - 48;
    for(i = 1; i < 12; i+=2) temp += 3 * (barcode[i] - 48);
    
    temp = 10 - (temp % 10);
    
    return barcode[12] == (uint8)(temp + 48);
}
*/
void calculateChecksum(char *_barcode) {
    int i, temp = 0;
    
    for(i = 0; i < 11; i+=2) temp += _barcode[i] - 48;
    for(i = 1; i < 12; i+=2) temp += 3 * (_barcode[i] - 48);
    
    temp = 10 - (temp % 10);
    
    
    temp = temp + 48;
    if(temp>=58) temp = temp - 10;
    _barcode[12] = temp;
    
}

/*
 * �izilecek �izginin ka��nc� byte ve byte'�n ka��nc� biti oldu�unu hesapla
 */
void calculateByteBit(void) {
    byte = startY - 1;
    byte /= 8;
    if((startY) % 8 == 0) byte++;
    
    bit = startY % 8;
}

/*
 * EAN13 te baz� �izgiler normalden uzun olabiliyor.
 * Birbirinden ay�rt etmek i�in 0'dan ba�ka olan normal de�erleri normal uzunluktaki �izgilere tekab�l ediyor.
 * normal de�eri 0 ise s�k�nt� yok.
 */
void drawBlackLine(int normal) {
    int i, height;
    
    height = HEIGHT;
    if(normal) height += 8;
    
    calculateByteBit();
    for(i = startX; i < height + startX; i++) final[(i*22) + (COLUMN - 1 - byte)]  = final[(i*22) + (COLUMN - 1 - byte)] |(0x01 << bit);
    startY++;
}

void drawWhiteLine(int normal) {
    int i, height;
    
    height = HEIGHT;
    if(normal) height += 8;
    
    calculateByteBit();
    for(i = startX; i < height + startX; i++) final[(i*22) + (COLUMN - 1 - byte)] = final[(i*22) + (COLUMN - 1 - byte)]  & ~ (0x01 << bit);
    startY++;
}
void drawLines(char firstDigit, char ch, int index, int right) {
    char /*firstDigit, */ LGR, LGR_Index;
    int i;
    
//    firstDigit = barcode[0];
    firstDigit -= 48;
    
    if(right) LGR_Index = 2;
    else {
        
        LGR = EAN13_Structure[firstDigit][index];
        
        switch(LGR) {
        case 'L':
            LGR_Index = 0;
            break;
        case 'G':
            LGR_Index = 1;
            break;
        case 'R':
            LGR_Index = 2;
            break;
            default:
            //printf("drawLines metodunda sorun var\n");
            break;
        }
    }
    
    for(i = 0; i < 7; i++) {
        if(EAN13_Digits[ch - 48][LGR_Index][i] == '1')    {
            drawBlackLine(NORMAL);
          //  drawBlackLine(NORMAL);
        }else{
            drawWhiteLine(NORMAL);
         //   drawWhiteLine(NORMAL);
        }
    }
}

void bitmapBarcode(char code[13], int x, int y) {
    int i;
    
    startX = x;
    startY = y;
    
//    memcpy(barcode, code, 13);
    
    drawBlackLine(LONG);//	drawBlackLine(LONG);
    drawWhiteLine(LONG);//	drawWhiteLine(LONG);
    drawBlackLine(LONG);//	drawBlackLine(LONG);
    
    for(i = 1; i < 7; i++) drawLines(code[0], code[i], i-1, LEFT);
    
    drawWhiteLine(LONG);//	drawWhiteLine(LONG);
    drawBlackLine(LONG);//	drawBlackLine(LONG);
    drawWhiteLine(LONG);//	drawWhiteLine(LONG);
    drawBlackLine(LONG);//	drawBlackLine(LONG);
    drawWhiteLine(LONG);//	drawWhiteLine(LONG);
    
    for(i = 7; i < 13; i++) drawLines(code[0], code[i], i-7, RIGHT);
    
    drawBlackLine(LONG);//	drawBlackLine(LONG);
    drawWhiteLine(LONG);//	drawWhiteLine(LONG);
    drawBlackLine(LONG);//	drawBlackLine(LONG);
}
