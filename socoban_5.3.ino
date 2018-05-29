#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

// For the Adafruit shield, these are the default.
#define TFT_DC 9
#define TFT_CS 10


#define PASILLO 0
#define PARED 1
#define OBJETIVO 2
#define SCENE_SIZE 10
#define MAX_CAJAS 10
#define HOR_JOY A8
#define VER_JOY A9
#define ARRIBA 0
#define DERECHA 1
#define ABAJO 2
#define IZQUIERDA 3
#define UP_TH 850
#define DOWN_TH 150
#define LEFT_TH 150
#define RIGHT_TH 850

#define lvl1 4
#define lvl2 5
#define lvl3 6




// Use hardware SPI (on Uno, #13, #12, #11) and the above for CS/DC
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
// If using the breakout, change pins as desired
//Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);
int sensorValue = 0;        // value read from the pot
int outputValue = 0;        // value output to the PWM (analog out)
int i ,j ;
int x=0,y=0,xx=0,xxx,yy=0,yyy,xo,yo;
unsigned char mat[10][10];
unsigned long cargadoniv();
char dato;
char S;
int datox,xant,yant ;
int datoy ;
int conter = 0;
int conCa =0;

// pasillo
const unsigned char pasilloBitmap [] PROGMEM = {
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x3f, 0xf0, 0x0f, 0xe0, 0x07, 0xc0, 0x07, 0xc3, 0xc3, 
0xc7, 0xc3, 0xc3, 0xc7, 0xe0, 0x07, 0xf0, 0x0f, 0xf8, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
};

// PARED 
const unsigned char paredBitmap [] PROGMEM = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x84, 0x00, 0x21, 0x84, 
0x00, 0x21, 0x84, 0x00, 0x21, 0xff, 0xff, 0xff, 0x80, 0x18, 0x01, 0x80, 0x18, 0x01, 0x80, 0x18, 
0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x84, 0x00, 0x21, 0x84, 0x00, 0x21, 0x84, 0x00, 0x21, 
0xff, 0xff, 0xff, 0x80, 0x18, 0x01, 0x80, 0x18, 0x01, 0x80, 0x18, 0x01, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};

// JUGADOR MONITO
const unsigned char jugadorBitmap [] PROGMEM = {
0x00, 0x70, 0x00, 0x01, 0xf0, 0x00, 0x03, 0xfc, 0x00, 0x07, 0xfc, 0x00, 0x0f, 0xfe, 0x00, 0x0f, 
0xfe, 0x00, 0x0f, 0xca, 0x00, 0x0f, 0xba, 0x00, 0x07, 0xba, 0x3e, 0x0f, 0xc7, 0xdf, 0x1f, 0xff, 
0x9f, 0x3d, 0xff, 0x9f, 0x3e, 0xff, 0x9e, 0x7f, 0x7f, 0xbc, 0x7f, 0x88, 0x00, 0x7f, 0x88, 0x00, 
0x7f, 0xd8, 0x00, 0x3f, 0xd8, 0x00, 0x0f, 0xfc, 0x00, 0x0f, 0xfe, 0x00, 0x1f, 0xbf, 0x00, 0x7f, 
0x1f, 0xc0, 0x81, 0x10, 0x20, 0xff, 0x1f, 0xe0, 
};

// COFRE 
const unsigned char cofreBitmap [] PROGMEM = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xf0, 0x03, 0xff, 0xe0, 0x01, 0xff, 0xc0, 0x10, 
0xff, 0x84, 0x18, 0x7f, 0x0c, 0x1c, 0x3e, 0x1c, 0x1e, 0x1c, 0x3c, 0x1f, 0x08, 0xfc, 0x1f, 0x81, 
0xfc, 0x1f, 0xc3, 0xfc, 0x1f, 0xc1, 0xfc, 0x1f, 0x80, 0xfc, 0x1f, 0x18, 0x7c, 0x1e, 0x3c, 0x3c, 
0x1c, 0x7e, 0x1c, 0x18, 0xff, 0x0c, 0x11, 0xff, 0x84, 0x03, 0xff, 0xc0, 0x07, 0xff, 0xe0, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};

