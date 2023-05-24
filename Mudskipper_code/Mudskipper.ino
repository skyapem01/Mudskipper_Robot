#include "Arduino.h"
#include <ESP32Servo.h>

#define _DEBUG_SERIAL

///define Servo 6 join 
Servo servoleft1; 
Servo servoleft2;
Servo servoright1;
Servo servoright2;
Servo tail1;
Servo tail2;

/// define var CPG 1st
double alpha,MI,Pi,Phi  ;
double W_1,W_2,W_3,W_4,W_13,W_24,W_35,W_46 ;
double B_1,B_2 ;
double H_1,H_2,H_3,H_4,H_5,H_6 ;
double O_1,O_2,O_3,O_4,O_5,O_6 ;
double use_cpg_1,use_cpg_2,use_cpg_3,use_cpg_4,use_cpg_5,use_cpg_6 ;


/// define var CPG 2nd
double alpha_2,MI_2,Phi_2;
double W_11,W_22,W_33,W_44,W_1133,W_2244 ;
double H_11,H_22,H_33,H_44 ;
double O_11,O_22,O_33,O_44 ;
double B_11,B_22 ;
double use_cpg2_1,use_cpg2_2,use_cpg2_3,use_cpg2_4 ;

/// define var command servo
double use_servo_1,use_servo_2,use_servo_3,use_servo_4,use_servo_5,use_servo_6 ;

