#ifndef _CONVERSION_H
#define _CONVERSION_H

double *RGBtoHSV(int red, int green, int blue);
unsigned char *HSVtoRGB(double hue, double saturation, double value);
double MAX(double r, double g, double b);
double MIN(double r, double g, double b);

#endif
