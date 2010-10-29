cmake_minimum_required(VERSION 2.6)

PROJECT(SEGMENTATION)

# Find Boost and add include dirs
FIND_PACKAGE(Boost COMPONENTS program_options REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})

# Find ITK.
FIND_PACKAGE(ITK REQUIRED)

FIND_LIBRARY(YAML_LIBRARY
  NAMES libyaml-cpp.dylib libyaml-cpp.so
  PATHS /usr/local/lib $ENV{HOME}/lib
  )

INCLUDE(${ITK_USE_FILE})
# INCLUDE( CMakePrintSystemInformation )

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR} /usr/local/include/yaml-cpp $ENV{HOME}/include/yaml-cpp)

ADD_EXECUTABLE(Threshold Threshold.cxx)
TARGET_LINK_LIBRARIES(Threshold ITKCommon ITKBasicFilters ITKIO ITKNumerics ${YAML_LIBRARY})

ADD_EXECUTABLE(VoxWriter VoxWriter.cxx)
TARGET_LINK_LIBRARIES(VoxWriter)

ADD_EXECUTABLE(ExtractVectors ExtractVectors.cxx)
TARGET_LINK_LIBRARIES(ExtractVectors ITKCommon ITKBasicFilters ITKIO)

ADD_EXECUTABLE(GenerateCentroids GenerateCentroids.cxx)
TARGET_LINK_LIBRARIES(GenerateCentroids)

ADD_EXECUTABLE(ConvertImageFormat ConvertImageFormat.cxx)
TARGET_LINK_LIBRARIES(ConvertImageFormat ${Boost_LIBRARIES} ITKCommon ITKIO)
