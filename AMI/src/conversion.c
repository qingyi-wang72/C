#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "conversion.h"
#include "bitmap.h"

/* Convert RGB to HSV */
double *RGBtoHSV(int red, int green, int blue){
    static double pixel[3];
    double max, min, range, H_prime, H, S, V, copy[3];

    pixel[RED] = red;
    pixel[GREEN] = green;
    pixel[BLUE] = blue;

    copy[RED] = pixel[RED]/255;
    copy[GREEN] = pixel[GREEN]/255;
    copy[BLUE] = pixel[BLUE]/255;

    max = MAX(copy[RED], copy[GREEN], copy[BLUE]);
    min = MIN(copy[RED], copy[GREEN], copy[BLUE]);
    range = max - min;

    if(range == 0){
        H_prime = 0;
    }
    else if(max == copy[RED]){
        H_prime = fmod((copy[GREEN]-copy[BLUE])/range, 6);
    }
    else if(max == copy[GREEN]){
        H_prime = (copy[BLUE]-copy[RED])/range + 2;
    }
    else if(max == copy[BLUE]){
        H_prime = (copy[RED]-copy[GREEN])/range + 4;
    }

    /* Hue */
    if(H_prime < 0){
        H = 360 + 60*H_prime;
    }
    else{
        H = 60 * H_prime;
    }

    /* Value */
    V = max;

    /* Saturation */
    if(V == 0){
        S = 0;
    }
    else{
        S = range/V;
    }

    pixel[RED] = H;
    pixel[GREEN] = S;
    pixel[BLUE] = V;

    return pixel;
}

/* Maximum number within red, green, blue */
double MAX(double r, double g, double b){
    return ((r > g)? (r > b ? r : b) : (g > b ? g : b));
}

/* Minimum number within red, green, blue */
double MIN(double r, double g, double b){
    return ((r < g)? (r < b ? r : b) : (g < b ? g : b));
}

/* Convert HSV to RGB */
unsigned char *HSVtoRGB(double hue, double saturation, double value){
    static unsigned char final_pixel[3]={};
    double C=0, H_prime=0, X=0, m=0, pixel[3]={};

    C = value * saturation;
    H_prime = hue/60;
    X = C * (1 - fabs(fmod(H_prime, 2)-1));
    m = value - C;

    if(H_prime >= 0 && H_prime <=1){
        pixel[RED] = C;
        pixel[GREEN] = X;
        pixel[BLUE] = 0;
    }
    else if(H_prime >1 && H_prime <= 2){
        pixel[RED] = X;
        pixel[GREEN] = C;
        pixel[BLUE] = 0;
    }
    else if(H_prime >2 && H_prime <= 3){
        pixel[RED] = 0;
        pixel[GREEN] = C;
        pixel[BLUE] = X;
    }
    else if(H_prime >3 && H_prime <= 4){
        pixel[RED] = 0;
        pixel[GREEN] = X;
        pixel[BLUE] = C;
    }
    else if(H_prime >4 && H_prime <= 5){
        pixel[RED] = X;
        pixel[GREEN] = 0;
        pixel[BLUE] = C;
    }
    else if(H_prime >5 && H_prime <= 6){
        pixel[RED] = C;
        pixel[GREEN] = 0;
        pixel[BLUE] = X;
    }
    
    final_pixel[RED] = round((pixel[RED]+m)*255);
    final_pixel[GREEN] = round((pixel[GREEN]+m)*255);
    final_pixel[BLUE] = round((pixel[BLUE]+m)*255);

    return final_pixel;
}
