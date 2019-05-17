#include <WebUSB.h>
#include <math.h>

/**
 * Creating an instance of WebUSBSerial will add an additional USB interface to
 * the device that is marked as vendor-specific (rather than USB CDC-ACM) and
 * is therefore accessible to the browser.
 *
 * The URL here provides a hint to the browser about what page the user should
 * navigate to to interact with the device.
 */
WebUSB WebUSBSerial(1 /* https:// */, "degroote22.github.io/loop-webusb-sim");

#define Serial WebUSBSerial
#define true 1
#define false 0

void setup()
{
    while (!Serial)
    {
        ;
    }
    Serial.begin(9600);
}

void light(int i, int r, int g, int b)
{
    Serial.print("l-");
    Serial.print(i); // i
    Serial.print("-");
    Serial.print(r); // r
    Serial.print("-");
    Serial.print(g); // g
    Serial.print("-");
    Serial.print(b); // b
    Serial.print("\r\n");
    Serial.flush();
}

// custom code

volatile float speed = 0;
volatile float position = 0;

#define _MAX 300
#define _SIMUL_RESOLUTION (1.0 / 10000.0)
#define _SPEED_FACTOR (1.0 / 4.0)
#define _GRAVITY_FACTOR (1.0 / 10.0)
#define _DRAG_FACTOR (1.0 / 1.0)
#define _FRICTION_FACTOR (1.0 / 15.0)
#define _MAX_TIME_SIMUL 16

float heights[_MAX] = {
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
    int tolerance = _MAX / 30;
    if (i >= tolerance + 1 * _MAX / 4 && i < -tolerance + 2 * _MAX / 4)
    {
        return true;
    }
    if (i >= tolerance + 3 * _MAX / 4 && i < -tolerance + 4 * _MAX / 4)
    {
        return true;
    }
    return false;
}

void moveColor(int i, int r, int g, int b)
{
    int j = i < 1 ? _MAX - 1 : i - 1;
    light(j < 3 ? _MAX - (3 - j) : j - 3, 0, 0, 0);
    light(j < 2 ? _MAX - (2 - j) : j - 2, 0, 0, 0);
    light(j < 1 ? _MAX - (1 - j) : j - 1, 0, 0, 0);

    light((j + 0) % _MAX, r, g, b);
    light((j + 1) % _MAX, r, g, b);
    light((j + 2) % _MAX, r, g, b);

    light((j + 3) % _MAX, 0, 0, 0);
    light((j + 4) % _MAX, 0, 0, 0);
    light((j + 5) % _MAX, 0, 0, 0);
}

void moveGreen(int i)
{
    moveColor(i, 0, 255, 0);
}
void gravity(float delta)
{
    int pos = position;
    float h = heights[pos];
    int next = (pos + 1) % _MAX;
    float nh = heights[next];
    float diff = h - nh;

    speed = speed + diff * _GRAVITY_FACTOR * _SIMUL_RESOLUTION * delta;
}
void drag(float delta)
{
    if (speed < 0)
    {
        speed = speed + _DRAG_FACTOR * speed * speed * _SIMUL_RESOLUTION + _FRICTION_FACTOR * _SIMUL_RESOLUTION * delta;
    }
    else
    {
        speed = speed - _DRAG_FACTOR * speed * speed * _SIMUL_RESOLUTION - _FRICTION_FACTOR * _SIMUL_RESOLUTION * delta;
    }
}
void clamp_speed()
{
    if (speed > 0.1)
    {
        if (in_curve(position))
            speed = 0;
    }
}
void update_position()
{
    position = (position + speed);

    if (position > _MAX)
    {
        position = position - _MAX;
    }
    else if (position < 0)
    {
        position = _MAX + position;
    }
}
void accelerate(float delta)
{
    speed += _SPEED_FACTOR * _SIMUL_RESOLUTION * delta;
}
void effect1()
{
    int init = millis();
    int last = init;
    while (last - init < _MAX_TIME_SIMUL)
    {
        int now = millis();
        int delta = now - last;
        if (delta == 0)
        {
            continue;
        }
        // TODO: pegar o input
        if (true)
        {
            accelerate(delta);
        }
        gravity(delta);
        drag(delta);

        clamp_speed();
        update_position();

        last = now;
    }
    moveGreen(position);
}

void loop()
{
    effect1();
}