// OBJETIVO
const unsigned char objetivoBitmap [] PROGMEM = {
0xff, 0xff, 0xff, 0xfc, 0x00, 0x3f, 0xfe, 0x00, 0x7f, 0xff, 0x00, 0xff, 0xff, 0x81, 0xff, 0xff, 
0x81, 0xff, 0xbf, 0xc3, 0xfd, 0x9f, 0xc3, 0xf9, 0x8f, 0xc3, 0xf1, 0x83, 0xe7, 0xc1, 0x80, 0x66, 
0x01, 0x80, 0x00, 0x01, 0x80, 0x00, 0x01, 0x80, 0x66, 0x01, 0x83, 0xe7, 0xc1, 0x8f, 0xc3, 0xf1, 
0x9f, 0xc3, 0xf9, 0xbf, 0xc3, 0xfd, 0xff, 0x81, 0xff, 0xff, 0x81, 0xff, 0xff, 0x00, 0xff, 0xfe, 
0x00, 0x7f, 0xfc, 0x00, 0x3f, 0xff, 0xff, 0xff, 
};

// MAPA 1
const unsigned char nivel1[10][10]=
                   { {1,1,1,1,1,1,1,1,1,1 },
                     {1,1,1,1,0,2,2,0,1,1 },
                     {1,1,1,1,0,0,0,0,1,1 },
                     {1,1,1,1,1,0,1,1,1,1 },
                     {1,1,1,1,1,0,1,1,1,1 },
                     {1,1,0,0,0,0,1,1,1,1 },
                     {1,1,0,1,0,0,0,1,1,1 },
                     {1,1,0,0,0,1,0,1,1,1 },
                     {1,1,1,1,0,0,0,1,1,1 },
                     {1,1,1,1,1,1,1,1,1,1 }};

const unsigned char n1cajas = 2;
const unsigned char n1PosInicialCajas [2][2]= { {5,2},{6,2}};
const unsigned char n1PosInicialJugador[] = {7, 1};
unsigned char posJugador[2];

//MAPA 2

const unsigned char nivel2[10][10]=
                   { {1,1,1,1,1,1,1,1,1,1 },
                     {1,1,1,1,1,1,1,1,1,1 },
                     {1,1,0,0,0,1,0,0,1,1 },
                     {1,1,0,1,0,1,0,2,1,1 },
                     {1,1,0,0,0,0,0,2,1,1 },
                     {1,1,0,1,0,1,0,2,1,1 },
                     {1,1,0,0,0,1,0,0,1,1 },
                     {1,1,1,1,1,1,0,0,1,1 },
                     {1,1,1,1,1,1,1,1,1,1 },
                     {1,1,1,1,1,1,1,1,1,1 }};

const unsigned char n2cajas = 3;
const unsigned char n2PosInicialCajas [3][2]= { {6,4},{6,5},{6,3}};
const unsigned char n2PosInicialJugador[] = {7, 7};

// MAPA 3

const unsigned char nivel3[10][10]=
                   { {1,1,1,1,1,1,1,1,1,1 },
                     {1,2,0,0,1,0,2,2,1,1 },
                     {1,1,0,0,1,0,0,1,1,1 },
                     {1,0,0,1,1,1,0,1,1,1 },
                     {1,1,0,0,1,1,0,1,1,1 },
                     {1,1,0,0,0,0,0,0,1,1 },
                     {1,1,0,0,0,0,0,0,1,1 },
                     {1,1,0,0,0,0,1,0,1,1 },
                     {1,1,1,1,1,1,1,1,1,1 },
                     {0,0,0,0,0,0,0,0,0,0 }};

const unsigned char n3cajas = 3;
const unsigned char n3PosInicialCajas [3][2]= { {5,6},{5,7},{6,7}};
const unsigned char n3PosInicialJugador[] = {8, 8};


// MAPA 4
const unsigned char nivel4 [10][10]= 
                   { {1,1,1,1,1,1,1,1,1,1 },
                     {1,1,1,0,0,0,0,0,1,1 },
                     {1,1,1,0,0,0,0,0,1,1 },
                     {1,1,1,1,1,0,1,0,1,1 },
                     {1,0,0,0,0,0,0,0,1,1 },
                     {1,0,0,1,0,1,1,0,1,1 },
                     {1,2,2,0,0,0,0,0,0,1 },
                     {1,2,2,0,0,0,0,0,0,1 },
                     {1,1,1,1,1,1,1,1,1,1 },
                     {1,1,1,1,1,1,1,1,1,1 }};
                    
