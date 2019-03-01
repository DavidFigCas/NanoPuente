//
#include <IRremote.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_NeoPixel.h>

#define PIN  A0
#define PIN1 A3
#define PIN2 A2
#define PIN3 A1
#define PIN4 A0
#define NUM_PIXELS 56


Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, PIN, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(NUM_PIXELS, PIN1, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(NUM_PIXELS, PIN2, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel strip3 = Adafruit_NeoPixel(NUM_PIXELS, PIN3, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel strip4 = Adafruit_NeoPixel(NUM_PIXELS, PIN4, NEO_GRBW + NEO_KHZ800);




char tecla;
char index;
int rr = 128; //rojo
int gg = 255; //verde
int bb = 255; //azul
int ww = 255; //blanco

String valSerial = "0001"; // Variable del puerto serial
String valColor = "0127";
int color;
int numero;

const int   RECV_PIN = 7;
IRrecv irrecv(RECV_PIN);
decode_results results;

byte numbers[] = {
  B11101110,    // 0
  B10001000,    // 1
  B01111100,    // 2    5555
  B11011100,    // 3   6    4
  B10011010,    // 4    3333
  B11010110,    // 5   2    0
  B11110110,    // 6    1111
  B10001100,    // 7
  B11111110,    // 8
  B10011110    // 9
};
byte letters[] = {
  B10111110,    // A      55555
  B11110010,    // B     6     4
  B01110000,    // C     6     4
  B11111000,    // D      33333
  B01110110,    // E     2     0
  B00110110,    // F     2     0
  B11011110,    // G      11111
  B10111010,    // H
  B01100010,    // L
  B00000000     //
};

//----------------------------------------------------- configuraciones
void setup(){
  Serial.begin(115200);

  pinMode(5,OUTPUT); //Fake 5V and GND
  pinMode(6,OUTPUT);
  digitalWrite(5,HIGH);
  digitalWrite(6,LOW);
  
  strip.begin();
  delay(100);
  strip.show();
  delay(100);
  strip1.begin();
  delay(100);
  strip1.show();
  delay(100);
  strip2.begin();
  delay(100);
  strip2.show();
  delay(100);
  strip3.begin();
  delay(100);
  strip3.show();
  delay(100);
  strip4.begin();
  delay(100);
  strip4.show();
  delay(100);


  //irrecv.enableIRIn(); // Start the receiver
  Serial.println("Iniciado");
}


//--------------------------------------------------- loop
void loop() 
{
  while(Serial.available())
  { //Para checar el puerto serial
   {
     tecla = Serial.read(); //Lee el primer caracter de la cadena
     //while(Serial.available() == 0); //espera a la siguiente
     
     valSerial = Serial.readStringUntil(',');// read the serial value
     valColor = Serial.readStringUntil(')');
     
     color = valColor.toInt();
     numero = valSerial.toInt();
     
     if(numero >= 10000)
      numero = 0;
      
     switch(tecla)
      {
        case '|':
        Serial.print("1: ");
        Serial.print(numero);
        strip = strip1;
        displayNum(numero,color);
        break;
        
        case '/':
        Serial.print("2: ");
        Serial.print(numero);
        strip = strip2;
        displayNum(numero,color);
        break;
        
        case '*':
        Serial.print("3: ");
        Serial.print(numero);
        strip = strip3;
        displayNum(numero,color);
        break;
        
        case '!':
        Serial.print("4: ");
        Serial.print(numero);
        strip = strip4;
        displayNum(numero,color);
        break;

    }
   }
 }
  
  
//  if (irrecv.decode(&results)) 
//  { //Para revisar Infrarrojo
//    IRselect();
//    irrecv.resume(); // Receive the next value
//  }
  
  
}


//--------------------------------------------------- displayNum
void displayNum(uint16_t h, uint32_t col) 
{ 
 apagaPixels();


 
  uint16_t firstDigit = h / 1000;
  uint16_t secondDigit = (h % 1000)/100;
  uint16_t thirdDigit = ((h%1000)%100)/ 10;
  uint16_t fourthDigit = ((h%1000)%100)%10;
  int j=0; //Para la posicion del neopixel.
  int i = 0;

  //Serial.print("\t");
  //---------------------------------------- firstDigit
  for ( i = 0; i < 7; i++) 
  {
    if ((numbers[firstDigit] & (1 << 7 - i)) && (firstDigit > 0)) // Que sea diferente de cero 
    { //Revisa el byte específico del vector y decide si debe encenderlo de algún color o apagarlo
      strip.setPixelColor(j + (14 * 3), Wheel(col)); //Le asigna el color o...
      //delay(10);
      strip.setPixelColor(j+1 + (14 * 3), Wheel(col));
      //delay(10);
      //Serial.print("1");
    }
    else 
    {
      strip.setPixelColor(j + (14 * 3 ), 0,0,0,0); //...lo apaga
      //delay(10);
      strip.setPixelColor(j+1 + (14 * 3 + 2), 0,0,0,0);
      //delay(10);
      //Serial.print("0");
    } 
    j=j+2; //Posicion del Neopixel
  }
  // ---------------------------------------- secondDigit
  j=0;
  //Serial.print("\t");
  for (i = 0; i < 7; i++) 
  {
    if ((numbers[secondDigit] & (1 << 7 - i)) && ((secondDigit >= 1) || (firstDigit > 0))) //el cero no se muestra
    {
      strip.setPixelColor(j + (14*2),Wheel(col));
      //delay(10);
      strip.setPixelColor(j+1 + (14*2),Wheel(col));
      //delay(10);
      //Serial.print("1");
    }
    else 
    {
      strip.setPixelColor(j + (14*2), 0,0,0,0); 
      //delay(10);
      strip.setPixelColor(j+1 + (14*2), 0,0,0,0);
      //delay(10);
      //Serial.print("0");
    }
    j=j+2;
  }
   //-------------------------------------------- thirdDigit
   j=0;
   //Serial.print("\t");
  for (i = 0; i < 7; i++) 
  {
    if ((numbers[thirdDigit] & (1 << 7 - i)) && ((thirdDigit >= 1) || (secondDigit > 0) || (firstDigit > 0))) //el cero no se muestra
    {
      strip.setPixelColor(j + 14, Wheel(col));
      //delay(10);
      strip.setPixelColor(j+1 + 14,Wheel(col));
      //delay(10);
      //Serial.print("1");
    }
    else 
    {
      strip.setPixelColor(j + 14, 0,0,0,0); 
      //delay(10);
      strip.setPixelColor(j+1 + 14, 0,0,0,0);
      //delay(10);
      //Serial.print("0");
    }
    j=j+2;
  }
   // -------------------------------------------- fourthDigit
   j=0;
  //Serial.print("\t");
  for (i = 0; i < 7; i++) 
  {
    if (numbers[fourthDigit] & (1 << 7 - i)) 
    {
      strip.setPixelColor(j ,Wheel(col));
      //delay(10);
      strip.setPixelColor(j+1 , Wheel(col));
      //delay(10);
      //Serial.print("1");
    }
    else 
    {
      strip.setPixelColor(j , 0,0,0,0); 
      //delay(10);
      strip.setPixelColor(j+1 , 0,0,0,0);
      //delay(10);
      //Serial.print("0");
    }
    j=j+2;
  }
   strip.show();
   Serial.println();
}

//---------------------------------------------------------------------- apagaPixels
void apagaPixels()
{
    uint16_t i;
    for( i =  0; i < strip.numPixels(); i++) //
    {
      strip.setPixelColor(i,0,0,0,0);
      //delay(10);
    }  strip.show();
}

//---------------------------------- Input a value 0 to 255 to get a color value r - g - b - back to r.
uint32_t Wheel(byte WheelPos) 
{
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) 
  {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3,0);
  } 
  else if (WheelPos < 170) 
  {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3,0);
  } 
  else 
  {
    WheelPos -= 170;
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0,0);
  }
}

