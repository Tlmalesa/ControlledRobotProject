

/**
  @author  Tshegofatso Malesa blessingmalesa@gmail.com
  @date    2020-11
  
           Example of commands for the project :
           
           USE YOUR IP ADDRESS
           Robot Control:
           - e.g 
           - http://192.168.43.75/move?dir=s (Stop)
           - http://192.168.43.75/move?dir=f (Forward)
           - http://192.168.43.75/move?dir=b (Backwad)
           - http://192.168.43.75/move?dir=l (Left)
           - http://192.168.43.75/move?dir=r (Right)
           
           Speed Control:
          - http://192.168.43.75/speedc?speed=1 (0 speed)
          - http://192.168.43.75/speedc?speed=2 (500 speed)
          - http://192.168.43.75/speedc?speed=3 (600 speed)
          - http://192.168.43.75/speedc?speed=4 (700 speed)
          - http://192.168.43.75/speedc?speed=5 (800 speed)
          - http://192.168.43.75/speedc?speed=6 (900 speed)
          - http://192.168.43.75/speedc?speed=7 (1000 speed)
          
           Buzzer Tone:
           - http://192.168.43.75/buzz?tone=1 (DO tone)
           - http://192.168.43.75/buzz?tone=2 (RE tone)
           - http://192.168.43.75/buzz?tone=3 (MI tone)
           - http://192.168.43.75/buzz?tone=4 (FA tone)
           - http://192.168.43.75/buzz?tone=5 (SOH tone)
           - http://192.168.43.75/buzz?tone=6 (LAH tone)
           - http://192.168.43.75/buzz?tone=7 (SI tone)
           - http://192.168.43.75/buzz?tone=8 (DOH tone)
           - http://192.168.43.75/buzz?tone=9 (siren tone)
           - http://192.168.43.75/buzz?tone=10 (DO-RE-MI-FA... tone)
           - http://192.168.43.75/buzz?tone=11 (Stop siren tone)
           
           RGB:
           - http://192.168.43.75 (drag for colour change)
           
           LDR:
           http://192.168.43.75/action?ld=s (LDR value will be displayed)

           Ultrasonic Sensor:
           http://192.168.43.75/action?ld=d (ultrasonic Sensor value will be displayed)

   Motor Pins
  GPIO 4 = D2 for pwmB
  GPIO 5 = D1 for pwmA
  GPIO 0 = D3 for dirA
  GPIO 2 = D4 for dirB

  RGB Pins
  GPIO 12 = D6 for green
  GPIO 13 = D7 for red
  GPIO 14 = D5  for blue

  buzzer pin
  GPIO 1 = D10 and Tx

  Ultrasonic Sensor Pins
  GPIO 15 = D8
  GPIO 16 = D0

  LDR Pin
  ADC0 = A0

  Servo Pin
  GPIO 3 = D9 and Rx
*/

#include <Servo.h>
#include<ESP8266WebServer.h>
#include <SimpleTimer.h>
ESP8266WebServer server(80);
const char* ssid = "AndroidAP1962";//change it to your WiFi Name
const char* password = "088e32878000";//change it to your WiFi password

//...........Motor A and B....................//
static const uint8_t pwm_A = 5 ; //D1 enable speed motor A
static const uint8_t pwm_B = 4;//D2 enable speed motor B
static const uint8_t dir_A = 0;//D3 motor A
static const uint8_t dir_B = 2;//D4 Motor B

//...........RGB....................//
const int red = 13;       //D7
const int green = 12;     //D6
const int blue = 14;      //D5

//...........Ultrasonic Sensor....................//
int trigPin = 16;//D0  Trig pin for HC-SR04
int echoPin = 15;//D8 or RX
long duration;
int distance;

//...........Buzzer....................//
const int buzzer = 1;    //D10 or TX

//...........LDR....................//
const int LDR =A0;

//.........Servo...................//
Servo Servo1; 
int servoPin = 3; 

String page1 = "";
double data;
String page11 = "";
double Lightdata;
int a = 0;

