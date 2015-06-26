#define ABS(my_val) ((my_val) < 0) ? -(my_val) : (my_val)
const int R = 160;
const unsigned char sin_lut[256] = {
    0x00, 0x01, 0x03, 0x04, 0x06, 0x07, 0x09, 0x0a, 0x0c, 0x0e, 0x0f, 0x11, 0x12, 0x14, 0x15, 0x17,
    0x19, 0x1a, 0x1c, 0x1d, 0x1f, 0x20, 0x22, 0x24, 0x25, 0x27, 0x28, 0x2a, 0x2b, 0x2d, 0x2e, 0x30,
    0x31, 0x33, 0x35, 0x36, 0x38, 0x39, 0x3b, 0x3c, 0x3e, 0x3f, 0x41, 0x42, 0x44, 0x45, 0x47, 0x48,
    0x4a, 0x4b, 0x4d, 0x4e, 0x50, 0x51, 0x53, 0x54, 0x56, 0x57, 0x59, 0x5a, 0x5c, 0x5d, 0x5f, 0x60,
    0x61, 0x63, 0x64, 0x66, 0x67, 0x69, 0x6a, 0x6c, 0x6d, 0x6e, 0x70, 0x71, 0x73, 0x74, 0x75, 0x77,
    0x78, 0x7a, 0x7b, 0x7c, 0x7e, 0x7f, 0x80, 0x82, 0x83, 0x84, 0x86, 0x87, 0x88, 0x8a, 0x8b, 0x8c,
    0x8e, 0x8f, 0x90, 0x92, 0x93, 0x94, 0x95, 0x97, 0x98, 0x99, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f, 0xa1,
    0xa2, 0xa3, 0xa4, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab, 0xad, 0xae, 0xaf, 0xb0, 0xb1, 0xb2, 0xb3,
    0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf, 0xc0, 0xc1, 0xc2, 0xc3, 0xc4,
    0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf, 0xd0, 0xd1, 0xd2, 0xd3, 0xd3,
    0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xdd, 0xde, 0xdf, 0xe0, 0xe1,
    0xe1, 0xe2, 0xe3, 0xe3, 0xe4, 0xe5, 0xe6, 0xe6, 0xe7, 0xe8, 0xe8, 0xe9, 0xea, 0xea, 0xeb, 0xeb,
    0xec, 0xed, 0xed, 0xee, 0xee, 0xef, 0xef, 0xf0, 0xf1, 0xf1, 0xf2, 0xf2, 0xf3, 0xf3, 0xf4, 0xf4,
    0xf4, 0xf5, 0xf5, 0xf6, 0xf6, 0xf7, 0xf7, 0xf7, 0xf8, 0xf8, 0xf9, 0xf9, 0xf9, 0xfa, 0xfa, 0xfa,
    0xfb, 0xfb, 0xfb, 0xfb, 0xfc, 0xfc, 0xfc, 0xfc, 0xfd, 0xfd, 0xfd, 0xfd, 0xfe, 0xfe, 0xfe, 0xfe,
    0xfe, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};
unsigned char buffer[64000];

inline int sin(int x)
{
    x &= 1023;
    if (x < 256)
        return sin_lut[x & 0xff];
    else if (x < 512)
        return sin_lut[(x & 0xff) ^ 0xff];
    else if (x < 768)
        return -sin_lut[x & 0xff];
    else
        return -sin_lut[(x & 0xff) ^ 0xff];
}

inline int cos(int x)
{
    x += 256;
    x &= 1023;
    if (x < 256)
        return sin_lut[(unsigned char)x];
    else if (x < 512)
        return sin_lut[0xff - (unsigned char)x];
    else if (x < 768)
        return -sin_lut[(unsigned char)x];
    else
        return -sin_lut[0xff - (unsigned char)x];
}

inline void outportb(unsigned int port, unsigned char value)
{
    asm volatile ("outb %%al,%%dx": :"d" (port), "a" (value));
}

inline unsigned char inportb(unsigned int port)
{
    unsigned char value;
    asm volatile ("inb %%dx": "=a" (value):"d" (port));
    return value;
}

void vsync()
{
    do { } while (inportb(0x3DA) & 8);
    do { } while (!(inportb(0x3DA) & 8));
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

unsigned char f(float x, float y)
{
    return (int)x > 150 ? 0 : 255;
}

unsigned char render(int x, int y, int phi, int size)
{
    int rx, ry;
    int c = cos(phi);
    int s = sin(phi);
    int s2 = size >> 1;
    x -= 160 * 256 - 128;
    y -= 100 * 256 - 128;
    x = ABS(x);
    y = ABS(y);
//     tx = x;
//     ty = y;
    rx = x * c;
    rx -= y * s;
    ry = x * s;
    ry += y * c;
    rx >>= 8;
    ry >>= 8;
    x = rx;
    y = ry;
//     return f(0, ((float)x) / 256.0);
    if ((y >> 8) % size < s2 ^ (x >> 8) % size < s2)
        return 0xff;
    return ((x + y) >> 7) & 0xff;
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
    unsigned char color;
    int x, y, dx, dy, i;
    int phi;
    unsigned int d1 = 1;

    // set grayscale palette
    outportb(0x03c6, 0xff);
    outportb(0x03c8, 0);
    for (i = 0; i < 64; i++)
    {
        outportb(0x03c9, i);
        outportb(0x03c9, i);
        outportb(0x03c9, i);
    }

    phi = 40;
    while (1)
    {
        unsigned char* vid_mem = buffer;
        unsigned int* source = buffer;
        unsigned int* target = 0xa0000;
        for (y = 0; y < 200; y++)
        {
            for (x = 0; x < 320; x++)
            {
                int sum = 0;
                for (dx = 0; dx < 4; dx++)
                {
                    for (dy = 0; dy < 4; dy++)
                    {
                        sum += render(x * 256 + dx * 64, y * 256 + dy * 64, phi * 4, (sin(phi * 8) + 258) / 8 + 32);
                    }
                }
//                 sum = 0;
                sum >>= 4;
                d1 = (d1 * 1103515245U + 12345U) & 0x7fffffffU;
                sum -= 2;
                sum += d1 >> 27;
                if (sum < 0)
                    sum = 0;
                if (sum > 255)
                    sum = 255;
                color = sum >> 2;
//                 color *= 20;
//                 color = sum >> 6;
//                 color = render(x * 256 + dx * 64, y * 256 + dy * 64, phi * 4, (sin(phi * 8) + 258) / 8 + 32) >> 2;
                *(vid_mem) = color;
                vid_mem += 1;
            }
        }
        vsync();
        for (i = 0; i < 16000; ++i)
        {
            *(target) = *(source);
            target += 1;
            source += 1;
        }
        phi += 1;
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
