cmake_minimum_required(VERSION 2.6)

PROJECT(SEGMENTATION)

# Find ITK.
FIND_PACKAGE(ITK REQUIRED)

FIND_LIBRARY(YAML_LIBRARY
  NAMES libyaml-cpp.dylib libyaml-cpp.so
  PATHS /usr/local/lib $ENV{HOME}/lib
  )

# MESSAGE( STATUS "HOME: $ENV{HOME}" )
  
INCLUDE(${ITK_USE_FILE})
# INCLUDE( CMakePrintSystemInformation )

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR} /usr/local/include/yaml-cpp $ENV{HOME}/include/yaml-cpp)

ADD_EXECUTABLE(Threshold Threshold.cxx)
TARGET_LINK_LIBRARIES(Threshold ITKCommon ITKBasicFilters ITKIO ITKNumerics ${YAML_LIBRARY})

ADD_EXECUTABLE(VoxWriter VoxWriter.cxx)
TARGET_LINK_LIBRARIES(VoxWriter)

# GET_TARGET_PROPERTY(LINK_FLAGS REGISTER LINK_FLAGS)