//...................HTML code for RGB....................
String page =
R"(
<html lang='en'>
    <head>
        <title>ROBOT CONTROL</title>
        <meta name='viewport' content='width=device-width, initial-scale=1'>
        <link rel='stylesheet' href='https://use.fontawesome.com/releases/v5.1.0/css/all.css'/>
        <link rel='stylesheet' href='https://stackpath.bootstrapcdn.com/bootstrap/4.1.2/css/bootstrap.min.css'/>
        <script src='https://code.jquery.com/jquery-3.3.1.min.js'></script>
    </head>
    
    <body>
    
    <div class='container-fluid text-center'>
        <div class='row'>
            <div class='col-sm-12' style='color:red;font-size:7vh'>
                <p>Use the sliders to control the brightness.</p>
            </div>
        </div>     
        <br/>
        <div class='row'>
            <div class='col-sm-12' style='color:red;font-size:3vh'>
                Red:
                <b id='status1'></b>
            </div>
        </div> 
        <div class='col-sm-12'>
              <input type='range' min='0' max='100' class='form-control-range' id='slider_red'/>
        </div>

        <div class='row'>
            <div class='col-sm-12' style='color:green;font-size:3vh'>
                Green: 
                <b id='status2'></b>
            </div>
        </div> 
        <div class='col-sm-12'>
              <input type='range' min='0' max='100' class='form-control-range' id='slider_green'/>
        </div>

        <div class='row'>
            <div class='col-sm-12' style='color:blue;font-size:3vh'>
                Blue:
                <b id='status3'></b>
            </div>
        </div> 
        <div class='col-sm-12'>
              <input type='range' min='0' max='100' class='form-control-range' id='slider_blue'/>
        </div>
    </div>
    </body>
    
    <script>
        $('#slider_red').on('change', function(){
            var state_slider_red = $('#slider_red').val();
            
            $('#status1').html(state_slider_red + ' %');

            $.ajax({
              url: '/redFunction',
              type: 'POST',
              data: {state1: state_slider_red}
            });
        });

        $('#slider_green').on('change', function(){
            var state_slider_green = $('#slider_green').val();
            
            $('#status2').html(state_slider_green + ' %');

            $.ajax({
              url: '/greenFunction',
              type: 'POST',
              data: {state2: state_slider_green}
            });
        });

        $('#slider_blue').on('change', function(){
            var state_slider_blue = $('#slider_blue').val();
            
            $('#status3').html(state_slider_blue + ' %');

            $.ajax({
              url: '/blueFunction',
              type: 'POST',
              data: {state3: state_slider_blue}
            });
        });
    </script>
</html>
)";
//.................Red Function.................//
void redFunction()
{
  int value1 = server.arg("state1").toInt();
  value1 = map(value1, 0, 100, 0, 1023);

  if(value1 == 0)
     digitalWrite(red, LOW);//turn of the led
  else
     analogWrite(red, value1);//change the brightness of red
  
  server.send(200, "text/html", "red");
}
//.................Green Function.................//
void greenFunction()
{
  int value2 = server.arg("state2").toInt();
  value2 = map(value2, 0, 100, 0, 1023);

  if(value2 == 0)
     digitalWrite(green, LOW);//turn of the led
  else
     analogWrite(green, value2);//change the brightness of green
  
  server.send(200, "text/html", "green");
}
//.................Blue Function.................//
void blueFunction()
{
  int value3 = server.arg("state3").toInt();
  value3 = map(value3, 0, 100, 0, 1023);

  if(value3 == 0)
     digitalWrite(blue, LOW);//turn of the led
  else
     analogWrite(blue, value3);//change the brightness of blue
  
  server.send(200, "text/html", "blue");
}

//.................handle Robot Control.................//
void handleMoveRequest() {
  if (!server.hasArg("dir")) {
    server.send(404, "text / plain", "control: undefined");
    return;
  }
  String direction = server.arg("dir");
  if (direction.equals("f") ) {
    forward();
  // servoFunction();
    server.send(200, "text / plain", "control: forward");
   
  }
  
  else if (direction.equals("b") ) {
    backward();
    server.send(200, "text / plain", "control: backward");
  }
  else  if (direction.equals("s") ) {
    stop_motors();
    server.send(200, "text / plain", "control: stop");
  }
  else  if (direction.equals("l")) {
    turn_left();
    server.send(200, "text / plain", "control: Left");
  }
  else  if (direction.equals("r")) {
    turn_right();
    server.send(200, "text / plain", "Control: Right");
  }

  else {
    server.send(404, "text / plain", "control: undefined");
  }
}

