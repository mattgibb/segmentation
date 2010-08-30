#ifndef _IMAGING_HPP_
#define _IMAGING_HPP_

using namespace std;

class Dirs {
  static string DataSet;
  
public:
  // makes sure DataSet has been set
  // before returning dependent path strings
  static void CheckDataSet() {
    if ( DataSet.empty()) {
      cerr << "Dirs::dataSet not set!\n";
      exit(1);
    }
  }
  
  static string ProjectRootDir() {
    char projectRootDir[1000], FILE[1000] = __FILE__;
    realpath(strcat(FILE, "/../.."), projectRootDir);
    return string(projectRootDir) + "/";
  }
  
  static string DTMRIDir() {
    CheckDataSet();
    return ProjectRootDir() + "images/" + DataSet + "/MRI/DTMRI/";
  }
  
  static string ResultsDir() {
    CheckDataSet();
    return ProjectRootDir() + "results/" + DataSet + "/segmentation/";
  }
  
  static void SetDataSet(string _DataSet) {
    DataSet = _DataSet;
  }
};

string Dirs::DataSet = "";


#endif