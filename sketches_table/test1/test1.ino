// Para conectar ao NeoPixel de verdade, usar essa parte comentada
/*
// #define NUMPIXELS 144
#include <Adafruit_NeoPixel.h>

#define PIN        6 // On Trinket or Gemma, suggest changing this to 1

Adafruit_NeoPixel strip(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
*/

// Código necessário para o simulador
#include <WebUSB.h>
WebUSB WebUSBSerial(1 /* https:// */, "degroote22.github.io/loop-webusb-sim/table.html");

#define Serial WebUSBSerial

class FakePixels
{

public:
    static uint32_t Color(uint8_t r, uint8_t g, uint8_t b)
    {
        return ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
    }
    void setPixelColor(int i, uint32_t c)
    {
        Serial.print("_l-");
        Serial.print(i); // i
        Serial.print("-");
        Serial.print(c); // r
        Serial.print("\r\n");
        Serial.flush();
    }

    void begin()
    {
        Serial.print("_begin");
        Serial.print("\r\n");
        Serial.flush();
    }

    void show()
    {
        Serial.print("_show");
        Serial.print("\r\n");
        Serial.flush();
        delay(5);
    }
};

FakePixels strip;

// fim do código necessário para o simulador

// custom code
#include <math.h>

void setup()
{
    while (!Serial)
    {
        ;
    }
    Serial.begin(9600);
    strip.begin();
    strip.show(); // Initialize all pixels to 'off'
}

int decode(int i, int j)
{
    int column = i * 12;
    int row = i % 2 == 0 ? j : 11 - j;
    return column + row;
}

void light(int i, int j, int r, int g, int b)
{
    int n = decode(i, j);
    strip.setPixelColor(n, strip.Color(r, g, b));
    strip.show();
    delay(8);
}

void reset()
{
    for (int i = 0; i < 12 * 12; i++)
    {
        strip.setPixelColor(i, strip.Color(0, 0, 0));
    }
    strip.show();
}

#include "ball.ino"
#include "fire.ino"
#include "spiral.ino"

void loop()
{
    setup_fire();
    fire();
    while (spiral())
    {
    }

    ball();
}
