#include "ReaderActions.h"

ReaderActions::ReaderActions(Adafruit_NeoPixel* pixels) {
    this->pixels = pixels;
}

void ReaderActions::cardApprovedAction() {
    uint8_t rand = random(0, 2);
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
    for (uint32_t i = 0; i < 65536 * 2; i++) {
        pixels->fill(pixels->ColorHSV(hue, 1, 0.5), 0, pixels->numPixels());
        pixels->show();
        hue++;
    }
    pixels->clear();
    pixels->show();
}
