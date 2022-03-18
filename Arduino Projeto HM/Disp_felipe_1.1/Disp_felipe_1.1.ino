/*
   Curso de Arduino WR Kits Aula 61

   www.wrkits.com.br
   facebook.com/wrkits
   youtube.com/user/canalwrkits

   Sub-Menu para Display LCD 16 x 2

   Autor: Eng. Wagner Rambo, Data: Abril de 2016

*/


//========================================================================================================== Caracteres Especiais ========================================================================================================


byte BlackInicio[8] = {B11111, B10000, B10111, B10111, B10111, B10111, B10000, B11111};
byte BlackMeio[8] = {B11111, B00000, B11111, B11111, B11111, B11111, B00000, B11111};
byte BlackFim[8] = {B11111, B00001, B11101, B11101, B11101, B11101, B00001, B11111};
byte BlackMeVazio[8] = {B11111, B00000, B00000, B00000, B00000, B00000, B00000, B11111};
byte BlackInVazio[8] = {B11111, B10000, B10000, B10000, B10000, B10000, B10000, B11111};
byte BlackFiVazio[8] = {B11111, B00001, B00001, B00001, B00001, B00001, B00001, B11111};
byte Cel[8] = {B00000, B01110, B01010, B01110, B00000, B00000, B00000, B00000};

// ========================================================================================================== Bibliotecas Auxiliares ========================================================================================================
#include <LiquidCrystal.h>                              //Biblioteca para o display LCD

// --- Mapeamento de Hardware ---
#define butUp    12                                     //Botão para selecionar tela acima no digital 12
#define butP     10                                     //Botão de ajuste mais no digital 10
#define select    8                                     //Botão de seleção no digital 8
#define Lamp1    A0                                     //Saída para lâmpada 1 no A0 (será usado como digital)
#define Black     9                                     //Saida para o BlackLight
#define temp     A2                                     //Entrada do sensor de temperatura
#define LDR      A3                                     //Entrada do sensor de luminosidade
#define boia     A1                                     //Entrada do sensor boia
#define volt     A5

// ========================================================================================================== Protótipo das Funções Auxiliares ================================================================================================
void changeMenu();                                      //Função para modificar o menu atual
void dispMenu();                                        //Função para mostrar o menu atual
void data_hora();                                       //Função do menu1, data e hora
void temperatura();                                     //Função do menu2, temperatura
void lights();                                          //Função do menu3, acionamento de lampadas
void menu4();                                           //Função do menu4

void dispSubMenu4();                                    //Função do sub menu4
void readSelect(char option);                           //Função de Leitura do botão select para seleção de subMenus
void subMenu4_1();                                      //Função para subMenu4_1
void subMenu4_2();                                      //Função para subMenu4_2
void subMenu4_3();                                      //Função para subMenu4_3

int lerBut(int but);                                    //Funcao de leitura dos botoes
void menuAuto();                                        //Funcao para passar os menus automaticamente
void sensorLDR();                                       //Funcao do sensor de luminosidade
void temperatura();
void nivelTanque();
void Barra(int PWM);
void BlackLight();
void BlackLightAuto();

// ========================================================================================================== Variaveis globais ==============================================================================================================

float PWM = 127.5;
byte dPWM = PWM / 25.5;
float Vi;

short valorLDR    = 0;
int comb        = 0;
int combDisp    = 0;
short i           = 0;

unsigned int tempo       = 0;
unsigned int tempoAuto   = 0;
unsigned int tempoMe     = 20;
unsigned int tempoMe2    = 0;
unsigned int tempoReser  = 0;

unsigned int tempoSerial = 0;

// ============================================================================================================= Flags ======================================================================================================================
char menu              = 0x01;                                  //Variável para selecionar o menu
char subMenu4          = 0x01;                                  //Variável para selecionar subMenu no menu4
char set1              = 0x00;                                  //Controle das lâmpadas
boolean t_butUp, t_back, t_butP, t_butM, t_select;              //Flags para armazenar o estado dos botões

char butSol          = 0;
char stopW           = 1;
// ====================================================================================================== Flags de inicializacao ============================================================================================================

char flagmenuAuto    = 1;               //Inicia passando os menus automaticos
char flagBlackAuto   = 0;               //Inicia com o blacklight automaticos
char flagReserva     = 1;               //Inicia com com a luz de reserva automatica

