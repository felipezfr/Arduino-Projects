/*
   Curso de Arduino WR Kits Aula 61

   www.wrkits.com.br
   facebook.com/wrkits
   youtube.com/user/canalwrkits

   Sub-Menu para Display LCD 16 x 2

   Autor: Eng. Wagner Rambo, Data: Abril de 2016

*/

// --- Bibliotecas Auxiliares ---
#include <LiquidCrystal.h>                              //Biblioteca para o display LCD

// --- Mapeamento de Hardware ---
#define butUp    12                                     //Botão para selecionar tela acima no digital 12
//#define butDown  11                                     //Botão para selecionar tela abaixo no digital 11
#define back     11
#define butP     10                                     //Botão de ajuste mais no digital 10
#define butM      9                                     //Botão de ajuste menos no digital 9
#define select    8                                     //Botão de seleção no digital 8
#define Lamp1    A0                                     //Saída para lâmpada 1 no A0 (será usado como digital)
#define Lamp2    A1                                     //Saída para lâmpada 2 no A1 (será usado como digital)

// --- Protótipo das Funções Auxiliares ---
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


// --- Variáveis Globais ---
char menu = 0x01;                                       //Variável para selecionar o menu
char subMenu4 = 0x01;                                   //Variável para selecionar subMenu no menu4
char set1 = 0x00, set2 = 0x00;                          //Controle das lâmpadas
boolean t_butUp, t_back, t_butP, t_butM, t_select;   //Flags para armazenar o estado dos botões

// --- Hardware do LCD ---
LiquidCrystal disp(7,  //RS no digital 7
                   6,  //EN no digital 6
                   5,  //D4 no digital 5
                   4,  //D5 no digital 4
                   3,  //D6 no digital 3
                   2); //D7 no digital 2

// --- Configurações Iniciais ---
void setup()
{
  disp.begin(16, 2);                                     //Inicializa LCD 16 x 2

  for (char i = 8; i < 13; i++) pinMode(i, INPUT_PULLUP); //Entrada para os botões (digitais 8 a 12) com pull-ups internos

  pinMode(Lamp1, OUTPUT);                                //Configura saída para lâmpada 1
  pinMode(Lamp2, OUTPUT);                                //Configura saída para lâmpara 2

  t_butUp   = 0x00;                                      //limpa flag do botão Up
  t_back    = 0x00;
  t_butP    = 0x00;                                      //limpa flag do botão P
  t_butM    = 0x00;                                      //limpa flag do botão M
  t_select  = 0x00;                                      //limpa flag do botão select

  digitalWrite(Lamp1, LOW);                              //Lâmpada 1 inicia apagada
  digitalWrite(Lamp2, LOW);                              //Lâmpada 2 inicia apagada


} //end setup


// --- Loop Infinito ---
void loop()
{


  changeMenu();
  dispMenu();

} //end loop

// --- Desenvolvimento das Funções Auxiliares ---
void changeMenu()                                       //Modifica o menu atual
{
  if (!digitalRead(butUp))   t_butUp   = 0x01;         //Botão Up pressionado? Seta flag

  if (digitalRead(butUp) && t_butUp)                   //Botão Up solto e flag setada?
  { //Sim...
    t_butUp = 0x00;                                   //Limpa flag

    disp.clear();                                     //Limpa display
    menu++;                                           //Incrementa menu

    if (menu > 0x04) menu = 0x01;                     //Se menu maior que 4, volta a ser 1

  } //end butUp


} //end changeMenu

void dispMenu()                                         //Mostra o menu atual
{
  switch (menu)                                       //Controle da variável menu
  {
    case 0x01:                                       //Caso 1
      data_hora();                               //Chama a função de relógio

      break;                                     //break
    case 0x02:                                       //Caso 2
      temperatura();                             //Chama a função do termômetro

      break;                                     //break
    case 0x03:                                       //Caso 3
      lights();                                  //Chama função para acionamento de lâmpadas

      break;                                     //break
    case 0x04:                                       //Caso 4
      menu4();                                   //Chama função para o menu4

      break;                                     //break

  } //end switch menu

} //end dispMenu

void data_hora()                                        //Data e Hora (menu1)
{

  disp.setCursor(0, 0);                                //Posiciona cursor na coluna 1, linha 1
  disp.print("1.Data e Hora");                           //Imprime mensagem
  disp.setCursor(0, 1);                                //Posiciona cursor na coluna 1, linha 2

  // Desenvolver uma função de data e hora...

  disp.print("27/08/15  15:14");                       //Mostrador (apenas ilustrativo......)

} //end data_hora

void temperatura()                                      //Temperatura (menu2)
{
  disp.setCursor(0, 0);                                //Posiciona cursor na coluna 1, linha 1
  disp.print("2.Temperatura");                           //Imprime mensagem
  disp.setCursor(1, 1);                                //Posiciona cursor na coluna 2, linha 2
  disp.print("25 Celsius");                            //Mostrador (apenas ilustrativo......)

} //end temperatura()