unsigned char cajas [10][10];
const unsigned char n4cajas = 4;
const unsigned char n4PosInicialCajas [4][4]= { {5,2},{3,4},{5,5},{4,6}};
const unsigned char n4PosInicialJugador[] = {3, 6};

// MAPA 5

const unsigned char nivel5[10][10]=
                   { {1,1,1,1,1,1,1,1,1,1 },
                     {1,1,1,1,1,1,1,1,1,1 },
                     {1,0,0,0,1,1,1,1,1,1 },
                     {1,0,1,0,1,0,0,0,1,1 },
                     {1,0,0,0,0,0,0,0,1,1 },
                     {1,2,2,1,0,1,0,1,1,1 },
                     {1,0,0,0,0,0,0,1,1,1 },
                     {1,2,2,0,0,1,1,1,1,1 },
                     {1,1,1,1,1,1,1,1,1,1 },
                     {1,1,1,1,1,1,1,1,1,1 }};


const unsigned char n5cajas = 4;
const unsigned char n5PosInicialCajas [4][2]= { {3,5},{6,5},{2,7},{4,8}};
const unsigned char n5PosInicialJugador[] = {7, 7};

void moverJugador(unsigned char movimiento, 
                  unsigned char posicion[2], 
                  const unsigned char nivel[SCENE_SIZE][SCENE_SIZE] )
{
   unsigned char x0 = posicion[0];
   unsigned char y0 = posicion[1];

     switch(movimiento)
       {
         case ARRIBA:
          if(nivel[y0 - 1][x0] != PARED)
            {
            if(cajas[y0 - 1][x0] == 1)
              {
              if(nivel[y0 - 2][x0] != PARED)
                {
                 cajas[y0 - 1][x0] = 0;
                 cajas[y0 - 2][x0] = 1;
                 posicion[1] = y0 - 1;
                 dibujarElemento(x0, y0 - 1, nivel[y0 - 1][x0]);
                 dibujarElemento(x0, y0, nivel[y0][x0]);
                 conter++;
                 conCa++;

                    tft.setRotation(1);
                    tft.setCursor(250,130);
                    tft.setTextColor(ILI9341_BLACK); tft.setTextSize(2);
                    tft.println(conCa-1);

                    tft.setRotation(1);
                    tft.setCursor(250,130);
                    tft.setTextColor(ILI9341_YELLOW); tft.setTextSize(2);
                    tft.println(conCa);

                    tft.setRotation(1);
                    tft.setCursor(250,50);
                    tft.setTextColor(ILI9341_BLACK); tft.setTextSize(2);
                    tft.println(conter-1);

                    tft.setRotation(1);
                    tft.setCursor(250,50);
                    tft.setTextColor(ILI9341_YELLOW); tft.setTextSize(2);
                    tft.println(conter);

                 Serial.println(conCa);
                }
              }
                 else
                  {
                    dibujarElemento(x0, y0, nivel[y0][x0]);
                    posicion[1] = y0 - 1;         
                  conter++;
                //  Serial.println(conter);
                    tft.setRotation(1);
                    tft.setCursor(250,50);
                    tft.setTextColor(ILI9341_BLACK); tft.setTextSize(2);
                    tft.println(conter-1);

                    tft.setRotation(1);
                    tft.setCursor(250,50);
                    tft.setTextColor(ILI9341_YELLOW); tft.setTextSize(2);
                    tft.println(conter);
                  } 
            }
    break;

    case DERECHA:
      if(nivel[y0][x0 + 1] != PARED)
      {
        if(cajas[y0][x0 + 1] == 1)
        {
          if(nivel[y0][x0 + 2] != PARED)
          {
            cajas[y0][x0 + 1] = 0;
            cajas[y0][x0 + 2] = 1;
            posicion[0] = x0 + 1;
            dibujarElemento(x0 + 1, y0, nivel[y0][x0 + 1]);
            dibujarElemento(x0, y0, nivel[y0][x0]);
          conter++;
          conCa++;

                    tft.setRotation(1);
                    tft.setCursor(250,130);
                    tft.setTextColor(ILI9341_BLACK); tft.setTextSize(2);
                    tft.println(conCa-1);

                    tft.setRotation(1);
                    tft.setCursor(250,130);
                    tft.setTextColor(ILI9341_YELLOW); tft.setTextSize(2);
                    tft.println(conCa);

                    tft.setRotation(1);
                    tft.setCursor(250,50);
                    tft.setTextColor(ILI9341_BLACK); tft.setTextSize(2);
                    tft.println(conter-1);


                    tft.setRotation(1);
                    tft.setCursor(250,50);
                    tft.setTextColor(ILI9341_YELLOW); tft.setTextSize(2);
                    tft.println(conter);

          Serial.println(conCa);
          }
        }
        else
        {
          dibujarElemento(x0, y0, nivel[y0][x0]);
          //actualizar la posicion
          posicion[0] = x0 + 1;
          conter++;
                    tft.setRotation(1);
                    tft.setCursor(250,50);
                    tft.setTextColor(ILI9341_BLACK); tft.setTextSize(2);
                    tft.println(conter-1);

                    tft.setRotation(1);
                    tft.setCursor(250,50);
                    tft.setTextColor(ILI9341_YELLOW); tft.setTextSize(2);
                    tft.println(conter);

        } 
      }
      
    break;
    case ABAJO:
      if(nivel[y0 + 1][x0] != PARED)
      {
        if(cajas[y0 + 1][x0] == 1)
        {
          if(nivel[y0 + 2][x0] != PARED)
          {
            cajas[y0 + 1][x0] = 0;
            cajas[y0 + 2][x0] = 1;
            posicion[1] = y0 + 1;
            dibujarElemento(x0, y0 + 1, nivel[y0 + 1][x0]);
            dibujarElemento(x0, y0, nivel[y0][x0]);
          conter++;
          conCa++;

                    tft.setRotation(1);
                    tft.setCursor(250,130);
                    tft.setTextColor(ILI9341_BLACK); tft.setTextSize(2);
                    tft.println(conCa-1);

                    tft.setRotation(1);
                    tft.setCursor(250,130);
                    tft.setTextColor(ILI9341_YELLOW); tft.setTextSize(2);
                    tft.println(conCa);

                    tft.setRotation(1);
                    tft.setCursor(250,50);
                    tft.setTextColor(ILI9341_BLACK); tft.setTextSize(2);
                    tft.println(conter-1);

                    tft.setRotation(1);
                    tft.setCursor(250,50);
                    tft.setTextColor(ILI9341_YELLOW); tft.setTextSize(2);
                    tft.println(conter);

          Serial.println(conCa);
          }
        }
        else
        {
          dibujarElemento(x0, y0, nivel[y0][x0]);
          posicion[1] = y0 + 1;         
        conter++;

                    tft.setRotation(1);
                    tft.setCursor(250,50);
                    tft.setTextColor(ILI9341_BLACK); tft.setTextSize(2);
                    tft.println(conter-1);

                    tft.setRotation(1);
                    tft.setCursor(250,50);
                    tft.setTextColor(ILI9341_YELLOW); tft.setTextSize(2);
                    tft.println(conter);
        } 
      }
    
    break;
    case IZQUIERDA:
      if(nivel[y0][x0 - 1] != PARED)
      {
        if(cajas[y0][x0 - 1] == 1)
        {
          if(nivel[y0][x0 - 2] != PARED)
          {
            cajas[y0][x0 - 1] = 0;
            cajas[y0][x0 - 2] = 1;
            posicion[0] = x0 - 1;
            dibujarElemento(x0 - 1, y0, nivel[y0][x0 - 1]);
            dibujarElemento(x0, y0, nivel[y0][x0]);
          conter++;
          conCa++;

                    tft.setRotation(1);
                    tft.setCursor(250,130);
                    tft.setTextColor(ILI9341_BLACK); tft.setTextSize(2);
                    tft.println(conCa-1);

                    tft.setRotation(1);
                    tft.setCursor(250,130);
                    tft.setTextColor(ILI9341_YELLOW); tft.setTextSize(2);
                    tft.println(conCa);

                    tft.setRotation(1);
                    tft.setCursor(250,50);
                    tft.setTextColor(ILI9341_BLACK); tft.setTextSize(2);
                    tft.println(conter-1);

                    tft.setRotation(1);
                    tft.setCursor(250,50);
                    tft.setTextColor(ILI9341_YELLOW); tft.setTextSize(2);
                    tft.println(conter);

          Serial.println(conCa);
          }
        }
        else
        {
          dibujarElemento(x0, y0, nivel[y0][x0]);
          //actualizar la posicion
          posicion[0] = x0 - 1;
          conter++;

                    tft.setRotation(1);
                    tft.setCursor(250,50);
                    tft.setTextColor(ILI9341_BLACK); tft.setTextSize(2);
                    tft.println(conter-1);

                    tft.setRotation(1);
                    tft.setCursor(250,50);
                    tft.setTextColor(ILI9341_YELLOW); tft.setTextSize(2);
                    tft.println(conter);
        } 
      }
    break;
  }
  
  dibujarJugador(posJugador[0], posJugador[1]);
  dibujarCajas(cajas);
}




