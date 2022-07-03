# Arduino Mega Musik Box "Party-ino"

Eine Gruppenarbeit für das Modul Hardwarenahe Programmierung im Rahmen des CAS Computer Science 1 an der ZHAW.

Um den Programmcode zu kompilieren, müssen folgende Bibliotheken explizit installiert
werden:
- RFID Library: MFRC522-spi-i2c-uart-async
- IR Sensor (Fernbedienung): IRremote
- MP3 Modul: DFRobotDFPlayerMini
- Arduino Threads: ArduinoThread

Sollen zusätzliche Songs abgespielt werden können, so müssen die Songs auf die SD Karte des MP3
Player Moduls kopiert werden. Das Modul erkennt mp3 und wma Files. Wichtig ist, dass die
Lieder fortlaufend mit 000x_lied-titel nummeriert werden und im Ordner MP3 gespeichert
werden.
Um weitere Songs von RFID Karten zu hinterlegen muss im Programmcode unter // Songs und RFID Karten / Batches ein Name definiert und der RFID Code hinterlegt werden.
Der RFID Code kann mit dem Setup ermittelt werden, in dem man am Ende der Funktion RFID_leser() Funktion, die Variable “code” im Monitor ausgibt. Das Lied muss mit dem Kürzel des RFID Codes in der play_song Funktion angegeben werden.

**Beispiel:**
case NEUES_LIED_KARTE:
player.play(NEUES_LIED);
break;

# Change Log
## Version 1.0 (03.07.2022)
- Arduino spielt Musik von der SD-Karte ab
- Einlesen von RFID-Tags
- Buttons für Play/Pause, Back, Forward
- Lautstärkeregulierung via Potentiometer
