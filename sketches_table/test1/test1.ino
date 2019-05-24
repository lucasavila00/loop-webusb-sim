#include <math.h>
#define NUMPIXELS 144
// Para conectar ao NeoPixel de verdade, usar essa parte comentada
/*
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
    strip.show(); // This sends the updated pixel color to the hardware.
    delay(8);
}

void reset()
{
    for (int i = 0; i < 12 * 12; i++)
    {
        strip.setPixelColor(i, strip.Color(0, 0, 0)); // Moderately bright green color.
    }
    strip.show(); // This sends the updated pixel color to the hardware.
}

#define _MAX 12

int max = _MAX;
int step = 0;
int i = 0;
int r = 50;
int g = 50;
int b = 50;

int rm = 0;
int gm = 0;
int bm = 0;
int removal = _MAX * _MAX;

void reset_colors()
{
    rm = 30 + rand() & 205;
    gm = 59 + rand() & 205;
    bm = 11 + rand() & 205;

    removal = _MAX * _MAX;
}

void loop_colors()
{
    if (removal > 0)
    {
        removal--;
        r = max(30, rm - 0.3 * removal);
        g = max(59, gm - 0.59 * removal);
        b = max(11, bm - 0.11 * removal);
    }
}

void _light(int i, int j)
{
    loop_colors();
    light(i, j, r, g, b);
}

bool effect1()
{
    if (i == 0 && step == 0)
    {
        reset_colors();
    }
    if (max == 0)
    {
        max = _MAX;
        step = 0;
        i = 0;
        return false;
    }
    // down
    if (step == 0)
    {
        if (i < max)
        {
            _light(i, _MAX - max);
            i++;
        }
        else
        {
            i = 1;
            step = (step + 1) % 4;
        }
    }

    // right
    if (step == 1)
    {

        if (i < max - (_MAX - max))
        {
            _light(max - 1, i + (_MAX - max));
            i++;
        }
        else
        {
            i = 1;
            step = (step + 1) % 4;
        }
    }

    if (step == 2)
    {

        if (i < max - (_MAX - max))
        {
            _light(max - i - 1, max - 1);
            i++;
        }
        else
        {
            i = 1;
            step = (step + 1) % 4;
        }
    }

    if (step == 3)
    {

        if (i < max - (_MAX - max) - 1)
        {
            _light(_MAX - max, max - 1 - i);
            i++;
        }
        else
        {

            max = max - 1;
            i = (_MAX - max);
            step = (step + 1) % 4;
        }
    }
    return true;
}

#define FIRE_WIDTH 12
#define FIRE_HEIGHT 12
#define PALLETE_LENGTH 9
uint8_t rgbs[PALLETE_LENGTH * 3] = {
    0x07, 0x07, 0x07,
    // 0x1F, 0x07, 0x07,
    // 0x2F, 0x0F, 0x07,
    // 0x47, 0x0F, 0x07,
    0x57, 0x17, 0x07,
    // 0x67, 0x1F, 0x07,
    // 0x77, 0x1F, 0x07,
    // 0x8F, 0x27, 0x07,
    0x9F, 0x2F, 0x07,
    // 0xAF, 0x3F, 0x07,
    // 0xBF, 0x47, 0x07,
    // 0xC7, 0x47, 0x07,
    0xDF, 0x4F, 0x07,
    // 0xDF, 0x57, 0x07,
    // 0xDF, 0x57, 0x07,
    // 0xD7, 0x5F, 0x07,
    0xD7, 0x5F, 0x07,
    // 0xD7, 0x67, 0x0F,
    // 0xCF, 0x6F, 0x0F,
    // 0xCF, 0x77, 0x0F,
    0xCF, 0x7F, 0x0F,
    // 0xCF, 0x87, 0x17,
    // 0xC7, 0x87, 0x17,
    // 0xC7, 0x8F, 0x17,
    0xC7, 0x97, 0x1F,
    // 0xBF, 0x9F, 0x1F,
    // 0xBF, 0x9F, 0x1F,
    // 0xBF, 0xA7, 0x27,
    0xBF, 0xA7, 0x27,
    // 0xBF, 0xAF, 0x2F,
    // 0xB7, 0xAF, 0x2F,
    // 0xB7, 0xB7, 0x2F,
    // 0xB7, 0xB7, 0x37,
    // 0xCF, 0xCF, 0x6F,
    // 0xDF, 0xDF, 0x9F,
    // 0xEF, 0xEF, 0xC7,
    0xFF, 0xFF, 0xFF};

volatile uint32_t pallete[PALLETE_LENGTH] = {0};
// Palette based framebuffer. Coordinate system origin upper-left.
volatile int firePixels[NUMPIXELS] = {0};
volatile bool wind_on = true;
volatile int c = 0;

void setup_fire()
{
    // Populate pallete.
    for (int i = 0; i < PALLETE_LENGTH; i++)
    {
        pallete[i] = strip.Color(rgbs[i * 3 + 0], rgbs[i * 3 + 1], rgbs[i * 3 + 2]);
    }

    // Set whole screen to 0 (color: 0x07,0x07,0x07)
    for (int i = 0; i < FIRE_WIDTH * FIRE_HEIGHT; i++)
    {
        firePixels[i] = 0;
    }

    // Set bottom line to PALLETE_LENGTH-1 (color white: 0xFF,0xFF,0xFF)
    for (int i = 0; i < FIRE_WIDTH; i++)
    {
        firePixels[(FIRE_HEIGHT - 1) * FIRE_WIDTH + i] = PALLETE_LENGTH - 5 - c;
    }
}

void turn_off()
{
    wind_on = false;
    // Set bottom line to PALLETE_LENGTH-1 (color white: 0xFF,0xFF,0xFF)
    for (int i = 0; i < FIRE_WIDTH; i++)
    {
        firePixels[(FIRE_HEIGHT - 1) * FIRE_WIDTH + i] = PALLETE_LENGTH - 5 - c;
    }
}
void turn_on()
{
    c = max(0, min(2, c + ((rand() & 1) ? 1 : -1)));
    wind_on = true;
    // Set bottom line to PALLETE_LENGTH-1 (color white: 0xFF,0xFF,0xFF)
    for (int i = 0; i < FIRE_WIDTH; i++)
    {
        firePixels[(FIRE_HEIGHT - 1) * FIRE_WIDTH + i] = PALLETE_LENGTH - 5 - c;
    }
}

/***********************************************/
/**************** MEAT STARTS HERE *************/
void spreadFire(int src)
{

    int pixel = firePixels[src];
    if (pixel == 0)
    {
        firePixels[src - FIRE_WIDTH] = 0;
    }
    else
    {
        if (wind_on)
        {
            int randIdx = rand() & 3;
            int dst = src - randIdx + 1;
            firePixels[dst - FIRE_WIDTH] = pixel - (randIdx & 1);
        }
        else
        {
            firePixels[src - FIRE_WIDTH] = firePixels[src] - (rand() & 1);
        }
    }
}

