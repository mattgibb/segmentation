// Defines numerous headers which are required
#include "itkImage.h"
#include "itkThresholdSegmentationLevelSetImageFilter.h"
#include "itkFastMarchingImageFilter.h"
#include "itkBinaryThresholdImageFilter.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkZeroCrossingImageFilter.h"
#include "itkImageRegionIteratorWithIndex.h"

// Defines extra C++ bits which are needed
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <iostream>
#include <time.h>

using namespace std;
using namespace itk;

int main( int argc, char *argv[] ) {
  if( argc < 7 ) {
    cerr << "Missing Parameters " << endl;
    cerr << "Usage: " << argv[0];
    cerr << " inputImage outputImage";
    cerr << " seedImage initialDistance";
    cerr << " lowerThreshold";
    cerr << " upperThreshold";
    cerr << " curvatureScaling";
    cerr << endl;
    return 1;
  }
  
  cout << "test\n";
  // name the inputs for clarity and interface flexibility
  // Change to GetPot
  const string inputImageName(argv[1]),
               outputImageName(argv[2]),
               seedImageName(argv[3]);
         
  const float initialDistance( atof( argv[4] ) ),
              lowerThreshold( atof(argv[5]) ),
              upperThreshold( atof(argv[6]) ),
              curvatureScaling( atof(argv[7]) );
  
  // Starts the clock
  clock_t start = clock();  
  
  typedef   float           InternalPixelType;
  const     unsigned int    Dimension = 3;
  typedef itk::Image< InternalPixelType, Dimension >  InternalImageType;
  
  typedef unsigned char OutputPixelType;
  typedef itk::Image< OutputPixelType, Dimension > OutputImageType;
  typedef OutputImageType SeedImageType;
  typedef itk::BinaryThresholdImageFilter<InternalImageType, OutputImageType> ThresholdingFilterType;
  
  ThresholdingFilterType::Pointer thresholder = ThresholdingFilterType::New();
  
  thresholder->SetLowerThreshold( -1000.0 );
  thresholder->SetUpperThreshold(     0.0 );
  
  thresholder->SetOutsideValue(  0  );
  thresholder->SetInsideValue(  255 );
  
  typedef  itk::ImageFileReader< InternalImageType > InputReaderType;
  typedef  itk::ImageFileReader< SeedImageType > SeedReaderType;
  typedef  itk::ImageFileWriter<  OutputImageType  > WriterType;
  
  InputReaderType::Pointer reader = InputReaderType::New();
  SeedReaderType::Pointer seedReader = SeedReaderType::New();
  WriterType::Pointer writer = WriterType::New();
  
  reader->SetFileName( inputImageName );
  seedReader->SetFileName( seedImageName );
  writer->SetFileName( outputImageName );
  
  typedef  itk::FastMarchingImageFilter< InternalImageType, InternalImageType > FastMarchingFilterType;
  FastMarchingFilterType::Pointer fastMarching = FastMarchingFilterType::New();
  
  typedef  itk::ThresholdSegmentationLevelSetImageFilter< InternalImageType, InternalImageType > ThresholdSegmentationLevelSetImageFilterType;
  ThresholdSegmentationLevelSetImageFilterType::Pointer thresholdSegmentation = ThresholdSegmentationLevelSetImageFilterType::New();
  
  thresholdSegmentation->SetPropagationScaling( 1.0 );
  thresholdSegmentation->SetCurvatureScaling( curvatureScaling );
  thresholdSegmentation->SetLowerThreshold( lowerThreshold );
  thresholdSegmentation->SetUpperThreshold( upperThreshold );
  thresholdSegmentation->SetIsoSurfaceValue(0.0);  
  thresholdSegmentation->SetMaximumRMSError( 0.02 );
  thresholdSegmentation->SetNumberOfIterations( 2000 );
  thresholdSegmentation->SetInput( fastMarching->GetOutput() );
  thresholdSegmentation->SetFeatureImage( reader->GetOutput() );
  thresholder->SetInput( thresholdSegmentation->GetOutput() );
  writer->SetInput( thresholder->GetOutput() );
  
  // makes a vector of seed positions
  typedef vector< InternalImageType::IndexType > SeedPositionsType;
  SeedPositionsType seedPositions;
  
  // creates an image iterator for the seed image
  try {
    seedReader->Update();
  }
  catch( itk::ExceptionObject & excep ) {
    std::cerr << "Exception caught !" << std::endl;
    std::cerr << excep << std::endl;
  }
  
  typedef itk::ImageRegionIteratorWithIndex< SeedImageType > IteratorType;
  IteratorType seedIt( seedReader->GetOutput(), seedReader->GetOutput()->GetLargestPossibleRegion() );
  
  // appends all pixels that aren't 0 to seedPositions
  for ( seedIt.GoToBegin(); !seedIt.IsAtEnd(); ++seedIt) {
    if ( seedIt.Get() ) {
      seedPositions.push_back( seedIt.GetIndex() );
    }
  }
  
  // construct nodes and node container from seedPositions
  typedef FastMarchingFilterType::NodeContainer NodeContainer;
  NodeContainer::Pointer seeds = NodeContainer::New();
  FastMarchingFilterType::NodeType node;
  const double seedValue = - initialDistance;
  node.SetValue( seedValue );
  
  for(unsigned int i=0; i<seedPositions.size(); i++) {
    node.SetIndex( seedPositions[i] );
    seeds->InsertElement( i, node );
  }
  
  fastMarching->SetTrialPoints( seeds );
  fastMarching->SetSpeedConstant( 1.0 );

  try {
    reader->Update();
    fastMarching->SetOutputSize( reader->GetOutput()->GetBufferedRegion().GetSize() );
    writer->Update();
  }
  catch( itk::ExceptionObject & excep ) {
    std::cerr << "Exception caught !" << std::endl;
    std::cerr << excep << std::endl;
  }
 
  
  // Print out some useful information 
  cout << endl;
  cout << "Max. no. iterations: " << thresholdSegmentation->GetNumberOfIterations() << endl;
  cout << "Max. RMS error: " << thresholdSegmentation->GetMaximumRMSError() << endl;
  cout << endl;
  cout << "No. elpased iterations: " << thresholdSegmentation->GetElapsedIterations() << endl;
  cout << "RMS change: " << thresholdSegmentation->GetRMSChange() << endl;
  
  
  // We write out some intermediate images for debugging.  These images can
  // help tune parameters.
  //
  typedef itk::ImageFileWriter< InternalImageType > InternalWriterType;
  
  InternalWriterType::Pointer mapWriter = InternalWriterType::New();
  mapWriter->SetInput( fastMarching->GetOutput() );
  mapWriter->SetFileName("fastMarchingImage.mhd");
  mapWriter->Update();

  InternalWriterType::Pointer speedWriter = InternalWriterType::New();
  speedWriter->SetInput( thresholdSegmentation->GetSpeedImage() );
  speedWriter->SetFileName("speedTermImage.mhd");
  speedWriter->Update();

  InternalWriterType::Pointer levelSetWriter = InternalWriterType::New();
  levelSetWriter->SetInput( thresholdSegmentation->GetOutput() );
  levelSetWriter->SetFileName("thresholdSegmentationImage.mhd");
  levelSetWriter->Update();

  clock_t ends = clock();

  cout << "Running Time : "
       << (double) (ends - start) / CLOCKS_PER_SEC << endl;


  return 0;
}