void llenarCajas( unsigned char cajas[SCENE_SIZE][SCENE_SIZE],
                  const unsigned char nCajas, 
                  const unsigned char posCajas[MAX_CAJAS][2])
{
  unsigned char x, y;
  for(int caja = 0; caja < nCajas; caja++)
  {
    x = posCajas[caja][0];
    y = posCajas[caja][1];
    cajas[y][x] = 1;
  }
}

void setup() {
  Serial.begin(9600);
  Serial.println("ILI9341 Test!"); 
 
      tft.begin();
     
         Caratula();
         delay (1000);

         Primer();
         delay (1000);

   /*      Segundo();
         delay (200);

         Tercer();
         delay (200);

         Cuarto();
         delay (200);

         Quinto();
         delay (200);
        
         Siguiente2();
         delay (200);

         Siguiente3();
         delay (800);

         Siguiente4();
         delay(8000);

         Siguiente5();
         delay (8000); 
*/
   tft.fillScreen(ILI9341_BLACK);
  tft.setTextSize(3);
  tft.setRotation(0);
  tft.setTextColor(ILI9341_WHITE);
  tft.setCursor(0, 0); tft.print("   ");
  tft.setTextSize(3);
  tft.setRotation(1);
    Serial.print(F("Text                     "));
 /*
  xx=10;
  yy=9;
  xo=9;yo=10;
*/

// division marcador
tft.drawLine(240, 0, 240,240, ILI9341_RED);
tft.drawLine(241, 0, 241,240, ILI9341_RED);
tft.drawLine(242, 0, 242,240, ILI9341_RED);
tft.drawLine(243, 0, 243,240, ILI9341_RED);
tft.drawLine(244, 0, 244,240, ILI9341_RED);

   Letras();

dibujarEscenario(SCENE_SIZE, nivel1, pasilloBitmap, paredBitmap, objetivoBitmap);

posJugador[0] = n1PosInicialJugador[0];
  posJugador[1] = n1PosInicialJugador[1];
  dibujarJugador(posJugador[0], posJugador[1]);
    llenarCajas(cajas, n1cajas, n1PosInicialCajas);

}
void loop(void) 
{ 
  unsigned char movimiento = leerJoystick();
    moverJugador(movimiento, posJugador, nivel1);
delay (150);

unsigned char nivelActual = verificarNivel();


  switch (nivelActual)
  {
    case 4: 
    {
    tft.fillScreen(ILI9341_BLACK);
        Segundo();
         delay (1000);

         Siguiente2();
         delay (2000);

         tft.fillScreen(ILI9341_BLACK);

     moverJugador(movimiento, posJugador, nivel1);

           dibujarEscenario(SCENE_SIZE, nivel2, pasilloBitmap, paredBitmap, objetivoBitmap);
           posJugador[0] = n2PosInicialJugador[0];
           posJugador[1] = n2PosInicialJugador[1];
           dibujarJugador(posJugador[0], posJugador[1]);
           llenarCajas(cajas, n2cajas, n2PosInicialCajas); 
    }
    break;
     case 5:
     {
      moverJugador(movimiento, posJugador, nivel2);

                       dibujarEscenario(SCENE_SIZE, nivel3, pasilloBitmap, paredBitmap, objetivoBitmap);
                       posJugador[0] = n3PosInicialJugador[0];
                       posJugador[1] = n3PosInicialJugador[1];
                       dibujarJugador(posJugador[0], posJugador[1]);
                       llenarCajas(cajas, n3cajas, n3PosInicialCajas); 
                    break;
  }
}
}

