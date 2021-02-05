

#include <SPI.h>
#include <Ethernet.h>

boolean incoming = 0;


byte mac[] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDA, 0x02 };
IPAddress ip(172,10,10,4); 
IPAddress gateway(172, 10, 10, 1);
IPAddress subnet(255, 255, 0, 0);


EthernetServer server(80);

void setup()
{
  pinMode(2, OUTPUT);

  //me
  digitalWrite(2, HIGH);

  Ethernet.begin(mac, ip, gateway, subnet);
  server.begin();
  Serial.begin(9600);
}

void loop()
{
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        
        //reads URL string from $ to first blank space
        if(incoming && c == ' '){ 
          incoming = 0;
        }
        if(c == '$'){ 
          incoming = 1; 
        }
        
        //Checks for the URL string $1 or $2
        if(incoming == 1){
          Serial.println(c);
          
          if(c == '1'){
            Serial.println("ON");
            //digitalWrite(2, HIGH);
            //me
            digitalWrite(2, LOW);//relay on (connect)
            //delay(1000);
            //digitalWrite(2, HIGH);//relay off (disconnect)
            //delay(1000);
          }
          if(c == '2'){
            Serial.println("OFF");
            //digitalWrite(2, LOW);
            //me
            //digitalWrite(2, LOW);
            //delay(2000);
            digitalWrite(2, HIGH);//relay off (disconnect)
            //delay(2000);
          }
        
        }

        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
  }
}

