/*
   Curso de Arduino Aula 23
   
   Escrevendo e lendo a memória EEPROM


*/

// --- Bibliotecas Auxiliares ---
#include <EEPROM.h>

// --- Protótipo das Funções Auxiliares ---
int disp7seg(int number);

// --- Variáveis Globais ---
int address = 0;                                          //Endereço da EEPROM
char counter;                                             //Variável de contagem

void setup()
{
   
  for(char i=0;i<7;i++) pinMode(i,OUTPUT);                //pinos digitais 0 a 6 como saída
  pinMode(8,INPUT_PULLUP);                                //pino digital 8 como entrada com pull-up interno
  
  PORTD = disp7seg(0);                                    //Primeiro inicialização
  counter = EEPROM.read(address);                         //Lê EEPROM e armazena em counter
}

void loop()
{
  
  if(!digitalRead(8))                                     //Botão pressionado?
  {                                                       //Sim...
     counter++;                                           //Incrementa contador
     delay(70);                                           //Delay anti bouncing
  }
  if(counter > 0x09) counter = 0x00;                      //Se contador maior que 9, volta a ser zero
  
  PORTD = disp7seg(counter);                              //PORTD recebe o valor do display passado como parâmetro
  
  
  EEPROM.write(address, counter);                         //Escreve valor atual de counter na EEPROM

  delay(100);
}

// --- Desenvolvimento Funções Auxiliares ---
int disp7seg(int number)
{
    int aux;
    //Vetor para o barramento de dados do display 7 segmentos (PORTD)
    int catodo[10] = {0x3f,    //'0'
                      0x06,    //'1'
                      0x5b,    //'2'
                      0x4f,    //'3'
                      0x66,    //'4'
                      0x6d,    //'5'
                      0x7d,    //'6'
                      0x07,    //'7'
                      0x7f,    //'8'
                      0x67};   //'9'
                      
    aux = catodo[number];      //aux recebe o vetor com o parâmetro number
    
    return(aux);               //retorna o valor do display
    
} //end disp7seg
