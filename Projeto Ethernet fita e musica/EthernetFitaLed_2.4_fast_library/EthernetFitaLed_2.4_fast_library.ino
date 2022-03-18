#include <FastLED.h>
#include <SPI.h>
#include <String.h>
#include <Ethernet.h>

#define COLOR_ORDER BGR
#define COLOR_ORDER GRB

#define NUM_LEDS 50
#define DATA_PIN 31
// Define the array of leds
CRGB leds[NUM_LEDS];


#define relay1 40
#define relay2 41


//====================================================== Config de rede ======================================================
byte mac[] = { 0x90, 0xA2, 0xDA, 0x00, 0x9B, 0x36 }; // Endereço Mac
byte ip[] = { 192, 168, 1, 4}; // Endereço de Ip da sua Rede
EthernetServer server(80); // Porta de serviço

//====================================================== Variaveis ======================================================
String readString = String(30); // string para buscar dados de endereço
float temperatura = 0;

byte Cor = 0;
byte delEfeito = 55;
byte efeito = 0;
boolean executar = false;

char LeBrilho[10];
int Brilho = 255;


byte n = 0;
byte Inicio = 0;


byte luz=1;
byte Musica = 0;


char Hexa[10];

//====================================================== Setup ======================================================
void setup() {
  
  FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  digitalWrite(relay1, LOW);
  digitalWrite(relay2, LOW);

  // Inicia o Ethernet
  Ethernet.begin(mac, ip);
  Serial.begin(9600);

}
int conectou = 0;
//====================================================== Loop ======================================================
void loop() {
  
  
  Fita();

  
  if (Cor == 9) {
    conectou = 0;
    for (uint16_t j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on wheel
      for (uint16_t i = 0; i < NUM_LEDS; i++)
      {
        
        Fita();
        
        if (conectou == 0)
        {
          //pixels.setPixelColor(i, Wheel(((i * 256 / pixels.numPixels()) + j) & 255));
          leds[i] = CRGB(Wheel(((i * 256 / NUM_LEDS) + j) & 255));
          
        }
        else break;
      }
      if (conectou == 1)break;

      FastLED.show();
      delay(10);
    }
  }



}

void Fita()
{
  EthernetClient client = server.available();
  if (client) {

    while (client.connected())
    {
      if (client.available())
      {
        char c = client.read();
        // ler caractere por caractere vindo do HTTP

        if (readString.length() < 30)
        {
          // armazena os caracteres para string
          readString += (c);

        }
        //se o pedido HTTP terminou
        if (c == '\n')
        {
          Serial.print("Comando: ");
          Serial.println(readString);

          if (readString.indexOf("chroma") >= 0)
          {
            Cor = 9;
          }
          if (readString.indexOf("Efeito=0") >= 0)
          {
            efeito = 0;
          }
          if (readString.indexOf("Efeito=1") >= 0)
          {
            efeito = 1;
          }
            if (readString.indexOf("Musica=1") >= 0)
          {
            Musica = 1;
          }
          if (readString.indexOf("Luz") >= 0)
          {
            if(luz==0){
              digitalWrite(relay1, LOW);
              digitalWrite(relay2, LOW);
            }
            if(luz==1){
            digitalWrite(relay1, HIGH);
            digitalWrite(relay2, HIGH);
            }
            luz ++;
            if(luz>1)luz=0;
            
          }
   

          if (readString.indexOf("Color") >= 0)
          {
            Inicio = achaInicio('r');

            Serial.print("Inicio Hexa: ");
            Serial.println(Inicio);
            n = 0;
            for (byte i = Inicio; i < Inicio + 9; i++) {
              if (n < 6) {
                Hexa[n] = readString[i];

              }
              else {
                LeBrilho[n - 6] = readString[i];
              }

              n++;
            }
            Hexa[n - 3] = NULL;
            LeBrilho[n - 6] = NULL;
            Brilho = atoi(LeBrilho);
            Brilho = map(Brilho, 0, 100, 0, 255);


            Serial.print("Valor Hexa: ");
            Serial.println(Hexa);
            Serial.print("Valor Brilho: ");
            Serial.println(Brilho);

            Converte(Hexa);

          }

          // dados HTML de saída começando com cabeçalho padrão
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println();

          client.print("<font size='20'>");
          if (Cor == 0) {
            client.print("Desligado");
          }
          if (Cor == 1) {
            client.print("Vermelho Ligado");
          }
          if (Cor == 2) {
            client.print("Verde Ligado");
          }
          if (Cor == 3) {
            client.print("Azul Ligado");
          }
          if (Cor == 4) {
            client.print("Branco Ligado");
          }
          if (Cor == 9) {
            client.print("Efeito Chroma em execucao");
          }
          if (luz == 0) {
            client.println("<br/>Luz desligada");
          }
          if (luz == 1) {
            client.println("<br/>Luz Ligada");
          }


          //limpa string para a próxima leitura
          readString = "";

          // parar cliente
          client.stop();

        }
      }
    }
    conectou = 1;
  }
}

byte achaInicio(char simb)
{
  byte j = 0;
  bool Parar = true;
  byte inici;
  while (Parar) {
    j++;
    if (readString[j] == simb) {
      inici = j;
      inici = inici + 1;
      Parar = false;
    }
  }
  return (inici);
}

//====================================================== Leds() ======================================================
void Leds(int x, int y, int z)
{

  if (efeito == 0) {
    for (int i = 0; i < NUM_LEDS; i++) {
      //pixels.setPixelColor(i, x, y, z);
      leds[i] = CRGB(x, y, z);
    }
    FastLED.show();
  }

  if (efeito == 1) {
    int n = 29;
    for (int i = 30; i < 60; i++) {

      //pixels.setPixelColor(i, x, y, z);
      leds[i] = CRGB(x, y, z);
      //pixels.setPixelColor(n, x, y, z);
      leds[n] = CRGB(x, y, z);

      FastLED.show();
      n--;
      delay(delEfeito);

    }
  }

  //====================================================== LedsOff() ======================================================
}
void LedsOff()
{

  for (int i = 0; i < 50; i++) {
    //pixels.setPixelColor(i, 0, 0, 0);
    leds[i] = CRGB::Black;
  }
  //pixels.show();
  FastLED.show();
}

//====================================================== Converte ======================================================
void Converte(String value)
{

  char charbuf[8];
  value.toCharArray(charbuf, 8);
  long int rgb = strtol(charbuf, 0, 16); //=>rgb=0x001234FE;
  byte b = (byte)(rgb >> 16);
  byte g = (byte)(rgb >> 8);
  byte r = (byte)(rgb);

  r = map(r, 0, 255, 0, Brilho);
  g = map(g, 0, 255, 0, Brilho);
  b = map(b, 0, 255, 0, Brilho);

  Serial.print("\nCores: ");
  Serial.print(r);
  Serial.print(", ");
  Serial.print(g);
  Serial.print(", ");
  Serial.println(b);

  if (r == Brilho)Cor = 1;
  if (g == Brilho)Cor = 2;
  if (b == Brilho)Cor = 3;
  if (r == Brilho && g == Brilho && b == Brilho)Cor = 0;
  Serial.print("Cor: ");
  Serial.println(Cor);

  Leds(r, b, g);

}

//====================================================== Chroma ======================================================

uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return CRGB(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return CRGB(0,WheelPos * 3 , 255 - WheelPos * 3);//pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
            
  }
  WheelPos -= 170;
  return CRGB(WheelPos * 3, 255 - WheelPos * 3, 0); //pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0); // pixels.setPixelColor(n, x, y, z);
         
}
