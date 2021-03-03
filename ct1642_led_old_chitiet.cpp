#include <Arduino.h>

#define _Data_Pin D6
#define _CLK_Pin D5
int povDelay = 1000;

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
  Serial.println((byte)digitNo);
  for (int i = 0; i < 4; i++)
  {
    digitalWrite(_CLK_Pin, LOW);
    if ((digitNo << i) & 0x80)
    {
      digitalWrite(_Data_Pin, HIGH);
      Serial.print("1");
    }
    else
    {

      Serial.print("0");
      digitalWrite(_Data_Pin, LOW);
    }

    digitalWrite(_CLK_Pin, HIGH);
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

  // switch (digitNo)
  // {
  // case 1:
  // {
  //   // X000
  //   // 0x7f = DEC-127, BIN-B01111111
  //   digit = 0x7f;
  //   break;
  // }
  // case 2:
  // {
  //   // 0X00
  //   // 0xbf = DEC-191, BIN-10111111
  //   digit = 0xbf;
  //   break;
  // }
  // case 3:
  // {
  //   // 00:X0
  //   // 0xdf = DEC-223, BIN-11011111
  //   digit = 0xdf;
  //   break;
  // }
  // case 4:
  // {
  //   // 00:0X
  //   // 0xef = DEC-239, BIN-11101111
  //   digit = 0xef;
  //   break;
  // }
  // case 5:
  // {
  //   /* Switch OFF the display for the keyboard scan */
  //   // 0xff = DEC-255, BIN-11111111
  //   digit = 0xff;
  //   break;
  // }
  // default:
  // {
  //   /* Display is switched off by default */
  //   digit = 0xff;
  //   break;
  // }
  //   return digit;
}

void setPreamble()
{
  //Bo 6 bit khong dung
  for (int i = 0; i < 6; i++)
  {
    digitalWrite(_Data_Pin, HIGH);
    digitalWrite(_CLK_Pin, LOW);
    digitalWrite(_CLK_Pin, HIGH);
  }
}

void setChar(byte ledChar)
{
  for (int i = 0; i < 8; i++)
  {
    digitalWrite(_CLK_Pin, LOW);
    if ((ledChar << i) & 0x80)
      digitalWrite(_Data_Pin, HIGH);
    else
      digitalWrite(_Data_Pin, LOW);
    digitalWrite(_CLK_Pin, HIGH);
  }
}

void latchDigit()
{
  digitalWrite(_Data_Pin, LOW);
  digitalWrite(_Data_Pin, HIGH);
  digitalWrite(_CLK_Pin, LOW);
  digitalWrite(_Data_Pin, LOW);
  digitalWrite(_Data_Pin, HIGH);
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

void clear()
{
  write(0x00, 1);
  write(0x00, 2);
  write(0x00, 3);
  write(0x00, 4);
}

void caiDat()
{
  pinMode(_Data_Pin, OUTPUT);
  digitalWrite(_Data_Pin, LOW);
  pinMode(_CLK_Pin, OUTPUT);
  digitalWrite(_CLK_Pin, LOW);

  //clear();
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

void showSingle(int number, int digit)
{
  int toWrite = 0;

  if (number >= 0 && number <= 9)
  {
    toWrite = number;
  }

  write((byte)bitmap_table[toWrite], digit);
  delay(povDelay);
  // clear();
}

void test(int data)
{
  digitalWrite(D5, LOW);
  if (data == 1)
    digitalWrite(D6, HIGH);
  else
    digitalWrite(D6, LOW);
  digitalWrite(D5, HIGH);
}

void setup()
{
  Serial.begin(9600);
  pinMode(16, OUTPUT);
  analogWrite(16, LOW);
  caiDat();
  //showSingle(7,3);

  //1
  byte DigitAddress = 0;

  //Lay dia chi cua led digitNo
  //vi du 1110 hien cai thu 4
  DigitAddress = getDigitAddress(3);

  //cai dat 4 bit dau tien
  setUpDigit(DigitAddress);
  //4
  
  //tu 5
  setPreamble();
  //den 10
  
  byte a =bitmap_table[9];
  
  setChar(a);
  
  latchDigit();
}
int count = 0;
const long interval = 200;
unsigned long prevousMilius = 0;

void loop()
{
  // unsigned long currentMilis = millis();

  // if(currentMilis - prevousMilius >=interval){
  //   prevousMilius = currentMilis;
  //   count++;
  //   Serial.print(count);
  // }
  // showNumber(count);
}