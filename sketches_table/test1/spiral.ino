#define MAX 12

int r = 50;
int g = 50;
int b = 50;

int rm = 0;
int gm = 0;
int bm = 0;
int removal = MAX * MAX;

void reset_colors()
{
    rm = 30 + rand() & 205;
    gm = 59 + rand() & 205;
    bm = 11 + rand() & 205;

    removal = MAX * MAX;
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

void spiral()
{
    int i = 0;
    int _max = MAX;
    int step = 0;

    reset_colors();

    while (_max > 0)
    {
        // down
        if (step == 0)
        {
            if (i < _max)
            {
                _light(i, MAX - _max);
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

            if (i < _max - (MAX - _max))
            {
                _light(_max - 1, i + (MAX - _max));
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

            if (i < _max - (MAX - _max))
            {
                _light(_max - i - 1, _max - 1);
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

            if (i < _max - (MAX - _max) - 1)
            {
                _light(MAX - _max, _max - 1 - i);
                i++;
            }
            else
            {

                _max = _max - 1;
                i = (MAX - _max);
                step = (step + 1) % 4;
            }
        }
    }
}