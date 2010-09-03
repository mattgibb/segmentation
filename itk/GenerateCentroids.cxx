#include <iostream>
#include <fstream>
#include <vector>
#include <assert.h>

// My files
#include "Dirs.hpp"

using namespace std;

void checkUsage(int argc, char *argv[]) {
	if (argc < 2)
	{
		cerr << "Missing Parameters " << endl;
    cerr << "Usage: " << argv[0] << " ";
    cerr << "dataSet " << endl;
		exit(EXIT_FAILURE);
	}
}

int main( int argc, char *argv[] ) {
  checkUsage(argc, argv);
  
  Dirs::SetDataSet(argv[1]);
  
  // initialise file names
  const string carpBasename( Dirs::ResultsDir() + "mesh" );
  
  const string ptsFilename      ( carpBasename + ".pts"       );
  const string elemFilename     ( carpBasename + ".elem"      );
  const string centroidsFilename( carpBasename + ".centroids" );
  
  
  // read points
  cout << "Reading points..." << flush;
  
  string dummy;
  ifstream ptsFile( ptsFilename.c_str() );
  vector< vector< double > > points;
  vector< double > point(3, 0.0);
  
  getline(ptsFile, dummy); // skip first line
  while( !ptsFile.eof() ) {
    // read point
    ptsFile >> point[0] >> point[1] >> point[2];
    
    // append to points
    points.push_back( point );
  }
  
  ptsFile.close();
  
  cout << "done." << endl;
  
  // calculate and write centroids
  cout << "Writing centroids..." << flush;
  
  ifstream elemFile( elemFilename.c_str() );
  ofstream centroidsFile( centroidsFilename.c_str() );
  vector< unsigned int > indeces(4, 0);
  unsigned int numberOfElements;
  
  elemFile >> numberOfElements; // read number of elements
  centroidsFile << numberOfElements << endl; // write it to centroids file
  
  while( !elemFile.eof() ) {
   // read tetrahedron vertex indeces
   // skip 'Tt' and ignore last empty line of file
    elemFile >> dummy;
    if ( !dummy.compare("Tt") ) {
      elemFile >> indeces[0] >> indeces[1] >> indeces[2] >> indeces[3];
      elemFile >> dummy; // skip last zone
    
      // find mean of all 4 vertices
      vector< double > centroid(3, 0.0);
      for(unsigned int dim=0; dim<3; dim++) {
        for(vector< unsigned int >::iterator point_index=indeces.begin(); point_index!=indeces.end(); point_index++) {
          centroid[dim] += points[*point_index][dim];
        }
        centroid[dim] /= 4;
      }
    
      // print result to file
      centroidsFile << centroid[0] << " " << centroid[1] << " " << centroid[2] << endl;
    }
  }
  
  cout << "done." << endl;
  
  elemFile.close();
  centroidsFile.close();
}

