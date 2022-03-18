#include <FastLED.h>
#include <SPI.h>
#include <String.h>
#include <Ethernet.h>

#define COLOR_ORDER BGR

#define NUM_LEDS 50
#define DATA_PIN 7
// Define the array of leds
CRGB leds[NUM_LEDS];

//====================================================== Variaveis Musica ======================================================
#define analogInPin A1
byte sensorValue = 0;
byte Qtdleds = 0;
byte i = 0;
byte numMin = 20; //Numero minimo do sinal para ligar todos os leds
byte maior = numMin;
long long unsigned int lastmillis = 0;
long long unsigned int lastmillis2 = 0;
byte Musica = 1;
byte efeitosMusica = 1;
int starthue = 140;

//====================================================== Rele ======================================================
#define relay1 9
#define relay2 8


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

byte luz = 1;

byte r = 0, g = 0, b = 0;

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
  Serial.begin(115200);

  Musica = 0;
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Black;
  }
  FastLED.show();

}
int conectou = 0;
//====================================================== Loop ======================================================
void loop() {


  Fita();
  MusicaSinc();

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

          if (readString.indexOf("chroma") > 0)
          {
            Cor = 9;
          }
          if (readString.indexOf("Efeito=0") > 0)
          {
            efeito = 0;
          }
          if (readString.indexOf("Efeito=1") > 0)
          {
            efeito = 1;
          }
          if (readString.indexOf("EfeitoMusic=0") > 0)
          {
            efeitosMusica = 0;
          }
          if (readString.indexOf("EfeitoMusic=1") > 0)
          {
            efeitosMusica = 1;
          }
          if (readString.indexOf("EfeitoMusic=2") > 0)
          {
            efeitosMusica = 2;
          }
          if (readString.indexOf("EfeitoMusic=3") > 0)
          {
            efeitosMusica = 3;
          }
          if (readString.indexOf("Musica=1") > 0)
          {
            Musica = 1;
          }
          if (readString.indexOf("Musica=0") > 0)
          {
            Musica = 0;
            Leds(r, b, g);
          }
          if (readString.indexOf("Luz") > 0)
          {
            if (luz == 0) {
              digitalWrite(relay1, LOW);
              digitalWrite(relay2, LOW);
            }
            if (luz == 1) {
              digitalWrite(relay1, HIGH);
              digitalWrite(relay2, HIGH);
            }
            luz ++;
            if (luz > 1)luz = 0;

          }


          if (readString.indexOf("Color") > 0)
          {
            //Musica = 0;
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
            Leds(b, r, g);

          }


          // dados HTML de saída começando com cabeçalho padrão
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println();
          client.println("<TITLE>Automacao Quarto</TITLE>");
          //client.println("<body bgcolor='#505050'>"); Falta memoria no arduino uno


          client.println("<input type=button value=Chroma style=height:60px; width:300px ");
          client.println(" onclick=\"document.location='/chroma'\" />");
          //client.println("<br>");
          client.println("<input type=button value=Azul style=height:60px; width:300px ");
          client.println(" onclick=\"document.location='/Color0000FF100'\" />");
          //client.println("<br>");
          client.println("<input type=button value=Vermelho style=height:60px; width:300px ");
          client.println(" onclick=\"document.location='/ColorFF0000100'\" />");
          //client.println("<br>");
          client.println("<input type=button value=Verde style=height:60px; width:300px ");
          client.println(" onclick=\"document.location='/Color00FF00100'\" />");
        
          client.println("<br>");
          client.println("<br>");

          client.println("<table border='1'>");//Inicia a tabela e coloca uma borda de espessura
          client.println("<tr>");//Cria a primeira linha da tabela>
          client.println("<TD ALIGN=MIDDLE WIDTH=200>Cor ligada</TD>");//Aqui foi criada uma célula>
          client.println("<TD ALIGN=MIDDLE WIDTH=200>");

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
            client.print("Chroma");
          }
          client.println("</TD>");//Termina o TD
          client.println("</TR>");

          client.println("</table>");
          
          

          client.print("<font size='10'>");
          client.println("<br>");
 

          if (luz == 0) {
            client.println("Luz desligada<br>");
          }
          if (luz == 1) {
            client.println("Luz Ligada<br>");
          }
          if (Musica == 0) {
            client.println("Musica Desligada<br>");

          }
          if (Musica == 1) {
            client.println("Musica Ligada<br>");
          }
          if (efeitosMusica == 0) {
            client.println("Efeito Musica 0<br>");
          }
          if (efeitosMusica == 1) {
            client.println("Efeito Musica 1<br>");
          }

          

          //limpa string para a próxima leitura
          readString = "";

          // parar cliente
          client.stop();

        }
      }
    }
  }
  if (Cor == 9) {
    if ((millis() - lastmillis2) > 15) // Efeito chroma
    {
      starthue--;
      lastmillis2 = millis();

    }
    fill_rainbow( leds, NUM_LEDS, starthue, 10);
    FastLED.show();
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

      leds[i] = CRGB(x, y, z);

    }
    FastLED.show();
  }

  if (efeito == 1) {
    int n = 29;
    for (int i = 30; i < 60; i++) {

    }
    leds[i] = CRGB(x, y, z);
    leds[n] = CRGB(x, y, z);

    FastLED.show();
    n--;
    delay(delEfeito);

  }
}
//====================================================== Converte ======================================================
void Converte(String value)
{
  
  char charbuf[8];
  value.toCharArray(charbuf, 8);
  long int rgb = strtol(charbuf, 0, 16); //=>rgb=0x001234FE;
  r = (byte)(rgb >> 16);
  g = (byte)(rgb >> 8);
  b = (byte)(rgb);

  r = map(r, 0, 255, 0, Brilho);
  g = map(g, 0, 255, 0, Brilho);
  b = map(b, 0, 255, 0, Brilho);

  Serial.print("\nCores: ");
  Serial.print(r);
  Serial.print(", ");
  Serial.print(g);
  Serial.print(", ");
  Serial.println(b);
  if(Cor==9)Cor = 0;//Desligar o efeito chroma se ele estiver ativado
  if (r == Brilho)Cor = 1;
  if (g == Brilho)Cor = 2;
  if (b == Brilho)Cor = 3;
  if (r == Brilho && g == Brilho && b == Brilho)Cor = 4;
  Serial.print("Cor: ");
  Serial.println(Cor);

}
//====================================================== Musica ======================================================
void MusicaSinc()
{
  if (Musica == 1) {
    if(Cor==9)Cor = 0;//Desligar o efeito chroma se ele estiver ativado
    
    sensorValue = analogRead(analogInPin);
    Serial.println(sensorValue);

    if (sensorValue > maior)
    {
      maior = sensorValue;
      lastmillis = millis(); //Quando acha um valor maior demora mais `o tempo` para comecar a diminuir

    }



    if ((millis() - lastmillis) > 7000) // resetar o maior numero de leds que vao ligar
    {
      maior--;

      if (maior <= numMin)
      {
        lastmillis = millis();
      }
    }

    musicaEfeitos();


    FastLED.show();
  }

}
//====================================================== Efeitos da Musica ======================================================
void musicaEfeitos()
{
  if (efeitosMusica == 0)
  {
    Qtdleds = map(sensorValue, 0,  maior, 0, 50);

    for ( int i = 0; i < 50; i++)//Acendo os led conforme o sinal
    {
      if ( i < Qtdleds) {
        leds[i] = CRGB(r, b, g);
      }
      if ( i >= Qtdleds) {
        leds[i] = CRGB::Black;
      }
    }

  }
  if (efeitosMusica == 1)
  {
    Qtdleds = map(sensorValue, 0,  maior, 0, 25);
    byte j = 24;
    for ( int i = 24; i < 50; i++)//Acendo os led conforme o sinal
    {
      if ( i < Qtdleds + 24) {
        leds[i] = CRGB(r, b, g);
      }
      if ( i >= Qtdleds + 24) {
        leds[i] = CRGB::Black;
      }
      if ( j > -Qtdleds + 24) {
        leds[j] = CRGB(r, b, g);
      }
      if ( j <= -Qtdleds + 24) {
        leds[j] = CRGB::Black;
      }
      j--;
    }
  }
  if (efeitosMusica == 2)
  {
    Qtdleds = map(sensorValue, 0,  maior, 0, 50);

    if ((millis() - lastmillis2) > 15) // Efeito chroma
    {
      starthue--;
      lastmillis2 = millis();

    }

    if (Qtdleds != 0)fill_rainbow( leds, NUM_LEDS, starthue, 5); //Nao ficar atualizando o tempo todo quando nao tem musica.
    for ( int i = 0; i < 50; i++)//Acendo os led conforme o sinal
    {
      if ( i >= Qtdleds) {
        leds[i] = CRGB::Black;
      }
    }

  }
  if (efeitosMusica == 3)
  {
    
    Qtdleds = map(sensorValue, 0,  maior, 0, 25);
    byte j = 24;

    if ((millis() - lastmillis2) > 15) // Efeito chroma
    {
      starthue--;
      lastmillis2 = millis();

    }
    if (Qtdleds != 0)fill_rainbow( leds, NUM_LEDS, starthue, 5); //Nao ficar atualizando o tempo todo quando nao tem musica.

    for ( int i = 24; i < 50; i++)//Acendo os led conforme o sinal
    {
      if ( i >= Qtdleds + 24) {
        leds[i] = CRGB::Black;
      }
      if ( j <= -Qtdleds + 24) {
        leds[j] = CRGB::Black;
      }
      j--;
    }
  }



}
