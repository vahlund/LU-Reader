#include "ReaderActions.h"

ReaderActions::ReaderActions(Adafruit_NeoPixel* pixels, Adafruit_NeoPixel* houseLight, Adafruit_NeoPixel* topLight) {
    this->pixels = pixels;
    this->houseLight = houseLight;
    this->topLight = topLight;

    randomSeed(analogRead(0));
}

void ReaderActions::cardApprovedAction() {
    long rand = random(300);
    rand = rand / 100;
    Serial.print(rand);
    switch (rand) {
    case 0:
        blink(pixels->Color(0, 150, 0)); //Green
        break;
    case 1:
        blink(pixels->Color(150, 0, 0)); //Red
        break;
    case 2:
        loopColors();
        break;
    default:
        blink(pixels->Color(150, 0, 0)); //Red
        break;
    }
    lightHouse(pixels->Color(170, 170, 170));
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
        pixels->show();
        hue += 40;
        //delay(10);
    }
    pixels->clear();
    pixels->show();
}

void ReaderActions::lightHouse(uint32_t color) {
    uint16_t hue = 0;
    for (uint32_t i = 0; i < 65536 * 2; i += 40) {
        houseLight->fill(houseLight->ColorHSV(hue, 255, 150), 0, houseLight->numPixels());
        topLight->fill(topLight->ColorHSV(hue, 255, 150), 0, topLight->numPixels());
        houseLight->show();
        topLight->show();
        hue += 40;
        //delay(10);
    }
    houseLight->clear();
    topLight->clear();
    houseLight->show();
    topLight->show();
}
