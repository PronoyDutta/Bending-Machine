#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <AccelStepper.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define home_switch 12   // Limit Switch
#define enable 7

#define MS1 8 
#define MS2  9  
#define SLEEP_PIN  10

#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define pic1_BMPWIDTH  60
#define pic1_BMPHEIGHT  6
const unsigned char pic1[] PROGMEM = {
  0x00,0x00,0x07,0xFF,0xF0,0x00,0x00,0x00, // .....................###############............................
  0x00,0x00,0xFF,0xFF,0xFF,0xC0,0x00,0x00, // ................##########################......................
  0x00,0x07,0xFF,0xC7,0xFF,0xFF,0x00,0x00, // .............#############...###################................
  0xFF,0xFF,0xE0,0x00,0x00,0xFF,0xFF,0xF0, // ###################.....................####################....
  0xFF,0xFE,0x00,0x00,0x00,0x07,0xFF,0xF0, // ###############..............................###############....
  0xFF,0xC0,0x00,0x00,0x00,0x00,0x07,0xF0  // ##########...........................................#######....
};

#define pic2_BMPWIDTH  60
#define pic2_BMPHEIGHT 14
const unsigned char pic2[] PROGMEM = {
  0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00, // ........................########................................
  0x00,0x00,0x07,0xFF,0xF8,0x00,0x00,0x00, // .....................################...........................
  0x00,0x00,0x1F,0xFF,0xFE,0x00,0x00,0x00, // ...................####################.........................
  0x00,0x00,0x3E,0x00,0x3F,0x80,0x00,0x00, // ..................#####...........#######.......................
  0x00,0x00,0x78,0x00,0x07,0xC0,0x00,0x00, // .................####................#####......................
  0x00,0x01,0xE0,0x00,0x01,0xF0,0x00,0x00, // ...............####....................#####....................
  0x00,0x03,0xC0,0x00,0x00,0xF8,0x00,0x00, // ..............####......................#####...................
  0x00,0x07,0x80,0x00,0x00,0x3E,0x00,0x00, // .............####.........................#####.................
  0x00,0x0F,0x00,0x00,0x00,0x0F,0x80,0x00, // ............####............................#####...............
  0x00,0x1E,0x00,0x00,0x00,0x07,0xE0,0x00, // ...........####..............................######.............
  0x00,0x7C,0x00,0x00,0x00,0x01,0xFE,0x00, // .........#####.................................########.........
  0xFF,0xF0,0x00,0x00,0x00,0x00,0x7F,0xF0, // ############.....................................###########....
  0xFF,0xE0,0x00,0x00,0x00,0x00,0x0F,0xF0, // ###########.........................................########....
  0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00  // ########........................................................
};

#define pic3_BMPWIDTH  60
#define pic2_BMPHEIGHT 22

