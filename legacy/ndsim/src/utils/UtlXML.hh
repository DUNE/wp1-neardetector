//____________________________________________________________________________
/*!

\class     xmlutils

\brief     XML utilities

\author    Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
           University of Liverpool & STFC Rutherford Appleton Laboratory
          
\created   Oct 20, 2010

\copyright (c) 2010-2014, The VALOR group.

*/
//____________________________________________________________________________

#ifndef _gastpc_xml_utils_h_
#define _gastpc_xml_utils_h_

#include <string>
#include <vector>
#include <complex>

#include <TXMLEngine.h>
#include <TFile.h>
#include <TTree.h>
#include <TVectorD.h>
#include <TMatrixD.h>
#include <TH1D.h>
#include <TSpline.h>

class TGraph;
class TH1;
class TH1F;
class TH1D;
class TH2D;
class TAxis;

using std::string;
using std::vector;

namespace util {

  XMLNodePointer_t FindNode          (TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path);
  bool             NodeExists        (TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path);

  bool             XML2Bool          (TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path);
  double           XML2Dbl           (TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path);
  vector<double>   XML2DblArray      (TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path);
  string           XML2String        (TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path);

  string           XML2RootFileName  (TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path);
  string           XML2RootObjName   (TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path);
  string           XML2RootObjType   (TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path);

  TFile *          XML2TFile         (TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path, string base_dir = "<env>");
  TH1F *           XML2TH1F          (TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path, string base_dir = "<env>");
  TH1D *           XML2TH1D          (TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path, string base_dir = "<env>");
  TH2D *           XML2TH2D          (TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path, string base_dir = "<env>");
  TTree *          XML2TTree         (TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path, string base_dir = "<env>");
  TVectorD *       XML2TVectorD      (TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path, string base_dir = "<env>");
  TMatrixDSym *    XML2TMatrixDSym   (TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path, string base_dir = "<env>");
  TMatrixD *       XML2TMatrixD      (TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path, string base_dir = "<env>");
  TMatrixD *       XML2TH2D2TMatrixD (TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path, string base_dir = "<env>");
  TSpline3 *       XML2TSpline3      (TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path, string base_dir = "<env>");

  TAxis *          XML2BinInfo       (TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path);
  
} // util
  
#endif


