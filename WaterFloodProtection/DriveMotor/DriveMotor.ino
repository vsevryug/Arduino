int M1_Left = 12; //Direccion
int M1_Right = 11; //Direccion

/*
- DC MOTOR #1 OA1 OB1 -
IA1  | IB1  | MOTOR   |
-----------------------
LOW  | LOW  | STOP    |
HIGH | LOW  | FORWARD |
LOW  | HIGH | REVERSE |
HIGH | HIGH | STOP    |


- DC MOTOR #2 OA2 OB2 -
IA2  | IB2  | MOTOR   |
-----------------------
LOW  | LOW  | STOP    |
HIGH | LOW  | FORWARD |
LOW  | HIGH | REVERSE |
HIGH | HIGH | STOP    |

*/

void setup()
{
  pinMode(M1_Left, OUTPUT);
  pinMode(M1_Right , OUTPUT);
}

void loop(){
  turn (1); 
  delay(1000); //1 sg 
  
  stop();
  delay(250); //250ms

  turn(2); 
  delay(1000); //1 sg 
  
  stop();
  delay(250); //250ms
}


void turn(int direction)
{
  boolean inPin1 = LOW;
  boolean inPin2 = HIGH;

  if(direction== 1){
    inPin1 = HIGH;
    inPin2 = LOW;
  }
    digitalWrite(M1_Left, inPin1);
    digitalWrite(M1_Right , inPin2);
}

void stop(){
    digitalWrite(M1_Left, LOW);
    digitalWrite(M1_Right , LOW);
}
