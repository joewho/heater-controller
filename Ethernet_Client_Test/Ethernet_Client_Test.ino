#include <SPI.h>
#include <Ethernet.h>

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
EthernetServer server(80);
byte  numPins = 5;
int  pins[] = {3, 4, 5, 6, 7};
int pinState[] = {1, 1, 0, 0, 0};
char line1[100];

void setup() {
  for (int i = 0; i < numPins; i++) {
    pinMode(pins[i], OUTPUT);
    digitalWrite(pins[i], pinState[i]);
  }
  Serial.begin(9600);
  Ethernet.begin(mac);
  server.begin();
  Serial.print("Server started on: ");
  Serial.println(Ethernet.localIP());
}

void loop() {
  //listen for incoming clients
  EthernetClient client = server.available();
  if (client) {

    while (client.connected()) {
      readHeader(client);
      if (!pageNameIs("/")) {
        //Serial.println("client stop");
        client.stop();
        return;
      }
      //send header
      client.println("HTTP/1.1 200 ok");
      client.println("Content-Type: text/html");
      client.println();


      //send body
      client.println("<html><body>");
      client.println("<h1>Outpin Pins</h1>");
      client.println("<form method='GET'>");
      setValuesFromParams();
      setPinStates();
      for (int i = 0; i < numPins; i++) {
        writeHTMLforPin(client, i);
      }
      client.println("<input type='submit' value='Update'/>");
      client.println("</form>");
      client.println("</body></html>");
      client.stop();
    }//while client connected
  }//if client
}//loop

void writeHTMLforPin(EthernetClient client, int i) {
  client.print("<p>D");
  client.print(pins[i]);
  client.print(" <select name='");
  client.print(i);
  client.println("'>");
  client.print("<option value='0'");
  if (pinState[i] == 0) client.print(" selected='0'");
  client.println(">Off</option>");
  client.print("<option value='1'");
  if (pinState[i] == 1) client.print(" selected='1'");
  client.println(">On</option>");
  client.println("</select></p>");
}

void setPinStates() {
  for (int i = 0; i < numPins; i++) {
    digitalWrite(pins[i], pinState[i]);
    //    Serial.println("setPinState");
    //    Serial.print(i);
    //    Serial.print(": ");
    //    Serial.println(pinState[i]);
  }
  //  Serial.println();
}

void setValuesFromParams() {
  for (int i = 0; i < numPins; i++) {
    pinState[i] = valueOfParam(i + '0');
  }
}

void readHeader(EthernetClient client) {
  //read first line of header
  char ch=' ';
  int i = 0;
  
  //Serial.println("readHeader() - client.read(): ");
  while (ch != '\n') {
    //Serial.print( "!==n");
    if (client.available()) {
      //Serial.println(" client available");
      //Serial.println(client.read())
      ch = client.read();
    //  Serial.print(ch);
      line1[i] = ch;
      i++;
    }
  }//while
  //Serial.println();
  line1[i] = '\0';
  //Serial.print("readHeader() - line1:");
  //Serial.println(line1);
}

bool pageNameIs(char* name) {
  //Serial.println("pageNameIs...");
  //page name starts at char pos 4
  //ends with space
  int i = 4;
  char ch = line1[i];
  while (ch != ' ' && ch != '\n' && ch != '?') {
    if (name[i - 4] != line1[i]) return false;
//    Serial.print(i);
    //Serial.println(line1[i]);
    i++;
    ch = line1[i];
  }
  return true;
}

int valueOfParam(char param) {
  //Serial.print("Searching for param...");
  //Serial.println(param);
  for (int i = 0; i < strlen(line1); i++) {
    if (line1[i] == param && line1[i + 1] == '=') {
    //  Serial.print("FOUND PARAM: ");
     // Serial.println(param);
      return (line1[i + 2] - '0');
    }
  }
  return 0;
}
