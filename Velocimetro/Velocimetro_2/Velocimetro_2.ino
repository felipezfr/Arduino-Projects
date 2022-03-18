
#define velo 13



int pulso = 0;
int pulsoPorVolta = 1;
double mmSeg;
int kmH;
int bot = 0;
unsigned long perimRoda = 1400;//mm 2*Pi*r

long tempo, tempoAnt, tempoPulso;


void setup() {

  Serial.begin(9600);

  pinMode(velo, INPUT);


}

void loop() {

    velocimetro();

}

void velocimetro()
{
  if (!digitalRead(velo))bot = 1;         //Botão M pressionado? Seta flag
  if (digitalRead(velo) && bot)            //Botão P solto e flag setada?
  {
    bot = 0;
    //tempo = millis();

    if (millis() != tempoAnt) {

      tempoPulso = millis() - tempoAnt;
      tempoAnt = millis();

      Serial.print("Pulso+: ");
      Serial.println(tempoPulso);


    
      mmSeg = ((1000*perimRoda) / tempoPulso);

      //Serial.print("mm/seg: ");
      //Serial.println(mmSeg);

      kmH = mmSeg * 0.0036;

      Serial.print("Km/h: ");
      Serial.println(kmH);
      Serial.println("\n");
    }
    
  }





}