void dibujarEscenario(unsigned char size,
                      const unsigned char nivel[10][10],
                      const unsigned char * pasilloBM,
                      const unsigned char * paredBM,
                      const unsigned char * objetivoBM)
{
  for(int fila = 0; fila < size; fila++)
  {
    for(int col = 0; col < size; col++)
    {
      switch(nivel[fila][col])
      {
        case PASILLO:
          tft.fillRect(col * 24,
                        fila * 24,
                        24,
                        24,
                        ILI9341_WHITE);
          break;
        case PARED:
          tft.drawBitmap(col * 24,
                         fila * 24,
                         paredBM,
                         24,
                         24, 
                        ILI9341_BLUE);
                         
          break;
        case OBJETIVO:
          tft.drawBitmap(col * 24,
                         fila * 24,
                         objetivoBM,
                         24,
                         24, 
                         ILI9341_WHITE);
          break;
      }
    }
  }

}

void dibujarElemento(unsigned char x, unsigned char y, unsigned char elemento)
{
      switch(elemento)
      {
        case PASILLO:
        tft.fillRect(x * 24,
                        y * 24,
                        24,
                        24,
                        ILI9341_WHITE);
          break;
        case PARED:
          tft.drawBitmap(x * 24,
                         y * 24,
                         paredBitmap,
                         24,
                         24, 
                         ILI9341_BLUE);
          break;
        case OBJETIVO:
          tft.drawBitmap(x * 24,
                         y * 24,
                         objetivoBitmap,
                         24,
                         24, 
                         ILI9341_WHITE);
          break;
      }
}