//----------------------------------------------------------------- IRselect
void IRselect()
{
    tecla = results.value;   
    switch(tecla)
    {
        case /**/ 0x20DF10EF: // ON
          Serial.println("on");
          
          break;
          
        case /**/ 0x20DFC23D: // MENU 
          Serial.println("menu"); 
          break;

        case /**/ 0x20DF906F: // MUTE 
          Serial.println("mute"); 
          break;
        case /*0xFF629D*/ 0x20DF40BF: // +
          Serial.println("+"); 
          break;
          
        case /*0xFFA857*/ 0x20DFC03F: // - 
          Serial.println("-"); 
          break; 

        case /*0xFF42BD*/ 0x20DF56A9: // *
          Serial.println("*"); 
          break;

        case /*0xFF02FD*/ 0x20DF22DD:  //ok
          Serial.println("ok"); 
          break;

        case /*0xFF6897*/ 0x20DF8877:  //1
          Serial.println("l"); 
          break;

        case /*0xFF9867*/ 0x20DF48B7:   //2
          Serial.println("2"); 
          break;

        case /*0xFFB04F*/ 0x20DFC837:  //3
          Serial.println("3"); 
          break;

        case /*0xFF30CF*/ 0x20DF28D7:  //4
           Serial.println("4"); 
          break;

        case /*0xFF18E7*/ 0x20DFA857:  //5
          Serial.println("5"); 
          break;

        case /*0xFF7A85*/ 0x20DF6897:  //6
          Serial.println("6"); 
          break;

        case /*0xFF10EF*/ 0x20DFE817:  //7
          Serial.println("7"); 
          break;

        case /*0xFF38C7*/ 0x20DF18E7:  //8
          Serial.println("8"); 
          break;

        case /*0xFF5AA5*/ 0x20DF9867:  //9
          Serial.println("9"); 
          break;

        case /*0xFF4AB5*/ 0x20DF08F7:  //0
          Serial.println("0"); 
          break;
    }
  
}