//========================================================================================================== Hardware do LCD ================================================================================================================
LiquidCrystal disp(7,  //RS no digital 7
                   6,  //EN no digital 6
                   5,  //D4 no digital 5
                   4,  //D5 no digital 4
                   3,  //D6 no digital 3
                   2); //D7 no digital 2
// ======================================================================================================  Configurações Iniciais ===========================================================================================================
void setup()
{
  Serial.begin(9600);
  disp.begin(16, 2);                                     //Inicializa LCD 16 x 2
  for (char i = 8; i < 13; i++) pinMode(i, INPUT_PULLUP); //Entrada para os botões (digitais 8 a 12) com pull-ups internos

  pinMode(Lamp1, OUTPUT);                                //Configura saída para lâmpada 1

  pinMode(Black, OUTPUT);

  t_butUp   = 0x00;                                      //limpa flag do botão Up
  t_back    = 0x00;
  t_butP    = 0x00;                                      //limpa flag do botão P
  t_butM    = 0x00;                                      //limpa flag do botão M
  t_select  = 0x00;                                      //limpa flag do botão select

  digitalWrite(Lamp1, LOW);                              //Lâmpada 1 inicia apagada

  disp.createChar(1, BlackInVazio);
  disp.createChar(2, BlackMeVazio);
  disp.createChar(3, BlackFiVazio);
  disp.createChar(4, BlackInicio);
  disp.createChar(5, BlackMeio);
  disp.createChar(6, BlackFim);
  disp.createChar(7, Cel);

  Serial.println("------ Bem vindo -------");

  if (flagBlackAuto) {
    BlackLightAuto();
  }
  else analogWrite(Black, PWM);

  disp.setCursor(3, 0);
  disp.print("Bem Vindo");
  delay(150);
  for (int i = 0; i < 11; i++) {

    delay(100);
    Barra(i);

  }

  disp.clear();

} //end setup
//========================================================================================================== Loop Infinito ==========================================================================================================
void loop()
{

  changeMenu();
  dispMenu();
  menuAuto();
  BlackLightAuto();
  reserva();


} //end loop


// ================================================================================================ Desenvolvimento das Funções Auxiliares ===================================================================================




//========================================================================================================== Mudanca de menus ==========================================================================================================
void changeMenu()                                       //Modifica o menu atual
{
  //Botão Up pressionado? Seta flag

  if (!digitalRead(butUp))    t_butUp    = 0x01;         //Botão M pressionado? Seta flag
  if (digitalRead(butUp) && t_butUp)                     //Botão P solto e flag setada?
  { //Sim...
    t_butUp = 0x00;
    disp.clear();                                     //Limpa display
    menu++;                                           //Incrementa menu

    if (menu > 0x05) menu = 0x01;                     //Se menu maior que 4, volta a ser 1

  } //end butUp

} //end changeMenu

void menuAuto()
{
  if (flagmenuAuto) {

    if (digitalRead(butUp) && digitalRead(butP) && digitalRead(select))// Se todos os botoes soltos
    {

      if ((millis() / 1000) >= tempoMe) { // So entra depois do tempo

        tempoMe = tempoMe + 40; // tempo para esperar
        butSol = 1;              //Sseta flag
      }
    }
    else {
      butSol = 0;
      tempoMe = (millis() / 1000) + 40;
    }

    if ((millis() / 1000) >= tempoMe2 && butSol == 1) {

      tempoMe2 = (millis() / 1000) + 10;
      disp.clear();
      menu++;
      if (menu > 0x04) menu = 0x01;
    }
  }
}
//========================================================================================================== Mostra os menus ==========================================================================================================
void dispMenu()                                         //Mostra o menu atual
{

  switch (menu)                                       //Controle da variável menu
  {
    case 0x01:                                       //Caso 1
      nivelTanque();

      break;                                     //break
    case 0x02:                                       //Caso 2
      Voltimetro();

      break;                                     //break
    case 0x03:                                       //Caso 3
      temperatura();                             //Chama a função do termômetro

      break;                                     //break
    case 0x04:                                       //Caso 4
      sensorLDR();                                   //Chama função para o menu4

      break;                                     //break
    case 0x05:                                       //Caso 4
      menu4();                                   //Chama função para o menu4

      break;

  } //end switch menu

} //end dispMenu