void lights()                                           //Acionamento de lâmpadas (menu3)
{

  disp.setCursor(0, 0);                                //Posiciona cursor na coluna 1, linha 1
  disp.print("3.Lampadas");                      //Imprime mensagem do menu 3


  if (!digitalRead(butP))    t_butP    = 0x01;         //Botão P pressionado? Seta flag
  if (!digitalRead(butM))    t_butM    = 0x01;         //Botão M pressionado? Seta flag    
    
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

  if (digitalRead(butM) && t_butM)                     //Botão D solto e flag setada?
  { //Sim...
    t_butM = 0x00;                                    //Limpa flag

    set2++;                                           //Incrementa set2

    if (set2 > 2) set2 = 0x01;                        //Se maior que 2, volta a ser 1

    switch (set2)                                     //Controle do set2
    {

      case 0x01:                                    //Caso 1
        disp.setCursor(8, 1);                   //Posiciona cursor na coluna 8, linha 2
        disp.print("L2 on ");                   //Imprime mensagem
        digitalWrite(Lamp2, HIGH);              //Liga lâmpada 2
        break;                                  //Break
      case 0x02:                                    //Caso 2
        disp.setCursor(8, 1);                   //Posiciona cursor na coluna 8, linha 2
        disp.print("L2 off");                   //Imprime mensagem
        digitalWrite(Lamp2, LOW);               //Liga lâmpada 1
        break;                                  //Break

    } //end switch set1

  } //end butM


} //end lights

void menu4()                                            //Função genérica para um quarto menu...
{
  disp.setCursor(0, 0);                                //Posiciona cursor na coluna 1, linha 1
  disp.print("4-Config");                                //Imprime mensagem

  dispSubMenu4();

} //end menu4


//pode-se implementar quantos menus quiser
//............


void dispSubMenu4()                                     //Mostra o sub menu atual para o menu 4
{

  if (!digitalRead(butP))    t_butP    = 0x01;         //Botão P pressionado? Seta flag
  if (!digitalRead(butM))    t_butM    = 0x01;         //Botão M pressionado? Seta flag

  if (digitalRead(butP) && t_butP)                     //Botão P solto e flag setada?
  { //Sim...
    t_butP = 0x00;                                    //Limpa flag

    subMenu4++;                                       //incrementa subMenu4

    if (subMenu4 > 3) subMenu4 = 0x01;                //se maior que 3, volta a ser 1


  } //end butP

  if (digitalRead(butM) && t_butM)                     //Botão D solto e flag setada?
  { //Sim...
    t_butM = 0x00;                                    //Limpa flag

    subMenu4--;                                       //decrementa subMenu4

    if (subMenu4 < 1) subMenu4 = 0x03;                //se menor que 1, volta a ser 3



  } //end butM



  switch (subMenu4)                                   //Controle da variável subMenu
  {
    case 0x01:                                       //Caso 1
      disp.setCursor(1, 1);                      //Posiciona cursor na coluna 2, linha 2
      disp.print("BlackLight    ");
      readSelect(1);                             //Lê botão select com parâmetro 1

      break;                                     //break
    case 0x02:                                       //Caso 2
      disp.setCursor(1, 1);                      //Posiciona cursor na coluna 2, linha 2
      disp.print("Contraste      ");
      readSelect(2);                             //Lê botão select com parâmetro 2

      break;                                     //break
    case 0x03:                                       //Caso 2
      disp.setCursor(1, 1);                      //Posiciona cursor na coluna 2, linha 2
      disp.print("Help            ");
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
      case 0x01: subMenu4_1(); break;

      case 0x02: subMenu4_2(); break;

      case 0x03: subMenu4_3(); break;

    } //end switch option

  } //end if


} //end readSelect


void subMenu4_1()
{
  while (1)
  {
    disp.setCursor(0, 0);                                //Posiciona cursor na coluna 1, linha 1
    disp.print("Tarefa 1");                              //Imprime mensagem
    disp.setCursor(0, 1);                                //Posiciona cursor na coluna 1, linha 2
    disp.print("Em funcionamento");                      //Mostrador (apenas ilustrativo......)
  }

} //end subMenu4_1


void subMenu4_2()
{
  while (1)
  {
    disp.setCursor(0, 0);                                //Posiciona cursor na coluna 1, linha 1
    disp.print("Tarefa 2");                              //Imprime mensagem
    disp.setCursor(0, 1);                                //Posiciona cursor na coluna 1, linha 2
    disp.print("Em funcionamento");                      //Mostrador (apenas ilustrativo......)
  }

} //end subMenu4_2


void subMenu4_3()
{
  while (1)
  {
    disp.setCursor(0, 0);                                //Posiciona cursor na coluna 1, linha 1
    disp.print("Tarefa 3");                              //Imprime mensagem
    disp.setCursor(0, 1);                                //Posiciona cursor na coluna 1, linha 2
    disp.print("Em funcionamento");                      //Mostrador (apenas ilustrativo......)
  }

} //end subMenu4_3

















