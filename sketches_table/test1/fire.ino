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