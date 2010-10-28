#include <iostream>
#include <fstream>

using namespace std;

void loadImage(char* file, char* &img)
{
	ifstream::pos_type size;
	ifstream image_file(file, ios::in|ios::binary|ios::ate);

	if (image_file.is_open())
	{
		cout << "Reading " << file << "..." << flush;

		size = image_file.tellg();
		img = new char [size];
		image_file.seekg (0, ios::beg);
		image_file.read (img, size);
		image_file.close();

		cout << "Image file read into memory!" << endl;
	}
	else
	{
		cerr << "Can't Open " << file << "!" << endl;
	}
}


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

