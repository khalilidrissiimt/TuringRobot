#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <MeMegaPi.h>

Movement(String currentValue, String valueToWrite);
Displacement(String displacement);

double angle_rad = PI/180.0;
double angle_deg = 180.0/PI;

MeEncoderOnBoard Encoder_1(SLOT1);
MeEncoderOnBoard Encoder_2(SLOT2);
MeEncoderOnBoard Encoder_3(SLOT3);
MeEncoderOnBoard Encoder_4(SLOT4);
MeUltrasonicSensor ultrasonic_6(6);

MeMegaPiDCMotor motor_12(12);

void isr_process_encoder1(void){
  if(digitalRead(Encoder_1.getPortB()) == 0){
    Encoder_1.pulsePosMinus();
  }else{
    Encoder_1.pulsePosPlus();
  }
}

void isr_process_encoder2(void){
  if(digitalRead(Encoder_2.getPortB()) == 0){
    Encoder_2.pulsePosMinus();
  }else{
    Encoder_2.pulsePosPlus();
  }
}

void isr_process_encoder3(void){
  if(digitalRead(Encoder_3.getPortB()) == 0){
    Encoder_3.pulsePosMinus();
  }else{
    Encoder_3.pulsePosPlus();
  }
}

void isr_process_encoder4(void){
  if(digitalRead(Encoder_4.getPortB()) == 0){
    Encoder_4.pulsePosMinus();
  }else{
    Encoder_4.pulsePosPlus();
  }
}



void _delay(float seconds) {
  if(seconds < 0.0){
    seconds = 0.0;
  }
  long endTime = millis() + seconds * 1000;
  while(millis() < endTime) _loop();
}

void setup() {
  Serial.begin(9600);
  TCCR1A = _BV(WGM10);
  TCCR1B = _BV(CS11) | _BV(WGM12);
  TCCR2A = _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(CS21);
  attachInterrupt(Encoder_3.getIntNum(), isr_process_encoder3, RISING);
  Encoder_3.setPulse(8);
  Encoder_3.setRatio(46.67);
  Encoder_3.setPosPid(1.8,0,1.2);
  Encoder_3.setSpeedPid(0.18,0,0);
  attachInterrupt(Encoder_1.getIntNum(), isr_process_encoder1, RISING);
  Encoder_1.setPulse(8);
  Encoder_1.setRatio(46.67);
  Encoder_1.setPosPid(1.8,0,1.2);
  Encoder_1.setSpeedPid(0.18,0,0);
  attachInterrupt(Encoder_2.getIntNum(), isr_process_encoder2, RISING);
}

void _loop() {
  Encoder_3.loop();
  Encoder_1.loop();
}
//-----------------------------------------------------------
void vide_pick_up(){
  _delay(2);
  motor_12.run(-70 / 100.0 * 255);
  _delay(2);
  motor_12.stop();
  _delay(2);
  Encoder_3.move(-480, abs(100));
  _delay(2);
  motor_12.run(100 / 100.0 * 255);
  _delay(2);
  motor_12.stop();
  _delay(2);
  Encoder_3.move(480, abs(100));
  _delay(2);
}

void vide_drop_down(){
  _delay(2);
  Encoder_3.move(-480, abs(100));
  _delay(2);
  motor_12.run(-70 / 100.0 * 255);
  _delay(2);
  motor_12.stop();
  _delay(2);
  Encoder_3.move(480, abs(100));
  _delay(2);
}
//-----------------------------------------------------------
void state_0_pick_up(){
  _delay(2);
  motor_12.run(-70 / 100.0 * 255);
  _delay(2);
  motor_12.stop();
  _delay(2);
  Encoder_1.move(+237 ,abs(100));
  _delay(2); 
  Encoder_3.move(-480, abs(100));
  _delay(3);
  motor_12.run(100 / 100.0 * 255);
  _delay(1);
  motor_12.stop();
  _delay(1);
  Encoder_3.move(400, abs(100));
  _delay(2);
  Encoder_1.move(-237 ,abs(100));
  _delay(2);
  Encoder_3.move(98, abs(100));
}