const unsigned char pic3[] PROGMEM = {
  0x00,0x00,0x00,0xFE,0x00,0x00,0x00,0x00, // ........................#######.................................
  0x00,0x00,0x03,0xFF,0xE0,0x00,0x00,0x00, // ......................#############.............................
  0x00,0x00,0x07,0xFF,0xF8,0x00,0x00,0x00, // .....................################...........................
  0x00,0x00,0x0F,0x00,0x7C,0x00,0x00,0x00, // ....................####.........#####..........................
  0x00,0x00,0x1E,0x00,0x1E,0x00,0x00,0x00, // ...................####............####.........................
  0x00,0x00,0x38,0x00,0x0F,0x00,0x00,0x00, // ..................###...............####........................
  0x00,0x00,0x70,0x00,0x03,0x80,0x00,0x00, // .................###..................###.......................
  0x00,0x00,0xF0,0x00,0x01,0xC0,0x00,0x00, // ................####...................###......................
  0x00,0x00,0xE0,0x00,0x01,0xE0,0x00,0x00, // ................###....................####.....................
  0x00,0x01,0xC0,0x00,0x00,0xF0,0x00,0x00, // ...............###......................####....................
  0x00,0x03,0xC0,0x00,0x00,0x78,0x00,0x00, // ..............####.......................####...................
  0x00,0x03,0x80,0x00,0x00,0x3C,0x00,0x00, // ..............###.........................####..................
  0x00,0x07,0x00,0x00,0x00,0x1E,0x00,0x00, // .............###...........................####.................
  0x00,0x07,0x00,0x00,0x00,0x0F,0x00,0x00, // .............###............................####................
  0x00,0x0E,0x00,0x00,0x00,0x07,0x80,0x00, // ............###..............................####...............
  0x00,0x1C,0x00,0x00,0x00,0x03,0xC0,0x00, // ...........###................................####..............
  0x00,0x3C,0x00,0x00,0x00,0x01,0xF0,0x00, // ..........####.................................#####............
  0x00,0x78,0x00,0x00,0x00,0x00,0xFC,0x00, // .........####...................................######..........
  0x00,0xF0,0x00,0x00,0x00,0x00,0x3F,0xD0, // ........####......................................########.#....
  0xFF,0xE0,0x00,0x00,0x00,0x00,0x0F,0xF0, // ###########.........................................########....
  0xFF,0x80,0x00,0x00,0x00,0x00,0x01,0xF0, // #########..............................................#####....
  0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x00  // #######.........................................................
};


#define joyPower 3

AccelStepper stepper(1,A2,A3); 

int VRx = A0;
int VRy = A1;
int SW = 2;

int  bends, bend_speed, bend_angle, confirm , stepper_speed;
long int_Post, stepper_position,final_position;
float time_remaining;
unsigned long start_time, time_required;

int xPosition = 0;
int yPosition = 0;
int SW_state = 1;

void setup() {
  pinMode(joyPower, OUTPUT); 
  pinMode(VRx, INPUT);
  pinMode(VRy, INPUT);
  pinMode(SW, INPUT_PULLUP); 
  pinMode(home_switch, INPUT_PULLUP);
  
  pinMode(MS1, OUTPUT);
  pinMode(MS2, OUTPUT);
  pinMode(enable, OUTPUT);

  digitalWrite(MS1,HIGH);      // Configures to Half Steps
  digitalWrite(MS2,HIGH);
  
  digitalWrite(joyPower,HIGH);

   stepper.setMaxSpeed(6000); 
   
//  pinMode(SLEEP_PIN,OUTPUT);
  


  xPosition = analogRead(VRx);
  yPosition = analogRead(VRy);
  SW_state = digitalRead(SW);

   // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
   Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  } 
  // Clear the buffer
  display.clearDisplay(); 
  display.setTextColor(WHITE);
  display.setCursor(20,25);
  display.setTextSize(2);
  display.write("BEND_IT");
  display.drawFastHLine(15, 15, 90, WHITE);
  display.drawFastHLine(15, 50, 90, WHITE);
  display.display();
  
  digitalWrite(enable,LOW);
  homing();
  digitalWrite(enable,HIGH);
  stepper.setCurrentPosition(0);
  display.clearDisplay();
  display.setCursor(20,40);
  display.setTextSize(2);
  display.write("BEND_IT");
  unsigned long temp_time = millis(), current_time;
  
  do{
    current_time = millis() - temp_time;
    if(current_time > 0 & current_time <= 1000) {
      display.fillRect(0, 10, 100, 30, BLACK);
      display.drawBitmap(30,20,pic1,60,6,WHITE);
      display.display();
      current_time = millis() - temp_time;
    } else if (current_time > 1000 & current_time <= 2000) {
      display.fillRect(0, 10, 100, 30, BLACK);     
      display.drawBitmap(30,10,pic2,60,14,WHITE);
      display.display(); 
      current_time = millis() - temp_time;     
    } else if (current_time > 2000 & current_time <= 3000) {
      display.fillRect(0, 10, 100, 30, BLACK);      
      display.drawBitmap(30,10,pic3,60,22,WHITE);
      display.display();      
      temp_time = millis();
    }
  }while(digitalRead(SW)!=0);   //stay in setup untill button is pressed
}

                                                                // Loop starts here
  
