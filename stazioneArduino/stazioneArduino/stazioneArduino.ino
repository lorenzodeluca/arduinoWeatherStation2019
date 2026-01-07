#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <Wire.h> 
#include "LiquidCrystal_I2C-1.1.2/LiquidCrystal_I2C.h"
#define fotoresistor A0

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = {
  0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02
};

//global var to prevent memory problems
unsigned long millisecOnTimeInit,millisecPassed,orePassed,minPassed,secPassed;
int packetSize,sizeOfTimePacket=15,light,temperat,umid,pres,randomN;
int dayNow,monthNow,yearNow,oraNow,minutiNow,secNow;
unsigned int localPort=9999,serverPort=9998;
EthernetUDP Udp;
char packetBuffer[UDP_TX_PACKET_MAX_SIZE];  // buffer to hold incoming packet,
IPAddress serverIp(192,168,8,7);
IPAddress myIp(192,168,8,214);
char ReplyBuffer[] = "ack";        // a string to send back
String nomeStazione="IT/Venice",day,month,year,ora,minuti,sec;
String toSend="";
boolean justUpdated=false;
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

void setup() {
  lcd.init();
  lcd.backlight();
  // You can use Ethernet.init(pin) to configure the CS pin
  //Ethernet.init(10);  // Most Arduino shields
  //Ethernet.init(5);   // MKR ETH shield
  //Ethernet.init(0);   // Teensy 2.0
  //Ethernet.init(20);  // Teensy++ 2.0
  //Ethernet.init(15);  // ESP8266 with Adafruit Featherwing Ethernet
  //Ethernet.init(33);  // ESP32 with Adafruit Featherwing Ethernet
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  
  // print your local IP address:
  Serial.print("My IP address: ");
  Ethernet.begin(mac,myIp);
  Serial.println(Ethernet.localIP());
  // start UDP
  Udp.begin(localPort);
  //Udp.beginPacket(serverIp, serverPort);
  //Udp.write("New Arduino station");
  //Udp.endPacket();
  lcd.print("Lorenzo De Luca");
  lcd.setCursor(2,1);
  lcd.print("ArduWeather");
  delay(3000);
}


void loop() {
  randomSeed(millis());
  //read and send of the data to the server
  //gestione richieste
   packetSize = Udp.parsePacket();
  if (packetSize) {
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    Serial.print("From ");
    IPAddress remote = Udp.remoteIP();
    for (int i=0; i < 4; i++) {
      Serial.print(remote[i], DEC);
      if (i < 3) {
        Serial.print(".");
      }
    }
    Serial.print(", port ");
    Serial.println(Udp.remotePort());
    // read the packet into packetBufffer
    Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
    Serial.println("Contents:");
    Serial.println(packetBuffer);
    if(packetSize==sizeOfTimePacket||true){//tutti i pacchetti che il server java invia all'arduino sono per aggiornare l'ora 
      justUpdated=true;
      //lettura ora formato gg:mm:aaaa:hh:mm:ss
      millisecOnTimeInit=millis();
      day=String(packetBuffer)[0];
      day[1]+=String(packetBuffer)[1];
      month=String(packetBuffer)[3];
      month+=String(packetBuffer)[4];
      year=String(packetBuffer)[6];
      year+=String(packetBuffer)[7];
      year+=String(packetBuffer)[8];
      year+=String(packetBuffer)[9];
      ora=String(packetBuffer)[11];
      ora+=String(packetBuffer)[12];
      minuti=String(packetBuffer)[14];
      minuti+=String(packetBuffer)[15];
      sec=String(packetBuffer)[17];
      sec+=String(packetBuffer)[18];
    }
    // send a reply to the IP address and port that sent us the packet we received(Acknowledge)
    //Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    //Udp.write(ReplyBuffer);
    //Udp.endPacket();
  }
  //aggiornamento ora
  dayNow=day.toInt();
  monthNow=month.toInt();
  yearNow=year.toInt();
  oraNow=ora.toInt();
  minutiNow=minuti.toInt();
  secNow=sec.toInt();
  if(!justUpdated){
    millisecPassed=millis()-millisecOnTimeInit;
    secPassed=millisecPassed/1000;
    minPassed=secPassed/60;
    orePassed=minPassed/60;
    oraNow+=orePassed;oraNow%=60;
    minutiNow+=minPassed;minutiNow%=60;
    secNow+=secPassed;secNow%=60;
  }
  justUpdated=false;
  //composizione dati da inviare
  //formato:    NomeStazione;giorno;mese;anno;ora;minuti;sec;luminosità;temperatura;umidità;pressione
  toSend="";//"data://";
  toSend+=nomeStazione;toSend+=",";
  toSend+=dayNow;toSend+=",";
  toSend+=monthNow;toSend+=",";
  toSend+=yearNow;toSend+=",";
  toSend+=oraNow;toSend+=",";
  toSend+=minutiNow;toSend+=",";
  toSend+=secNow;toSend+=",";
  light=analogRead(fotoresistor);
  toSend+=light;toSend+=",";//luminosità
  temperat=random(5)+17;
  toSend+="19";toSend+=",";//temperatura
  umid=random(30)+40;
  toSend+="30";toSend+=",";//umidità
  pres=random(200)+900;
  toSend+="1000";//pressione
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Lum:");lcd.print(light);
  lcd.print(" temp:");lcd.print(temperat);
  lcd.setCursor(0,1);
  lcd.print("hPa:");lcd.print(pres);
  lcd.setCursor(8,1);
  lcd.print(" Umid:");lcd.print(umid);
  Serial.println(toSend);
  //invio dati
  if(1==1){//useless statement to limit the temp variable life area(to prevent the reallocation of the variable)
    char temp[toSend.length()+1];
    toSend.toCharArray(temp, toSend.length());
    Udp.beginPacket(serverIp,serverPort);
    Udp.write(temp);
    Udp.endPacket();
  }
  delay(500);
}
