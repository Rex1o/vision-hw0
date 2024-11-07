#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "image.h"

// x = col, y = row c channel
float get_pixel(image im, int x, int y, int c)
{ 
    x = x <= 0 ? 0 : (x >= im.w ? im.w - 1: x);
    y = y <= 0 ? 0 : (y >= im.h ? im.h - 1: y);
    c = c <= 0 ? 0 : (c >= im.c ? im.c - 1: c);
    return im.data[c * im.h * im.w + y * im.w + x];
}

void set_pixel(image im, int x, int y, int c, float v)
{
    if (x >= im.w || y >= im.h || c >= 3)
        return;
    im.data[c * im.h * im.w + y * im.w + x] = v;
}

image copy_image(image im)
{
    image copy = make_image(im.w, im.h, im.c);
    // TODO Fill this in
    return copy;
}

image rgb_to_grayscale(image im)
{
    assert(im.c == 3);
    image gray = make_image(im.w, im.h, 1);
    // TODO Fill this in
    return gray;
}

void shift_image(image im, int c, float v)
{
    // TODO Fill this in
}

void clamp_image(image im)
{
    // TODO Fill this in
}


// These might be handy
float three_way_max(float a, float b, float c)
{
    return (a > b) ? ( (a > c) ? a : c) : ( (b > c) ? b : c) ;
}

float three_way_min(float a, float b, float c)
{
    return (a < b) ? ( (a < c) ? a : c) : ( (b < c) ? b : c) ;
}

void rgb_to_hsv(image im)
{
    // TODO Fill this in
}

void hsv_to_rgb(image im)
{
    // TODO Fill this in
}
