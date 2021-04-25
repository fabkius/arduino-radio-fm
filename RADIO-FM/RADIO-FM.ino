//Libraries:
#include <TEA5767.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//Constants:
TEA5767 Radio; //Pinout SLC and SDA - Arduino uno pins A5 and A4


//Variables:
double old_frequency;
double frequency;
int search_mode = 0;
int search_direction;
unsigned long last_pressed;
unsigned char buf[5];
int stereo;
int signal_level;
double current_freq;
unsigned long current_millis = millis();
int inByte;
int flag=0;

const int analogPin = A2;
int valorVolumen;
int valorVolumenAnt;
int valorM;   
int position;   
int positionAnterior;  

const int nextup = 2;
const int nextdown = 3;
int buttonStateUp;   
int buttonStateDown;            
int lastButtonStateUp = LOW; 
int lastButtonStateDown = LOW; 
int count = 0;
int flagInit = 0;

LiquidCrystal_I2C lcd(0x3F, 16, 2);

void setup () {
  //Init
  Serial.begin(9600);
  Radio.init();
  Radio.set_frequency(94.10); //Estacion radio por defecto
  
  pinMode(nextup, INPUT);
  pinMode(nextdown, INPUT);
  
  lcd.begin();
  lcd.backlight();
  
  lcd.setCursor(0,0);
  lcd.print("** Radio v1.2 **");        
  delay(500);
  lcd.clear();
  
}

void loop () {

   if(flagInit==0){
    findRadio();
    getScreen();
    flagInit=1;
  }
    
  valorVolumen = analogRead(analogPin);
   
  
  if (digitalRead(nextup)==1) {
    findRadio();
    last_pressed = current_millis;
    search_mode = 1;
    search_direction = TEA5767_SEARCH_DIR_UP;
    Radio.search_up(buf);
    flagInit=0;
  }
  
  if (digitalRead(nextdown)==1) {
    findRadio();
    last_pressed = current_millis;
    search_mode = 1;
    search_direction = TEA5767_SEARCH_DIR_DOWN;
    Radio.search_down(buf);
    flagInit=0;
  }
  
 int dif1 = valorVolumenAnt - valorVolumen;

 if(abs(dif1-5)<30){
   getVolumen(valorVolumen);
 }

  valorVolumenAnt = valorVolumen;
}


void findRadio(){
  
    if (Radio.read_status(buf) == 1) {
    current_freq =  floor (Radio.frequency_available (buf) / 100000 + .5) / 10;
    stereo = Radio.stereo(buf);
    signal_level = Radio.signal_level(buf);
  }

  if (search_mode == 1) {
      if (Radio.process_search (buf, search_direction) == 1) {
          search_mode = 0;
      }
  }
}

void getScreen(){

    lcd.setCursor(0,0);
    //delay(100);
    lcd.print("FM: "); lcd.print(current_freq);
    //delay(100);
    lcd.print(" MHz ");
    //delay(100);
    lcd.setCursor(0,1);
    //delay(100);
    if (stereo) lcd.print("STEREO"); else lcd.print("MONO  ");
    //delay(100);
    lcd.print((char) 11101111);
    //delay(100);
    lcd.print(getPercent(signal_level));
    //delay(100);
    lcd.print("%");
    //delay(100);
}

int getPercent(int val){
  int per = (val * 100)/15;
  return per;
}

void getVolumen(int value){
  
    lcd.setCursor(11,1); 
    delay(500);
    
   if(value > 0 && value < 55){
    lcd.print("V: 0%"); 
    delay(500);
   }else if(value > 50 && value < 100){
     lcd.print("V:10%"); 
     delay(500);
   }else if(value > 120 && value < 150){
     lcd.print("V:20%"); 
     delay(500);
   }else if(value > 170 && value < 200){
     lcd.print("V:30%");
     delay(500); 
   }else if(value > 220 && value < 250){
     lcd.print("V:40%"); 
     delay(500);
   }else if(value > 270 && value < 300){
     lcd.print("V:50%"); 
     delay(500);
   }else if(value > 320 && value < 350){
     lcd.print("V:60%"); 
     delay(500); 
   }else if(value > 350 && value < 400){
     lcd.print("V:70%"); 
     delay(500);
   }else if(value > 420 && value < 450){
     lcd.print("V:80%"); 
     delay(500);
   }else if(value > 470 && value < 500){
     lcd.print("V:90%");  
     delay(500);
   }else if(value > 520 && value < 600){
     lcd.print("V:MAX"); 
     delay(500); 
   }
  
}
