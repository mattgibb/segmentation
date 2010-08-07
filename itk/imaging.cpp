#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include <algorithm>
#include <list>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>

using namespace std;

void loadImage(char* file, char* &img)
{
	ifstream::pos_type size;
	ifstream image_file(file, ios::in|ios::binary|ios::ate);

	if (image_file.is_open())
	{
		cout << "Reading " << file << " " << flush;

		size = image_file.tellg();
		img = new char [size];
		image_file.seekg (0, ios::beg);
		image_file.read (img, size);
		image_file.close();

		cout << "Image file read into memory!" << endl << flush;
	}
	else
	{
		cerr << "Can't Open " << file << "!" << endl << flush;
	}
}

void loadMhaImageFile(char* file, double &xres, double &yres, double &zres, int &sizex, int &sizey, int &sizez)
{
	ifstream mha_file(file);

	if (mha_file.is_open())
	{
		cout << "Reading " << file << " " << flush << endl;

		string dummy;
		mha_file >> dummy >> dummy >> dummy >> dummy >> dummy;
		mha_file >> sizex >> sizey >> sizez;
		cout << "Image dimension: " << sizex << " " << sizey << " " << sizez << endl;
		mha_file >> dummy >> dummy >> dummy >> dummy >> dummy;
		mha_file >> xres >> yres >> zres;
		cout << "Image resolution: " << xres << " " << yres << " " << zres << endl;
		
	}
	else
	{
		cerr << "Can't Open " << file << "!" << endl << flush;
	}
}