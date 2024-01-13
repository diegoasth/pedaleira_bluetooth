#include <Arduino.h>

#include <BleKeyboard.h>

#include <SSD1306Wire.h>
#include "splash.h"

#include <MAX17043.h>

// Initialize the OLED display using Wire library
// SSD1306Wire display(ADDRESS, SDA, SDC)
SSD1306Wire display(0x3c, 18, 19);   // initialize with the I2C addr 0x3C (for the 128x64) 
                                     // Samir - 0x3C
                                     // Diego - 0x78

#define DISPLAY_HEIGHT display.height()
#define DISPLAY_WIDTH  display.width()

MAX17043 batteryMonitor;

const int button1Pin = 12;     // the number of the pushbutton pin
const int button2Pin = 13;     // the number of the pushbutton pin
const int ledPin =     22;     // the number of the LED pin

BleKeyboard bleKeyboard;

void desenhaTelaConectada(){
  
  display.clear();
  
  display.setColor(BLACK);
  display.fillRect(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT-13);
  display.setColor(WHITE);
  
  display.setTextAlignment(TEXT_ALIGN_CENTER_BOTH);  
  // display.setFont(Kranky_Regular_16);
  display.setFont(ArialMT_Plain_16);  
  
  display.drawRect(                  0, 0, (DISPLAY_WIDTH/2)-1, DISPLAY_HEIGHT-13);
  display.drawRect((DISPLAY_WIDTH/2)+1, 0, (DISPLAY_WIDTH/2)-1, DISPLAY_HEIGHT-13);

  display.drawString(DISPLAY_WIDTH*.25, ( DISPLAY_HEIGHT-13)/2, "Up");
  display.drawString(DISPLAY_WIDTH*.75, ( DISPLAY_HEIGHT-13)/2, "Down");
  
}

void desenhaTelaDesconectada(){

  display.clear();
  
  display.setColor(BLACK);
  display.fillRect(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT-13);
  display.setColor(WHITE);
    
  display.setTextAlignment(TEXT_ALIGN_CENTER_BOTH);  
  display.setFont(ArialMT_Plain_10);
  display.drawRect(0, 0, DISPLAY_WIDTH-1, DISPLAY_HEIGHT-13);
  
  display.drawString(DISPLAY_WIDTH/2, (DISPLAY_HEIGHT-13)/2, "BT Disconnected");
 
}

void setup() {
  
  Serial.begin(115200);
  Serial.println("Starting BLE work!");
  
  bleKeyboard.setName("Flipper-O-Matic");
  bleKeyboard.begin();

  batteryMonitor.reset();
  batteryMonitor.quickStart();
  
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);

  //display.invertDisplay();

  display.clear();
  display.drawFastImage(0, 0, 128, 64, Logo1);
  display.display();  
  delay(500);

  display.clear();
  display.drawFastImage(0, 0, 128, 64, Logo2);
  display.display();  
  delay(500);

  display.clear();
  display.drawFastImage(0, 0, 128, 64, Logo3);
  display.display();  
  delay(500);

  display.clear();
  display.drawFastImage(0, 0, 128, 64, Logo4);
  display.display();  
  delay(1000);

  pinMode(button1Pin, INPUT_PULLUP);
  pinMode(button2Pin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);

  digitalWrite(ledPin, HIGH);

}

void loop() {
  
  if(bleKeyboard.isConnected()) {

    desenhaTelaConectada();

    while (digitalRead(button1Pin) == LOW ){
    
      Serial.println("teclado seta CIMA");

      digitalWrite(ledPin, LOW);
      display.invertDisplay();
      
      
      bleKeyboard.write(KEY_UP_ARROW);
      delay(100);
      
    }
    
    while (digitalRead(button2Pin) == LOW ){
      
      Serial.println("teclado seta BAIXO");

      digitalWrite(ledPin, LOW);
      display.invertDisplay();
      
      
      bleKeyboard.write(KEY_DOWN_ARROW);
      delay(100);
    
    }


  } else {
      
      desenhaTelaDesconectada();
  
  }

  //atualiza a bateria na progressBar
  display.setColor(BLACK);
  display.fillRect(0, DISPLAY_HEIGHT-10-1, DISPLAY_WIDTH-1, 10);
  display.setColor(WHITE);
  display.drawProgressBar(0, DISPLAY_HEIGHT-10-1, DISPLAY_WIDTH-1, 10, batteryMonitor.getSoC());
  display.display();  

  //volta o display ao normal
  digitalWrite(ledPin, HIGH);
  display.normalDisplay();    
  delay(200);

}