//========================================================================================================== Sensor LDR ==========================================================================================================
void sensorLDR()                                        //Sensor LDR
{
  disp.setCursor(3, 0);                                //Posiciona cursor na coluna 2, linha 1
  disp.print("Sensor LDR");                           //Imprime mensagem

  valorLDR = analogRead(LDR);
  valorLDR = map(valorLDR, 500, 1023, 0, 1023);
  if (valorLDR < 0)valorLDR = 0;

  if (valorLDR < 1000) {
    disp.setCursor(9, 1);
    disp.print("  ");                           //Imprime mensagem
  }
  if (valorLDR < 100) {
    disp.setCursor(8, 1);
    disp.print(" ");
  }
  if (valorLDR < 10) {
    disp.setCursor(7, 1);
    disp.print(" ");
  }
  disp.setCursor(6, 1);
  disp.print(valorLDR);

} //end sensor LDR

//========================================================================================================== Voltimetro ==========================================================================================================
void Voltimetro()
{
  disp.setCursor(1, 0);
  disp.print("Tensao da Bat");
  float R1 = 300.0;
  float R2 = 100.0;
  short value;
  float Vo;

  value = analogRead(volt);

  Vo = (value * 5.0) / 1023.0;
  Vi = Vo / (R2 / (R1 + R2));

  if (Vi < 0.09) {
    Vi = 0.0;             //Filtrando medições
  }

  //if (Vi < 10) {
  disp.setCursor(10, 1);
  disp.print(" V");
  //}

  disp.setCursor(6, 1);
  disp.print(Vi);


}
//========================================================================================================== Tanque de combustivel  ==========================================================================================================
void reserva()
{
  comb = analogRead(boia);
  comb = map( comb, 0 , 1023, 0 , 65);

  if (flagReserva) {
    if ((millis() / 1000) >= tempoReser) {

      if (comb < 7) {
        digitalWrite(Lamp1, HIGH);
      }
      else {
        digitalWrite(Lamp1, LOW);

      }//end else
      tempoReser = tempoReser + 5;
    }//end if millis
  }//end if flag
}//end reserva();

void nivelTanque()
{

  disp.setCursor(2, 0);
  disp.print("Combustivel");

  if (comb < 10) {
    disp.setCursor(0, 1);
    disp.print("0");
    disp.setCursor(1, 1);
    disp.print(comb);
    disp.setCursor(2, 1);
    disp.print("L");
  }
  else {
    disp.setCursor(0, 1);
    disp.print(comb);
    disp.setCursor(2, 1);
    disp.print("L");
  }
  combDisp = map(comb, 0, 65, 0, 10);
  Barra(combDisp);

}

//========================================================================================================== Termometro  ==========================================================================================================
void temperatura()                                      //Temperatura (menu2)
{

  float temperatura;

  disp.setCursor(2, 0);                                //Posiciona cursor na coluna 2, linha 1
  disp.print("Temperatura");                           //Imprime mensagem

  if ((millis() / 1000) >= tempo) {
    short valorLido = analogRead(temp);
    temperatura = (valorLido  * 0.48828125);

    disp.setCursor(5, 1);                                //Posiciona cursor na coluna 5, linha 2
    disp.print(temperatura);
    disp.setCursor(10, 1);
    disp.write(7);
    disp.print("C");


    tempo = tempo + 1;
  }

} //end temperatura()
//========================================================================================================== Barra ==========================================================================================================
void Barra(int PWM)
{

  for (i = 10; i > PWM ; i--)                       //Laco comeca em 10 e vai ate o numero diminuindo
  {

    if (i == 1 && i < 10) {                              //Se o numero eh 1 ele exibe o caracter de inicio da lista vazia
      disp.setCursor(i + 2, 1);
      disp.write(1);
    }
    else {                                                //Se eh um dos numero entre 1 e 10 mostra a barra vazia
      disp.setCursor(i + 2, 1);
      disp.write(2);
    }
    if (i == 10) {                                      //Se eh 10 mostra o caracter que fecha a barra vazia
      disp.setCursor(i + 2, 1);
      disp.write(3);

    }
  }//end for VAZIA

  for (i = 1; i <= PWM; i++)                              //Laco que percore ate o numero (vai enchendo a barra)
  {

    if (PWM >= 1) {                                           //Se o numero eh 1 ele exibe o caracter de inicio da lista cheia
      disp.setCursor(3, 1);
      disp.write(4);
    }
    if (PWM > 1 && PWM <= 10) {                              //Se eh um dos numero entre 0 e 10 mostra a barra cheia
      disp.setCursor(i + 2, 1);
      disp.write(5);
    }
    if (PWM == 10) {                                        //Se eh 10 mostra o caracter que fecha a barra cheia
      disp.setCursor(12, 1);
      disp.write(6);
    }

  }// END FOR PWM
}

