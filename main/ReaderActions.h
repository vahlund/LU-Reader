#ifndef READERACTIONS_H
#define READERACTIONS_H

#include <Adafruit_NeoPixel.h>

class ReaderActions {

private:
    Adafruit_NeoPixel* pixels;

public:

    /**
     * @brief Construct a new Reader Actions object
     * 
     * @param pixels Card Reader light bar
     */
    ReaderActions(Adafruit_NeoPixel* pixels);

    void cardApprovedAction();

    /**
     * @brief Blinks for one second
     * 
     * @param color from .Color()
     */
    void blink(uint32_t color);

    void loopColors();
};

#endif // READERACTIONS_H