//.................handle Robot Speed Control.................//
void handleRobotSpeedRequest() {
  if (!server.hasArg("speed")) {
    server.send(404, "text / plain", "speed: undefined");
    return;
  }
  String speedcont = server.arg("speed");
  if (speedcont.equals("1")) {
    // TODO : Speed Control 1
      analogWrite(pwm_A, 0);
      analogWrite(pwm_B, 0);
    server.send(200, "text / plain", "0 speed");
  }
  else if (speedcont.equals("2")) {
    // TODO :Speed Control 2
      analogWrite(pwm_A, 500);
      analogWrite(pwm_B, 500);
    server.send(200, "text / plain", "500 speed");
  }
  else if (speedcont.equals("3")) {
    // TODO : Speed Control 3
      analogWrite(pwm_A, 600);
      analogWrite(pwm_B, 600);
    server.send(200, "text / plain", "600 speed");
  }
  else if (speedcont.equals("4")) {
    // TODO : Speed Control 4
      analogWrite(pwm_A, 700);
      analogWrite(pwm_B, 700);
    server.send(200, "text / plain", "700 speed");
  }
  else if (speedcont.equals("5")) {
    // TODO : Speed Control 5
     analogWrite(pwm_A, 800);
      analogWrite(pwm_B, 800);
    server.send(200, "text / plain", "800 speed");
  }
  else if (speedcont.equals("6")) {
    // TODO : Speed Control 6
      analogWrite(pwm_A, 900);
      analogWrite(pwm_B, 900);
    server.send(200, "text / plain", "900 speed");
  }
   else if (speedcont.equals("7")) {
    // TODO : Speed Control 7
      analogWrite(pwm_A, 1000);
      analogWrite(pwm_B, 1000);
    server.send(200, "text / plain", "1000 speed");
  }
  else {
    server.send(404, "text / plain", "speed: undefined");
  }
}

//.................handle Buzzer.................//
void handleActionRequest() {
  if (!server.hasArg("tone")) {
    server.send(404, "text / plain", "buzz: undefined");
    return;
  }
  String type = server.arg("tone");
  if (type.equals("1")) {
    // TODO : Buzzer Tone 1
    tone(buzzer,264,500);
    server.send(200, "text / plain", "DO tone");
  }
  else if (type.equals("2")) {
    // TODO : Buzzer Tone 2
    tone(buzzer,297,500);
    server.send(200, "text / plain", "RE tone");
  }
  else if (type.equals("3")) {
    // TODO : Buzzer Tone 3
    tone(buzzer,330,500);
    server.send(200, "text / plain", "MI tone");
  }
  else if (type.equals("4")) {
    // TODO : Buzzer Tone 4
     tone(buzzer,352,500);
    server.send(200, "text / plain", "FA tone");
  }
  else if (type.equals("5")) {
    // TODO : Buzzer Tone 5
    tone(buzzer,396,500);
    server.send(200, "text / plain", " SOH tone");
  }
  else if (type.equals("6")) {
    // TODO : Buzzer Tone 6
    tone(buzzer,440,500);
    server.send(200, "text / plain", "LAH tone");
  }
  else if (type.equals("7")) {
    // TODO : Buzzer Tone 7
    tone(buzzer,495,500);
    server.send(200, "text / plain", "SI tone");
  }
  else if (type.equals("8")) {
    // TODO : Buzzer Tone 8
    tone(buzzer,528,500);
    server.send(200, "text / plain", "DOH tone");
  }
  else if (type.equals("9")) {
    server.send(200, "text / plain", "siren tone");
  }
   else if (type.equals("9")) {
    server.send(200, "text / plain", "siren tone stopped");
  }
    else if (type.equals("10")) {
    // TODO : Buzzer Tone  DoReMiFa.... 9
     DoReMiFaFunction();
     
    server.send(200, "text / plain", "DO-RE-MI-FA... tone");
  }  
  else {
    server.send(404, "text / plain", "buzz: undefined");
  }
}

//.................handle LDR and Ultrasonic Sensor.................//
void handleLDRRequest() {
  if (!server.hasArg("ld")) {
    server.send(404, "text / plain", "ldr: undefined");
    return;
  }
  String type = server.arg("ld");
  if (type.equals("s")) {

  Lightdata = analogRead(A0);
  if(Lightdata < 100){
     page11 = String(Lightdata);
  server.send(200, "text / plain", page11 + " >>>>>>>>>>>");
      digitalWrite(red,HIGH);
     digitalWrite(green,HIGH);
    digitalWrite(blue,HIGH);
  }else{
     page11 = String(Lightdata);
  server.send(200, "text / plain", page11 + " ...........");
     digitalWrite(red,LOW);
     digitalWrite(green,LOW);
    digitalWrite(blue,LOW);
  }
 
  }
  else  if (type.equals("d")) {

    page11= String(distance);
    
    server.send(200, "text / plain", page11 + " CM");
  
  }
   else
    server.send(404, "text / plain", "ldr: undefined");

}

//.................Buzzer Function.................//
void DoReMiFaFunction(){
 //Do-Re-Mi-Fa-Soh-Lah-Si-Do
    tone(buzzer,264,500);
    delay(500);
    tone(buzzer,297,500);
    delay(500);
     tone(buzzer,330,500);
    delay(500);
     tone(buzzer,352,500);
    delay(500);
     tone(buzzer,396,500);
    delay(500);
     tone(buzzer,440,500);
    delay(500);
      tone(buzzer,495,500);
    delay(500);
     tone(buzzer,528,500);
    delay(5000);
}