//========================================================================================================== BlackLights ==========================================================================================================
void BlackLight()
{
  flagBlackAuto = 0;                                  //Desliga o BlackLight automatico

  disp.setCursor(3, 0);
  disp.print("BlackLigh");

  Barra(dPWM);                                    //Exite a barra de acordo com o valor de dPWM


  if (!digitalRead(butP))   t_butP   = 0x01;         //Botão Up pressionado? Seta flag
  if (digitalRead(butP) && t_butP)                   //Botão Up solto e flag setada?
  { //Sim...
    t_butP = 0x00;                                   //Limpa flag


    PWM -= 25.5;                                     //Cada clique diminui 25.5 do PWM, acendendo tudo com 10 cliques.
    dPWM += 1;                                       //Cada clique aumenta a contagem em 1 para depois exibir a barra

    if (PWM < 0)PWM = 255;                           //Se o numero for menor que zero apaga o blackligh
    if (dPWM > 10) {                                 //Se o numero de cliques for maior que 10 zera a variavel e limpa o dispay

      dPWM = 0;
      disp.clear();
    }


  } //end butUp

  if (!digitalRead(select))t_select    = 0x01;      //Botão select pressionado? Seta flag
  if (digitalRead(select) && t_select)              //Botão select solto e flag setada?
  { //Sim...
    t_select = 0x00;                                //Limpa flag
    stopW = 0;                                      //Flag para parar a execucao do laco WHILE da funcao de submenu
  } //end if

  analogWrite(Black, PWM);                         //Acende o display de acordo com a variavel PWM
}

void BlackLightAuto()
{

  if (flagBlackAuto == 1) {
    if ((millis() / 1000) >= tempoAuto) {

      valorLDR = analogRead(LDR);                   //Faz a leitura do LDR
      valorLDR = map(valorLDR, 500, 1023, 0, 200);  //Faz a conversao para usar como PWM
      if (valorLDR < 0) valorLDR = 0;               //Se for menor que zero volta a ser zero

      analogWrite(Black, valorLDR);                 //Acende o blacklight de acordo com o valor do LDR
      tempoAuto = tempoAuto + 1;

    }
  }
}

//========================================================================================================== Menu 4 ==========================================================================================================
void menu4()                                            //Função para o quarto menu...
{
  disp.setCursor(4, 0);                                //Posiciona cursor
  disp.print("Ajustes");                                //Imprime mensagem

  dispSubMenu4();

} //end menu4

//............
void dispSubMenu4()                                     //Mostra o sub menu atual para o menu 4
{

  if (!digitalRead(butP))    t_butP    = 0x01;         //Botão P pressionado? Seta flag
  if (digitalRead(butP) && t_butP)                     //Botão P solto e flag setada?
  { //Sim...
    t_butP = 0x00;                                    //Limpa flag

    subMenu4++;                                       //incrementa subMenu4

    if (subMenu4 > 3) subMenu4 = 0x01;                //se maior que 3, volta a ser 1


  } //end butP

  switch (subMenu4)                                   //Controle da variável subMenu
  {
    case 0x01:                                       //Caso 1
      disp.setCursor(1, 1);                      //Posiciona cursor
      disp.print("BlackLight >    ");
      readSelect(1);                             //Lê botão select com parâmetro 1

      break;                                     //break
    case 0x02:                                       //Caso 2
      disp.setCursor(1, 1);                      //Posiciona cursor
      disp.print("Black.L Auto >  ");
      readSelect(2);                             //Lê botão select com parâmetro 2

      break;                                     //break
    case 0x03:                                       //Caso 2
      disp.setCursor(1, 1);                      //Posiciona cursor
      disp.print("Menu Auto >     ");
      readSelect(3);                             //Lê botão select com parâmetro 3

      break;                                     //break


  } //end switch menu

} //end dispMenu


