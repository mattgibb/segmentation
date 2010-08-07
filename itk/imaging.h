#ifndef _IMAGING_H_
#define _IMAGING_H_

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

void loadImage(char* file, char* &img);

void loadMhaImageFile(char* file, double &xres, double &yres, double &zres, int &sizex, int &sizey, int &sizez);


#endif