//setting timer
long Time;
unsigned long prevTime = 0;  // global variable
unsigned long interval = 75; // 50 ms

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  /// define var CPG 1st
  alpha = 1.15 ; 
  MI = 0.05;
  Pi = 3.14 ;
  Phi = (Pi/8) ;
  B_1 = 0 ;
  B_2 = 0 ;
  H_1 = 0.5 ;
  H_2 = 0.5 ;
  H_3 = 0.5 ;
  H_4 = 0.5 ;
  H_5 = 0.5 ;
  H_6 = 0.5 ;
  O_1 = tanh(H_1) ;
  O_2 = tanh(H_2) ;
  O_3 = tanh(H_3) ;
  O_4 = tanh(H_4) ;
  O_5 = tanh(H_5) ;
  O_6 = tanh(H_6) ;
  W_1 = cos(Phi)*alpha ;
  W_2 = cos(Phi)*alpha ;
  W_3 = (sin(Phi)*alpha) + MI ; 
  W_4 = -((sin(Phi)*alpha) + MI) ;
  W_13 = 0.5 ;
  W_24 = 0.5 ;
  W_35 = 0.5 ;
  W_46 = 0.5 ;


  //// /// define var CPG 2nd
  alpha_2 = 1.15 ;
  MI_2 = 0.05;
  Phi_2 = (3.14/16) ;
  H_11 = 0.5 ;
  H_22 = 0.5 ;
  H_33 = 0.5 ;
  H_44 = 0.5 ;
  O_11 = tanh(H_11) ;
  O_22 = tanh(H_22) ;
  O_33 = tanh(H_33) ;
  O_44 = tanh(H_44) ;
  W_11 = cos(Phi_2)*alpha ;
  W_22 = cos(Phi_2)*alpha ;
  W_33 = (sin(Phi_2)*alpha) + MI_2 ; 
  W_44 = -((sin(Phi_2)*alpha) + MI_2) ;
  B_11 = 0 ;
  B_22 = 0 ;
  W_1133 = 0.5 ;
  W_2244 = 0.5 ;

  /// setting servo
  servoleft1.attach(12); //รู11 
  servoleft1.write(90);  //135องศา-ขึ้น
  servoleft2.attach(14); //รู12 
  servoleft2.write(45);  //90องศา-ไปหลัง

  servoright1.attach(26); //14 
  servoright1.write(50);  //0องศา-ขึ้น  
  servoright2.attach(25); //15 
  servoright2.write(90);  //45องศา-ไปหลัง
  

  tail1.attach(15);  //รู8ขวาบอร์ด
  tail1.write(86);  //ขึ้น135
  tail2.attach(2);  //รู9ขวาบอร์ด
  tail2.write(90);  //180ปัดซ้าย
  
  delay(3000) ; //wait 3 sec

}
  //// function mapping arduino
  double mapf(double val, double in_min, double in_max, double out_min, double out_max) {
    return (val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void loop() {
  
  unsigned long currentTime = millis(); //set currentTime
  
  if(currentTime - prevTime >= interval){
    prevTime = currentTime;  
    
    //// start CPG 1st equation
    H_1 = (W_1*O_1)+(W_4*O_2)+B_1 ;
    H_2 = (W_2*O_2)+(W_3*O_1)+B_2 ;
    O_1 = tanh(H_1) ;
    O_2 = tanh(H_2) ;
    
    H_3 = (W_13*O_3)+O_1 ;
    H_4 = (W_24*O_4)+O_2 ;
    O_3 = tanh(H_3) ;
    O_4 = tanh(H_4) ;  
    
    H_5 = (W_35*O_5)+O_3 ;
    H_6 = (W_46*O_6)+O_4 ;
    O_5 = tanh(H_5) ;
    O_6 = tanh(H_6) ;    
    
    use_cpg_1 = O_1   ;
    use_cpg_2 = O_2  ;
    use_cpg_3 = O_3  ;
    use_cpg_4 = O_4  ;
    use_cpg_5 = O_5  ;
    use_cpg_6 = O_6  ;

    //// start CPG 1st equation
    H_11 = (W_11*O_11)+(W_44*O_22)+B_11 ;
    H_22 = (W_22*O_22)+(W_33*O_11)+B_22 ;
    O_11 = tanh(H_11) ;
    O_22 = tanh(H_22) ;

    use_cpg2_1 = O_11  ;
    use_cpg2_2 = O_22 ;  

    H_33 = (W_1133*O_3)+O_1 ;
    H_44 = (W_24*O_4)+O_2 ;
    O_33 = tanh(H_3) ;
    O_44 = tanh(H_4) ; 

    use_cpg2_3 = O_33  ;
    use_cpg2_4 = O_44 ; 


  use_servo_1 = mapf(use_cpg_1, -1.0, 1.0, 10.0, 170.0);  // front-back /// mapping tanh to angle servo
  use_servo_2 = mapf(use_cpg_2, -1.0, 1.0, 55.0, 135.0);  // down-top /// mapping tanh to angle servo
  
  #ifdef _DEBUG_SERIAL
  Serial.print("-------------------servo1 = ");
  Serial.println(use_servo_1); 
  Serial.print("-------------------servo2 = ");
  Serial.println(use_servo_2);
  #endif
  
  servoleft1.write(use_servo_2);  /// motor running following tanh
  servoleft2.write(use_servo_1);  /// motor running following tanh
  
  use_servo_3 = mapf(use_cpg_1, -1.0, 1.0, 130.0, -20.0); // front-back /// mapping tanh to angle servo
  use_servo_4 = mapf(use_cpg_2, -1.0, 1.0, 88.0, 0.0); // down-top /// mapping tanh to angle servo
  #ifdef _DEBUG_SERIAL
  Serial.print("-------------------servo3 = ");
  Serial.println(use_servo_3); 
  Serial.print("-------------------servo4 = ");
  Serial.println(use_servo_4);
  #endif
  
  servoright1.write(use_servo_4);  /// motor running following tanh
  servoright2.write(use_servo_3);  /// motor running following tanh

  use_servo_5 = mapf(use_cpg2_1, -1.0, 1.0, 180.0, 0.0); // left-right /// mapping tanh to angle servo
  use_servo_6 = mapf(use_cpg2_4, -1.0, 1.0, 75.0, 130.0); // down-top /// mapping tanh to angle servo

  #ifdef _DEBUG_SERIAL
  Serial.print("-------------------servo5 = ");
  Serial.println(use_servo_5);
  Serial.print("-------------------servo6 = ");
  Serial.println(use_servo_6);
  #endif
  
  tail1.write(use_servo_6);   /// motor running following tanh
   tail2.write(use_servo_5);  /// motor running following tanh
  
  
  #ifdef _DEBUG_SERIAL  
  Serial.print("H1 = ");
  Serial.println(H_1);
  Serial.print("H2 = ");
  Serial.println(H_2);
  Serial.print("O1 = ");
  Serial.println(O_1); 
  Serial.print("O2 = ");
  Serial.println(O_2);
  Serial.print("O3 = ");
  Serial.println(O_3); 
  Serial.print("O4 = ");
  Serial.println(O_4);
  Serial.print("O5 = ");
  Serial.println(O_6); 
  Serial.print("O6 = ");
  Serial.println(O_6);
  #endif
  }

  
}



  
