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

    Serial.println(value);  
    Serial.println("\n"); 
    
    lcd.setCursor(11,1); 
    delay(1000);
    
   if(value > 0 && value < 5){
    lcd.print("V: 0%"); 
    
   }else if(value > 10 && value < 15){
     lcd.print("V: 2%"); 
     
   }else if(value > 20 && value < 25){
     lcd.print("V: 5%"); 
     
   }else if(value > 30 && value < 35){
     lcd.print("V: 8%");
     
   }else if(value > 40 && value < 45){
     lcd.print("V:11%"); 
     
   }else if(value > 50 && value < 55){
     lcd.print("V:14%"); 
     
   }else if(value > 60 && value < 65){
     lcd.print("V:17%"); 
      
   }else if(value > 70 && value < 75){
     lcd.print("V:20%"); 
     
   }else if(value > 80 && value < 85){
     lcd.print("V:23%"); 
     
   }else if(value > 90 && value < 95){
     lcd.print("V:26%");  
     
   }else if(value > 100 && value < 105){
     lcd.print("V:29%"); 
     
   }else if(value > 110 && value < 115){
    lcd.print("V:32%"); 
    
   }else if(value > 120 && value < 125){
     lcd.print("V:35%"); 
     
   }else if(value > 130 && value < 135){
     lcd.print("V:38%"); 
     
   }else if(value > 140 && value < 145){
     lcd.print("V:41%");
      
   }else if(value > 150 && value < 155){
     lcd.print("V:44%"); 
     
   }else if(value > 160 && value < 165){
     lcd.print("V:47%"); 
     
   }else if(value > 170 && value < 175){
     lcd.print("V:50%"); 
     
   }else if(value > 180 && value < 185){
     lcd.print("V:53%"); 
     
   }else if(value > 190 && value < 195){
     lcd.print("V:56%"); 
     
   }else if(value > 200 && value < 205){
     lcd.print("V:59%");  
     
   }else if(value > 210 && value < 215){
     lcd.print("V:62%"); 
      
   }else if(value > 220 && value < 225){
    lcd.print("V:65%"); 
    
   }else if(value > 230 && value < 235){
     lcd.print("V:68%"); 
     
   }else if(value > 240 && value < 245){
     lcd.print("V:71%"); 
     
   }else if(value > 250 && value < 255){
     lcd.print("V:74%");
     
   }else if(value > 260 && value < 265){
     lcd.print("V:77%"); 
     
   }else if(value > 270 && value < 275){
     lcd.print("V:80%"); 
     
   }else if(value > 280 && value < 285){
     lcd.print("V:83%"); 
      
   }else if(value > 290 && value < 295){
     lcd.print("V:86%"); 
     
   }else if(value > 300 && value < 999){
     lcd.print("V:MAX"); 
     
   }
  
}
