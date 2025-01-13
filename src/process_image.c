#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>
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
    int imgsize = sizeof(float) * copy.c * copy.h * copy.w;
    memcpy(copy.data, im.data, imgsize);
    return copy;
}

image rgb_to_grayscale(image im)
{
    assert(im.c == 3);
    image gray = make_image(im.w, im.h, 1);
    for (size_t x = 0; x < im.w; x++)
    {
        for (size_t y = 0; y < im.h; y++)
        {
            float r = get_pixel(im, x, y, 0);
            float g = get_pixel(im, x, y, 1);
            float b = get_pixel(im, x, y, 2);
            float grayscale = 0.299f * r + 0.587f * g + 0.114f * b;
            set_pixel(gray, x, y, 0, grayscale);
        }
    }

    return gray;
}

void shift_image(image im, int c, float v)
{
    for (size_t x = 0; x < im.w; x++)
    {
        for (size_t y = 0; y < im.h; y++)
        {
            float channel = get_pixel(im, x, y, c);
            set_pixel(im, x, y, c, channel + v);
        }
    }
}

void clamp_image(image im)
{
    for (size_t x = 0; x < im.w; x++)
    {
        for (size_t y = 0; y < im.h; y++)
        {
            for (size_t c = 0; c < im.c; c++)
            {
                float channel = get_pixel(im, x, y, c);
                if (channel > 1) {
                    channel = 1;
                    set_pixel(im, x, y, c, channel);
                }
                else if (channel < 0){
                    channel = 1;
                    set_pixel(im, x, y, c, channel);
                }
            }
        }
    }
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
    for (size_t h = 0; h < im.h; h++)
    {
        for (size_t w = 0; w < im.w; w++)
        {
            float r = get_pixel(im, w,h, 0); // R
            float g = get_pixel(im, w,h, 1); // G
            float b = get_pixel(im, w,h, 2); // B

            float value = three_way_max(r,g,b); // Value of V

            float saturation = 0;
            float minValue = three_way_min(r,g,b);
            float diff = value - minValue;
            // Si toutes les valeurs sont egal a 0 eviter de diviser par 0
            if (r != 0 && g != 0 && b != 0){
                float saturation = diff / value;
            }
            float hue = 0;
            if (r == value)
                hue = (g - b)/diff;
            else if (g == value)
                hue = (b - r)/diff + 2;
            else
                hue = (b - g)/diff + 4;

            hue = hue/6;            
            if (hue < 0)
                hue++;

            set_pixel(im, w,h, 0, hue); // H
            set_pixel(im, w,h, 1, saturation); // S
            set_pixel(im, w,h, 2, value); // V
        }
    }
}

void hsv_to_rgb(image im)
{
    for (size_t h = 0; h < im.h; h++)
    {
        for (size_t w = 0; w < im.w; w++)
        {
            float h = get_pixel(im, w,h, 0); // H
            float s = get_pixel(im, w,h, 1); // S
            float v = get_pixel(im, w,h, 2); // V

            float value = three_way_max(r,g,b); // Value of V

            float saturation = 0;
            float minValue = three_way_min(r,g,b);
            float diff = value - minValue;
            // Si toutes les valeurs sont egal a 0 eviter de diviser par 0
            if (r != 0 && g != 0 && b != 0){
                float saturation = diff / value;
            }
            float hue = 0;
            if (r == value)
                hue = (g - b)/diff;
            else if (g == value)
                hue = (b - r)/diff + 2;
            else
                hue = (b - g)/diff + 4;

            hue = hue/6;            
            if (hue < 0)
                hue++;

            set_pixel(im, w,h, 0, hue); // R
            set_pixel(im, w,h, 1, saturation); // G
            set_pixel(im, w,h, 2, value); // B
        }
    }
}
