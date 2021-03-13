#include <Arduino.h>
#include <ct1642.h>

int povDelay = 2;
int CLK = 0;
int DATA = 0;

#define DATA_0 BIT_A | BIT_B | BIT_C | BIT_D | BIT_E | BIT_F
#define DATA_1 BIT_B | BIT_C
#define DATA_2 BIT_A | BIT_B | BIT_D | BIT_E | BIT_G
#define DATA_3 BIT_A | BIT_B | BIT_C | BIT_D | BIT_G
#define DATA_4 BIT_B | BIT_C | BIT_F | BIT_G
#define DATA_5 BIT_A | BIT_C | BIT_D | BIT_F | BIT_G
#define DATA_6 BIT_A | BIT_C | BIT_D | BIT_E | BIT_F | BIT_G
#define DATA_7 BIT_A | BIT_B | BIT_C
#define DATA_8 BIT_A | BIT_B | BIT_C | BIT_D | BIT_E | BIT_F | BIT_G
#define DATA_9 BIT_A | BIT_B | BIT_C | BIT_D | BIT_F | BIT_G
#define DATA_A BIT_A | BIT_B | BIT_C | BIT_E | BIT_F | BIT_G
#define DATA_b BIT_C | BIT_D | BIT_E | BIT_F | BIT_G
#define DATA_C BIT_A | BIT_D | BIT_E | BIT_F
#define DATA_c BIT_D | BIT_E | BIT_G
#define DATA_d BIT_B | BIT_C | BIT_D | BIT_E | BIT_G
#define DATA_E BIT_A | BIT_D | BIT_E | BIT_F | BIT_G
#define DATA_e BIT_A | BIT_B | BIT_D | BIT_E | BIT_F | BIT_G
#define DATA_F BIT_A | BIT_E | BIT_F | BIT_G
#define DATA_g BIT_A | BIT_B | BIT_C | BIT_D | BIT_F | BIT_G
#define DATA_H BIT_B | BIT_C | BIT_E | BIT_F | BIT_G
#define DATA_I BIT_E | BIT_F
#define DATA_i BIT_E
#define DATA_J BIT_B | BIT_C | BIT_D | BIT_E
#define DATA_L BIT_D | BIT_E | BIT_F
#define DATA_n BIT_C | BIT_E | BIT_G
#define DATA_o BIT_C | BIT_D | BIT_E | BIT_G
#define DATA_P BIT_A | BIT_B | BIT_E | BIT_F | BIT_G
#define DATA_r BIT_E | BIT_G
#define DATA_S BIT_A | BIT_F | BIT_G | BIT_C | BIT_D
#define DATA_t BIT_D | BIT_E | BIT_F | BIT_G
#define DATA_U BIT_B | BIT_C | BIT_D | BIT_E | BIT_F
#define DATA_y BIT_B | BIT_C | BIT_D | BIT_F | BIT_G
#define DATA_DARK 0x00
#define DATA_DOT BIT_P
#define DATA_COLON BIT_P
#define UPRSCR BIT_A
#define HYPN BIT_G
#define UNDRSCR BIT_D

/***************************************************************************************************
* Mapping of CT1642 Pins to bits for the 7Seg Led Display
* Alter this structure to resemble your layout.
* byte: [ABCDEFGp]
****************************************************************************************************/
#define BIT_A (1 << 4) /*          A           Q2 is connected to segment A, represented by bit 7 */
#define BIT_B (1 << 6) /*       -------        Q3 is connected to segment B, represented by bit 6 */
#define BIT_C (1 << 1) /*      |       |       Q4 is connected to segment C, represented by bit 5 */
#define BIT_D (1 << 2) /*    F |       |  B    Q5 is connected to segment D, represented by bit 4 */
#define BIT_E (1 << 3) /*       ---G---        Q6 is connected to segment E, represented by bit 3 */
#define BIT_F (1 << 5) /*      |       |  C    Q7 is connected to segment F, represented by bit 2 */
#define BIT_G (1 << 0) /*    E |       |       Q8 is connected to segment G, represented by bit 1 */
#define BIT_P (1 << 7) /*       ---D---   P    Q9 is connected to segment dP,represented by bit 0 */

byte bitmap_table[17] = {
    DATA_0, DATA_1, DATA_2, DATA_3,
    DATA_4, DATA_5, DATA_6, DATA_7,
    DATA_8, DATA_9, DATA_DARK, DATA_P,
    DATA_DOT, DATA_H, UPRSCR, HYPN,
    UNDRSCR};

void setUpDigit(byte digitNo)
{
  for (int i = 0; i < 4; i++)
  {
    digitalWrite(CLK, LOW);
    if ((digitNo << i) & 0x80)
    {
      digitalWrite(DATA, HIGH);
    }
    else
    {

      digitalWrite(DATA, LOW);
    }

    digitalWrite(CLK, HIGH);
  }
}