void state_0_drop_down(){
  _delay(2);
  Encoder_1.move(+237 ,abs(100));
  _delay(2); 
  Encoder_3.move(-480, abs(100));
  _delay(3);
  motor_12.run(-100 / 100.0 * 255);
  _delay(2);
  motor_12.stop();
  _delay(1);
  Encoder_3.move(400, abs(100));
  _delay(2);
  Encoder_1.move(-237 ,abs(100));
  _delay(2);
  Encoder_3.move(98, abs(100));
}
//-----------------------------------------------------------
void state_1_pick_up(){
  _delay(2);
  motor_12.run(-70 / 100.0 * 255);
  _delay(2);
  motor_12.stop();
  _delay(2);
  Encoder_1.move(1050 ,abs(100));
  _delay(2);
  Encoder_3.move(- (560 + 170) ,abs(100));
  _delay(3); 
   motor_12.run(100 / 100.0 * 255);
  _delay(1);
  motor_12.stop();
  _delay(1);
  Encoder_3.move(560, abs(100)); 
  _delay(2); 
  Encoder_1.move(-1050 ,abs(100));
  _delay(2); 
  Encoder_3.move(170, abs(100));
}

void state_1_drop_down(){
  _delay(2);
  Encoder_1.move(1050 ,abs(100));
  _delay(2);
  Encoder_3.move(- (560 + 170) ,abs(100));
  _delay(3); 
   motor_12.run(-100 / 100.0 * 255);
  _delay(2);
  motor_12.stop();
  _delay(1);
  Encoder_3.move(560, abs(100)); 
  _delay(2); 
  Encoder_1.move(-1050 ,abs(100));
  _delay(2); 
  Encoder_3.move(170, abs(100));
}
//------------------------------------------------------------
void vide_to_0() {
  vide_pick_up();
  state_0_drop_down();
    
}

void vide_to_1() {
    vide_pick_up();
    state_1_drop_down();
}

void _0_to_vide() {
    state_0_pick_up();
    vide_drop_down();
}

void _0_to_1() {
    state_0_pick_up();
    state_1_drop_down();
    
}

void _1_to_vide() {
    state_1_pick_up();
    vide_drop_down();
}

void _1_to_0() {
    state_1_pick_up();
    state_0_drop_down();
}

//-----------------------------------------------------------

void move_right()
{
  Encoder_2.setTarPWM(-50/100.0*255);
  _delay(0.5);
}

void move_left()
{
  Encoder_2.setTarPWM(50/100.0*255);
  _delay(0.5);
}

//-----------------------------------------------------------

String states[numberOfEntries] = {"init", "init", "init", "e1", "e1", "e1", "e2", "e2", "e3", "e3"};
String current_values[numberOfEntries] = {"vide", "0", "1", "vide", "0", "1", "0", "1", "vide", "0"};
String values_to_write[numberOfEntries] = {"vide", "0", "1", "vide", "0", "1", "1", "0", "1", "1"};
String displacements[numberOfEntries] = {"droite", "droite", "droite", "gauche", "droite", "droite", "droite", "gauche", "droite", "droite"};
String next_states[numberOfEntries] = {"init", "e1", "e1", "e2", "e1", "e1", "fin", "e3", "fin", "fin"};

int findNextAction(String currentState, String currentValue) {
    for (int i = 0; i < numberOfEntries; i++) {
        if (states[i] == currentState && current_values[i] == currentValue) {
            return i;
        }
    }
    return -1; // Indicate no match found
}

String determineCurrentValue(float distance) {
    if ((distance >= 6.7) && (distance <= 11.7)) {
      //Serial.println("vide");
        return "vide";
    } else if ((distance >= 16.7) && (distance <= 21.7)) {
      //Serial.println("0");
        return "0";
    } else if ((distance >= 26.7) && (distance <= 31.7)) {
      //Serial.println("1");
        return "1";
    } else {
      //Serial.println("unkown");
        return "unknown";
    }
}

