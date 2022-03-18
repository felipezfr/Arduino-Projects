
#define velo 2

long long timeold;
int perimRoda = 140;//2*Pi*r
int pulso = 0;
int pulsoPorVolta = 1;
int cmSeg, kmH;
int bot=0;

void contador()
{
 // if (!digitalRead(velo))    bot = 1;         //Botão M pressionado? Seta flag
 // if (digitalRead(velo) && bot)                //Botão P solto e flag setada?
 // {
   // bot = 0;
    pulso++;
    Serial.print("Pulso: ");
    Serial.println(pulso);
  //}

}
void setup() {

  Serial.begin(9600);

  pinMode(velo, INPUT);

  attachInterrupt(0, contador, RISING);

}
void loop() {

  /*int bot=0;
    if (!digitalRead(velo))    bot = 1;         //Botão M pressionado? Seta flag
    if (digitalRead(velo) && bot)                //Botão P solto e flag setada?
    {
    bot = 0;

    Serial.println("Pulso ++");
    pulso++;
    }*/

  if (millis() - timeold >= 1000)
  {
    timeold = millis();
    detachInterrupt(0);
    cmSeg = ((pulso / pulsoPorVolta) * perimRoda);
    kmH = (cmSeg * 0.036);
    pulso = 0;

    // Serial.print("Km/h: ");
    // Serial.println(kmH);
    attachInterrupt(0, contador, RISING);


  }




}
