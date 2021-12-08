#include "ReaderActions.h"

ReaderActions::ReaderActions(Adafruit_NeoPixel* pixels, Adafruit_NeoPixel* houseLight, Adafruit_NeoPixel* topLight) {
    this->pixels = pixels;
    this->houseLight = houseLight;
    this->topLight = topLight;

    randomSeed(analogRead(0));
}

void ReaderActions::cardApprovedAction() {
    long rand = random(400);
    rand = rand / 100;
    Serial.print(rand);
    if ((millis() - lastCardApproved) > 60000 || lastCardApproved == 0) {
        rand = 1; // Set Red if first scan (within 60sec)
    }
    lastCardApproved = millis();

    switch (rand) {
    case 0:
        blink(pixels->Color(0, 150, 0)); //Green
        lightHouse(pixels->Color(120, 120, 120), pixels->Color(250, 250, 250));
        break;
    case 1:
        blink(pixels->Color(150, 0, 0)); //Red
        break;
    case 2:
        loopColors();
        break;
    case 3:
        blink(pixels->Color(150, 150, 150)); //White
        spinLight(pixels->Color(150, 150, 150));
        break;
    default:
        blink(pixels->Color(150, 0, 0)); //Red

        break;
    }
}

void ReaderActions::blink(uint32_t color) {
    pixels->fill(color, 0, pixels->numPixels());
    pixels->show();
    delay(1000);
    pixels->clear();
    pixels->show();
}

void ReaderActions::loopColors() {
    uint16_t hue = 0;
    for (uint32_t i = 0; i < 65536 * 2; i += 40) {
        pixels->fill(pixels->ColorHSV(hue, 255, 150), 0, pixels->numPixels());
        houseLight->fill(houseLight->ColorHSV(hue, 255, 150), 0, houseLight->numPixels());
        //topLight->fill(topLight->ColorHSV(hue, 255, 150), 0, topLight->numPixels());
        pixels->show();
        houseLight->show();
        //topLight->show();
        hue += 40;
        //delay(10);
    }
    pixels->clear();
    houseLight->clear();
    topLight->clear();
    houseLight->show();
    topLight->show();
    pixels->show();
}

void ReaderActions::lightHouse(uint32_t colorLED, uint32_t colorStrip) {
    houseLight->clear();
    for (uint16_t i = 0; i < houseLight->numPixels(); i += 4) {
        houseLight->setPixelColor(i, colorStrip);
        houseLight->setPixelColor(i + 1, colorStrip);
        houseLight->setPixelColor(i + 2, colorStrip);
        houseLight->setPixelColor(i + 3, colorStrip);
        houseLight->setPixelColor(i + 4, colorStrip);
        houseLight->show();
        delay(400);
    };
    topLight->clear();
    topLight->show();
    topLight->setPixelColor(0, colorLED);
    topLight->setPixelColor(2, colorLED);
    topLight->setPixelColor(4, colorLED);
    topLight->show();
    delay(1000);
    topLight->setPixelColor(1, colorLED);
    topLight->setPixelColor(3, colorLED);
    topLight->setPixelColor(5, colorLED);
    topLight->show();
    delay(1000);
    topLight->fill(colorStrip, 6, topLight->numPixels() - 6);
    topLight->show();
    delay(4000);
    houseLight->clear();
    houseLight->show();
    topLight->clear();
    topLight->show();
    
}

void ReaderActions::spinLight(uint32_t color) {
    uint8_t topLED = 0;
    for (uint16_t i = 0; i < houseLight->numPixels(); i++) {
        houseLight->clear();
        houseLight->setPixelColor(i, color);
        houseLight->show();
        topLight->clear();
        topLight->setPixelColor(topLED, color);
        topLight->show();
        topLED++;
        if (topLED > 5) {
            topLED = 0;
        }

        delay(100);
    };

    topLED = 5;
    for (uint16_t i = (houseLight->numPixels() - 1); i == 0; i--) {
        houseLight->clear();
        houseLight->setPixelColor(i, color);
        houseLight->show();
        topLight->clear();
        topLight->setPixelColor(topLED, color);
        topLight->show();
        topLED--;
        if (topLED > 5) {
            topLED = 5;
        }
        delay(100);
    };

    topLight->clear();
    topLight->show();
    houseLight->clear();
    houseLight->show();
}