//..................Motor Control...................//
void stop_motors() {
  analogWrite(pwm_A, 0);
  analogWrite(pwm_B, 0);
 digitalWrite(dir_A, LOW);
 digitalWrite(dir_B, LOW);

}

void backward() {
  analogWrite(pwm_A, 800);
  analogWrite(pwm_B, 800);
  digitalWrite(dir_A, LOW);
  digitalWrite(dir_B, HIGH);
}

void forward() {
  analogWrite(pwm_A, 1000);
 analogWrite(pwm_B, 1000);
 digitalWrite(dir_A, HIGH);
 digitalWrite(dir_B, LOW);

}
void servoFunction(){
     // Make servo go to 0 degrees 
   Servo1.write(0); 
   delay(500); 
   // Make servo go to 90 degrees 
   Servo1.write(90); 
   delay(500); 
}
void distanceSensor(){
digitalWrite(trigPin,HIGH);
delayMicroseconds(10);
digitalWrite(trigPin,LOW);//Transmit waves for 10us
duration = pulseIn(echoPin,HIGH);//Receive reflected waves
distance = duration/58.2;//get distanc
  String direction = server.arg("dir");
   int i=0;
   int hz =440;
  String type = server.arg("tone");
if(distance < 20 && direction.equals("f") ){
  stop_motors();  
}
else if(type.equals("9")){
for(hz = 440; hz < 1000; hz++){
    tone(buzzer, hz);
    delay(5);
  }
  // Whoop down
  for( hz = 1000; hz > 440; hz--){
  tone(buzzer, hz);
   delay(5);
  }
for(int x =0;x < 7;x++){
 for( hz = 440; hz < 1000; hz+=25){
 digitalWrite(blue, HIGH); 
 digitalWrite(red, LOW);   
 tone(buzzer, hz);
  delay(5);
}
 // Whoop down
 for(int hz = 1000; hz > 440; hz-=25){    
  digitalWrite(blue, LOW); 
 digitalWrite(red, HIGH);
 tone(buzzer, hz);
 delay(5);
  }
 
  digitalWrite(red, LOW);
}
 noTone(buzzer);

  
}
else if (type.equals("11")) {
//   NoTone();
digitalWrite(buzzer,LOW);
//noTone(buzzer);
if(distance < 20 && direction.equals("f")){
  stop_motors();  
}

  }

}

void turn_left() {
 analogWrite(pwm_A, 800);
 analogWrite(pwm_B, 800);
 digitalWrite(dir_A, HIGH);
  digitalWrite(dir_B, HIGH);
  delay(1000);
  stop_motors();

}

void turn_right() {
   analogWrite(pwm_A, 800);
   analogWrite(pwm_B, 800);
 digitalWrite(dir_A, LOW);
  digitalWrite(dir_B, LOW);
  delay(1000);
  stop_motors();
  
}

//........................Not found..................
void handleNotFound() {
  server.send(404, "text / plain", "404: Not found");
}
//..............Connect to WiFi Function............
void connectToWiFi()
{ 
  Serial.println("Connecting to the WiFi");
  
  WiFi.mode(WIFI_STA);

  WiFi.begin(ssid, password);

  Serial.println("Waiting for connection");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

//..............Setup Server Function..............//
void setupServer()
{ 

    server.on("/", htmlIndex);
    server.on("/redFunction", redFunction);
    server.on("/greenFunction", greenFunction);
    server.on("/blueFunction", blueFunction);
    server.on("/buzz", handleActionRequest);
    server.on("/action", handleLDRRequest);
    server.on("/move",handleMoveRequest);
    server.on("/speedc",handleRobotSpeedRequest);
    server.onNotFound(handleNotFound);
    server.begin();
  Serial.println("HTTP server started");

}
void htmlIndex() 
{
  server.send(200, "text/html", page);
}
void setup() {
  // put your setup code here, to run once:
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);
  pinMode(A0, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);
  digitalWrite(red,LOW);
  digitalWrite(green,LOW);
  digitalWrite(blue,LOW);
  pinMode(pwm_A, OUTPUT); 
  pinMode(pwm_B, OUTPUT);
  pinMode(dir_A, OUTPUT); 
  pinMode(dir_B, OUTPUT);
  Servo1.attach(servoPin); 
  delay(500);
Servo1.write(0);
  Serial.begin(115200);
  //delay(1000);
  connectToWiFi();
  setupServer();
}

void loop() {
  // put your main code here, to run repeatedly:

distanceSensor();

//   Servo1.write(0); 
//   delay(500); 
//   // Make servo go to 90 degrees 
//   Servo1.write(90); 
//   delay(500);

server.handleClient();
  
}
