inline void outportb(unsigned int port, unsigned char value)
{
   asm volatile ("outb %%al,%%dx": :"d" (port), "a" (value));
}


void main() {
    unsigned char* vid_mem = (unsigned char*)0xa0000;
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
        dy = y - 100;
        for (x = 0; x < 320; x++)
        {
            dx = x - 160;
            unsigned char color = 0;
            if ((dx * dx + dy * dy) < 4000)
                color = 0xf;
            else
                color = x & 0xff;
            *(vid_mem) = color;
            vid_mem += 1;
        }
    }
}