void loop() {
  display.clearDisplay();
  display.setTextSize(2);      // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(45,10);
  display.write("Set"); 
  display.setCursor(26,30);
  display.write("length");
  display.setCursor(0,25);
  display.write("<");  
  display.setCursor(115,25);
  display.write(">");

  display.display();delay(500);
  
  digitalWrite(enable,LOW);
  
  do{
    xPosition= analogRead(VRx);
        if(xPosition>1000){
          if(digitalRead(home_switch)!=1){
            stepper.setSpeed(-1000);
            stepper.runSpeed();
            stepper_position = stepper.currentPosition();    
          }
        } else if (xPosition>500 & xPosition <=1000){
              if(digitalRead(home_switch)!=1){
                stepper.setSpeed(-500);
                stepper.runSpeed();
                stepper_position = stepper.currentPosition();    
              } 
        } else if (xPosition < 100) {
                  stepper.setSpeed(1000);
                  stepper.runSpeed(); 
                  stepper_position = stepper.currentPosition();            
        } else if (xPosition < 400 & xPosition >= 100) {
                  stepper.setSpeed(500);
                  stepper.runSpeed(); 
                  stepper_position = stepper.currentPosition();            
        }

  } while (digitalRead(SW)!=0);
  
  digitalWrite(enable,HIGH);
  
  display.clearDisplay();
  display.setCursor(25,10);
  display.write("Length");
  display.setCursor(20,40);
  display.println((0.8/1600)*stepper_position);
  display.setCursor(90,40);
  display.write("cm");
  int_Post = stepper_position;
  display.display();
  delay(2000);
    
  confirm = 0;
  do {
  bends = bends_func();
  delay(200);
  bend_speed = speed_func();
  delay(200);
  bend_angle = angle_func();
  delay(200);
  confirm = confirm_func(bends, bend_speed, bend_angle);
  } while (confirm ==0); 

  final_position = int_Post*sin((acos(-1)/180)* bend_angle/2);
  stepper_speed = (2*(int_Post - final_position))/bend_speed;
  time_required = (bends*bend_speed);                              // Time in s
  
  display.clearDisplay();
  display.setTextSize(1.5);
  display.setCursor(10,10);
  display.write("Length:");
  display.setCursor(10,20);
  display.println((0.8/1600)*stepper_position);
  display.setCursor(10,40);
  display.write("Final Pos:");
  display.setCursor(10,50);
  display.println((0.8/1600)*final_position);
  display.display();
  delay(1000);
  
  digitalWrite(enable,LOW);
  int i=0;
  
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(5,0);
  display.write("Bends:");
  display.setTextSize(1);
  display.setCursor(71,20);
  display.write("of ");
  display.println(bends);
  display.setTextSize(1);
  display.setCursor(1,50);
  display.write("Time left:");
  display.display();
  
  start_time = 0;
  start_time = millis();
  while (i != bends){
    
    stepper.setSpeed(-stepper_speed);
   do{
          stepper.runSpeed();
         } while (stepper.currentPosition()>final_position);
    stepper.setSpeed(stepper_speed);
   do{
          stepper.runSpeed();
         } while (stepper.currentPosition() <int_Post);  
  i++; 
  experiment_status(i,start_time, time_required);
  display.display();        
  }
  
  digitalWrite(enable,HIGH);
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(20,20);
  display.write("Job done");
  display.setTextSize(1);
  display.setCursor(0,50);
  display.write("Press key to continue");
  display.display();
  
  do{
    
  }while(digitalRead(SW)!=0);   //stay in setup untill button is pressed  
}

                                                                            
                                                                            
                                                                            
                                                                            
                                                                            
                                                                            
                                                                            //Function definations

