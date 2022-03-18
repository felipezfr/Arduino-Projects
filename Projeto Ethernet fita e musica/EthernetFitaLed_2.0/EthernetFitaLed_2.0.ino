#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            31

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      50

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel pixelss you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);



#include <SPI.h>
#include <String.h>
#include <Ethernet.h>

byte mac[] = { 0x90, 0xA2, 0xDA, 0x00, 0x9B, 0x36 }; // Endereço Mac
byte ip[] = { 192, 168, 1, 3}; // Endereço de Ip da sua Rede
EthernetServer server(80); // Porta de serviço


String readString = String(30); // string para buscar dados de endereço
float temperatura = 0;

byte Cor = 1;
byte delEfeito = 100;
byte efeito = 1;
boolean executar = false;

char LeBrilho[3];
int Brilho = 255;


byte n = 0;

byte j = 0;
int Inicio = 0;
byte Parar = 1;

void setup() {

#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  pixels.begin(); // This initializes the NeoPixel library.
  pixels.show();

  // Inicia o Ethernet
  Ethernet.begin(mac, ip);

  Serial.begin(9600);

}




void loop() {
  // Criar uma conexão de cliente
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
          // vamos verificar se o LED deve ser ligado
          // Se a string possui o texto L=Ligar
          if (readString.indexOf("TurnOnRed") >= 0)
          {
            // O Led vai ser ligado na cor vermelha
            executar = true;
            Cor = 1;

          }
          if (readString.indexOf("TurnOnGreen") >= 0)
          {
            // O Led vai ser ligado na cor verde
            executar = true;
            Cor = 2;

          }
          if (readString.indexOf("TurnOnBlue") >= 0)
          {
            // O Led vai ser ligado na cor azul
            executar = true;
            Cor = 3;

          }
          if (readString.indexOf("TurnOnWhite") >= 0)
          {
            // O Led vai ser ligado na cor branca
            executar = true;
            Cor = 4;

          }
          // Se a string possui o texto L=Desligar
          if (readString.indexOf("Desligar") >= 0)
          {
            // O Led vai ser desligado

            executar = true;
            Cor = 0;
          }
          if (readString.indexOf("chroma") >= 0)
          {
            executar = true;
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


          if (readString.indexOf("Pwm=") >= 0)
          {


            j = 0;
            Parar = 1;
            while (Parar) {
              j++;
              if (readString[j] == '=') {
                Inicio = j;
                Inicio = Inicio + 1;
                Parar = 0;
              }
            }

            Serial.print("Inicio: ");
            Serial.println(Inicio);
            n=0;
            for (int i = Inicio; i < Inicio + 3; i++) {


              if (readString[i] == ' ') {
                LeBrilho[n] = NULL;
              }
              else {
                LeBrilho[n] = readString[i];
              }
              n++;

            }
            LeBrilho[n] = NULL;
            Brilho = atoi(LeBrilho);

            executar = true;
          }

          if (readString.indexOf("Color") >= 0)
          {
            char Hexa[7];
            int k = 0;
            int Parar2 = 1;
            int Inicio2;
            
            while (Parar2) {
              k++;
              if (readString[k] == 'r') {
                Inicio2 = k;
                Inicio2 = Inicio2 + 1;
                Parar2 = 0;
              }
            }

            Serial.print("InicioHexa: ");
            Serial.println(Inicio2);
            byte e=0;
            for (int i = Inicio2; i < Inicio2 + 6; i++) {


             
              Hexa[e] = readString[i];
              e++;

            }
            Hexa[e] = NULL;
            Serial.print("VALOR HEXA: ");
            Serial.print(Hexa);

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
            client.print("Efeito Chroma executado");
          }


          //limpa string para a próxima leitura
          readString = "";

          // parar cliente
          client.stop();



        }
      }
    }

    Serial.print("Cor: ");
    Serial.println(Cor);
    Serial.print("Brilho: ");
    Serial.println(Brilho);
    Ligar();
  }



}


void Ligar()
{
  if (executar) {
    executar = false;

    Serial.println("Cor Atualizada.");
    if (Cor == 0) {
      //LedsOff();
      Leds(0, 0, 0);
    }
    if (Cor == 1) {
      Leds(Brilho, 0, 0);
    }
    if (Cor == 2) {
      Leds(0, 0, Brilho);
    }
    if (Cor == 3) {
      Leds(0, Brilho, 0);
    }
    if (Cor == 4) {
      Leds(Brilho, Brilho, Brilho);
    }
    if (Cor == 9) {
      rainbowCycle(10);
    }
  }
}

void Leds(int x, int y, int z)
{


  if (efeito == 0) {
    for (int i = 0; i < 60; i++) {
      pixels.setPixelColor(i, x, y, z);
    }
    pixels.show();
  }

  if (efeito == 1) {
    int n = 29;
    for (int i = 30; i < 60; i++) {

      pixels.setPixelColor(i, x, y, z);
      pixels.setPixelColor(n, x, y, z);

      pixels.show();
      n--;
      delay(delEfeito);

    }
  }



}
void LedsOff()
{

  for (int i = 0; i < 50; i++) {
    pixels.setPixelColor(i, 0, 0, 0);
  }
  pixels.show();
}

void Converte(String value)
{


  char charbuf[8];
  value.toCharArray(charbuf, 8);
  long int rgb = strtol(charbuf, 0, 16); //=>rgb=0x001234FE;
  byte r = (byte)(rgb >> 16);
  byte g = (byte)(rgb >> 8);
  byte b = (byte)(rgb);
  Serial.print("\nCores: ");
  Serial.print(r);
  Serial.print(".");
  Serial.print(g);
  Serial.print(".");
  Serial.println(b);
  Leds(r, b, g);

}


void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on wheel
    for (i = 0; i < pixels.numPixels(); i++) {
      pixels.setPixelColor(i, Wheel(((i * 256 / pixels.numPixels()) + j) & 255));
    }

    pixels.show();
    delay(wait);
  }
}

uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

