

//////////////////////////////////////Khai bao///////////////////////

#include <LiquidCrystal.h> // Thư viện LCD
#define Temp A0// Do nhiet do
#define SetA A1// 
#define Ampe A2// Do dong dien
#define Volt A3// Do dien ap
#define CCCV A5// đo CCCV
#define Relay 2// Chan kich hoat relay
#define Key1 3// 
#define Fan 4// 
#define Buzz 11// 
#define AVG_NUM 10// 


////////////////////////////////////Khai bao bien///////////////////

float Val_Temp=0;
float Val_SetA=0;
float Val_Ampe=0;
float Val_Volt=0;
float Val_CCCV=0;
float Load_Status=0;
float msec=0;
float last_msec=0;
float elasped_msec=0;
float elasped_time=0;
float ampSecs = 0;
float ampHours=0;
float watts=0;
float wattSecs = 0;
float wattHours=0;
int LCDstatus =0;
int Relaystatus =0;
int Vo;
float R1 = 10000;
float logR2, R2, T, Tc, Tf;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;
unsigned long time1 = 0;


////////////////////////////////////////////////////////////////////


byte SET_A[8] =  // icon for SET_AMP
{
  0b00010,0b00100,0b01000,0b11111,0b00010,0b00100,0b01000,0b00000
};

byte C1[8] =  // icon for [C
{
  0b11111,0b11111,0b11001,0b10111,0b10111,0b10111,0b11001,0b11111
  
};

byte C2[8] =  // icon for  C]
{
  0b11111,0b11111,0b11001,0b10111,0b10111,0b10111,0b11001,0b11111
};

byte V[8] =  // icon for  V]
{
  0b11111,0b11111,0b10101,0b10101,0b10101,0b10101,0b11011,0b11111
};

byte V2[8] =  // icon for temp
{
  0b00100,0b01010,0b01010,0b01110,0b01110,0b11111,0b11111,0b01110
};

byte V3[8] =  // icon for *
{
  0b00111,0b00101,0b00111,0b00000,0b00000,0b00000,0b00000,0b00000
};

byte V4[8] =  // icon for .
{
  0b00000,0b00000,0b00100,0b01110,0b00100,0b00000,0b00000,0b00000
};

byte V5[8] =  // icon for A
{
  0b00000,0b00000,0b00100,0b01010,0b10001,0b11111,0b10001,0b00000
};



LiquidCrystal lcd(9, 10, 5, 6, 7, 8);// Khai bao chan LCD
/////////////////////////////////Chong trinh chinh//////////////////

void setup()////////////////////Cai dat chong trinh////////////////
{

Serial.begin(9600);
pinMode(Key1,INPUT_PULLUP);
pinMode(Relay,OUTPUT);
pinMode(Fan,OUTPUT);
pinMode(Buzz,OUTPUT);
lcd.createChar(1,SET_A);
lcd.createChar(2,C1);
lcd.createChar(3,C2);
lcd.createChar(4,V);
lcd.createChar(5,V2);
lcd.createChar(6,V3);
lcd.createChar(7,V4);
lcd.createChar(8,V5);


lcd.begin(16,2);   // Khai bao man hinh
lcd.clear();
lcd.setCursor(4, 0);
lcd.print("Booting!");
digitalWrite(Buzz,HIGH);
delay (100);
digitalWrite(Buzz,LOW);
delay(2000);

lcd.clear();
}

/////////////////////////////////Lap chuong trinh///////////////////
void loop()
{
 Read_data();             // 
 power();                 //
 Relay_control();
 print_data();            // 
 lcd_display();           // 
 delay(20);
}

///////////////////////////////Cac chuong trinh con////////////////



//---------------------------- Tinh gia tri trung binh ADC--------------------------------------------------
int read_adc(int adc_parameter)
{
  
  int sum = 0;
  int sample ;
  for (int i=0; i<AVG_NUM; i++) 
  {                                        // 
    sample = analogRead(adc_parameter);    // 
    sum += sample;                        // 
    delayMicroseconds(50);              // Dung 50ms 
  }
  return(sum / AVG_NUM);                // 
}
//----------------------------------------------------------------------------------------------------------


////////////////////////////////////Doc gia tri ADC/////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------
 void Read_data(void) 
 {
    //5V = ADC value 1024 => 1 ADC value = (5/1024)Volt= 0.0048828Volt
    // Vout=Vin*R2/(R1+R2) => Vin = Vout*(R1+R2)/R2   R1=200k and R2=10k
     Val_Volt = read_adc(Volt)*0.00485*(210/10); // Doc dien ap
     Val_SetA = read_adc(SetA)*0.00485*(120/20);//
     Val_Ampe = (read_adc(Ampe)*.07386 -37.75);
     Val_CCCV = analogRead(CCCV);
     Vo = analogRead(Temp);//Do nhiet do. NTC10K. R10K 1%
     R2 = R1 * (1023.0 / (float)Vo - 1.0);
     logR2 = log(R2);
     T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
     Val_Temp = T - 273.15;
  }
  //----------------------------------------------------------------------------------------------------------

  /////////////////////////////////Tinh toan cong suat Wh ////////////////////////////////////////////////////
