#include <Parse.h>


/*
 * RedButton Arduino Yun
 * 
 * Read the remote receiver input pin
 * and report to a webservice
 * 
 * HARDWARE SETUP:
 * 
 */
#include <Bridge.h>
#include <Console.h>
#include <Process.h>

// Constants
#define BUTTONPIN 2
#define LEDPIN 13

const int delayTime = 200; // miliseconds

Process proc;
char buffer[64];


// Variables
int buttonState;
int previousButtonState;


// the setup routine runs once when you press reset:
void setup() {

  // We'll send debugging information via the Serial monitor
  Bridge.begin();
  Console.begin();

  while (!Console){
    ; // wait for Console port to connect.
  }
  Console.println("Connected");

  pinMode(BUTTONPIN, INPUT);
  pinMode(LEDPIN, OUTPUT);

  previousButtonState = digitalRead(BUTTONPIN);
}

// the loop routine runs over and over again forever:
void loop() {
  
  // read the state of the pushbutton value:
  buttonState = digitalRead(BUTTONPIN);

  // check if the pushbutton is pressed.
  // if it is, turn on the led
  if (buttonState == HIGH) {
    // turn LED on:
    digitalWrite(LEDPIN, HIGH);
    
    // check if there is a change in button state since last cycle
    if(buttonState!=previousButtonState){
      // something changed
      // notify 
      //sendUpdate();
    }
  } else {
    // turn LED off:
    digitalWrite(LEDPIN, LOW);
  }


  // save current button state to compare against in next cycle
  previousButtonState = buttonState;

  // delay in between reads for saving battery
  delay(delayTime); 
}


void sendUpdate() {

  String body = "{ \"deviceId\": \"\", \"volume\": \"\" }";
  String url = "https://api.parse.com/1/functions/updateStatus";
  
  send_request(body, url);

  wait_response();
  read_response();
}


void send_request_registration(){
}

void send_request_statusupdate(int value){
}

void send_request(String body, String url){
  Console.print("sending ");
  Console.println(url);

  proc.begin("curl");
  proc.addParameter("-k");
  proc.addParameter("-X");
  proc.addParameter("POST");
  proc.addParameter("-H");
  proc.addParameter("Content-Type:application/json");
  // POST body
  proc.addParameter("-d");
  proc.addParameter(body);
  proc.addParameter(url);

  // Run the command synchronously
  proc.run();
}

/*
** Wait for response
*/

void wait_response(){
  while (!proc.available()) {
    delay(100);
  }
}

/*
** Read the response and dump to console
*/

void read_response(){
  bool print = true;
  
  while (proc.available()) {
    char c = proc.read();
    // Print only until the first carriage return
    if (c == '\n')
      print = false;
    if (print)
      Console.print(c);
  }
  Console.println();
}


