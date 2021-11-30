#ifndef READERACTIONS_H
#define READERACTIONS_H

#include <Adafruit_NeoPixel.h>

class ReaderActions {

private:
    Adafruit_NeoPixel* pixels;
    Adafruit_NeoPixel* houseLight;
    Adafruit_NeoPixel* topLight;

    unsigned long lastCardApproved = 0;

public:

    /**
     * @brief Construct a new Reader Actions object
     * 
     * @param pixels Card Reader light bar
     */
    ReaderActions(Adafruit_NeoPixel* pixels, Adafruit_NeoPixel* houseLight, Adafruit_NeoPixel* topLight);

    void cardApprovedAction();

    /**
     * @brief Blinks for one second
     * 
     * @param color from .Color()
     */
    void blink(uint32_t color);

    void loopColors();

    void lightHouse(uint32_t color);
};

#endif // READERACTIONS_H
