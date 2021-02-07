// Ejemplo de programar para ver los codigos de un mando IR
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#include <IRremote.h>

// En la documentacion nos decian los cambios... https://github.com/Arduino-IRremote/Arduino-IRremote

#define IR_RECEIVER_PIN 10

LiquidCrystal_I2C lcd(0x3F,16,2);  // COMPRUEBA QUE TIENES LA DIRECCION CORRECTA DEL LCD

// Caracteres definidos por el usuario

// Las posiciones de los caracteres 
#define RWNDCHAR 0
#define STOPCHAR 1
#define FFCHAR   2
#define PLAYCHAR 3

// La definicion de los caracteres https://maxpromer.github.io/LCD-Character-Creator/

byte PlayChar[] = {
  B00000,
  B01000,
  B01100,
  B01110,
  B01100,
  B01000,
  B00000,
  B00000
};


byte FFchar[] = {
  B00000,
  B10100,
  B10110,
  B10111,
  B10110,
  B10100,
  B00000,
  B00000
};


byte STOPchar[] = {
  B00000,
  B01110,
  B01110,
  B01110,
  B01110,
  B01110,
  B00000,
  B00000
};

byte RWNDchar[] = {
  B00000,
  B10001,
  B10011,
  B10111,
  B10011,
  B10001,
  B00000,
  B00000
};


void setup(){
  Serial.begin(9600);
  lcd.init(); // Inicializamos
  lcd.backlight(); // Encendemos la luz trasera

  // Definimos 4 caracteres de usuario
  lcd.createChar(RWNDCHAR, RWNDchar);
  lcd.createChar(STOPCHAR, STOPchar);
  lcd.createChar(FFCHAR, FFchar);
  lcd.createChar(PLAYCHAR, PlayChar);

  pinMode(LED_BUILTIN, OUTPUT); // Usaremos el Led del pin 13 como indicador de haber recibido un codigo
  IrReceiver.begin(IR_RECEIVER_PIN, ENABLE_LED_FEEDBACK); // Activamos el receptor 
}



void loop(){

    /*
     * Check if received data is available and if yes, try to decode it.
     * Decoded result is in the IrReceiver.decodedIRData structure.
     */
    if (IrReceiver.decode()) { // Si hay datosHay datos
        if (IrReceiver.decodedIRData.flags & IRDATA_FLAGS_WAS_OVERFLOW) {
            IrReceiver.decodedIRData.flags = false; // yes we have recognized the flag :-)
            // no need to call resume, this is already done by decode()
            Serial.println(F("Overflow detected"));
        } else {
            // Print a short summary of received data
            
        }
        Serial.println();
        
        IrReceiver.resume(); // Hemos procesado los datos y vamos a recibir el siguiente
        /*
         * Check the received data
         */
        lcd.clear();
        switch (IrReceiver.decodedIRData.command ){
          case 64: 
            lcd.setCursor(0,0);
            lcd.write(PLAYCHAR);
            lcd.print("Play");
            break;
          case 70:
            lcd.setCursor(0,0);
            lcd.print("Volumen +");
            break;
          case 21:
            lcd.setCursor(0,0);
            lcd.print("Volumen -");
            break;        
          case 67:
            lcd.setCursor(0,0);
            lcd.write(FFCHAR);
            lcd.print(" FFWD");
            break;  
          case 68:
            lcd.setCursor(0,0);
            lcd.write(RWNDCHAR);
            lcd.print(" RWD");
            break;      
          case 71:
            lcd.setCursor(0,0);
            lcd.write(STOPCHAR);
            lcd.print(" STOP");
            break;
          case 69:
            lcd.setCursor(0,0);
            lcd.print("Off");
            lcd.noBacklight();
            delay(2000);
            lcd.clear();
            lcd.backlight();
            lcd.setCursor(0,0);
            lcd.print("Tricked!!");
            break;            
          default:
            IrReceiver.printIRResultShort(&Serial);
            lcd.setCursor(0,0);
            lcd.print(IrReceiver.decodedIRData.command);
            lcd.print(" desconocido");
            Serial.print("Recibido el comando desconocido"); Serial.println(IrReceiver.decodedIRData.command);
            break;           
        }
    }
  
}