//---------------------------------------------------------------

String currentState = "init";
String valueToWrite;
String nextState;
String displacement;
float distance;
int actionIndex;
bool flag = false;
//---------------------------------------------------------------

void loop()
{
    distance = ultrasonic_6.distanceCm();
    currentValue = determineCurrentValue(distance);
    actionIndex = findNextAction(currentState, currentValue);

    if (actionIndex == -1)
    {
        Serial.println("No valid action found");
    }

    else
    {
        valueToWrite = values_to_write[actionIndex];
        nextState = next_states[actionIndex];
        displacement = displacements[actionIndex];

        //-------------------------------------------------------
        if (currentValue == "vide" && currentState == "init")
        {
            Movement(currentValue, valueToWrite);
            Displacement(displacement);
            currentState = nextState;
        }

        if (currentValue == "0" && currentState == "init")
        {
            Movement(currentValue, valueToWrite);
            Displacement(displacement);
            currentState = nextState;
        }

        if (currentValue == "1" && currentState == "init")
        {
            Movement(currentValue, valueToWrite);
            Displacement(displacement);
            currentState = nextState;
        }
        //------------------------------------------------------------

        if (currentValue == "vide" && currentState == "e1")
        {
            Movement(currentValue, valueToWrite);
            Displacement(displacement);
            currentState = nextState;
        }
        
        if (currentValue == "0" && currentState == "e1")
        {
            Movement(currentValue, valueToWrite);
            Displacement(displacement);
            currentState = nextState;
        }

        if (currentValue == "1" && currentState == "e1")
        {
            Movement(currentValue, valueToWrite);
            Displacement(displacement);
            currentState = nextState;
        }
            
        //------------------------------------------------------------------

        if (currentValue == "vide" && currentState == "e2")
        {
            Movement(currentValue, valueToWrite);
            Displacement(displacement);
            currentState = nextState;
        }
        
        if (currentValue == "0" && currentState == "e2")
        {
            Movement(currentValue, valueToWrite);
            Displacement(displacement);
            currentState = nextState;
        }

        if (currentValue == "1" && currentState == "e2")
        {
            Movement(currentValue, valueToWrite);
            Displacement(displacement);
            currentState = nextState;
        }
        //-----------------------------------------------------------

        if (currentValue == "vide" && currentState == "e3")
        {
            Movement(currentValue, valueToWrite);
            Displacement(displacement);
            currentState = nextState;
        }
        
        if (currentValue == "0" && currentState == "e3")
        {
            Movement(currentValue, valueToWrite);
            Displacement(displacement);
            currentState = nextState;
        }

        if (currentValue == "1" && currentState == "e3")
        {
            Movement(currentValue, valueToWrite);
            Displacement(displacement);
            currentState = nextState;
        }
        //----------------------------------------------------
        if (nextState == "fin")
        {
            flag = true;
            break
        }
        
    }


}

void Movement(String currentValue, String valueToWrite)
{
    if(currentValue == "vide" && valueToWrite == "0")
    {
        vide_to_0();
    }
        
    if(currentValue == "0"  && valueToWrite == "vide")
    {
        _0_to_vide();
    }
    //----------------------------------------------

    if(currentValue == "0"  && valueToWrite == "1")
    {
        _0_to_1();
    }
    if(currentValue == "1"  && valueToWrite == "0")
    {
        _1_to_0();
    }
    //------------------------------------------------

    if(currentValue == "vide"  && valueToWrite == "1")
    {
        vide_to_1();
    }
    if(currentValue == "1"  && valueToWrite == "vide")
    {
        _1_to_vide();
    }

    //-----------------------------------------------

    if(currentValue == valueToWrite )
    {
        return;
    }

}


void Displacement(String displacement)
{
    if (displacement == "droite")
    {
        move_right();
    }

    if (displacement =="gauche")
    {
        move_left();
    }
}