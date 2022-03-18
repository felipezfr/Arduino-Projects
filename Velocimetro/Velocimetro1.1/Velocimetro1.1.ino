#include "U8glib.h"

#define velo 2
#define menu 13
#define volt A0

U8GLIB_ST7920_128X64_1X u8g(4, 5, 6, 3);	// SPI Com: SCK = en = 18, MOSI = rw = 16, CS = di = 17

float mmSeg;
int kmH, bot = 0, bot2 = 0;
unsigned long perimRoda = 1400;//mm 2*Pi*r
unsigned long tempoAnt, tempoPulso;

unsigned long timeold;

int tensao;
int espera;


int disp = 2;


void setup(void) {

  Serial.begin(9600);
  pinMode(velo, INPUT);
  // flip screen, if required
  u8g.setRot180();

  // set SPI backup if required
  //u8g.setHardwareBackup(u8g_backup_avr_spi);

  // assign default color value
  if ( u8g.getMode() == U8G_MODE_R3G3B2 ) {
    u8g.setColorIndex(255);     // white
  }
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT ) {
    u8g.setColorIndex(3);         // max intensity
  }
  else if ( u8g.getMode() == U8G_MODE_BW ) {
    u8g.setColorIndex(1);         // pixel on
  }
  else if ( u8g.getMode() == U8G_MODE_HICOLOR ) {
    u8g.setHiColorByRGB(255, 255, 255);
  }
  u8g_prepare();

  attachInterrupt(digitalPinToInterrupt(velo), velocimetro, HIGH);


}

void loop(void) {
  // picture loop


  botao();
  if (millis() - timeold >= espera)
  {
    timeold = millis();

    u8g.firstPage();
    do {
      draw();
    } while ( u8g.nextPage() );
  }

}

void botao()
{
  if (!digitalRead(menu))bot2 = 1;         //Botão M pressionado? Seta flag
  if (digitalRead(menu) && bot2)            //Botão P solto e flag setada?
  {
    bot2 = 0;
    
    disp ++;
    if(disp > 2)disp=1;
  }
}

void draw(void)
{


  switch (disp) //Carrega a tela correspondente
  {
    case 1:
      espera = 500;
      u8g_Tela1();
      break;
    case 2:
      voltimetro();
      espera = 150;
      u8g_Tela2();
      break;

  }

}

void u8g_Tela1()
{
  u8g.setColorIndex(1);
  u8g.setFont(u8g_font_fub30);

  if (kmH < 10) {

    u8g.setPrintPos(20, 50);
    u8g.print("00");
    u8g.print(kmH);
  }
  if (kmH > 9 && kmH < 100) {

    u8g.setPrintPos(20, 50);
    u8g.print("0");
    u8g.print(kmH);
  }
  if (kmH > 99 && kmH < 1000) {
    u8g.setPrintPos(20, 50);
    u8g.print(kmH);

  }
  if (kmH > 999) {
    u8g.setPrintPos(20, 50);
    u8g.print("----");
  }

  u8g.setFont(u8g_font_7x14);
  u8g.print("Km/h");
  u8g.drawFrame(0, 0, 128, 64);

}


void u8g_Tela2()
{
  u8g.setFont(u8g_font_7x14);
  u8g.setPrintPos(45, 13);
  u8g.print("Tensao");
  u8g.drawFrame(0, 0, 128, 64);

  u8g.setFont(u8g_font_fub30);
  u8g.setPrintPos(30, 50);

  if (tensao < 10) {

    u8g.print("  ");
    u8g.print(tensao);
  }
  if (tensao > 9 && tensao < 100) {


    u8g.print(" ");
    u8g.print(tensao);
  }
  if (tensao > 99 && tensao < 1000) {
    u8g.print(tensao);

  }



}




void velocimetro()
{
  if (!digitalRead(velo))bot = 1;         //Botão M pressionado? Seta flag
  if (digitalRead(velo) && bot)            //Botão P solto e flag setada?
  {
    bot = 0;
    if (millis() != tempoAnt) {

      tempoPulso = millis() - tempoAnt;
      tempoAnt = millis();

      Serial.print("Pulso+: ");
      Serial.println(tempoPulso);

      mmSeg = ((1000 * perimRoda) / tempoPulso);

      //Serial.print("mm/seg: ");
      //Serial.println(mmSeg);

      kmH = mmSeg * 0.0036;

      Serial.print("Km/h: ");
      Serial.println(kmH);
      Serial.println("\n");
    }

  }

}

void voltimetro()
{

  tensao = map(analogRead(volt), 0, 1023, 0, 500);

  //tensao = (sensorValue * 5.0) / 1023;

  Serial.print("Tensao1: ");
  Serial.println(tensao);



}



void u8g_prepare()
{
  u8g.setFont(u8g_font_6x10);
  u8g.setFontRefHeightExtendedText();
  u8g.setDefaultForegroundColor();
  u8g.setFontPosTop();
}



