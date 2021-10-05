
#include <types.h>          /* Commonly used type definitions */



//char barcode[13];

int startX, startY, byte, bit;


const char EAN13_Structure[10][6] = {
    {"LLLLLL"}, // 0
    {"LLGLGG"}, // 1
    {"LLGGLG"}, // 2
    {"LLGGGL"}, // 3
    {"LGLLGG"}, // 4
    {"LGGLLG"}, // 5
    {"LGGGLL"}, // 6
    {"LGLGLG"}, // 7
    {"LGLGGL"}, // 8
    {"LGGLGL"}  // 9
};

const char EAN13_Digits[10][3][7] = {
    //	  L			   G			R
    {{"0001101"}, {"0100111"}, {"1110010"}}, // 0
    {{"0011001"}, {"0110011"}, {"1100110"}}, // 1
    {{"0010011"}, {"0011011"}, {"1101100"}}, // 2
    {{"0111101"}, {"0100001"}, {"1000010"}}, // 3
    {{"0100011"}, {"0011101"}, {"1011100"}}, // 4
    {{"0110001"}, {"0111001"}, {"1001110"}}, // 5
    {{"0101111"}, {"0000101"}, {"1010000"}}, // 6
    {{"0111011"}, {"0010001"}, {"1000100"}}, // 7
    {{"0110111"}, {"0001001"}, {"1001000"}}, // 8
    {{"0001011"}, {"0010111"}, {"1110100"}}  // 9
};



//bool checkChecksum(void);
void calculateChecksum(char *_barcode);
void calculateByteBit(void);
void drawBlackLine(int normal);
void drawWhiteLine(int normal);
void drawLines(char firstDigit, char ch, int index, int right);
void bitmapBarcode(char code[13], int x, int y);

