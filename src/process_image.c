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
                if (channel > 1 || channel < 0) {
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
            float r = get_pixel(im, w, h, 0); // R
            float g = get_pixel(im, w, h, 1); // G
            float b = get_pixel(im, w, h, 2); // B

            float max = three_way_max(r, g, b); // Value of V
            float value = max;

            float min = three_way_min(r,g,b);
            float C = max - min;

            float saturation = 0;
            if (value != 0) { // If all value are equal to 0 we dont want to divide by 0
                saturation = C / value;
            }
            
            float huePrime = 0;
            if (C == 0){}
            else if (r == value)
                huePrime = (g - b) / C;
            else if (g == value)
                huePrime = ((b - r) / C) + 2.0;
            else if (b == value)
                huePrime = ((r - g) / C) + 4.0;

            float hue = huePrime / 6.0;
            if (hue < 0)
                hue++;

            set_pixel(im, w, h, 0, hue); // H
            set_pixel(im, w, h, 1, saturation); // S
            set_pixel(im, w, h, 2, value); // V
        }
    }
}

void hsv_to_rgb(image im)
{
       for (size_t h = 0; h < im.h; h++)
    {
        for (size_t w = 0; w < im.w; w++)
        {
            float hue = get_pixel(im, w, h, 0); // H
            float saturation = get_pixel(im, w, h, 1); // S
            float value = get_pixel(im, w, h, 2); // V
            
            float r = 0, g= 0, b = 0;
            float C = saturation * value; // diff
		    float min = value - C;
		    float huePrime = hue * 6.0;
		    float X = (1 - fabs(fmod(huePrime, 2) - 1)); 

            float midValue = (C * X) + min;
            if (huePrime < 1 && huePrime >= 0) { // Goes towards red
                r = value;
                g = midValue;
                b = min;
            } else if (huePrime < 2) { // Goes towards yellow
                r = midValue;
                g = value;
                b = min;
            } else if (huePrime < 3) { // Goes towards green
                r = min;
                g = value;
                b = midValue;
            } else if (huePrime < 4) { // Goes towards light blue
                r = min;
                g = midValue;
                b = value;
            } else if (huePrime < 5) { // Goes towards blue
                r = midValue;
                g = min;
                b = value;
            } else if (huePrime < 6) { // Goes towards cyan
                r = value;
                g = min;
                b = midValue;
            }else {
                r = 0;
                g = 0;
                b = 0;
            }

            set_pixel(im, w, h, 0, r); // R
            set_pixel(im, w, h, 1, g); // G
            set_pixel(im, w, h, 2, b); // B
	    }
    }
}