void doFire()
{

    for (int x = 0; x < FIRE_WIDTH; x++)
    {
        for (int y = 1; y < FIRE_HEIGHT; y++)
        {
            spreadFire(y * FIRE_WIDTH + x);
        }
    }
}
/***********************************************/
/*************** MEAT ENDS HERE ****************/
/***********************************************/

void printAll()
{
    for (int i = 0; i < FIRE_WIDTH; i++)
    {
        for (int j = 0; j < FIRE_HEIGHT; j++)
        {
            int n = decode(i, j);
            int index = firePixels[i * 12 + j];
            uint32_t color = pallete[index];
            strip.setPixelColor(n, color);
        }
    }
    strip.show(); // This sends the updated pixel color to the hardware.
}

void fire()
{
    reset();

    for (int k = 0; k < 12; k++)
    {

        if (rand() & 1)
        {
            turn_on();
        }
        else
        {
            turn_off();
        }

        for (int j = 0; j < 4; j++)
        {
            doFire();
            printAll();
        }
    }
}

void ball()
{
    int i = 0;
    int j = 0;

    int di = 1;
    int dj = 1;

    uint32_t color = strip.Color(64, 32, 16);
    uint32_t colorbg = strip.Color(0, 0, 0);

    for (int x = 0; x < 32; x++)
    {
        for (int y = 0; y < 16; y++)
        {
            // clear
            strip.setPixelColor(decode(i, j), colorbg);

            // update
            if (di == 1)
            {
                if (i == 11)
                {
                    // bateu na parede direita
                    di = -1;
                }
                i = i + di;
            }
            else
            {
                if (i == 0)
                {
                    // bateu na parede esquerda
                    di = 1;
                }
                i = i + di;
            }

            if (dj == 1)
            {
                if (j == 11)
                {
                    // bateu na parede superior
                    dj = -1;
                }
                if (rand() & 1)
                {
                    j = j + dj;
                }
            }
            else
            {
                if (j == 0)
                {
                    // bateu na parede inferior
                    dj = 1;
                }

                j = j + dj;
            }
            // print
            strip.setPixelColor(decode(i, j), color);
            strip.show();
            delay(32);
        }
    }
}

void loop()
{
    setup_fire();
    fire();
    while (effect1())
    {
    }

    ball();
}
