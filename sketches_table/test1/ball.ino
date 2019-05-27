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