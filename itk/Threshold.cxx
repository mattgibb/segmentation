///////////////////////////////////////////////////////////////////
// Martin Bishop
// 18th December 2007
// Computational Biology Group, University of Oxford
//
// Threshold Level Set segmentation code for the bunny MRI data
///////////////////////////////////////////////////////////////////
// #if defined(_MSC_VER)
// #pragma warning ( disable : 4786 )
// #endif

// #ifdef __BORLANDC__
// #define ITK_LEAN_AND_MEAN
// #endif



// Defines numerous headers which are required
#include "itkImage.h"
#include "itkThresholdSegmentationLevelSetImageFilter.h"
#include "itkFastMarchingImageFilter.h"
#include "itkBinaryThresholdImageFilter.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkZeroCrossingImageFilter.h"

// Defines extra C++ bits which are needed
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <iostream>
#include <time.h>

using namespace std;
using namespace itk;

int main( int argc, char *argv[] )
{
  if( argc < 7 )
    {
    std::cerr << "Missing Parameters " << std::endl;
    std::cerr << "Usage: " << argv[0];
    std::cerr << " inputImage  outputImage";
    std::cerr << " #seeds InitialDistance";
    std::cerr << " LowerThreshold";
    std::cerr << " UpperThreshold";
    std::cerr << " [CurvatureScaling == 1.0]";
    std::cerr << std::endl;
    return 1;

    }

  // Starts the clock
  clock_t start = clock();
  
  typedef   float           InternalPixelType;
  const     unsigned int    Dimension = 3;
  typedef itk::Image< InternalPixelType, Dimension >  InternalImageType;
  

  typedef unsigned char OutputPixelType;
  typedef itk::Image< OutputPixelType, Dimension > OutputImageType;
  typedef itk::BinaryThresholdImageFilter<InternalImageType, OutputImageType>
    ThresholdingFilterType;
  
  ThresholdingFilterType::Pointer thresholder = ThresholdingFilterType::New();
                        
  thresholder->SetLowerThreshold( -1000.0 );
  thresholder->SetUpperThreshold(     0.0 );

  thresholder->SetOutsideValue(  0  );
  thresholder->SetInsideValue(  255 );

  typedef  itk::ImageFileReader< InternalImageType > ReaderType;
  typedef  itk::ImageFileWriter<  OutputImageType  > WriterType;

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  reader->SetFileName( argv[1] );
  writer->SetFileName( argv[2] );


  typedef  itk::FastMarchingImageFilter< InternalImageType, InternalImageType >
    FastMarchingFilterType;

  FastMarchingFilterType::Pointer  fastMarching = FastMarchingFilterType::New();
  
  typedef  itk::ThresholdSegmentationLevelSetImageFilter< InternalImageType, 
    InternalImageType > ThresholdSegmentationLevelSetImageFilterType;
  ThresholdSegmentationLevelSetImageFilterType::Pointer thresholdSegmentation =
    ThresholdSegmentationLevelSetImageFilterType::New();
  
  // Looks for an inputted value of the curvature; if none found, then sets default to 1
  thresholdSegmentation->SetPropagationScaling( 1.0 );
  if ( argc > 7 )
    {
    thresholdSegmentation->SetCurvatureScaling( atof(argv[7]) );
    }
  else
    {
    thresholdSegmentation->SetCurvatureScaling( 1.0 );
    }
  
    thresholdSegmentation->SetMaximumRMSError( 0.02 );
    thresholdSegmentation->SetNumberOfIterations( 2000 );

  
    // Sets upper and lower threshold criterior
  thresholdSegmentation->SetUpperThreshold( ::atof(argv[6]) );
  thresholdSegmentation->SetLowerThreshold( ::atof(argv[5]) );
  thresholdSegmentation->SetIsoSurfaceValue(0.0);
  
  thresholdSegmentation->SetInput( fastMarching->GetOutput() );
  thresholdSegmentation->SetFeatureImage( reader->GetOutput() );
  thresholder->SetInput( thresholdSegmentation->GetOutput() );
  writer->SetInput( thresholder->GetOutput() );
  
  
  typedef FastMarchingFilterType::NodeContainer           NodeContainer;
  typedef FastMarchingFilterType::NodeType                NodeType;

  NodeContainer::Pointer seeds = NodeContainer::New();

  // defines the variables x,y,z as the coordinates of the seed points and n as the total number of seeds
  int n;
  int x,y,z;

  // reads-in the file containing the locations of the seedpoints
  std::ifstream input_seeds("positions_test.dat");

  // converts the string "#seeds" into a double
  n = atoi( argv[3] );

  // outputs to screen the #seeds
  std::cout << "Number of seed points = " << n << "\n";

  // makes a vector of seed positions of length n
  std::vector <   InternalImageType::IndexType  > seedPosition( n );

 // loops-over all seed positions adding them to the seedPosition vector
  for(unsigned int i=0; i< seedPosition.size(); i++)
    {
      input_seeds >> y >> x >> z;
      
      seedPosition[i][0] = x-1;
      seedPosition[i][1] = y-1;
      seedPosition[i][2] = z-1;
      
    }

  // InternalImageType::IndexType  seedPosition;
//   seedPosition[0] = atoi( argv[3] );
//   seedPosition[1] = atoi( argv[4] );


  const double initialDistance = atof( argv[4] );

 //  NodeType node;
//   const double seedValue = - initialDistance;
//   node.SetValue( seedValue );
//   node.SetIndex( seedPosition );

  NodeType node;
  const double seedValue = - initialDistance;
  seeds->Initialize();
  for(int i=0;i<n;i++)
    {
      node.SetValue( seedValue );
      node.SetIndex( seedPosition[i] );
      
      seeds->InsertElement( i, node );

    }

  // seeds->Initialize();
//   seeds->InsertElement( 0, node );

  // seeds->InsertElement( 0, node );
  std:: cout << "Hello, I am here \n";
  
  fastMarching->SetTrialPoints(  seeds  );
  
  fastMarching->SetSpeedConstant( 1.0 );

  try
    {
    reader->Update();
    fastMarching->SetOutputSize( 
      reader->GetOutput()->GetBufferedRegion().GetSize() );
    writer->Update();
    }
  catch( itk::ExceptionObject & excep )
    {
    std::cerr << "Exception caught !" << std::endl;
    std::cerr << excep << std::endl;
    }
 

  // Print out some useful information 
  std::cout << std::endl;
  std::cout << "Max. no. iterations: " << thresholdSegmentation->GetNumberOfIterations() << std::endl;
  std::cout << "Max. RMS error: " << thresholdSegmentation->GetMaximumRMSError() << std::endl;
  std::cout << std::endl;
  std::cout << "No. elpased iterations: " << thresholdSegmentation->GetElapsedIterations() << std::endl;
  std::cout << "RMS change: " << thresholdSegmentation->GetRMSChange() << std::endl;


  // We write out some intermediate images for debugging.  These images can
  // help tune parameters.
  //
  typedef itk::ImageFileWriter< InternalImageType > InternalWriterType;

  InternalWriterType::Pointer mapWriter = InternalWriterType::New();
  mapWriter->SetInput( fastMarching->GetOutput() );
  mapWriter->SetFileName("fastMarchingImage.mha");
  mapWriter->Update();

  InternalWriterType::Pointer speedWriter = InternalWriterType::New();
  speedWriter->SetInput( thresholdSegmentation->GetSpeedImage() );
  speedWriter->SetFileName("speedTermImage.mha");
  speedWriter->Update();

  clock_t ends = clock();

  cout << "Running Time : "

       << (double) (ends - start) / CLOCKS_PER_SEC << endl;


  return 0;
}



