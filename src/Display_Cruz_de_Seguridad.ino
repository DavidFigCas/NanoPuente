//Programa para la Cruz de Seguridad
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_NeoPixel.h>

#define PIN A0
#define NUM_PIXELS 369

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, PIN, NEO_GRBW + NEO_KHZ800);

int tecla;

 // Variables del puerto serial
String valDia= "0001";
String valFecha= "0001";
String valMesAnio= "0001";
int nfecha,ndia,nmesanio;

String valColor= "0127";
String valColorDef= "0127";
int color, fcolor;

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
//Cuántos segmentos ocupa cada número de día del mes
int segmDia[]{2,5,5,4,5,6,3,7,6,8,4,7,7,6,7,8,5,9,8,11,7,10,10,9,10,11,8,12,11,11,7};
//Suma total de segmentos iluminados hasta el día actual
int segmDiaTotal[]{2,7,12,16,21,27,30,37,43,51,55,62,69,75,82,90,95,104,112,123,130,140,150,159,169,180,188,200,211,222,229};
int colorFecha[31];

//----------------------------------------------------- configuraciones
void setup(){
  Serial.begin(115200);

  strip.begin();
  delay(100);
  strip.show();

  Serial.println("Iniciado");
}

//--------------------------------------------------- loop
void loop()
{
  if (Serial.available()>0)
  { //Para checar el puerto serial
     tecla = Serial.read();
     valFecha = Serial.readStringUntil(',');// read the serial value
     valDia = Serial.readStringUntil(',');
     valMesAnio = Serial.readStringUntil(',');

     valColor = Serial.readStringUntil(',');
     valColorDef = Serial.readStringUntil(')'); //El color de todo lo demás menos los días del mes

     nfecha = valFecha.toInt();
     ndia = valDia.toInt();
     nmesanio = valMesAnio.toInt();
     fcolor = valColor.toInt();
     color = valColorDef.toInt();

     Serial.print("Fecha: ");
     Serial.println(nfecha);
     Serial.print("Dia: ");
     Serial.println(ndia);
     Serial.print("MesAnio: ");
     Serial.println(nmesanio);

    displayNumFecha(nfecha,fcolor);
    displayNumDia(ndia,color);
    displayNumMesAnio(nmesanio,color);
  }
}


//--------------------------------------------------- displayNumDia---------------------------------------------------------------
void displayNumDia(uint16_t h, uint32_t col) // ARGUMENTOS: Número a mostrar, color para Wheel().
{
  uint16_t firstDigit = h / 1000;
  uint16_t secondDigit = (h % 1000)/100;
  uint16_t thirdDigit = ((h%1000)%100)/ 10;
  uint16_t fourthDigit = ((h%1000)%100)%10;
  int j = 0; //Para la posicion del neopixel.
  int i = 0;

  //---------------------------------------- firstDigit
  for ( i = 0; i < 7; i++)
  {
    if ((numbers[firstDigit] & (1 << 7 - i)) && (firstDigit > 0)) // Que sea diferente de cero
    { //Revisa el byte específico del vector y decide si debe encenderlo de algún color o apagarlo
      strip.setPixelColor(j + (21 * 3), Wheel(col)); //Le asigna el color o...
      strip.setPixelColor(j+1 + (21 * 3), Wheel(col));
      strip.setPixelColor(j+2 + (21 * 3), Wheel(col));
    }
    else
    {
      strip.setPixelColor(j + (21 * 3 ), 0,0,0,0); //...lo apaga
      strip.setPixelColor(j+1 + (21 * 3 + 2), 0,0,0,0);
      strip.setPixelColor(j+2 + (21 * 3 + 2), 0,0,0,0);
    }
    j=j+3; //Posicion del Neopixel
  }

  // ---------------------------------------- secondDigit
  j=0;
  for (i = 0; i < 7; i++)
  {
    if ((numbers[secondDigit] & (1 << 7 - i)) && ((secondDigit >= 1) || (firstDigit > 0))) //el cero no se muestra
    {
      strip.setPixelColor(j + (21*2),Wheel(col));
      strip.setPixelColor(j+1 + (21*2),Wheel(col));
      strip.setPixelColor(j+2 + (21*2),Wheel(col));
    }
    else
    {
      strip.setPixelColor(j + (21*2), 0,0,0,0);
      strip.setPixelColor(j+1 + (21*2), 0,0,0,0);
      strip.setPixelColor(j+2 + (21*2), 0,0,0,0);
    }
    j=j+3;
  }
   //-------------------------------------------- thirdDigit
   j=0;
  for (i = 0; i < 7; i++)
  {
    if ((numbers[thirdDigit] & (1 << 7 - i)) && ((thirdDigit >= 1) || (secondDigit > 0) || (firstDigit > 0))) //el cero no se muestra
    {
      strip.setPixelColor(j + 21, Wheel(col));
      strip.setPixelColor(j+1 + 21,Wheel(col));
      strip.setPixelColor(j+2 + 21,Wheel(col));
    }
    else
    {
      strip.setPixelColor(j + 21, 0,0,0,0);
      strip.setPixelColor(j+1 + 21, 0,0,0,0);
      strip.setPixelColor(j+2 + 21, 0,0,0,0);
    }
    j=j+3;
  }
   // -------------------------------------------- fourthDigit
   j=0;
  for (i = 0; i < 7; i++)
  {
    if (numbers[fourthDigit] & (1 << 7 - i))
    {
      strip.setPixelColor(j ,Wheel(col));
      strip.setPixelColor(j+1 , Wheel(col));
      strip.setPixelColor(j+2 , Wheel(col));
    }
    else
    {
      strip.setPixelColor(j , 0,0,0,0);
      strip.setPixelColor(j+1 , 0,0,0,0);
      strip.setPixelColor(j+2 , 0,0,0,0);
    }
    j=j+3;
  }
   strip.show();
}

