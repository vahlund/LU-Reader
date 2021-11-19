#include <Adafruit_NeoPixel.h>
#include <MFRC522.h>
#include <SPI.h>

#define RST_PIN 9 // Configurable, see typical pin layout above
#define SS_PIN 10 // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance.

MFRC522::MIFARE_Key key;

void cardScanned();
void buttonPressed();
void buttonPressedISR();

const int buzzer = 5;  //buzzer to arduino pin
const int buttons = 2; //Keypad pin

int lastButtonState = LOW;
unsigned long lastDebounceTime = 0; // the last time the output pin was toggled
unsigned long debounceDelay = 50;

volatile int numButtonPresses = 0;
bool awaitsPin = false;

const int lightbarPin = 6;     // On Trinket or Gemma, suggest changing this to 1
const int numLightbarPins = 3; // Popular NeoPixel ring size

Adafruit_NeoPixel pixels(numLightbarPins, lightbarPin, NEO_RGB + NEO_KHZ800);
bool blinkYellow = false;
int blinkItr = 0;

volatile bool buttonSTATE = false;

/**
   Initialize.
*/
void setup() {
    Serial.begin(9600); // Initialize serial communications with the PC
    while (!Serial)
        ; // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)

    SPI.begin();        // Init SPI bus
    mfrc522.PCD_Init(); // Init MFRC522 card

    /* read and printout the MFRC522 version (valid values 0x91 & 0x92)*/
    Serial.print(F("Ver: 0x"));
    byte readReg = mfrc522.PCD_ReadRegister(mfrc522.VersionReg);
    Serial.println(readReg, HEX);

    pinMode(buttons, INPUT); //interrupt pin for buttons
    attachInterrupt(digitalPinToInterrupt(buttons), buttonPressedISR, RISING);

    pinMode(buzzer, OUTPUT); // Set up buzzer pin

    pixels.begin(); // INITIALIZE NeoPixel strip object
    pixels.clear();
    pixels.show();

    Serial.println(F("End setup"));
}

void loop() {
    //Detect card being scanned
    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
        cardScanned();
    }

    if (buttonSTATE) {
        buttonPressed();
        buttonSTATE = false;
    }

    if (numButtonPresses == 4) {
        //Call success/fail function here
        pixels.fill(pixels.Color(0, 150, 0), 0, numLightbarPins);
        pixels.show();
        delay(1000);
        pixels.clear();
        pixels.show();
        blinkItr = 0;
        blinkYellow = false;
        awaitsPin = false;
    }

    if (blinkYellow) {
        if (blinkItr == 20) {
            pixels.fill(pixels.Color(150, 0, 0), 0, numLightbarPins);
            pixels.show();
            delay(1000);
            pixels.clear();
            pixels.show();
            blinkItr = 0;
            blinkYellow = false;
            awaitsPin = false;
        } else if (blinkItr % 2 == 0) {
            pixels.fill(pixels.Color(150, 150, 0), 0, numLightbarPins);
            pixels.show();
        } else {
            pixels.clear();
            pixels.show();
        }
        delay(200);
        blinkItr++;
    } else {
        blinkItr = 0;
    }

    delay(10);
}

//Function to call when a card has been scanned.
//Resets button press count, beeps and starts to flash yellow.
void cardScanned() {
    numButtonPresses = 0;
    awaitsPin = true;
    blinkYellow = true;
    buttonSTATE = false;
    
    // 3820Hz for 125ms
    tone(buzzer, 3820);
    delay(125);
    noTone(buzzer);
}

//Longer(Time wise) function for handling button presses.
void buttonPressed() {
    //Debouncing for handling/filtering noise and rapid button presses. Uses a timer to only allow button presses with minimum specified delay.

    //Read button GPIO pin
    int reading = digitalRead(buttons);
    // If the switch changed, due to noise or pressing:
    if (reading != buttonSTATE) {
        // reset the debouncing timer
        lastDebounceTime = millis();
    }

    //Check debounce criteria
    if ((millis() - lastDebounceTime) > debounceDelay) {
        //Only increment the counter if a card has recently been scanned.
        if (awaitsPin) {
            numButtonPresses++;
        }
        // 3370Hz for 60ms
        tone(buzzer, 3370);
        delay(60);
        noTone(buzzer);
        lastDebounceTime = millis();
    }
}

//Short(time wise) function called by ISR (Interrupt Service Routine) when the button is pressed.
//Cant do long processing since it interrupts other processing.
void buttonPressedISR() {
    buttonSTATE = true;
}
