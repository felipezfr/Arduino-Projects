/*
   Curso de Arduino e AVR WR Kits Channel
   
   Aula 101 - Salvando e Recuperando dados na EEPROM


   Autor: Eng. Wagner Rambo  Data: Janeiro de 2017
   
   www.wrkits.com.br | facebook.com/wrkits | youtube.com/user/canalwrkits
   
*/

// =============================================================================================================
// --- Bibliotecas ---
#include <LiquidCrystal.h>      //Biblioteca para o display LCD
#include <EEPROM.h>             //Biblioteca para escrever/ler da EEPROM
 
 
// =============================================================================================================
// --- Mapeamento de Hardware ---
#define     botao1      10     //botão1
#define     botao2      11     //botão2
 
 
// =============================================================================================================
// --- Protótipo das Funções ---
void read_but();                               //função para leitura do botão


// =============================================================================================================
// --- Variáveis Globais --- 

boolean        but1_flag = 0x00,               //flag auxiliar para o botão 1
               but2_flag = 0x00;               //flag auxiliar para o botão 2
               
short          option    = 0x00;               //variável para definir opção inicial

int            addr      = 0x00;               //armazena o endereço atual da EEPROM

unsigned short counter   = 0x00;               //contador auxiliar
         

// =============================================================================================================
// --- Hardware do LCD ---
LiquidCrystal disp(8,  //RS no digital 8
                   7,  //EN no digital 7
                   5,  //D4 no digital 5
                   4,  //D5 no digital 4
                   3,  //D6 no digital 3
                   2); //D7 no digital 2


// =============================================================================================================
// --- Configurações Iniciais ---
void setup()
{

  pinMode(botao1  , INPUT_PULLUP);      //entrada para o botão1 com pull-up interno
  pinMode(botao2  , INPUT_PULLUP);      //entrada para o botão2 com pull-up interno
   

  disp.begin(16,2);                    //Inicializa LCD 16 x 2
  disp.clear();                        //Limpa LCD

  read_but();                          //Chama função para leitura dos botões

  disp.clear();                        //limpa LCD

  disp.setCursor(2,0);                 //posiciona cursor na coluna 3 linha 1
  disp.print("Contador:");             //imprime
  
   
} //end setup


// =============================================================================================================
// --- Loop Infinito ---
void loop()
{
   counter++;                           //incrementa contador
   delay(500);                          //a cada 500ms 

   disp.setCursor(2,1);                 //posiciona curso na coluna 3 linha 2
   disp.print(counter);                 //imprime

   EEPROM.write(addr, counter);         //escreve o valor no endereço atual da EEPROM

 
  //Descomentar linhas a seguir se quiser escrever em outros endereços da EEPROM:
  //addr = addr + 1;
  //if (addr == EEPROM.length()) addr = 0;
    
  
} //end loop



// =============================================================================================================
// --- Desenvolvimento das Funções ---

// =============================================================================================================
void read_but()                                //função para leitura do botão
{

  do                                           //inicio do loop finito
  {
     if(!digitalRead(botao1)) but1_flag = 0x01;  //botão1 pressionado? Seta flag
     if(!digitalRead(botao2)) but2_flag = 0x01;  //botão2 pressionado? Seta flag

     if(digitalRead(botao1) && but1_flag)      //botão1 solto e flag setada?
     {                                         //sim...
         but1_flag = 0x00;                     //limpa flag

         option   = 0x02;                      //option 2

     } //end if

     if(digitalRead(botao2) && but2_flag)      //botão2 solto e flag setada?
     {                                         //sim...
         but2_flag = 0x00;                     //limpa flag

         option   = 0x01;                      //option 1

     } //end if

     disp.setCursor(0,0);                 //posiciona cursor na coluna 1 linha 1
     disp.print("Recupera EEPROM?");      //imprime
     disp.setCursor(0,1);                 //posiciona cursor na coluna 1 linha 2
     disp.print("1 - Sim  2 - Nao");      //imprime


  }  while(option == 0x00);               //roda enquanto option for igual a zero



     if(option == 0x01)                   //opção escolhida igual a 1?
     {
       
        counter = EEPROM.read(addr);      //lê valor no endereço atual e armazena em counter
     }

     else if(option == 0x02)              //opção escolhida igual a 2?
     {
      
        EEPROM.write(addr, 0xFF);         //limpa endereço atual da EEPROM
        counter = 0x00;                   //inicia contador do zero
     }
     
  
} //end read_but




