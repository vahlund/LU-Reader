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
        lightHouse(pixels->Color(170, 170, 100));
        break;
    case 1:
        blink(pixels->Color(150, 0, 0)); //Red
        break;
    case 2:
        loopColors();
        break;
    case 3:
        blink(pixels->Color(150, 150, 150)); //White
        spinLight(pixels->Color(150, 0, 0));
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

void ReaderActions::lightHouse(uint32_t color) {
    for (uint16_t i = 0; i < houseLight->numPixels(); i += 4) {
        houseLight->setPixelColor(i, color);
        houseLight->setPixelColor(i + 1, color);
        houseLight->setPixelColor(i + 2, color);
        houseLight->setPixelColor(i + 3, color);
        houseLight->setPixelColor(i + 4, color);
        houseLight->show();
        delay(400);
    };
    topLight->setPixelColor(0, color);
    topLight->setPixelColor(2, color);
    topLight->setPixelColor(4, color);
    topLight->show();
    delay(1000);
    topLight->setPixelColor(1, color);
    topLight->setPixelColor(3, color);
    topLight->setPixelColor(5, color);
    topLight->show();
    delay(1000);
    topLight->fill(color, 0, topLight->numPixels());
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
            topLED = 0;
        }
        delay(100);
    };

    topLight->clear();
    topLight->show();
    houseLight->clear();
    houseLight->show();
}
