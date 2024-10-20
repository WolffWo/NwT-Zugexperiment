/*

Dieser Arduino Sketch dient zur Steuerung und messtechnischen Erfassung eines 
Zugexperimentes mit Hilfe einer Waegezelle und eines Schrittmotors. Mit Hilfe 
der gewonnenen Daten kann ein Zug-Verlängerungsdiagramm erstellt werden und 
z.B. das E-Modul des verwendeten Testkörpers (Stahldraht Druckmesser 0,2mm^2) 
erstellt werden.   

Als Basis diese Sketches diente ein Arduino Sketch den ich auf einer Forbildung 
erhalten habe. Leider ist mir der Author nicht gekannt.  

Weitere Informationen: https://github.com/WolffWo/NwTZugexperiment

Author: Wolfgang Wolff

*/


#include "HX711.h"    //Bibliothek der Wägezelle --> muss heruntergeladen werden
#define DOUT  3       // Anschlüsse der Wägezelle
#define CLK  4        // Anschlüsse der Wägezelle
#include <Stepper.h>

HX711 kraftMesser;

/* Parameter zur Berechung der Verlängerung:

0,8mm pro Umdrehung bei M5-Gewinde und einer Übersetzung von 2,5 (Getriebe von 20 Zaehnen auf 50 Zaehne)
Sollte ein anderes Gewinde und/oder Uebersetzung verwendet werden, muessen die Werte entsprechend angepasst werden

*/

float gewindeSteigung = 0.8;         
float getriebeUebersetzung = 2.5; 


/* Kallibrierung der Waegezelle

  Vorgehen: 
  
  1.) Zunächst folgende Werte bei der Variblen eintragen:
  long yAchsenAbschnitt = 0;
  long kalibrierSteigung = 1; 

  2.) Mindestens 10 Messungen ohne Last auf der Waegezelle durchführen und den 
  gemittelten Ausgabewert durch 10 teilen und als Startwert yAchsenAbschnitt 
  eintragen, z.B:
  long yAchsenAbschnitt = 94401;

  3.) Anschließend mit 10 N Last am Sensor (z.B. Gewicht mit ein 1kg über Umlenkrolle oder 10N Kraftmesser) mindestens 10 Messungen durchführen, 
  den gemittelten Ausgabewert durch 10 teilen und bei kalibriertsteigung eintragen, z.B.
  long kalibrierSteigung = 421424;

  Anschließend kann das Experiment durchgeführt werden.

*/

long yAchsenAbschnitt = 94401; // für yAchsenAbschnitt eine 0 eintragen, für kalibrierSteigung eine 1 eintragen, eine Messung ohne Last am Sensor durchführen, den gemittelten Ausgabewert bei yAchsenAbschnitt eintragen. 
long kalibrierSteigung = 421424; //Anschließend mit 10 N Last am Sensor eine Messung durchführen, den gemittelten Ausgabewert durch 10 teilen und bei kalibriertsteigung eintragen, 

const int stepsPerRevolution = 2048;        //Schritte pro Umdrehung beim Schrittmotor
Stepper schrittmotor(stepsPerRevolution, 17, 15, 16, 14);  //Aufeinanderfolgende Anschlüsse des Schrittmotors. Es werden die Pins 17 (A3) - 14 (A0) verwendet, damit die Anschlüsse der Stepperplatine auf dem Arduinoboard alle 6 nebeneinander liegen.


float deltaL = 0; // Laengenaenderung des Drahtes



void setup() {
  Serial.begin(9600);
  kraftMesser.begin(DOUT, CLK);
  schrittmotor.setSpeed(15);
  pinMode(2,OUTPUT);
  digitalWrite(2,LOW);  //hier kann der GDN-Anschluss der Wägezellenplatine angeschlossen werden.
  pinMode(5,OUTPUT);
  digitalWrite(5,HIGH);   //hier kann der VCC-Anschluss der Wägezellenplatine angeschlossen werden. So liegen alle 4 Anschlüsse der Wägezelle auf dem Arduino-Board nebeneinander.
  }

void loop() {
  schrittmotor.step(stepsPerRevolution/20);
  Serial.print(deltaL);   //nach jeder Umdrehung (4s) wird die Zeit abgelesen
  deltaL += gewindeSteigung / getriebeUebersetzung/20;    // Laengenaenderung entspricht Steigung des Gewindes pro Übersetzungsverhaeltnis
  Serial.print(";");
  //Die Werte sollten vor der Messung kalibriert werden:
  Serial.print((kraftMesser.get_units() - yAchsenAbschnitt) / kalibrierSteigung, 3);
  Serial.println();
}
