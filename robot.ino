const int leftForward = 2;          //Pin links vooruit 
const int rightForward = 3;         //Pin rechts vooruit
const int leftBackward = 5;         //Pin Links achteruit
const int rightBackward = 4;        //Pin Rechts achteruit
const int sensor1 = 9;             //Pin naar de IR-sensor 1 --- analoog omdat er geen pins meer waren
const int sensor2 = 8;              //Pin naar de IR-sensor 2
const int sensor3 = 7;              //Pin naar de IR-sensor 3
const int sensor4 = 6;              //Pin naar de IR-sensor 4
const int trigPin1 = 12;            //Trig pin 2
const int echoPin1 = 11;            //Echo pin 2
//Variabelen voor Echo sensor
long duration1; 
int distance1;
//Variabelen voor de robot
int positie; //ten opzichte van de lijn
int timer;

void setup (){
  Serial.begin(9600);
  pinMode(leftForward , OUTPUT);
  pinMode(leftBackward , OUTPUT);
  pinMode(rightForward , OUTPUT);
  pinMode(rightBackward , OUTPUT);
  pinMode(sensor1, INPUT);
  pinMode(sensor2, INPUT);
  pinMode(sensor3, INPUT);
  pinMode(sensor4, INPUT);
  pinMode(trigPin1, OUTPUT); // Zet de TigPin als Output
  pinMode(echoPin1, INPUT); // Zet de echoPin als Input 
}
//1 = vooruit
//2 = links
//3 = rechts
//4 = achteruit
//5 = stoppen

void keren(){
    //90 graden - rechts stil > links draaien achter
    //90 graden terug -- rechts naar voren > rechts stil
   digitalWrite(leftBackward , LOW);
   digitalWrite(rightBackward , LOW);
   digitalWrite(leftForward , LOW);
   digitalWrite(rightForward , LOW);
   delay(500);
   digitalWrite(leftBackward , HIGH);
   delay(1250);
   digitalWrite(leftBackward , LOW);
   digitalWrite(rightForward , HIGH);
   delay(1250);
   digitalWrite(leftForward , HIGH);
}

void motor(int stand){
   digitalWrite(leftBackward , LOW);
   digitalWrite(rightBackward , LOW);
   digitalWrite(leftForward , LOW);
   digitalWrite(rightForward , LOW);
   if(stand == 1){
      digitalWrite(leftForward , HIGH);
      digitalWrite(rightForward , HIGH);
   }else if(stand == 2){
      digitalWrite(rightForward , HIGH);
   }else if(stand == 3){
      digitalWrite(leftForward , HIGH);
   }else if(stand == 4){
      digitalWrite(leftBackward , HIGH);
      digitalWrite(rightBackward , HIGH);
   }else if(stand == 6){
      keren();
   }
}

int sensor(){
  //Checken of de robot er is met detect
  int detect1 = digitalRead(sensor1);
  int detect2 = digitalRead(sensor2); 
  int detect3 = digitalRead(sensor3); 
  int detect4 = digitalRead(sensor4);
  int zwart = HIGH;
  int geen_zwart = LOW;

  //Niks
  if(detect1 == geen_zwart && detect2 == geen_zwart && detect3 == geen_zwart && detect4 == geen_zwart){  
      Serial.println("Geen lijn");
      timer++; //tel een bij de timer op
      if(timer > 75){ //als de timer groter is dan 75 dan gaat de robot keren
        timer = 0; //timer op 0
        return 6; //keren
      }else{
        return 1; //door blijven rijden
      }
  }else{
      timer = 0; //reset de timer want er is een lijn gezien
      //Op de lijn
      if(detect1 == geen_zwart && detect2 == zwart && detect3 == zwart && detect4 == geen_zwart){
          Serial.println("Op de lijn");
          return 1;
      //Kruising
      }else if(detect1 == zwart && detect2 == zwart && detect3 == zwart && detect4 == zwart){  
          //alles
          Serial.println("Kruising");
          return 1;
    
      //LINKS
      
      //Links - 3 sensoren
      }else if(detect1 == zwart && detect2 == zwart && detect3 == zwart && detect4 == geen_zwart){ 
          //links   
          Serial.println("Naar links");
          return 2;
      //Links - 2 sensoren
      }else if(detect1 == zwart && detect2 == zwart && detect3 == geen_zwart && detect4 == geen_zwart){ 
          //links   
          Serial.println("Naar links");
          return 2;
      }else if(detect1 == zwart && detect2 == geen_zwart && detect3 == geen_zwart && detect4 == geen_zwart){ 
          //links   
          Serial.println("Naar links");
          return 2;
      }else if(detect1 == geen_zwart && detect2 == zwart && detect3 == geen_zwart && detect4 == geen_zwart){ 
          //links   
          Serial.println("Naar links");
          return 2;
      
      //RECHTS
      
      //Rechts - 3 sensoren
      }else if(detect1 == geen_zwart && detect2 == zwart && detect3 == zwart && detect4 == zwart){    
          Serial.println("Naar rechts");
          return 3;
      //Rechts - 2 sensoren
      }else if(detect1 == geen_zwart && detect2 == geen_zwart && detect3 == zwart && detect4 == zwart){    
          Serial.println("Naar rechts");
          return 3;
      //Rechts - 1 sensor
      }else if(detect1 == geen_zwart && detect2 == geen_zwart && detect3 == geen_zwart && detect4 == zwart){ 
          //links   
          Serial.println("Naar rechts");
          return 3;
      }else if(detect1 == geen_zwart && detect2 == geen_zwart && detect3 == zwart && detect4 == geen_zwart){ 
          //links   
          Serial.println("Naar rechts");
          return 3;
      }
   }
}
void loop(){
    Ultra1(); //Roep de Ultra sensor functie aan
    if(distance1 < 12){ //Als de afstand kleiner is dan 12 --> remmen
      if(distance1 < 6){ //Als de afstand kleiner is dan 6 --> achteruit
        positie = 4; //achteruit
        Serial.print("Achteruit!\n");
      }else{
        positie = 5; //remmen
        Serial.print("Remmen!\n");
      }
    }else{
      positie = sensor(); //Vraag de positie aan
    }
    //print de positie
    Serial.print("Positite: ");
    Serial.println(positie);
    motor(positie); //Zet de motorstand gelijk aan de positie
}

void Ultra1(){
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  
  duration1 = pulseIn(echoPin1, HIGH);
  distance1= duration1*0.034/2;
}