//------------------------------------------------------------------------------------------------------------
void power(void)

{
msec = millis();
elasped_msec = msec - last_msec; //
elasped_time = elasped_msec / 1000.0; //
watts = Val_Ampe * Val_Volt; //
ampSecs = (Val_Ampe * elasped_time); //
wattSecs = ampSecs * Val_Volt; //
ampHours = ampHours + ampSecs/3600; // 
wattHours = wattHours + wattSecs/3600; // 
last_msec = msec; //
}

//------------------------------------------------------------------------------------------------------------

/////////////////////////////////In du lieu len may tinh//////////////////////////////////////////////////////
//------------------------------------------------------------------------------------------------------------
  void print_data(void) 
  {
    delay(20);
    Serial.print("Dien ap: ");
    Serial.print(Val_Volt);
    Serial.println("V");
    Serial.print("Dong dien cai dat: ");
    Serial.print(Val_SetA);
    Serial.println("A");
    Serial.print("Dong dien: ");
    Serial.print(Val_Ampe);
    Serial.println("A");
    Serial.print("Nhiet do:");
    Serial.println(Val_Temp);
    Serial.print("*C");
    Serial.print("Cong suat: ");
    Serial.print( watts);
    Serial.println("***************************");
 }
//----------------------------------------------------------------------------------------------------------------------


//////////////////////// LCD DISPLAY///////////////////////////////////////////////////////////
//------------------------------------------------------------------------------------------------------
void lcd_display()
  
   {
//---------------------------có
   lcd.setCursor(0, 0);
   lcd.print(Val_Volt,1);
   lcd.setCursor(4, 0);
   lcd.print("v|");
//---------------------------co
   lcd.setCursor(0, 1);
   lcd.print(Val_Ampe,1);
   lcd.setCursor(4, 1);
   lcd.write(8);
   lcd.print("|");

//---------------------------co
   lcd.setCursor(6, 0);
   lcd.print(watts,1);
   lcd.setCursor(10, 0);
   lcd.print("W");
//---------------------------ko
   if(Val_Ampe < Val_SetA)
    {
     lcd.setCursor(14, 0);
     lcd.write(2);
     lcd.setCursor(15, 0);
     lcd.write(4); 
    }
    else
    {
     lcd.setCursor(14, 0);
     lcd.write(2);
     lcd.setCursor(15, 0);
     lcd.write(3);   
    }
//--------------------------   
    if (Val_Temp > 45)
       {
       digitalWrite(Fan, HIGH);
       digitalWrite(Buzz,HIGH);
       delay (50);
       digitalWrite(Buzz,LOW);   
       }
    else
       {
       digitalWrite(Fan, LOW);   
       }
//--------------------------
    if(Relaystatus==1)
     {
      lcd.setCursor(12, 0);
      lcd.print("*");
     }
   else if(Relaystatus==0)
     {
      lcd.setCursor(12, 0);
      lcd.write(7);
     }
//---------------------------
   lcd.setCursor(13, 1);
   lcd.print(Val_Temp,0);
   lcd.setCursor(15, 1);
   lcd.write(6);
//---------------------------ko
{
    if ( (unsigned long) (millis() - time1) > 4000 )
    {
        if ( LCDstatus==0 )
        {
            LCDstatus=1;
        } else {
            LCDstatus=0;
        }
        time1 = millis();
    }


//--------------------------   
    
 if ( LCDstatus==0 )
   

//---------------------------ko
 {
   lcd.setCursor(6, 1);
   lcd.print(wattHours,1);
   lcd.setCursor(10, 1);
   lcd.print("Wh");

 }
//---------------------------ko
   if ( LCDstatus==1 )
   {
   lcd.setCursor(6, 1);
   lcd.print(ampHours,1);
   lcd.setCursor(10, 1);
   lcd.print("Ah");
   }
//---------------------------ko
 }
   }

  

 

 


//-------------------------------------------------------------------------------------------------

/////////////////////////////////////////////Relay CONTROL/////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------  
  void Relay_control()
{
 if(digitalRead(Key1) == 0 && Relaystatus == 0) 
   {   
       digitalWrite(Relay,HIGH);
       digitalWrite(Buzz,HIGH);
       delay (50);
       digitalWrite(Buzz,LOW);
       delay(300); // Chờ buông tay.
       Relaystatus = 1;
       
    }
    
  if(digitalRead(Key1) == 0 && Relaystatus == 1)
    {   
       digitalWrite(Relay,LOW );
       digitalWrite(Buzz,HIGH);
       delay (50);
       digitalWrite(Buzz,LOW);
       delay(300); // Chờ buông tay.
       Relaystatus = 0;
    }

   

 }





//-------------------------------------------------------------------------------------------------





  