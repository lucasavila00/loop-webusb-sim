#include <math.h>
#define NUMPIXELS 300

// Para conectar ao NeoPixel de verdade, usar essa parte comentada
/*
#include <Adafruit_NeoPixel.h>

#define PIN        6 // On Trinket or Gemma, suggest changing this to 1

Adafruit_NeoPixel strip(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
*/

// Código necessário para o simulador
#include <WebUSB.h>
WebUSB WebUSBSerial(1 /* https:// */, "degroote22.github.io/loop-webusb-sim");

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
        delay(10);
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

volatile float speed[3] = {0.05, 0.03, 0};
volatile float position[3] = {0.0, 50.0, 150.0};
volatile int laps[3] = {0, 0, 0};

#define _SIMUL_RESOLUTION (1.0 / 10000.0)
#define _SPEED_FACTOR (1.0 / 4.0)
#define _GRAVITY_FACTOR (1.0 / 10.0)
#define _DRAG_FACTOR (1.0 / 1.0)
#define _FRICTION_FACTOR (1.0 / 15.0)
#define NUMPIXELS_TIME_SIMUL 16

float heights[NUMPIXELS] = {
    0,
    -0.03555555555555555,
    -0.1422222222222222,
    -0.32,
    -0.5688888888888888,
    -0.8888888888888888,
    -1.28,
    -1.7422222222222217,
    -2.275555555555555,
    -2.88,
    -3.5555555555555554,
    -4.302222222222222,
    -5.12,
    -6.008888888888888,
    -6.968888888888887,
    -8.000000000000002,
    -9.10222222222222,
    -10.275555555555554,
    -11.52,
    -12.83111111111111,
    -14.111111111111112,
    -15.32,
    -16.45777777777778,
    -17.52444444444444,
    -18.519999999999996,
    -19.444444444444443,
    -20.29777777777777,
    -21.08,
    -21.79111111111111,
    -22.431111111111107,
    -23,
    -23.497777777777774,
    -23.924444444444443,
    -24.28000000000001,
    -24.56444444444444,
    -24.777777777777782,
    -24.919999999999998,
    -24.99111111111111,
    -24.99111111111111,
    -24.92,
    -24.77777777777778,
    -24.564444444444444,
    -24.28,
    -23.924444444444443,
    -23.497777777777777,
    -23,
    -22.43111111111111,
    -21.79111111111111,
    -21.08,
    -20.297777777777778,
    -19.444444444444446,
    -18.52,
    -17.524444444444445,
    -16.45777777777778,
    -15.32,
    -14.111111111111112,
    -12.831111111111113,
    -11.519999999999996,
    -10.275555555555554,
    -9.102222222222228,
    -8,
    -6.968888888888884,
    -6.00888888888889,
    -5.120000000000001,
    -4.302222222222223,
    -3.5555555555555607,
    -2.879999999999999,
    -2.2755555555555596,
    -1.7422222222222246,
    -1.2799999999999976,
    -0.8888888888888928,
    -0.5688888888888926,
    -0.3199999999999932,
    -0.14222222222222314,
    -0.035555555555550455,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0.03555555555555555,
    0.1422222222222222,
    0.32,
    0.5688888888888888,
    0.8888888888888888,
    1.28,
    1.7422222222222217,
    2.275555555555555,
    2.88,
    3.5555555555555554,
    4.302222222222222,
    5.12,
    6.008888888888888,
    6.968888888888887,
    8.000000000000002,
    9.10222222222222,
    10.275555555555554,
    11.52,
    12.83111111111111,
    14.111111111111112,
    15.32,
    16.45777777777778,
    17.52444444444444,
    18.519999999999996,
    19.444444444444443,
    20.29777777777777,
    21.08,
    21.79111111111111,
    22.431111111111107,
    23,
    23.497777777777774,
    23.924444444444443,
    24.28000000000001,
    24.56444444444444,
    24.777777777777782,
    24.919999999999998,
    24.99111111111111,
    24.99111111111111,
    24.92,
    24.77777777777778,
    24.564444444444444,
    24.28,
    23.924444444444443,
    23.497777777777777,
    23,
    22.43111111111111,
    21.79111111111111,
    21.08,
    20.297777777777778,
    19.444444444444446,
    18.52,
    17.524444444444445,
    16.45777777777778,
    15.32,
    14.111111111111112,
    12.831111111111113,
    11.519999999999996,
    10.275555555555554,
    9.102222222222228,
    8,
    6.968888888888884,
    6.00888888888889,
    5.120000000000001,
    4.302222222222223,
    3.5555555555555607,
    2.879999999999999,
    2.2755555555555596,
    1.7422222222222246,
    1.2799999999999976,
    0.8888888888888928,
    0.5688888888888926,
    0.3199999999999932,
    0.14222222222222314,
    0.035555555555550455,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
};
int in_curve(int i)
{
    int tolerance = NUMPIXELS / 30;
    if (i >= tolerance + 1 * NUMPIXELS / 4 && i < -tolerance + 2 * NUMPIXELS / 4)
    {
        return true;
    }
    if (i >= tolerance + 3 * NUMPIXELS / 4 && i < -tolerance + 4 * NUMPIXELS / 4)
    {
        return true;
    }
    return false;
}

