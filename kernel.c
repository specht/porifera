#define ABS(my_val) ((my_val) < 0) ? -(my_val) : (my_val)
const int R = 24;
// unsigned char prev_line[321];

inline void outportb(unsigned int port, unsigned char value)
{
    asm volatile ("outb %%al,%%dx": :"d" (port), "a" (value));
}

int render_circle(int cx, int cy, int x, int y)
{
    x -= cx * 256;
    y -= cy * 256;
    if (x >= -R * 256 && x <= R * 256 &&
        y >= -R * 256 && y <= R * 256 &&
        x * x + y * y < R * R * 256 * 256)
        return 1;
    else
        return 0;
}

unsigned char render(int x, int y)
{
    int c;
    x -= 80 * 256;
    y -= 20 * 256;
//     if (render_circle(0, 0, x, y) ||
//         render_circle(160, 0, x, y) ||
//         render_circle(0, 160, x, y) ||
//         render_circle(160, 160, x, y))
//         return 0xff;
//     if (render_circle(0, 160, x, y))
//         return 0xff;
    if (x > 0 && x < 160 * 256 && y > 0 && y < 160 * 256)
    {
        if (y < R * 256)
        {
            if (x < R * 256)
            {
                if (render_circle(R, R, x, y))
                    return 0xff;
            }
            else if (x > (160 - R) * 256)
            {
                if (render_circle((160 - R), R, x, y))
                    return 0xff;
            }
            else
                return 0xff;
        }
        else if (y > (160 - R) * 256)
        {
            if (x < R * 256)
            {
                if (render_circle(R, (160 - R), x, y))
                    return 0xff;
            }
            else if (x > (160 - R) * 256)
            {
                if (render_circle((160 - R), (160 - R), x, y))
                    return 0xff;
            }
            else
                return 0xff;
        }
        else
            return 0xff;
    }
    return 0;
}

void main() {
    unsigned char* vid_mem = (unsigned char*)0xa0000;
    unsigned color;
    int x, y, dx, dy, i;

    // set grayscale palette
    outportb(0x03c6, 0xff);
    outportb(0x03c8, 0);
    for (i = 0; i < 64; i++)
    {
        outportb(0x03c9, i);
        outportb(0x03c9, i);
        outportb(0x03c9, i);
    }

    for (y = 0; y < 200; y++)
    {
        for (x = 0; x < 320; x++)
        {
            int sum = 0;
            for (dx = 0; dx < 4; dx++)
            {
                for (dy = 0; dy < 4; dy++)
                {
                    sum += render(x * 256 + dx * 64, y * 256 + dy * 64);
                }
            }
            color = sum >> 4 >> 2;
            *(vid_mem) = color;
            vid_mem += 1;
        }
    }
}

// void main()
// {
//     double a = 0.0;
//     double x;
//     a += 1.0;
//     if (a < 0.0)
//         x = 2.0;
//     else
//         x = 3.0;
//
//
// }