//--------------------------------------------------- displayNumMesAnio----------------------------------------------------
void displayNumMesAnio(uint16_t h, uint32_t col)
{
  uint16_t firstDigit = h / 1000;
  uint16_t secondDigit = (h % 1000)/100;
  uint16_t thirdDigit = ((h%1000)%100)/ 10;
  uint16_t fourthDigit = ((h%1000)%100)%10;
  int j = 0;
  int i = 0;

  //---------------------------------------- firstDigit
  for ( i = 0; i < 7; i++)
  {
    if ((numbers[firstDigit] & (1 << 7 - i)) && (firstDigit > 0))
    {
      strip.setPixelColor(j + (14 * 3), Wheel(col));
      strip.setPixelColor(j+1 + (14 * 3), Wheel(col));
    }
    else
    {
      strip.setPixelColor(j + (14 * 3 ), 0,0,0,0);
      strip.setPixelColor(j+1 + (14 * 3 + 2), 0,0,0,0);
    }
    j=j+2;
  }

  // ---------------------------------------- secondDigit
  j=0;
  for (i = 0; i < 7; i++)
  {
    if ((numbers[secondDigit] & (1 << 7 - i)) && ((secondDigit >= 1) || (firstDigit > 0)))
    {
      strip.setPixelColor(j + (14*2),Wheel(col));
      strip.setPixelColor(j+1 + (14*2),Wheel(col));
    }
    else
    {
      strip.setPixelColor(j + (14*2), 0,0,0,0);
      strip.setPixelColor(j+1 + (14*2), 0,0,0,0);
    }
    j=j+2;
  }
   //-------------------------------------------- thirdDigit
   j=0;
  for (i = 0; i < 7; i++)
  {
    if ((numbers[thirdDigit] & (1 << 7 - i)) && ((thirdDigit >= 1) || (secondDigit > 0) || (firstDigit > 0)))
    {
      strip.setPixelColor(j + 14, Wheel(col));
      strip.setPixelColor(j+1 + 14,Wheel(col));
    }
    else
    {
      strip.setPixelColor(j + 14, 0,0,0,0);
      strip.setPixelColor(j+1 + 14, 0,0,0,0);
    }
    j=j+2;
  }
   // -------------------------------------------- fourthDigit
   j=0;
  for (i = 0; i < 7; i++)
  {
    if (numbers[fourthDigit] & (1 << 7 - i))
    {
      strip.setPixelColor(j ,Wheel(col));
      strip.setPixelColor(j+1 , Wheel(col));
    }
    else
    {
      strip.setPixelColor(j , 0,0,0,0);
      strip.setPixelColor(j+1 , 0,0,0,0);
    }
    j=j+2;
  }
   strip.show();
}
//--------------------------------------------------- displayNumFecha--------------------------------------------------------------
void displayNumFecha(uint16_t h, uint32_t col)
{
   uint16_t i;
   for( i =  0; i < strip.numPixels(); i++) //
   {
     strip.setPixelColor(i,0,0,0,0);
   }  strip.show();
   strip.show();
}
//---------------------------------------------------------------------- apagaPixels
void apagaPixels()
{
    uint16_t i;
    for( i =  0; i < strip.numPixels(); i++) //
    {
      strip.setPixelColor(i,0,0,0,0);
    }  strip.show();
}

//---------------------------------- Input a value 0 to 255 to get a color value r - g - b - back to r + a number (0-255) for brightness!!
uint32_t Wheel(byte WheelPos)
{
  if (WheelPos == 0)
  { //Para el color blanco
    return strip.Color(0, 0, 0, 255);
  }

  WheelPos = 255 - WheelPos;
  if (WheelPos < 85 && WheelPos >0 )
  {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  else if (WheelPos < 170)
  {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  else
  {
    WheelPos -= 170;
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}
