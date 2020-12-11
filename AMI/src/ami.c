#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "bitmap.h"
#include "annotate.h"
#include "conversion.h"

/* Function prototype */
void help();
int *getBoundary(int a, int b, char **argument);
int main(int argc, char *argv[]);

/* Main function */
int main(int argc, char *argv[]){
    Bmp img, img1, img2;
    char annotation[6], *name;
    int option, number_of_flag, *boundary, xmin, ymin, xmax, ymax, count;
    unsigned char *pointer_to_each_pixels, *RGB, *pointer_to_alphabet_pixels;
    double radius, degree, saturation_factor, brightness_factor, red, green, blue, *HSV;
    
    /* Getting input */
    while((option = getopt(argc, argv, "higb:H:s:v:a:")) != -1){
        switch(option){

            /* Help option */
            case 'h':
                help();
                return 0;

            /* Invert image */
            case 'i':

                /* Input image */
                img = read_bmp(argv[optind]);
                img1 = copy_bmp(img);

                /* Given boundary */
                if(argc - optind != 2){
                    boundary = getBoundary(optind, argc, argv);
                    xmin = boundary[3];
                    ymin = boundary[2];
                    xmax = boundary[1];
                    ymax = boundary[0];

                    for(int i=ymin; i<ymax; i++){
                        for(int j=xmin; j<xmax; j++){
                            for(int k=0; k<3; k++){
                                img1.pixels[i][j][k] = 255 - img1.pixels[i][j][k];
                            }
                        }
                    }
                }

                /* Without given boundary */
                else{
                    for(int i=0; i<img1.height; i++){
                        for(int j=0; j<img1.width; j++){
                            for(int k=0; k<3; k++){
                                img1.pixels[i][j][k] = 255 - img1.pixels[i][j][k];
                            }
                        }
                    }
                }

                /* Output image */
                name = argv[optind+1];
                write_bmp(img1, name);
                free_bmp(img1);
                break;

            /* Greyscale */
            case 'g':

                /* Input image */
                img = read_bmp(argv[optind]);
                img1 = copy_bmp(img);

                /* Given boundary */
                if(argc - optind != 2){
                    boundary = getBoundary(optind, argc, argv);
                    xmin = boundary[3];
                    ymin = boundary[2];
                    xmax = boundary[1];
                    ymax = boundary[0];

                    for(int i=ymin; i<ymax; i++){
                        for(int j=xmin; j<xmax; j++){
                            int average = 0;
                            double sum = 0;
                            
                            /* Get average of each pixel */
                            for(int k=0; k<3; k++){
                                sum = sum + img1.pixels[i][j][k];
                            }
                            average = floor(sum/3);

                            for(int k=0; k<3; k++){
                               img1.pixels[i][j][k] = average; 
                            }
                        }
                    }
                }

                /* Without given boundary */
                else{
                    for(int i=0; i<img1.height; i++){
                        for(int j=0; j<img1.width; j++){
                            int average = 0;
                            double sum = 0;

                            /* Get average of each pixel */
                            for(int k=0; k<3; k++){
                                sum = sum + img1.pixels[i][j][k];
                            }
                            average = floor(sum/3);

                            for(int k=0; k<3; k++){
                               img1.pixels[i][j][k] = average; 
                            }
                        }
                    }
                }

                /* Output image */
                name = argv[optind+1];
                write_bmp(img1, name);
                free_bmp(img1);
                break;

            case 'b':
                radius = atof(optarg);

                /* Input image */
                img = read_bmp(argv[optind]);
                img1 = copy_bmp(img);

                /* Given boundary */
                if(argc - optind != 2){
                    boundary = getBoundary(optind, argc, argv);
                    xmin = boundary[3];
                    ymin = boundary[2];
                    xmax = boundary[1];
                    ymax = boundary[0];

                    for(int i=ymin; i<ymax; i++){
                        for(int j=xmin; j<xmax; j++){
                            pointer_to_each_pixels = img1.pixels[i][j];

                            /* Initialise red, green, blue, count */
                            red = 0;
                            green = 0;
                            blue = 0;
                            count = 0;

                            /* Average each component for all the pixels in radius */
                            for(int x=i-radius; x<=i+radius; x++){
                                if(x<ymin || x>=ymax){
                                    continue;
                                }
                                for(int y=j-radius; y<=j+radius; y++){
                                    if(y<xmin || y>=xmax){
                                        continue;
                                    }
                                    red += img.pixels[x][y][RED];
                                    green += img.pixels[x][y][GREEN];
                                    blue += img.pixels[x][y][BLUE];
                                    count++;
                                }
                            }
                            pointer_to_each_pixels[RED] = red/count;
                            pointer_to_each_pixels[GREEN] = green/count;
                            pointer_to_each_pixels[BLUE] = blue/count;
                        }
                    }
                }

                /* Without given boundary */
                else{
                    for(int i=0; i<img1.height; i++){
                        for(int j=0; j<img1.width; j++){
                            pointer_to_each_pixels = img1.pixels[i][j];

                            /* Given boundary */
                            red = 0;
                            green = 0;
                            blue = 0;
                            count = 0;

                            /* Average each component for all the pixels in radius */
                            for(int x=i-radius; x<=i+radius; x++){
                                if(x<0 || x>=img1.height){
                                    continue;
                                }
                                for(int y=j-radius; y<=j+radius; y++){
                                    if(y<0 || y>=img1.width){
                                        continue;
                                    }
                                    red += img.pixels[x][y][RED];
                                    green += img.pixels[x][y][GREEN];
                                    blue += img.pixels[x][y][BLUE];
                                    count++;
                                }
                            }
                            pointer_to_each_pixels[RED] = red/count;
                            pointer_to_each_pixels[GREEN] = green/count;
                            pointer_to_each_pixels[BLUE] = blue/count;
                        }
                    }
                }

                /* Output image */
                name = argv[optind+1];
                write_bmp(img1, name);
                free_bmp(img1);
                break;

            /* Hue */    
            case 'H':
                degree = atof(optarg);

                /* Input image */
                img = read_bmp(argv[optind]);
                img1 = copy_bmp(img);
                
                /* Given boundary */
                if(argc - optind != 2){
                    boundary = getBoundary(optind, argc, argv);
                    xmin = boundary[3];
                    ymin = boundary[2];
                    xmax = boundary[1];
                    ymax = boundary[0];

                    for(int i=ymin; i<ymax; i++){
                        for(int j=xmin; j<xmax; j++){
                            pointer_to_each_pixels = img1.pixels[i][j];

                            /* Convert RGB to HSv, and get the values of hue, saturation and brightness */
                            HSV = RGBtoHSV(pointer_to_each_pixels[RED], pointer_to_each_pixels[GREEN], pointer_to_each_pixels[BLUE]);

                            /* Check whether H is within the range of 0 and 360, 
                                whether S, V are within the range of 0 and 1 */
                            if(HSV[GREEN]>1){
                                HSV[GREEN] = 1;
                            }
                            else if(HSV[GREEN]<0){
                                HSV[GREEN] = 0;
                            }
                            else if(HSV[BLUE]>1){
                                HSV[BLUE] = 1;
                            }
                            else if(HSV[BLUE]<0){
                                HSV[BLUE] = 0;
                            }
                            else if(HSV[RED]<0){
                                HSV[RED] = 0;
                            }
                            else if(HSV[RED]>360){
                                HSV[RED] = 360;
                            }

                            /* Roate the hue by <degree> degree */
                            if(HSV[RED]>=0 && HSV[RED]<=360){
                                HSV[RED] = HSV[RED]+degree;
                                if(HSV[RED]>360){
                                    HSV[RED] = HSV[RED] - 360;
                                }
                            }

                            /* Convert HSV to RGB, and the new pixels */
                            RGB = HSVtoRGB(HSV[RED], HSV[GREEN], HSV[BLUE]);
                            pointer_to_each_pixels[RED] = RGB[RED];
                            pointer_to_each_pixels[GREEN] = RGB[GREEN];
                            pointer_to_each_pixels[BLUE] = RGB[BLUE];
                        }
                    }
                }

                /* Without given boundary */
                else{
                    for(int i=0; i<img1.height; i++){
                        for(int j=0; j<img1.width; j++){
                            pointer_to_each_pixels = img1.pixels[i][j];

                            /* Convert RGB to HSv, and get the values of hue, saturation and brightness */
                            HSV = RGBtoHSV(pointer_to_each_pixels[RED], pointer_to_each_pixels[GREEN], pointer_to_each_pixels[BLUE]);

                            /* Check whether H is within the range of 0 and 360, 
                                whether S, V are within the range of 0 and 1 */
                            if(HSV[GREEN]>1){
                                HSV[GREEN] = 1;
                            }
                            else if(HSV[GREEN]<0){
                                HSV[GREEN] = 0;
                            }
                            else if(HSV[BLUE]>1){
                                HSV[BLUE] = 1;
                            }
                            else if(HSV[BLUE]<0){
                                HSV[BLUE] = 0;
                            }
                            else if(HSV[RED]<0){
                                HSV[RED] = 0;
                            }
                            else if(HSV[RED]>360){
                                HSV[RED] = 360;
                            }

                            /* Roate the hue by <degree> degree */
                            if(HSV[RED]>=0 && HSV[RED]<=360){
                                HSV[RED] = HSV[RED]+degree;
                                if(HSV[RED]>360){
                                    HSV[RED] = HSV[RED] - 360;
                                }
                            }

                            /* Convert HSV to RGB, and the new pixels */
                            RGB = HSVtoRGB(HSV[RED], HSV[GREEN], HSV[BLUE]);
                            pointer_to_each_pixels[RED] = RGB[RED];
                            pointer_to_each_pixels[GREEN] = RGB[GREEN];
                            pointer_to_each_pixels[BLUE] = RGB[BLUE];
                        }
                    }
                }

                /* Output image */
                name = argv[optind+1];
                write_bmp(img1, name);
                free_bmp(img1);
                break;

            /* Saturation */
            case 's':
                saturation_factor = atof(optarg);

                /* Input image */
                img = read_bmp(argv[optind]);
                img1 = copy_bmp(img);

                /* Given boundary */
                if(argc - optind != 2){
                    boundary = getBoundary(optind, argc, argv);
                    xmin = boundary[3];
                    ymin = boundary[2];
                    xmax = boundary[1];
                    ymax = boundary[0];

                    for(int i=ymin; i<ymax; i++){
                        for(int j=xmin; j<xmax; j++){
                            pointer_to_each_pixels = img1.pixels[i][j];

                            /* Convert RGB to HSv, and get the values of hue, saturation and brightness */
                            HSV = RGBtoHSV(pointer_to_each_pixels[RED], pointer_to_each_pixels[GREEN], pointer_to_each_pixels[BLUE]);

                            /* saturation value increases or decreases by saturation_factor */
                            HSV[GREEN] = HSV[GREEN]*(1+saturation_factor);

                            /* Check whether H is within the range of 0 and 360, 
                                whether S, V are within the range of 0 and 1 */
                            if(HSV[GREEN]>1){
                                HSV[GREEN] = 1;
                            }
                            else if(HSV[GREEN]<0){
                                HSV[GREEN] = 0;
                            }
                            else if(HSV[BLUE]>1){
                                HSV[BLUE] = 1;
                            }
                            else if(HSV[BLUE]<0){
                                HSV[BLUE] = 0;
                            }
                            else if(HSV[RED]<0){
                                HSV[RED] = 0;
                            }
                            else if(HSV[RED]>360){
                                HSV[RED] = 360;
                            }

                            /* Convert HSV to RGB, and the new pixels */
                            RGB = HSVtoRGB(HSV[RED], HSV[GREEN], HSV[BLUE]);
                            pointer_to_each_pixels[RED] = RGB[RED];
                            pointer_to_each_pixels[GREEN] = RGB[GREEN];
                            pointer_to_each_pixels[BLUE] = RGB[BLUE];
                        }
                    }
                }

                /* Without given boundary */
                else{
                    for(int i=0; i<img1.height; i++){
                        for(int j=0; j<img1.width; j++){
                            pointer_to_each_pixels = img1.pixels[i][j];

                            /* Convert RGB to HSv, and get the values of hue, saturation and brightness */
                            HSV = RGBtoHSV(pointer_to_each_pixels[RED], pointer_to_each_pixels[GREEN], pointer_to_each_pixels[BLUE]);

                            /* saturation value increases or decreases by saturation_factor */
                            HSV[GREEN] = HSV[GREEN]*(1+saturation_factor);

                            /* Check whether H is within the range of 0 and 360, 
                                whether S, V are within the range of 0 and 1 */
                            if(HSV[GREEN]>1){
                                HSV[GREEN] = 1;
                            }
                            else if(HSV[GREEN]<0){
                                HSV[GREEN] = 0;
                            }
                            else if(HSV[BLUE]>1){
                                HSV[BLUE] = 1;
                            }
                            else if(HSV[BLUE]<0){
                                HSV[BLUE] = 0;
                            }
                            else if(HSV[RED]<0){
                                HSV[RED] = 0;
                            }
                            else if(HSV[RED]>360){
                                HSV[RED] = 360;
                            }

                            /* Convert HSV to RGB, and the new pixels */
                            RGB = HSVtoRGB(HSV[RED], HSV[GREEN], HSV[BLUE]);
                            pointer_to_each_pixels[RED] = RGB[RED];
                            pointer_to_each_pixels[GREEN] = RGB[GREEN];
                            pointer_to_each_pixels[BLUE] = RGB[BLUE];
                        }
                    }
                }

                /* Output image */
                name = argv[optind+1];
                write_bmp(img1, name);
                free_bmp(img1);
                break;

            case 'v':
                brightness_factor = atof(optarg);

                /* Input image */
                img = read_bmp(argv[optind]);
                img1 = copy_bmp(img);

                /* Given boundary */
                if(argc - optind != 2){
                    boundary = getBoundary(optind, argc, argv);
                    xmin = boundary[3];
                    ymin = boundary[2];
                    xmax = boundary[1];
                    ymax = boundary[0];

                    for(int i=ymin; i<ymax; i++){
                        for(int j=xmin; j<xmax; j++){
                            pointer_to_each_pixels = img1.pixels[i][j];

                            /* Convert RGB to HSv, and get the values of hue, saturation and brightness */
                            HSV = RGBtoHSV(pointer_to_each_pixels[RED], pointer_to_each_pixels[GREEN], pointer_to_each_pixels[BLUE]);

                            /* Brightness value increases or decreases by brightness_factor */
                            HSV[BLUE] = HSV[BLUE]*(1+brightness_factor);

                            /* Check whether H is within the range of 0 and 360, 
                                whether S, V are within the range of 0 and 1 */
                            if(HSV[GREEN]>1){
                                HSV[GREEN] = 1;
                            }
                            else if(HSV[GREEN]<0){
                                HSV[GREEN] = 0;
                            }
                            else if(HSV[BLUE]>1){
                                HSV[BLUE] = 1;
                            }
                            else if(HSV[BLUE]<0){
                                HSV[BLUE] = 0;
                            }
                            else if(HSV[RED]<0){
                                HSV[RED] = 0;
                            }
                            else if(HSV[RED]>360){
                                HSV[RED] = 360;
                            }

                            /* Convert HSV to RGB, and the new pixels */
                            RGB = HSVtoRGB(HSV[RED], HSV[GREEN], HSV[BLUE]);
                            pointer_to_each_pixels[RED] = RGB[RED];
                            pointer_to_each_pixels[GREEN] = RGB[GREEN];
                            pointer_to_each_pixels[BLUE] = RGB[BLUE];
                            }
                        }
                }

                /* Without given boundary */
                else{
                    for(int i=0; i<img1.height; i++){
                        for(int j=0; j<img1.width; j++){
                            pointer_to_each_pixels = img1.pixels[i][j];

                            /* Convert RGB to HSv, and get the values of hue, saturation and brightness */
                            HSV = RGBtoHSV(pointer_to_each_pixels[RED], pointer_to_each_pixels[GREEN], pointer_to_each_pixels[BLUE]);

                            /* Brightness value increases or decreases by brightness_factor */
                            HSV[BLUE] = HSV[BLUE]*(1+brightness_factor);

                            /* Check whether H is within the range of 0 and 360, 
                                whether S, V are within the range of 0 and 1 */
                            if(HSV[GREEN]>1){
                                HSV[GREEN] = 1;
                            }
                            else if(HSV[GREEN]<0){
                                HSV[GREEN] = 0;
                            }
                            else if(HSV[BLUE]>1){
                                HSV[BLUE] = 1;
                            }
                            else if(HSV[BLUE]<0){
                                HSV[BLUE] = 0;
                            }
                            else if(HSV[RED]<0){
                                HSV[RED] = 0;
                            }
                            else if(HSV[RED]>360){
                                HSV[RED] = 360;
                            }

                            /* Convert HSV to RGB, and the new pixels */
                            RGB = HSVtoRGB(HSV[RED], HSV[GREEN], HSV[BLUE]);
                            pointer_to_each_pixels[RED] = RGB[RED];
                            pointer_to_each_pixels[GREEN] = RGB[GREEN];
                            pointer_to_each_pixels[BLUE] = RGB[BLUE];
                            }
                        }
                    }

                /* Output image */
                name = argv[optind+1];
                write_bmp(img1, name);
                free_bmp(img1);
                break;

            /* Annotation */
            case 'a':
                strcpy(annotation, optarg);

                /* Input image */
                img = read_bmp(argv[optind]);
                img1 = copy_bmp(img);
                img2 = read_bmp("alphabet.bmp");

                /* Pass two images to annotation function */
                Annotation(img1, img2, annotation);

                /* Output image */
                name = argv[optind+1];
                write_bmp(img1, name);
                free_bmp(img1);
                free_bmp(img2);
                break;

            /* Invalid argument */
            case '?':
                printf("Invalid flag: %c.\n", optopt);
                return 0;
        }
    }
    
    number_of_flag = optind;
    if(number_of_flag == 1){

        /* Single argument */
        if((argc - number_of_flag) == 1){
            img = read_bmp(argv[optind]);

            /* Output the height and width of the image */
            printf("Width: %u\n", img.width);
            printf("Height: %u\n", img.height);
            free_bmp(img);
            return 0;
        }

        /* No flags */
        else if((argc - number_of_flag) == 2){
            img = read_bmp(argv[optind]);
            img1 = copy_bmp(img);

            /* Output the input image */
            name = argv[number_of_flag+1];
            write_bmp(img1, name);
            free_bmp(img1);
            return 0;
        }
    }
    return 0;
}

/* Function */
void help(){
    printf("Usage: ./ami [options ...] <input-file> <output-file> [<xmin> <ymin> <xmax> <ymax>]\n");
    printf("\n");
    printf("  -i       Invert the image\n");
    printf("  -g       Apply a grayscale filter to the image\n");
    printf("  -b <r>   Apply a box blur on the image with radius <r>\n");
    printf("  -H <d>   Rotate the hue of the image by <d> degrees\n");
    printf("  -s <p>   Modify the saturation of the image by a factor of <p>\n");
    printf("  -v <b>   Modify the brightness of the image by a factor of <b>\n");
    printf("  -a \"<s>\" Annotate the image with a 6 letter string <s>\n");
}

int *getBoundary(int a, int b, char **argument){
    static int r[4] = {};
    for(; a < b; a++){
        if(isalpha(*argument[a]) == 0){
            r[b-a-1] = atoi(argument[a]);
        }    
    } 
    return r;
}