void light(int i, int r, int g, int b)
{
    strip.setPixelColor(i, strip.Color(r, g, b));
}

void moveColor(int i, int r, int g, int b)
{
    int j = i < 1 ? NUMPIXELS - 1 : i - 1;
    light(j < 3 ? NUMPIXELS - (3 - j) : j - 3, 0, 0, 0);
    light(j < 2 ? NUMPIXELS - (2 - j) : j - 2, 0, 0, 0);
    light(j < 1 ? NUMPIXELS - (1 - j) : j - 1, 0, 0, 0);

    int dif1 = 100;
    int dif2 = 200;

    light((j + 0) % NUMPIXELS, max(0, r - dif2), max(0, g - dif2), max(0, b - dif2));
    light((j + 1) % NUMPIXELS, max(0, r - dif1), max(0, g - dif1), max(0, b - dif1));
    light((j + 2) % NUMPIXELS, max(0, r), max(0, g), max(0, b));

    light((j + 3) % NUMPIXELS, 0, 0, 0);
    light((j + 4) % NUMPIXELS, 0, 0, 0);
    light((j + 5) % NUMPIXELS, 0, 0, 0);
    strip.show();
}

void moveGreen(int i, float mult)
{
    moveColor(i, 0, 255 * mult, 0);
}

void moveRed(int i, float mult)
{
    moveColor(i, 255 * mult, 0, 0);
}

void moveBlue(int i, float mult)
{
    moveColor(i, 100 * mult, 100 * mult, 255 * mult);
}

void gravity(int arrpos)
{
    int pos = position[arrpos];
    float h = heights[pos];
    int next = (pos + 1) % NUMPIXELS;
    float nh = heights[next];
    float diff = h - nh;

    speed[arrpos] = speed[arrpos] + diff * _GRAVITY_FACTOR * _SIMUL_RESOLUTION;
}
void drag(int arrpos)
{
    if (speed < 0)
    {
        speed[arrpos] = speed[arrpos] + _DRAG_FACTOR * speed[arrpos] * speed[arrpos] * _SIMUL_RESOLUTION + _FRICTION_FACTOR * _SIMUL_RESOLUTION;
    }
    else
    {
        speed[arrpos] = speed[arrpos] - _DRAG_FACTOR * speed[arrpos] * speed[arrpos] * _SIMUL_RESOLUTION - _FRICTION_FACTOR * _SIMUL_RESOLUTION;
    }
}
void clamp_speed(int arrpos)
{
    if (speed[arrpos] > 0.1)
    {
        if (in_curve(position[arrpos]))
            speed[arrpos] = 0;
    }
}
void update_position(int arrpos)
{
    position[arrpos] = (position[arrpos] + speed[arrpos]);

    if (position[arrpos] > NUMPIXELS)
    {
        laps[arrpos]++;
        Serial.print("\r\n");
        Serial.print("car: ");
        Serial.print(arrpos);
        Serial.print(" tem voltas: ");
        Serial.print(laps[arrpos]);
        Serial.print("\r\n");
        Serial.flush();
        position[arrpos] = position[arrpos] - NUMPIXELS;
    }
    else if (position[arrpos] < 0)
    {
        position[arrpos] = NUMPIXELS + position[arrpos];
    }
}

void accelerate(int arrpos)
{
    speed[arrpos] += _SPEED_FACTOR * _SIMUL_RESOLUTION;
}

void simulate(int arrpos, bool acc)
{
    for (int i = 1; i < NUMPIXELS_TIME_SIMUL; i++)
    {

        // TODO: pegar o input
        if (acc)
        {
            accelerate(arrpos);
        }
        gravity(arrpos);
        drag(arrpos);

        clamp_speed(arrpos);
        update_position(arrpos);
    }
}

void loop()
{
    simulate(0, rand() & 1);
    simulate(1, rand() & 1);
    simulate(2, rand() & 1);

    moveRed(position[0], min(1.0, 0.3 + speed[0] * 10));
    moveGreen(position[1], min(1.0, 0.3 + speed[1] * 10));
    moveBlue(position[2], min(1.0, 0.3 + speed[2] * 10));
}