void readSelect(char option)                            //Leitura do botão select para seleção de subMenus
{

  if (!digitalRead(select))     t_select    = 0x01;     //Botão select pressionado? Seta flag
  if (digitalRead(select) && t_select)                 //Botão select solto e flag setada?
  { //Sim...
    t_select = 0x00;                                //Limpa flag

    switch (option)
    {
      case 0x01:
        subMenu4_1();
        disp.clear();
        break;

      case 0x02:
        subMenu4_2();
        break;

      case 0x03:
        subMenu4_3();
        disp.clear();
        break;

    } //end switch option

  } //end if


} //end readSelect
//========================================================================================================== Sub menus 4 ==========================================================================================================
void subMenu4_1()
{
  disp.clear();                 //Limpa o display
  while (stopW)
  {
    BlackLight();               //Executa a funcao do blacklight
  }
  stopW = 1;                    //Seta a flag, para a proxima vez entrar no laco while

} //end subMenu4_1


void subMenu4_2()
{

  while (1)
  {
    flagBlackAuto = 1;                            //Liga o BlackLight automatico
    disp.setCursor(2, 0);
    disp.print("BlackLigh Auto");

    BlackLightAuto();                   //Executa a funcao

    disp.setCursor(1, 1);
    disp.print("PWM do LDR: ");
    disp.setCursor(13, 1);
    disp.print(valorLDR);              //Mostra o valor do PWM setado
    delay(1500);                       //Da uma pausa
    disp.clear();                      //Limpa o display
    break;                             //Finaliza o submenu
  }

} //end subMenu4_2


void subMenu4_3()
{
  disp.clear();
  while (1)
  {
    disp.setCursor(3, 0);
    disp.print("Menu auto");

    if (flagmenuAuto) {                               //Se a flag do menu automatico ta setada exibe que a funcao ta ligada
      disp.setCursor(4, 1);
      disp.print("Ligado");
    }
    else {                                          //Se a flag eh 0, mostra desligado
      disp.setCursor(3, 1);
      disp.print("Desligado");
    }

    if (!digitalRead(butP))   t_butP   = 0x01;         //Botão Up pressionado? Seta flag
    if (digitalRead(butP) && t_butP)                   //Botão Up solto e flag setada?
    { //Sim...
      t_butP = 0x00;                                   //Limpa flag

      flagmenuAuto++;                                    //Se o botao foi precionado muda o estado on/off

      if (flagmenuAuto > 1)flagmenuAuto = 0;
      disp.clear();
    }

    if (!digitalRead(select))     t_select    = 0x01;     //Botão select pressionado? Seta flag
    if (digitalRead(select) && t_select)                  //Botão select solto e flag setada?
    { //Sim...
      t_select = 0x00;                                     //Limpa flag
      break;                                              //Finaliza a funcao do menu automatico
    } //end if
  }
} //end subMenu4_3


/*
//========================================================================================================== Lights ==========================================================================================================
void lights()                                           //Acionamento de lâmpadas (menu3)
{
  disp.setCursor(3, 0);                                //Posiciona cursor na coluna 1, linha 1
  disp.print("Lampadas");                      //Imprime mensagem do menu 3

  if (!digitalRead(butP))    t_butP    = 0x01;         //Botão P pressionado? Seta flag

  if (digitalRead(butP) && t_butP)                     //Botão P solto e flag setada?
  { //Sim...

    t_butP = 0x00;                                    //Limpa flag
    set1++;                                           //Incrementa set1

    if (set1 > 2) set1 = 0x01;                        //Se maior que 2, volta a ser 1

    switch (set1)                                     //Controle do set1
    {
      case 0x01:                                    //Caso 1
        disp.setCursor(0, 1);                   //Posiciona cursor na coluna 1, linha 2
        disp.print("L1 on ");                   //Imprime mensagem
        digitalWrite(Lamp1, HIGH);              //Liga lâmpada 1
        break;                                  //Break
      case 0x02:                                    //Caso 2
        disp.setCursor(0, 1);                   //Posiciona cursor na coluna 1, linha 2
        disp.print("L1 off");                   //Imprime mensagem
        digitalWrite(Lamp1, LOW);               //Desliga lâmpada 1
        break;                                  //Break

    } //end switch set1

  } //end butP

} //end lights
*/
/*
// ========================================================================================================== Serial monitor ===========================================================================================================
void serial()
{
  if ((millis() / 1000) >= tempoSerial) { // So entra depois do tempo

    Serial.print("Combustivel: ");
    Serial.println(analogRead(boia));
    Serial.print("Voltagem: ");
    Serial.println(Vi);
    Serial.print("Tempo de execucao: ");
    Serial.println(millis() / 1000);

    Serial.println("\n\n\n\n\n\n");

    tempoSerial = tempoSerial + 2; // tempo para esperar
  }
}
*/