byte getDigitAddress(int digitNo)
{
  byte digit = 0;

  switch (digitNo)
  {
  case 1:
    //1101 1111 nam vi tri thu 3 cua xung
    digit = 0xdf;
    break;
  case 2:
    //1011 1111 nam vi tri thu 2 cua xung
    digit = 0xbf;
    break;
  case 3:
    //0111 1111 nam vi tri thu 1 cua xung
    digit = 0x7f;
    break;
  case 4:
    //1110 1111 nam vi tri thu 4 cua xung
    digit = 0xef;
    break;
  case 5:
    digit = 0xff;
    break;

  default:
    digit = 0xff;
    break;
  }
  return digit;
}

void setPreamble()
{
  //Bo 6 bit khong dung
  for (int i = 0; i < 6; i++)
  {

    digitalWrite(CLK, LOW);
    digitalWrite(DATA, HIGH);
    // if (i % 2 == 0)
    // {
    //   digitalWrite(DATA, HIGH);
    // }
    // else
    //   digitalWrite(DATA, LOW);
    digitalWrite(CLK, HIGH);
  }
}

void setChar(byte ledChar)
{
  for (int i = 0; i < 8; i++)
  {
    digitalWrite(CLK, LOW);
    if ((ledChar << i) & 0x80)
      digitalWrite(DATA, HIGH);
    else
      digitalWrite(DATA, LOW);
    digitalWrite(CLK, HIGH);
  }
}

void latchDigit()
{
  digitalWrite(DATA, LOW);
  digitalWrite(DATA, HIGH);
  digitalWrite(CLK, LOW);
  digitalWrite(DATA, LOW);
  digitalWrite(DATA, HIGH);
}

void write(byte bitChar, int digitNo)
{
  byte DigitAddress = 0;

  //Lay dia chi cua led digitNo
  //vi du 1110 hien cai thu 4
  DigitAddress = getDigitAddress(digitNo);

  //cai dat 4 bit dau tien
  setUpDigit(DigitAddress);

  //thiet dat mo dau
  setPreamble();

  //viet data
  setChar(bitChar);

  //chot ha char
  latchDigit();
}
byte getCharByte(char character)
{

  switch (character)
  {
  case '0':
    return bitmap_table[0];
    break;
  case '1':
    return bitmap_table[1];
    break;
  case '2':
    return bitmap_table[2];
    break;
  case '3':
    return bitmap_table[3];
    break;
  case '4':
    return bitmap_table[4];
    break;
  case '5':
    return bitmap_table[5];
    break;
  case '6':
    return bitmap_table[6];
    break;
  case '7':
    return bitmap_table[7];
    break;
  case '8':
    return bitmap_table[8];
    break;
  case '9':
    return bitmap_table[9];
    break;
  case '-':
    return (byte)HYPN;
    break;
  case 'A':
    return (byte)DATA_A;
    break;
  case 'b':
    return (byte)DATA_b;
    break;
  case 'c':
    return (byte)DATA_c;
    break;
  case 'C':
    return (byte)DATA_C;
    break;
  case 'd':
    return (byte)DATA_d;
    break;
  case 'E':
    return (byte)DATA_E;
    break;
  case 'e':
    return (byte)DATA_e;
    break;
  case 'F':
    return (byte)DATA_F;
    break;
  case 'g':
    return (byte)DATA_g;
    break;
  case 'H':
    return (byte)DATA_H;
    break;
  case 'I':
    return (byte)DATA_I;
    break;
  case 'i':
    return (byte)DATA_i;
    break;
  case 'J':
    return (byte)DATA_J;
    break;
  case 'L':
    return (byte)DATA_L;
    break;
  case 'n':
    return (byte)DATA_n;
    break;
  case 'o':
    return (byte)DATA_o;
    break;
  case 'P':
    return (byte)DATA_P;
    break;
  case 'r':
    return (byte)DATA_r;
    break;
  case 'S':
    return (byte)DATA_S;
    break;
  case 't':
    return (byte)DATA_t;
    break;
  case 'U':
    return (byte)DATA_U;
    break;
  case 'y':
    return (byte)DATA_y;
    break;
  case ' ':
    return (byte)DATA_DARK;
    break;
  default:
    return (byte)HYPN;
  }
}

void showChars(char char1, char char2, char char3, char char4)
{
  byte c1 = getCharByte(char1);
  byte c2 = getCharByte(char2);
  byte c3 = getCharByte(char3);
  byte c4 = getCharByte(char4);

  write(c1, 1);
  delay(povDelay);
  write(c2, 2);
  delay(povDelay);
  write(c3, 3);
  delay(povDelay);
  write(c4, 4);
  delay(povDelay);
}

