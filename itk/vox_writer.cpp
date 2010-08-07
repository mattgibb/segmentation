////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
// Martin J Bishop 
// Computational Biology Group, University of Oxford 
// 15th September 2008
// 
// Calculates radial vector from each element to surface
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include <algorithm>
#include <list>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include "imaging.h"

using namespace std;

int main( int argc, char *argv[] )
{

	if (argc < 8)
	{
		std::cerr << "Missing Parameters " << std::endl;
		std::cerr << "Usage: " << argv[0] << std::endl;
		std::cerr << " Image Filename " << std::endl;
		std::cerr << " x_dim y_dim z_dim " << std::endl;
		std::cerr << " x_res y_res z_res " << std::endl;
		std::cerr << " Output Filename " << std::endl;
		return 1;
	}

	// Reads-in the binary image
	char* img = NULL;

	loadImage(argv[1],img);
	
	int t;
	
	// Defines image dimensions
	int sizex = atoi(argv[2]);
	int sizey = atoi(argv[3]);
	int sizez = atoi(argv[4]);

	// Defines physical resolution of image
	double x_res = atof(argv[5]);
	double y_res = atof(argv[6]);
	double z_res = atof(argv[7]);	

	cout << "SIZE = " << sizex*sizey*sizez << "\n";

	// Write-out data to vox file
	ofstream output(argv[8]);
	output << sizex << " " << sizey << " " << sizez << "\n";
	output << x_res << " " << y_res << " " << z_res << "\n";
	for(int i=0;i<sizex*sizey*sizez;i++)
	{
		//cout << i << "\n";
		t = img[i];
		output << t << "\n"; 
	}

	output.close();

	delete [] img;

	return 0;

}