void dibujarJugador(unsigned char x, unsigned char y)
{
  tft.drawBitmap(x * 24,
                y * 24,
                jugadorBitmap,
                24,
                24, 
                ILI9341_MAROON);
}

void dibujarCajas(unsigned char cajas[SCENE_SIZE][SCENE_SIZE])
{
  for(int fila = 0; fila < SCENE_SIZE; fila++)
  {
    for(int col = 0; col < SCENE_SIZE; col++)
    {
      if(cajas[fila][col] == 1)
      {
        
          tft.drawBitmap(col * 24,
                         fila * 24,
                         cofreBitmap,
                         24,
                         24, 
                         ILI9341_YELLOW);    
      }
    }
  }
}

unsigned long Caratula ()
{
  tft.fillScreen(ILI9341_BLACK);
  tft.setRotation(1);
  tft.setCursor(35,50);
  
  tft.setTextColor(ILI9341_RED); tft.setTextSize(6);
  tft.println("SOKOBAN");

  tft.setTextColor(ILI9341_NAVY); tft.setTextSize(1);
  tft.setCursor(0,110);
  tft.println("         ********      ********      ********       ");
  tft.println("  ********      ********      ********      ********");
  tft.println("         ********      ********      ********   ");
 
  tft.setTextColor(ILI9341_BLUE); tft.setTextSize(3);
  tft.setCursor(20,150);
  tft.println("Empieza el juego");
    
  tft.setTextColor(ILI9341_NAVY); tft.setTextSize(1);
  tft.setCursor(0,200);
  tft.println("         ********      ********      ********       ");
  tft.println("  ********      ********      ********      ********");
  tft.println("         ********      ********      ********   ");
}

unsigned long Siguiente2 ()
{
   tft.fillScreen(ILI9341_BLACK);
   tft.setRotation(1);

   tft.setTextColor(ILI9341_RED); tft.setTextSize(2);
   tft.setCursor(20,20);
   tft.println("PASASTE EL PRIMER NIVEL");

   tft.setTextColor(ILI9341_NAVY); tft.setTextSize(2);
   tft.setCursor(20,50);
   tft.println("Interesante veremos como       te va despues");
 
   tft.setTextColor(ILI9341_RED); tft.setTextSize(1);
   tft.setCursor(0,100);
   
   tft.println("                    ############      ");
   tft.println("                  ######        ###   ");
   tft.println("                 #   ###           #  ");
   tft.println("                #   #########       # ");
   tft.println("                #  ###    ###       # ");
   tft.println("               ####         ##########");
   tft.println("               ####         ###    ###");
   tft.println("               #  ##        ##      ##");
   tft.println("               #   ###    #####    ###");
   tft.println("               #   ###################");
   tft.println("               #  ################### ");
   tft.println("                ###    #    #    #### ");
   tft.println("                 #     #    #      #  ");
   tft.println("                 #                #   ");
   tft.println("                  #              #    ");
   tft.println("                   ##############     ");            
  
}