int bends_func()                                                          // Set up no of bends         
{   
  int bends=1, v1=0, v2=0, v3=0, v4=1, v5=0, place_holder=1, navigate=1;
  
  delay(100);
  display.clearDisplay();
  SW_state = 1;
  
  while(SW_state!=0){ 
    navigate=1;
    
    display.setTextSize(2);      
    display.setTextColor(WHITE);
    display.drawFastHLine(0, 25, 128, WHITE);
    display.drawFastHLine(0, 60, 128, WHITE);
    display.setTextSize(2);
    display.setCursor(35, 0);
    display.write("Bends");
    display.fillRect(0, 27, 125, 31, BLACK);
    
    SW_state = digitalRead(2);

  // For 1st decimal place
   
    
   if(place_holder==1 && SW_state==1){
      while(navigate!=0 && SW_state!=0){ 
	      SW_state = digitalRead(2);
	      xPosition= analogRead(VRx);
        if(xPosition<300){
         navigate=0; 
         place_holder=place_holder+1; 
        } else if (xPosition >800) {
         navigate = 0; 
         place_holder = 5;          
        }
        yPosition= analogRead(VRy);
        if(yPosition<300){
          if (v1<9){
          v1=v1+1;
          } else{
            v1 = 0;
          }
        }else if (yPosition>800){
            if (v1>0) {
            v1=v1-1;
            } else if (v1 == 0) {
              v1 = 9;
            }
        }
        display.setTextColor(WHITE);
        display.setCursor(20, 35); 
        display.println(v5); 
        display.setCursor(40, 35); 
        display.println(v4);    
        display.setCursor(60, 35); 
        display.println(v3); 
        display.setCursor(80, 35); 
        display.println(v2);   
        display.setCursor(100,35);
        display.setTextColor(WHITE);
        display.println(v1);
        display.display();
        delay(80);
        display.setCursor(100,35);
        display.setTextColor(BLACK);
        display.println(v1);
        display.display();delay(80);
        SW_state = digitalRead(2);
      }
    }
    navigate=1;

 // For 2nd decimal place
     display.fillRect(0, 27, 125, 31, BLACK);
 
     if(place_holder==2 && SW_state==1){
      while(navigate!=0 && SW_state!=0){ 
        SW_state = digitalRead(2);
        xPosition= analogRead(VRx);
        if(xPosition<300){
         navigate=0; 
         place_holder=place_holder+1; 
        } else if (xPosition >800) {
         navigate = 0; 
         place_holder = place_holder-1;          
        }
        yPosition= analogRead(VRy);
        if(yPosition<300){
          if (v2<9){
          v2=v2+1;
          } else{
            v2 =0;
          }
        }else if (yPosition>800){
            if (v2>0) {
            v2=v2-1;
            } else if (v2 == 0) {
              v2 = 9;
            }
        }
        display.setTextColor(WHITE);
        display.setCursor(20, 35); 
        display.println(v5); 
        display.setCursor(40, 35); 
        display.println(v4);    
        display.setCursor(60, 35); 
        display.println(v3); 
        display.setCursor(100, 35); 
        display.println(v1);   
        display.setCursor(80,35);
        display.setTextColor(WHITE);
        display.println(v2);
        display.display();
        delay(80);
        display.setCursor(80,35);
        display.setTextColor(BLACK);
        display.println(v2);
        display.display();delay(80);
        SW_state = digitalRead(2);
      }
    }
    navigate=1;    

     // For 3rd decimal place
    display.fillRect(0, 27, 125, 31, BLACK);
         
     if(place_holder==3 && SW_state==1){
      while(navigate!=0 && SW_state!=0){ 
        SW_state = digitalRead(2);
        xPosition= analogRead(VRx);
        if(xPosition<300){
         navigate=0; 
         place_holder=place_holder+1; 
        } else if (xPosition >800) {
         navigate = 0; 
         place_holder = place_holder-1;          
        }
        yPosition= analogRead(VRy);
        if(yPosition<300){
          if (v3<9){
          v3=v3+1;
          } else{
            v3 =0;
          }
        }else if (yPosition>800){
            if (v3>0) {
            v3=v3-1;
            } else if (v3 == 0) {
              v3 = 9;
            }
        }
        display.setTextColor(WHITE);
        display.setCursor(20, 35); 
        display.println(v5); 
        display.setCursor(40, 35); 
        display.println(v4);    
        display.setCursor(80, 35); 
        display.println(v2); 
        display.setCursor(100, 35); 
        display.println(v1);   
        display.setCursor(60,35);
        display.setTextColor(WHITE);
        display.println(v3);
        display.display();
        delay(80);
        display.setCursor(60,35);
        display.setTextColor(BLACK);
        display.println(v3);
        display.display();delay(80);
        SW_state = digitalRead(2);
      }
    }
    navigate=1; 

       // For 4th decimal place
    display.fillRect(0, 27, 125, 31, BLACK);
           
     if(place_holder==4 && SW_state==1){
      while(navigate!=0 && SW_state!=0){ 
        SW_state = digitalRead(2);
        xPosition= analogRead(VRx);
        if(xPosition<300){
         navigate=0; 
         place_holder=place_holder+1; 
        } else if (xPosition >800) {
         navigate = 0; 
         place_holder = place_holder-1;          
        }
        yPosition= analogRead(VRy);
        if(yPosition<300){
          if (v4<9){
          v4=v4+1;
          } else{
            v4 =0;
          }
        }else if (yPosition>800){
            if (v4>0) {
            v4=v4-1;
            } else if (v4 == 0) {
              v4 = 9;
            }
        }
        display.setTextColor(WHITE);
        display.setCursor(20, 35); 
        display.println(v5); 
        display.setCursor(80, 35); 
        display.println(v2);    
        display.setCursor(60, 35); 
        display.println(v3); 
        display.setCursor(100, 35); 
        display.println(v1);   
        display.setCursor(40,35);
        display.setTextColor(WHITE);
        display.println(v4);
        display.display();
        delay(80);
        display.setCursor(40,35);
        display.setTextColor(BLACK);
        display.println(v4);
        display.display();delay(80);
        SW_state = digitalRead(2);
      }
    }
    navigate=1;

    // For 5th decimal place
    display.fillRect(0, 27, 125, 31, BLACK);
        
     if(place_holder==5 && SW_state==1){
      while(navigate!=0 && SW_state!=0){ 
        SW_state = digitalRead(2);
        xPosition= analogRead(VRx);
        if(xPosition >800) {
         navigate = 0; 
         place_holder = place_holder-1;
        } else if (xPosition <300) {
         navigate = 0; 
         place_holder = 1;          
        }
        yPosition= analogRead(VRy);
        if(yPosition<300){
          if (v5<9){
          v5=v5+1;
          } else{
            v5 =0;
          }
        }else if (yPosition>800){
            if (v5>0) {
            v5=v5-1;
            } else if (v5 == 0) {
              v5 = 9;
            }
        }
        display.setTextColor(WHITE);
        display.setCursor(80, 35); 
        display.println(v2); 
        display.setCursor(40, 35); 
        display.println(v4);    
        display.setCursor(60, 35); 
        display.println(v3); 
        display.setCursor(100, 35); 
        display.println(v1);   
        display.setCursor(20,35);
        display.setTextColor(WHITE);
        display.println(v5);
        display.display();
        delay(80);
        display.setCursor(20,35);
        display.setTextColor(BLACK);
        display.println(v5);
        display.display();delay(80);
        SW_state = digitalRead(2);
      }
    }
    navigate=1;    
    delay(100); 
  } 
   return bends= 10000*v5+1000*v4+100*v3+10*v2+v1;
}
 
 int speed_func()
 {
  bend_speed = 5; 
  SW_state = 1;
  delay(100);
  
  while(SW_state!=0){
    SW_state = digitalRead(2);
    xPosition= analogRead(VRx);
      if(xPosition>800){
        bend_speed++;
        delay(200);
      } else if (xPosition<200) {
        if (bend_speed > 2){
          bend_speed--;
          delay(200);
        }        
      }
   display.clearDisplay();
   display.drawFastHLine(0, 25, 128, WHITE);
   display.drawFastHLine(0, 60, 128, WHITE);
   display.setTextSize(2);
   display.setTextColor(WHITE);
   display.setCursor(10, 0);
   display.write("Time/bend");
   display.setCursor(00, 35);
   display.write("<");
   display.setCursor(115, 35);
   display.write(">");
   display.setCursor(50,35);
   display.println(bend_speed);
   display.setCursor(90, 45);
   display.setTextSize(1);
   display.write("sec");
    display.display();  
  }
   return bend_speed;
 }


 int angle_func()
 {
  bend_angle = 90;
  SW_state = 1;
  delay(100);
  
  while(SW_state!=0){
    SW_state = digitalRead(2);
    xPosition= analogRead(VRx);
      if(xPosition>800){
        if (bend_angle<180){
          bend_angle++;
          delay(50);
        }  
      } else if (xPosition<200) {
        if (bend_angle > 10){
          bend_angle--;
          delay(50);
        }        
      }
  display.clearDisplay();
  display.drawFastHLine(0, 25, 128, WHITE);
  display.drawFastHLine(0, 60, 128, WHITE);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(5, 0);
  display.write("Bend angle");
  display.setCursor(00, 35);
  display.write("<");
  display.setCursor(115, 35);
  display.write(">");
  display.setCursor(50,35);
  display.println(bend_angle);
  display.display();  
  }
   return bend_angle;
 }
 
 int confirm_func(int bends, int bend_speed, int bend_angle)
 {
  int var = 1;  
  display.clearDisplay(); 
  display.setTextSize(2);
  display.setCursor(0,5);
  display.write("Bends:"); 
  display.setCursor(70,5);
  if (bends>9999){
    display.setTextSize(1); 
    display.println(bends); 
  } else {
      display.println(bends);
  };
  display.setTextSize(2);  
  display.setCursor(0,25);
  display.write("Speed:"); 
  display.setCursor(80,25);
  display.println(bend_speed);
  display.setCursor(0,45);
  display.write("Angle:"); 
  display.setCursor(80,45);
  display.println(bend_angle);
  display.display();
  delay(1000);
  
  SW_state = digitalRead(2);
  while (SW_state != 0){
    SW_state = digitalRead(2);
    delay(100);
  }
  delay(100);
  SW_state = digitalRead(2);
  
  
  while (SW_state != 0){
    SW_state = digitalRead(SW);
    yPosition= analogRead(VRy);
   if(yPosition<100){
       if (var == 0){
         var =1;
       } else if (var ==1) {
            var =0;
         }
     }else if (yPosition>900){
            if (var == 1) {
            var =0 ;
            } else if (var == 0) {
              var = 1;
            }
     }
    display.clearDisplay(); 
    display.setCursor(20,20);
    display.write("Confirm?");
    display.setCursor(20,40);
    display.write("Reset");
    if (var==1){
      display.setCursor(5,20);
      display.write(">");
      } else if (var ==0) {
      display.setCursor(5,40);
      display.write(">");    
      }
      display.display();
      delay(100);
      SW_state = digitalRead(SW);
    }
 return var; 
 delay(500);
}

 void homing(){
  int temp_pos =1000;
  stepper.setSpeed(-500);
  while(!digitalRead(home_switch)){
    stepper.runSpeed();
  }
   stepper.setSpeed(100);
   while(digitalRead(home_switch)){
    stepper.runSpeed();
  }   
 }

void experiment_status(int i, unsigned long start_time,unsigned long time_required){
   
   display.setTextSize(2);
   display.fillRect(0, 20, 70, 15, BLACK);
   display.setCursor(5,20);
   display.println(i);
   display.fillRect(70, 50, 70, 15, BLACK);
   display.setTextSize(1);
   display.setCursor(61,50);
   time_remaining = (time_required-((millis()-start_time)/1000))/60;  // time in minutes
   display.println(int(time_remaining/60));
   display.setCursor(72,50);
   display.write("hr");
   display.setCursor(90,50);
   display.println( int(time_remaining) % 60);
   display.setCursor(105,50);   
   display.write("min");
}
