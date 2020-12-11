#include <stdio.h>
#include "bitmap.h"
#include "annotate.h"

Bmp Annotation(Bmp img1, Bmp img2, char *annotation){
    unsigned char *pointer_to_each_pixels, *pointer_to_alphabet_pixels;
    int xmin, xmax;

    for(int k=0; k<6; k++){
        switch(annotation[k]){
            case 'a':
                xmin = 0;
                xmax = 10;
                break;
            case 'b':
                xmin = 10;
                xmax = 20;
                break;
            case 'c':
                xmin = 20;
                xmax = 30;
                break;
            case 'd':
                xmin = 30;
                xmax = 40;
                break;
            case 'e':
                xmin = 40;
                xmax = 50;
                break;
            case 'f':
                xmin = 50;
                xmax = 60;
                break;
            case 'g':
                xmin = 60;
                xmax = 70;
                break;
            case 'h':
                xmin = 70;
                xmax = 80;
                break;
            case 'i':
                xmin = 80;
                xmax = 90;
                break;
            case 'j':
                xmin = 90;
                xmax = 100;
                break;
            case 'k':
                xmin = 100;
                xmax = 110;
                break;
            case 'l':
                xmin = 110;
                xmax = 120;
                break;
            case 'm':
                xmin = 120;
                xmax = 130;
                break;
            case 'n':
                xmin = 130;
                xmax = 140;
                break;
            case 'o':
                xmin = 140;
                xmax = 150;
                break;
            case 'p':
                xmin = 150;
                xmax = 160;
                break;
            case 'q':
                xmin = 160;
                xmax = 170;
                break;
            case 'r':
                xmin = 170;
                xmax = 180;
                break;
            case 's':
                xmin = 180;
                xmax = 190;
                break;
            case 't':
                xmin = 190;
                xmax = 200;
                break;
            case 'u':
                xmin = 200;
                xmax = 210;
                break;
            case 'v':
                xmin = 210;
                xmax = 220;
                break;
            case 'w':
                xmin = 220;
                xmax = 230;
                break;
            case 'x':
                xmin = 230;
                xmax = 240;
                break;
            case 'y':
                xmin = 240;
                xmax = 250;
                break;
            case 'z':
                xmin = 250;
                xmax = 260;
                break;
            case ' ':
                xmin = 260;
                xmax = 270;
                break;
        }

        for(int x=0; x<20; x++){
            for(int y=xmin, z=k*10; y<xmax && z<k*10+10; y++, z++){
                pointer_to_each_pixels = img1.pixels[x][z];
                pointer_to_alphabet_pixels = img2.pixels[x][y];
                pointer_to_each_pixels[RED] = pointer_to_alphabet_pixels[RED];
                pointer_to_each_pixels[GREEN] = pointer_to_alphabet_pixels[GREEN];
                pointer_to_each_pixels[BLUE] = pointer_to_alphabet_pixels[BLUE];
            }
        }
    }
}