unsigned long Siguiente3 ()
{
  tft.fillScreen(ILI9341_BLACK);
  tft.setRotation(1);

  tft.setTextColor(ILI9341_RED); tft.setTextSize(2);
    tft.setCursor(20,20);
  tft.println("PASASTE EL SEGUNDO NIVEL");


  tft.setTextColor(ILI9341_BLUE); tft.setTextSize(2);
    tft.setCursor(20,50);
    tft.println("Una maravilla pero las    cosas se pondran mejor");

    tft.setTextColor(ILI9341_YELLOW); tft.setTextSize(1);
    tft.setCursor(0,100);
    tft.println("                   000000000000000        ");
    tft.println("                 0000000000000000000       ");
    tft.println("               00000   00000   0000000        ");
    tft.println("              000000   00000   00000000      ");
    tft.println("             0000000   00000   000000000          ");
    tft.println("            00000000   00000   000000000");
    tft.println("           000000000   00000   0000000000");
    tft.println("           000000000   00000   0000000000");
    tft.println("           000000000   00000   0000000000");
    tft.println("           000000000000000000000000000000");
    tft.println("           000000                  000000");
    tft.println("           000000                  000000           ");
    tft.println("            000000                 00000           ");
    tft.println("             0000000             000000       ");
    tft.println("              00000000         0000000");
    tft.println("               0000000000000000000000         ");
    tft.println("                 000000000000000000    ");
 
          
}

unsigned long Siguiente4 ()
{
  tft.fillScreen(ILI9341_BLACK);
  tft.setRotation(1);

  tft.setTextColor(ILI9341_RED); tft.setTextSize(2);
    tft.setCursor(20,20);
  tft.println("PASASTE EL TERCER NIVEL");


  tft.setTextColor(ILI9341_BLUE); tft.setTextSize(2);
    tft.setCursor(20,50);
    tft.println("  FALTA POCO NO PODRAS");

    tft.setTextColor(ILI9341_YELLOW); tft.setTextSize(1);
    tft.setCursor(0,100);
    tft.println("                     00000000000");
    tft.println("                  00000000000000000");
    tft.println("               0000000000000000000000");
    tft.println("              000000000000000000000000");
    tft.println("             0000000000000000000000    ");
    tft.println("            00000000000000000000       ");
    tft.println("           000000000000000000          ");
    tft.println("           000000000000000             ");
    tft.println("           000000000000                ");
    tft.println("           000000000000000             ");
    tft.println("           0000000000000000000         ");
    tft.println("           0000000000000000000000      ");
    tft.println("            000000000000000000000000   ");
    tft.println("             00000000000000000000000000");
    tft.println("               00000000000000000000000");
    tft.println("                  00000000000000000");
    tft.println("                     00000000000");
 
}

unsigned long Siguiente5 ()
{
  tft.fillScreen(ILI9341_BLACK);
  tft.setRotation(1);

  tft.setTextColor(ILI9341_GREEN); tft.setTextSize(1);
    tft.setCursor(0,20);
  
  tft.println("                        PP    PP    PP  ");
  tft.println("                        PP    PP    PP  ");
  tft.println("                 PP      PP    PP    PP  ");
  tft.println("                  PPPP     PP    PP    PP  ");
  tft.println("             PP      PP                 PP  ");
  tft.println("              PPPP                        PP  ");
  tft.println("                PPP           PP         PP  ");
  tft.println("                 PP     PP     PP      PP  ");
  tft.println("                  PP     PP            PP  ");
  tft.println("                  PP                   PP  ");
  tft.println("     PP   PP      PP            PP     PP  ");
  tft.println("      PP  PP      PP    PP     PP     PP  ");
  tft.println("       PP PP       PP      PPPPP     PP  ");
  tft.println("      PPPPPPP       PP              PP  ");
  tft.println("     PPPPPPPPPP       PPPPPPPPPPPPPP  PP  ");
  tft.println("     PPPPPPPPPP           PP           PP  ");
  tft.println("       PPPPPPPPPPPPPPPPPPPPP            PP  ");
  tft.println("         PPPPPPPPPPPPPPPPPPP             PP");
  tft.println("                          PP             PP");
  tft.println("                          PP             PP");
  tft.println("                          PPPPPPPPPPPPPPPPP");
  tft.println("                          PP P    PP   P PP");
  tft.println("                          PPP     PP    PPP");
  tft.println("                          PP      PP     PP");
  tft.println("                          PPPPPPPPPPPPPPPPP");
  tft.println("                        PP     PPP    PPP");
  tft.println("                      PPPPPPPPPPPPPPPPPPPP");

}

