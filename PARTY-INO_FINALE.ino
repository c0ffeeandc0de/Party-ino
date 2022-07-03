// *** RFID Reader ***
// RFID Bibliotheken
#include <SPI.h>                       // SPI-Bibiothek hinzufügen
#include <MFRC522.h>                   // RFID-Bibiothek hinzufügen
// RFID Port Konfig
#define SS_PIN 53                      // SDA: Arduino Mega Pin 53, Arduino Uno Pin 10
#define RST_PIN 5                      // RST: Arduino Mega Pin 5, Arduino Uno Pin 9 
MFRC522 mfrc522(SS_PIN, RST_PIN);      // RFID-Empfänger benennen (instanz)

// *** MP3 Player ***
// MP* Player Bibliotheken
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
// MP3 Player Port Konfig
SoftwareSerial softwareSerial(15, 14);  // RX, TX
DFRobotDFPlayerMini player;             // Instanz player

// *** Fernbedienung ***
#include <IRremote.h>                   // Infrarot Sensor für Fernbedienung hinzu fügen. 
int RECV_PIN = 11;                      // Infrarot Receiver Pin    
IRrecv irrecv(RECV_PIN);                // IRrecv Objekt -> um PIN 11 zu lesen
decode_results results;                 // Decodiert übertrag

// *** Threads ***
// Importieren
#include <Thread.h>
#include <ThreadController.h>

// Kontrolliert alle Threads
ThreadController controll = ThreadController();

//new Thread
Thread tasten = Thread();

// *** Definieren der Namen der Ein und Ausgänge 

// Variable
int vol_level = 15;

// Buttons
#define BUT_PRE 8
#define BUT_STA_STO 9
#define BUT_NEXT 10
#define MP3_STATUS 4
#define LED_PLAY 7

// Fernbedienung
#define VOL_PLUS 16736925
#define VOL_MINUS 16754775
#define FERN_NEXT 16761405
#define FERN_PRE 16720605
#define FERN_PLAY_PAUSE 16712445

// Songs und RFID Karten / Batches
  //Jonathan
#define JON_BATCH 52020
#define JON_CARD 2153860
#define JON_SONG_BATCH 1
#define JON_SONG_CARD 2

  // Robert
#define ROB_BATCH 927010
#define ROB_CARD 1526150
#define ROB_SONG_BATCH 3
#define ROB_SONG_CARD 4

  // Rolf
#define ROL_BATCH 2604750
#define ROL_CARD 706200
#define ROL_SONG_BATCH 5
#define ROL_SONG_CARD 6

void setup() {

  Serial.begin(9600);                  // Serielle Verbindung starten (für Debugging)

  // Thread tasten Konfigurieren
  tasten.onRun(readSwitches);
  tasten.setInterval(10);
  //Threads zum Kontroller hinzufügen
  controll.add(&tasten);

  //Verbindung mit RFID aufbauen
  SPI.begin();                         // SPI-Verbindung aufbauen
  mfrc522.PCD_Init();                  // Initialisierung des RFID-Empfängers

  irrecv.enableIRIn();                 //initialisiert des Infrarotempfänger.

  // Verbindung mit Player aufbauen
  softwareSerial.begin(9600);
  player.begin(softwareSerial);
  player.volume(15);

  // Konfig Ein und Ausgänge
  pinMode(MP3_STATUS,INPUT);
  pinMode(BUT_PRE,INPUT);
  pinMode(BUT_STA_STO,INPUT);
  pinMode(BUT_NEXT,INPUT);
  pinMode(LED_PLAY,OUTPUT);
}

// callback für den Thread tasten
void readSwitches(){
  if(digitalRead(BUT_PRE)== HIGH){
    player.previous();
    }
  if(digitalRead(BUT_NEXT)== HIGH){
    player.next();
    }
  if(digitalRead(BUT_STA_STO)== HIGH){
    if(digitalRead(MP3_STATUS)==HIGH){
      player.start();
      }
      else{
        player.pause();
      }
    } 
  }

void play_song(long RFID_song){

  Serial.println(RFID_song);        // fuer Debugging resp neue Karte hinzufügen
  switch (RFID_song){
    case JON_BATCH:
      player.play(JON_SONG_BATCH);
      break;
    case JON_CARD:
      player.play(JON_SONG_CARD);
      break;
    case ROB_BATCH:
      player.play(ROB_SONG_BATCH);
      break;        
    case ROB_CARD:
      player.play(ROB_SONG_CARD);
      break;   
    case ROL_BATCH:
      player.play(ROL_SONG_BATCH);
      break;   
    case ROL_CARD:
      player.play(ROL_SONG_CARD);
      break;   
  }
  delay(100);                     // geht auch ohne, um zu verhindern dass Song mehrfach gestartet wird
  }

void RFID_leser() {
  
  long code = 0;

  for (byte i = 0; i < mfrc522.uid.size; i++)  {
    // statt 4 Zahlenbloecke (HEX, DEZ) wird eine Zahl erstellt:
    code = ((code + mfrc522.uid.uidByte[i]) * 10);       // jeder Block wird ausgelesen und mit 10 "gestreckt" (eigentlich 1000, dann wird aber Zahl zu gross
    }
  play_song(code);  
}

void fernbedienung(long tasten_code){
  switch (tasten_code){
    case VOL_PLUS:                    // wenn Fernbedienung statt Poti
      /*Serial.println("vol+");       // wenn Fernbedienung -> in loop volume() auskommentieren
      if(vol_level < 30){
        vol_level = vol_level + 1;
        }
      player.volume(vol_level);*/
      break;
    case VOL_MINUS:                   // wenn Fernbedienung statt Poti
      /*Serial.println("vol-");       // wenn Fernbedienung -> in loop volume() auskommentieren
      if(vol_level > 0){
        vol_level = vol_level - 1;
        } 
      player.volume(vol_level);*/     
      break;
    case FERN_PRE:
      player.previous();
      break;
    case FERN_NEXT:
      player.next();
      break;   
    case FERN_PLAY_PAUSE:
      if(digitalRead(MP3_STATUS)==HIGH){  // Wenn MP3 Status HIGH -> es wird kein Song gespielt -> also "starten" nach pressen des Play/Pause Button
      player.start();
      }
      else{                               // wenn MP3 Status LOW -> es wird ein Song gespielt -> also "pause" nach pressen des Play/Pause Button
        player.pause();
        }
      break;       
    }  
  }

void volume(){
  // skalliert analog Werte 0 - 1023 auf 0 - 25
  player.volume(analogRead(A1)/34); //1023/30 -> ca 34
  }

void loop() {

  controll.run();

  // set Volume
  volume();

  // read RFID
  if ( mfrc522.PICC_IsNewCardPresent() and mfrc522.PICC_ReadCardSerial()) //schaut ob Sender vorhand und Karte in Reichweite
  {
    RFID_leser();
  }

  // Fernbedienung Auslesen
  if (irrecv.decode(&results)) {         //Wenn Daten empfangen wurden,  mit & wird eine Referenz gemacht 
    fernbedienung(results.value);
    irrecv.resume();                    //Der nächste Wert soll vom IR-Empfänger eingelesen werden
  }

  // PLAY LED An
  if(digitalRead(MP3_STATUS) == LOW){
    digitalWrite(LED_PLAY,HIGH);
    }
  else{
    digitalWrite(LED_PLAY,LOW);
    }
}
