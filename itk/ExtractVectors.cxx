#include <iostream>
#include <fstream>
#include <sstream>
#include <assert.h>
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkLinearInterpolateImageFunction.h"

// My files
#include "Dirs.hpp"

using namespace std;

void checkUsage(int argc, char *argv[]) {
	if (argc < 5)
	{
		std::cerr << "Missing Parameters " << std::endl;
    std::cerr << "Usage: " << argv[0] << " ";
    std::cerr << "dataSet ";
    std::cerr << "vectorImageBasename ";
		std::cerr << "centroidsFilename ";
		std::cerr << "lonFilename ";
		exit(EXIT_FAILURE);
	}
}

int main( int argc, char *argv[] ) {
  checkUsage(argc, argv);
  
  Dirs::SetDataSet(argv[1]);
  
  // name inputs
  const string vectorImageBasename( Dirs::DTMRIDir() + argv[2] ),
               centroidsFilename( Dirs::ResultsDir() + argv[3] ),
               lonFilename( Dirs::ResultsDir() + argv[4] );
  
  // Open files for reading and writing
  // Open vector images
  typedef unsigned char PixelType;
  typedef itk::Image< PixelType, 3 > VolumeType;
  typedef itk::ImageFileReader< VolumeType > ReaderType;
  
  ReaderType::Pointer xReader = ReaderType::New();
  ReaderType::Pointer yReader = ReaderType::New();
  ReaderType::Pointer zReader = ReaderType::New();
  
  xReader->SetFileName( vectorImageBasename + "1.mhd" );    
  yReader->SetFileName( vectorImageBasename + "2.mhd" );    
  zReader->SetFileName( vectorImageBasename + "3.mhd" );    
  
  xReader->Update();
  yReader->Update();
  zReader->Update();
  
  // open mesh.centroids
  ifstream centroidsFile( centroidsFilename.c_str() );
  
  // open mesh.lon
  ofstream lonFile( lonFilename.c_str() );
  
  // skip first line of centroidsFile and
  // write number of elements to lonFile
  string numberOfElements;
  centroidsFile >> numberOfElements;
  lonFile       << numberOfElements << endl;
  
  // configure interpolators
  typedef itk::LinearInterpolateImageFunction< VolumeType > InterpolatorType;
  InterpolatorType::Pointer xInterpolator = InterpolatorType::New();
  InterpolatorType::Pointer yInterpolator = InterpolatorType::New();
  InterpolatorType::Pointer zInterpolator = InterpolatorType::New();
  
  xInterpolator->SetInputImage( xReader->GetOutput() );
  yInterpolator->SetInputImage( yReader->GetOutput() );
  zInterpolator->SetInputImage( zReader->GetOutput() );
  
  // for all points in mesh.centroids
  while ( !centroidsFile.eof() ) {
    VolumeType::PointType centroid;
    
    // read point
    centroidsFile >> centroid[0] >> centroid[1] >> centroid[2];
    
    // check image bounds
    assert( xInterpolator->IsInsideBuffer(centroid) );
    
    // write intensities to lon file
    lonFile << xInterpolator->Evaluate( centroid ) << " "
            << yInterpolator->Evaluate( centroid ) << " "
            << zInterpolator->Evaluate( centroid ) << endl;
  }
  
  // close mesh.centroids and mesh.lon
  centroidsFile.close();
  lonFile.close();
  
}