unsigned long Letras ()
{
  tft.setRotation(1);
  tft.setCursor(250,20);
  tft.setTextColor(ILI9341_YELLOW); tft.setTextSize(2);
  tft.println("Pasos");

  tft.setRotation(1);
  tft.setCursor(250,100);
  tft.setTextColor(ILI9341_YELLOW); tft.setTextSize(2);
  tft.println("Cajas");

  tft.setRotation(1);
  tft.setCursor(250,180);
  tft.setTextColor(ILI9341_YELLOW); tft.setTextSize(1);
  tft.println("Version de");

  tft.setRotation(1);
  tft.setCursor(250,200);
  tft.setTextColor(ILI9341_YELLOW); tft.setTextSize(1);
  tft.println("SOKOBAN");

  tft.setRotation(1);
  tft.setCursor(250,220);
  tft.setTextColor(ILI9341_YELLOW); tft.setTextSize(1);
  tft.println("5.2");

}

unsigned long Primer ()
{
  tft.fillScreen(ILI9341_BLACK);
  tft.setRotation(1);

  tft.setTextColor(ILI9341_GREEN); tft.setTextSize(25);
    tft.setCursor(100,30);
  tft.println("1");
}
unsigned long Segundo ()
{
  tft.fillScreen(ILI9341_BLACK);
  tft.setRotation(1);

  tft.setTextColor(ILI9341_GREEN); tft.setTextSize(25);
    tft.setCursor(100,30);
  tft.println("2");
}
unsigned long Tercer ()
{
  tft.fillScreen(ILI9341_BLACK);
  tft.setRotation(1);

  tft.setTextColor(ILI9341_GREEN); tft.setTextSize(25);
    tft.setCursor(100,30);
  tft.println("3");
}
unsigned long Cuarto ()
{
  tft.fillScreen(ILI9341_BLACK);
  tft.setRotation(1);

  tft.setTextColor(ILI9341_GREEN); tft.setTextSize(25);
    tft.setCursor(100,30);
  tft.println("4");
}

unsigned long Quinto ()
{
  tft.fillScreen(ILI9341_BLACK);
  tft.setRotation(1);

  tft.setTextColor(ILI9341_GREEN); tft.setTextSize(25);
    tft.setCursor(100,30);
  tft.println("5");
}

unsigned char verificarCajas(const unsigned char cajas[SCENE_SIZE][SCENE_SIZE],
                    const unsigned char nivel[SCENE_SIZE][SCENE_SIZE])
{
  unsigned char contador = 0;
  for(int fila = 0; fila < SCENE_SIZE; fila++)
  {
    for(int col = 0; col < SCENE_SIZE; col++)
    {
      if(cajas[fila][col] == 1 && nivel[fila][col] == OBJETIVO)
      {
        contador++;         
      }
    }
  }
  return contador;
}


unsigned char leerJoystick()
{
  
  int jx = analogRead(HOR_JOY);
  int jy = analogRead(VER_JOY);

  if(jy > UP_TH)
  {
    return ABAJO;
  }
  if(jx > RIGHT_TH)
  { 
    return DERECHA;
  }
  if(jy < DOWN_TH)
  {
    return ARRIBA;
  }
  if(jx < LEFT_TH)
  {
    return IZQUIERDA;
  }
}

unsigned int verificarNivel()
{
  if(verificarCajas(cajas, nivel1) == n1cajas)
  {
    return lvl1;
  }
    
if(verificarCajas(cajas, nivel2) == n2cajas)
  {
    return lvl2;
  }
  
}