void showTime(int hours, int minutes)
{ // time
  int hd, md;
  int hd1, hd2, md1, md2;

  if (hours >= 10 && hours <= 23)
  {

    hd = 2;

    // Get tens
    hd1 = (int)(hours / 10);
    hours = hours - (hd1 * 10);
    // Get units
    hd2 = (int)hours;
  }
  else if (hours >= 0 && hours <= 9)
  {
    hd = 1;
  }
  else
  {
    hd = 1;
    hours = 0;
  }

  if (minutes >= 10 && minutes <= 59)
  {
    md = 2;

    // Get tens
    md1 = (int)(minutes / 10);
    minutes = minutes - (md1 * 10);
    // Get units
    md2 = (int)minutes;
  }
  else if (minutes >= 0 && minutes <= 9)
  {
    md = 1;
  }
  else
  {
    md = 1;
    minutes = 0;
  }

  if (hd == 2)
  {
    write(bitmap_table[hd1], 1);
    delay(povDelay);
    //write(DATA_DARK, 1);
    write((byte)bitmap_table[hd2] | DATA_COLON, 2);
    delay(povDelay);
    //write(DATA_DARK, 2);
  }
  else
  {
    write(bitmap_table[0], 1);
    delay(povDelay);
    //write(DATA_DARK, 1);
    write(bitmap_table[hours] | DATA_COLON, 2);
    delay(povDelay);
    //write(DATA_DARK, 2);
  }

  if (md == 2)
  {
    write(bitmap_table[md1], 3);
    delay(povDelay);
    //write(DATA_DARK, 3);
    write(bitmap_table[md2], 4);
    delay(povDelay);
    //write(DATA_DARK, 4);
  }
  else
  {
    write(bitmap_table[0], 3);
    delay(povDelay);
    //write(DATA_DARK, 3);
    write(bitmap_table[minutes], 4);
    delay(povDelay);
    //write(DATA_DARK, 4);
  }
}



void showNumber(int number)
{ // int
  // Temporary values for thousands, hundreds, tens and units
  int d1, d2, d3, d4;
  int digits = 0;

  // Do some boundary checks
  if (number > 9999)
    number = 9999;
  if (number < 0)
    number = 0;

  // Define the number of digits
  digits = 1;
  if (number >= 10)
  {
    digits = 2;
  }
  if (number >= 100)
  {
    digits = 3;
  }
  if (number >= 1000)
  {
    digits = 4;
  }

  switch (digits)
  {
  case 1:
  {
    // Get units
    d4 = (int)number;

    write(bitmap_table[d4], 4);
    delay(povDelay);
    //write(DATA_DARK, 4);

    break;
  }
  case 2:
  {
    // Get tens
    d3 = (int)(number / 10);
    number = number - (d3 * 10);
    // Get units
    d4 = (int)number;

    write(bitmap_table[d3], 3);
    delay(povDelay);
    //write(DATA_DARK, 3);

    write(bitmap_table[d4], 4);
    delay(povDelay);
    //write(DATA_DARK, 4);

    break;
  }
  case 3:
  {
    // Get hundreds
    d2 = (int)(number / 100);
    number = number - (d2 * 100);
    // Get tens
    d3 = (int)(number / 10);
    number = number - (d3 * 10);
    // Get units
    d4 = (int)number;

    write(bitmap_table[d2], 2);
    delay(povDelay);
    //write(DATA_DARK, 2);

    write(bitmap_table[d3], 3);
    delay(povDelay);
    //write(DATA_DARK, 3);

    write(bitmap_table[d4], 4);
    delay(povDelay);
    //write(DATA_DARK, 4);
    break;
  }
  case 4:
  {
    // Get thousands
    d1 = (int)(number / 1000);
    number = number - (d1 * 1000);
    // Get hundreds
    d2 = (int)(number / 100);
    number = number - (d2 * 100);
    // Get tens
    d3 = (int)(number / 10);
    number = number - (d3 * 10);
    // Get units
    d4 = (int)number;

    write(bitmap_table[d1], 1);
    delay(povDelay);
    //write(DATA_DARK, 1);

    write(bitmap_table[d2], 2);
    delay(povDelay);
    //write(DATA_DARK, 2);

    write(bitmap_table[d3], 3);
    delay(povDelay);
    //write(DATA_DARK, 3);

    write(bitmap_table[d4], 4);
    delay(povDelay);
    //write(DATA_DARK, 4);

    break;
  }
  }
}

void setupCT1642(int clk,int data){
    CLK = clk;
    DATA = data;

    pinMode(CLK,OUTPUT);
    digitalWrite(CLK,LOW);
    pinMode(DATA,OUTPUT);
    digitalWrite(DATA,LOW);
}

void chao(){
    Serial.println("Ban da chay duoc trong ct1642");
}