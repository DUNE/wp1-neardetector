//_____________________________________________________________________________________________________
/*

Copyright (c) 2010-2014, The VALOR group. All rights reserved.

ChangeLog:

*/
//_____________________________________________________________________________________________________

#include <cassert>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <iomanip>

#include <TROOT.h>
#include <TMath.h>
#include <TStyle.h>
#include <TColor.h>
#include <TGraph.h>
#include <TH1.h>
#include <TH2.h>
#include <TAxis.h>
#include <TVectorD.h>
#include <TMatrixD.h>
#include <TXMLEngine.h>
#include <TFile.h>
#include <TSystem.h>

#include "Messenger.hh"
#include "UtlXML.hh"
#include "UtlStr.hh"
#include "Rnd.hh"
#include "Env.hh"

using std::setw;
using std::setfill;
using std::fixed;
using std::scientific;
using std::setprecision;
using std::ofstream;
using std::ostringstream;

//using namespace valor;

//____________________________________________________________________________________________________
XMLNodePointer_t util::FindNode(
  TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path)
{
// Find & return the XMLNodePointer_t at the specified node_path
//
  vector<string> node_path_vec = Tokens(node_path,"/");

  unsigned int ndepth = node_path_vec.size();
  unsigned int idepth = 0;

  XMLNodePointer_t curr_node = xml_eng->GetChild(top_node);
  while(curr_node) {
    string curr_node_name = xml_eng->GetNodeName(curr_node);
    if(node_path_vec[idepth] == curr_node_name) {
        idepth++;
        if(idepth == ndepth) {
          return curr_node;
        } else {
          curr_node = xml_eng->GetChild(curr_node);
        }
    } else {
       curr_node = xml_eng->GetNext(curr_node);
    }
  }
  return 0;
}
//____________________________________________________________________________________________________
bool util::NodeExists(
  TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path)
{
// Find & return the XMLNodePointer_t at the specified node_path
//
  vector<string> node_path_vec = Tokens(node_path,"/");

  unsigned int ndepth = node_path_vec.size();
  unsigned int idepth = 0;

  XMLNodePointer_t curr_node = xml_eng->GetChild(top_node);
  while(curr_node) {
    string curr_node_name = xml_eng->GetNodeName(curr_node);
    if(node_path_vec[idepth] == curr_node_name) {
        idepth++;
        if(idepth == ndepth) {
          return true;
        } else {
          curr_node = xml_eng->GetChild(curr_node);
        }
    } else {
       curr_node = xml_eng->GetNext(curr_node);
    }
  }
  return false;
}
//____________________________________________________________________________________________________
bool util::XML2Bool(
  TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path)
{
// Get the specified node content as a boolean
//
  LOG("XML", pINFO) << "Reading info at XML node node_path: " << node_path;

  XMLNodePointer_t found_node = FindNode(xml_eng,top_node,node_path);
  if(!found_node) {
    LOG("XML", pFATAL) 
       << "No node was found at input XML node node_path: " << node_path;
    exit(1);
  }
  string content = TrimSpaces(xml_eng->GetNodeContent(found_node));

  if(content == "true" || 
     content == "TRUE" || 
     content == "True" || 
     content == "1"    || 
     content == "I") return true;

  if(content == "false" || 
     content == "FALSE" || 
     content == "False" || 
     content == "0"    || 
     content == "O") return false;

   LOG("XML", pFATAL) 
       << "Could not interpret content (" << content 
       << ") found at node_path: " << node_path << " as a boolean!";
    exit(1);

  return false;
}
//____________________________________________________________________________________________________
double util::XML2Dbl(
  TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path)
{
// Get the specified node content as a floating-point number
//
  LOG("XML", pINFO) << "Reading info at XML node node_path: " << node_path;

  XMLNodePointer_t found_node = FindNode(xml_eng,top_node,node_path);
  if(!found_node) {
    LOG("XML", pFATAL) 
       << "No node was found at input XML node node_path: " << node_path;
    exit(1);
  }
  string content = xml_eng->GetNodeContent(found_node);
  double value = atof(content.c_str());
  return value;
}
//____________________________________________________________________________________________________
vector<double> util::XML2DblArray(
  TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path)
{
// Get the specified node content as a vector floating-point number
// The node content should be a comma-separed list of numbers

  LOG("XML", pINFO) << "Reading info at XML node node_path: " << node_path;

  XMLNodePointer_t found_node = FindNode(xml_eng,top_node,node_path);
  if(!found_node) {
    LOG("XML",pFATAL) 
       << "No node was found at input XML node node_path: " << node_path;
    exit(1);
  }
  vector<double> array;
  const char * nc = xml_eng->GetNodeContent(found_node);
  if (not nc) return array;
  
  string content = TrimSpaces(nc);
  
  vector<string> str_tokens = Tokens(content, ",");
  vector<string>::const_iterator str_tokens_iter = str_tokens.begin();
  for( ; str_tokens_iter != str_tokens.end(); ++str_tokens_iter) {
     string token = TrimSpaces(*str_tokens_iter);
     if (not token.size() ) continue;
     array.push_back( atof(token.c_str()) );
  }
  
  return array;
}
//____________________________________________________________________________________________________
string util::XML2String(
  TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path)
{
// Get the specified node content as a floating-point number
//
  LOG("XML", pINFO) << "Reading info at XML node node_path: " << node_path;

  XMLNodePointer_t found_node = FindNode(xml_eng,top_node,node_path);
  if(!found_node) {
    LOG("XML",pFATAL)
       << "No node was found at input XML node node_path: " << node_path;
    exit(1);
  }
  const char * nc = xml_eng->GetNodeContent(found_node);
  if (not nc) return string("");
  string content(nc);
  return TrimSpaces(content);
}
//____________________________________________________________________________________________________
//
// When we specify a ROOT object to be obtained from within a file,
// the XML schema for specifying the ROOT filename, the object name and the 
// object type is:
//
// <some_node>
//    ...
//      <another_node>
//            <filename> blah </filename>
//            <objname>  blah </objname>
//            <objtype>  blah </objtype>
//      </another_node>
//    ...
// </some_node>
//
// The following three methods return the filename, the object name and the
// object type for the specified node path (<another_node> in above example).
//
//.....................................................................................................
string util::XML2RootFileName(
   TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path)
{
  string filename = XML2String(xml_eng,top_node,node_path+"/filename");
  LOG("XML", pINFO) << "Filename: " << filename;
  return filename;
}
//.....................................................................................................
string util::XML2RootObjName (
   TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path)
{
  string objname = XML2String(xml_eng,top_node,node_path+"/objname");
  LOG("XML", pINFO) << "Object name: " << objname;
  return objname;
}
//.....................................................................................................
string util::XML2RootObjType (
   TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path)
{
  string objtype = XML2String(xml_eng,top_node,node_path+"/objtype");
  LOG("XML", pINFO) << "Object type: " << objtype;
  return objtype;
}
//____________________________________________________________________________________________________
//
// The following set of methods return a ROOT object from a file.
// The path specified in the XML file file
//  eg 
//    <mc_sample>
//          <filename> data/t2k/run1_4/nue.root <filename>
//          ...
//    </mc_sample>
// does not need to be the absolute path.
// The following methods shall append path specified in the XML file to
// the input base_dir.
// If the default value of base_dir is unchanged, it will be set to the
// value of Env::TopLevelDataDir()
//
//.....................................................................................................

TFile * util::XML2TFile(
  TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path, string base_dir)
{
  LOG("XML", pINFO) << "Reading info at XML node node_path: " << node_path;

  string filename = XML2RootFileName(xml_eng,top_node,node_path);

  string used_base_dir = base_dir;
  if(base_dir.find("<env>") != string::npos) {
    used_base_dir = Env::Instance()->TopLevelDataDir();
  }

  const char * full_filename = Form("%s/%s", used_base_dir.c_str(), filename.c_str());

  TFile * file = new TFile(full_filename,"read");
  if(!file) {
    LOG("XML",pFATAL) << "No file: " << full_filename << " found";
    exit(1);
  }
  if(file->IsZombie()) {
    LOG("XML",pFATAL) << "File is a zombie: " << full_filename;
    exit(1);
  }

  return file;
}
//____________________________________________________________________________________________________
TH1F * util::XML2TH1F(
  TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path, string base_dir)
{
// Get a TH1F instance from the info in the input XML config file
//
  LOG("XML", pINFO) << "Reading info at XML node node_path: " << node_path;

  string filename = XML2RootFileName(xml_eng,top_node,node_path);
  string objname  = XML2RootObjName (xml_eng,top_node,node_path);

  string used_base_dir = base_dir;
  if(base_dir.find("<env>") != string::npos) {
    used_base_dir = Env::Instance()->TopLevelDataDir();
  }

  const char * full_filename = Form("%s/%s", used_base_dir.c_str(), filename.c_str());

  TFile file(full_filename,"read");
  TH1F * h = (TH1F*)file.Get(objname.c_str());
  if(!h) {
    LOG("XML",pFATAL) << "No " << objname << " in " << full_filename;
    file.Close();
    exit(1);
  }

  TH1F * hcopy = new TH1F(*h);
  hcopy->SetDirectory(0);

  file.Close();

  return hcopy;
}
//____________________________________________________________________________________________________
TH1D * util::XML2TH1D(
  TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path, string base_dir)
{
// Get a TH1D instance from the info in the input XML config file
//
  LOG("XML", pINFO) << "Reading info at XML node node_path: " << node_path;

  string filename = XML2RootFileName(xml_eng,top_node,node_path);
  string objname  = XML2RootObjName (xml_eng,top_node,node_path);

  string used_base_dir = base_dir;
  if(base_dir.find("<env>") != string::npos) {
    used_base_dir = Env::Instance()->TopLevelDataDir();
  }

  const char * full_filename = Form("%s/%s", used_base_dir.c_str(), filename.c_str());

  TFile file(full_filename,"read");
  TH1D * h = (TH1D*)file.Get(objname.c_str());
  if(!h) {
    LOG("XML",pFATAL) << "No " << objname << " in " << filename;
    file.Close();
    exit(1);
  }

  TH1D * hcopy = new TH1D(*h);
  hcopy->SetDirectory(0);

  file.Close();

  return hcopy;
}
//____________________________________________________________________________________________________
TH2D * util::XML2TH2D(
  TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path, string base_dir)
{
// Get a TH2D instance from the info in the input XML config file
//
  LOG("XML", pINFO) << "Reading info at XML node node_path: " << node_path;

  string filename = XML2RootFileName(xml_eng,top_node,node_path);
  string objname  = XML2RootObjName (xml_eng,top_node,node_path);

  string used_base_dir = base_dir;
  if(base_dir.find("<env>") != string::npos) {
    used_base_dir = Env::Instance()->TopLevelDataDir();
  }

  const char * full_filename = Form("%s/%s", used_base_dir.c_str(), filename.c_str());

  TFile file(full_filename,"read");
  TH2D * h2 = (TH2D*)file.Get(objname.c_str());
  if(!h2) {
    LOG("XML",pFATAL) << "No " << objname << " in " << filename;
    file.Close();
    exit(1);
  }

  TH2D * h2copy = new TH2D(*h2);
  h2copy->SetDirectory(0);

  file.Close();

  return h2copy;
}
//____________________________________________________________________________________________________
TTree * util::XML2TTree(
   TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path, string base_dir)
{
// Get a TH2D instance from the info in the input XML config file
//
  LOG("XML", pINFO) << "Reading info at XML node node_path: " << node_path;

  string filename = XML2RootFileName(xml_eng,top_node,node_path);
  string objname  = XML2RootObjName (xml_eng,top_node,node_path);

  string used_base_dir = base_dir;
  if(base_dir.find("<env>") != string::npos) {
    used_base_dir = Env::Instance()->TopLevelDataDir();
  }

  const char * full_filename = Form("%s/%s", used_base_dir.c_str(), filename.c_str());

  TFile * file = new TFile(full_filename,"read");
  TTree * tree = (TTree*)file->Get(objname.c_str());
  if(!tree) {
    LOG("XML",pFATAL) << "No " << objname << " in " << filename;
//  file.Close();
    exit(1);
  }

  return tree;
//TTree * treecopy = tree->CloneTree();
//file.Close();
//return treecopy;
}
//____________________________________________________________________________________________________
TVectorD * util::XML2TVectorD(
   TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path, string base_dir)
{
// Get a TVectorD instance from the info in the input XML config file
//
  LOG("XML", pINFO) << "Reading info at XML node node_path: " << node_path;

  string filename = XML2RootFileName(xml_eng,top_node,node_path);
  string objname  = XML2RootObjName (xml_eng,top_node,node_path);

  string used_base_dir = base_dir;
  if(base_dir.find("<env>") != string::npos) {
    used_base_dir = Env::Instance()->TopLevelDataDir();
  }

  const char * full_filename = Form("%s/%s", used_base_dir.c_str(), filename.c_str());

  TFile file(full_filename,"read");
  TVectorD * vec = (TVectorD*)file.Get(objname.c_str());
  if(!vec) {
    LOG("XML",pFATAL) << "No " << objname << " in " << filename;
    file.Close();
    //return 0;
    exit(1);
  }

  TVectorD * vec_copy = new TVectorD(*vec);

  file.Close();

  return vec_copy;
}
//____________________________________________________________________________________________________
TMatrixDSym * util::XML2TMatrixDSym(
   TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path, string base_dir)
{
// Get a TMatrixDSym instance from the info in the input XML config file
//
  string filename = XML2RootFileName(xml_eng,top_node,node_path);
  string objname  = XML2RootObjName (xml_eng,top_node,node_path);

  string used_base_dir = base_dir;
  if(base_dir.find("<env>") != string::npos) {
    used_base_dir = Env::Instance()->TopLevelDataDir();
  }

  const char * full_filename = Form("%s/%s", used_base_dir.c_str(), filename.c_str());

  TFile file(full_filename,"read");
  TMatrixDSym * matrix = (TMatrixDSym*)file.Get(objname.c_str());
  if(!matrix) {
    LOG("XML",pFATAL) << "No " << objname << " in " << filename;
    file.Close();
    exit(1);
  }

  TMatrixDSym * matrix_copy = new TMatrixDSym(*matrix);
  file.Close();

  return matrix_copy;
}
//____________________________________________________________________________________________________
TMatrixD * util::XML2TMatrixD(
    TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path, string base_dir)
{
// Get a TMatrixD instance from the info in the input XML config file
//
  string objtype  = XML2RootObjType (xml_eng,top_node,node_path);
  if(objtype == "TH2D")
    return XML2TH2D2TMatrixD(xml_eng,top_node,node_path);

  string filename = XML2RootFileName(xml_eng,top_node,node_path);
  string objname  = XML2RootObjName (xml_eng,top_node,node_path);

  string used_base_dir = base_dir;
  if(base_dir.find("<env>") != string::npos) {
    used_base_dir = Env::Instance()->TopLevelDataDir();
  }

  const char * full_filename = Form("%s/%s", used_base_dir.c_str(), filename.c_str());

  TFile * file = new TFile(full_filename,"read"); 
  TMatrixD * matrix = (TMatrixD*) file->Get(objname.c_str());
  if(!matrix) {
    LOG("XML",pFATAL) << "No " << objname << " in " << filename;
    file->Close();
    exit(1);
  }

  TMatrixD * matrix_copy = new TMatrixD(*matrix);
  file->Close();

  return matrix_copy;
}
//____________________________________________________________________________________________________
TMatrixD * util::XML2TH2D2TMatrixD(
  TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path, string base_dir)
{
// Get a TH2D instance from the info in the input XML config file
//
  LOG("XML", pINFO) << "Reading info at XML node node_path: " << node_path;

  string filename = XML2RootFileName(xml_eng,top_node,node_path);
  string objname  = XML2RootObjName (xml_eng,top_node,node_path);

  string used_base_dir = base_dir;
  if(base_dir.find("<env>") != string::npos) {
    used_base_dir = Env::Instance()->TopLevelDataDir();
  }

  const char * full_filename = Form("%s/%s", used_base_dir.c_str(), filename.c_str());

  TFile file(full_filename,"read");
  TH2D * h2 = (TH2D*)file.Get(objname.c_str());
  if(!h2) {
    LOG("XML",pFATAL) << "No " << objname << " in " << filename;
    file.Close();
    exit(1);
  }

  TH2D * h2copy = new TH2D(*h2);
  h2copy->SetDirectory(0);

  file.Close();

  const int nrows = h2copy->GetNbinsX();
  const int ncols = h2copy->GetNbinsY();

  TMatrixD matrix(nrows,ncols);
  for(int i = 1; i <= nrows; i++) {
    for(int j = 1; j <= ncols; j++) {
      matrix(i-1,j-1)=h2copy->GetBinContent(i,j);
      LOG("XML",pDEBUG) << i << "\t" << j << "\t" << h2copy->GetBinContent(i,j);
    }
  }

  TMatrixD * matrix_copy = new TMatrixD(matrix);
#ifdef __show_xml_mesg_debug__
  matrix_copy->Print();
#endif

  return matrix_copy;
}
//____________________________________________________________________________________________________
TSpline3 * util::XML2TSpline3(
  TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path, string base_dir)
{
  string filename = XML2RootFileName(xml_eng,top_node,node_path);
  string objname  = XML2RootObjName (xml_eng,top_node,node_path);

  string used_base_dir = base_dir;
  if(base_dir.find("<env>") != string::npos) {
    used_base_dir = Env::Instance()->TopLevelDataDir();
  }

  const char * full_filename = Form("%s/%s", used_base_dir.c_str(), filename.c_str());

  TFile * file = new TFile(full_filename,"read"); 
  TSpline3 * spl = (TSpline3*) file->Get(objname.c_str());
  if(!spl) {
    LOG("XML",pFATAL) << "No " << objname << " in " << filename;
    file->Close();
    exit(1);
  }

  TSpline3 * spl_copy = new TSpline3(*spl);
  file->Close();

  return spl_copy;
}
//____________________________________________________________________________________________________
TAxis *  util::XML2BinInfo(
  TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path)
{
//
//
  LOG("XML", pINFO) << "Reading info at XML node node_path: " << node_path;

  XMLNodePointer_t found_node = FindNode(xml_eng,top_node,node_path);
  if(!found_node) {
    LOG("XML",pFATAL) << "No node was found at input XML node node_path: " << node_path;
    //return 0;
    exit(1);
  }

  string binsz = xml_eng->GetAttr(found_node,"bin_size");
  if(binsz == "fixed") {
    int    nbins = 0;
    double min   = 0;
    double max   = 0;
    XMLNodePointer_t child_node = xml_eng->GetChild(found_node);
    while(child_node) {
      string child_node_name = xml_eng->GetNodeName(child_node);
      string content = xml_eng->GetNodeContent(child_node);
      if(child_node_name == "nbins") { nbins = atoi(TrimSpaces(content).c_str()); }
      if(child_node_name == "min"  ) { min   = atof(TrimSpaces(content).c_str()); }
      if(child_node_name == "max"  ) { max   = atof(TrimSpaces(content).c_str()); }
      child_node = xml_eng->GetNext(child_node);    
    }
    LOG("XML",pINFO) 
       << "nbins = " << nbins << ", range = (" << min << ", " << max << ")";
    assert(nbins>0 && min<max);

    TAxis * axis = new TAxis(nbins,min,max);
    return axis;
  } 
  else   
  if(binsz == "variable") {
    LOG("XML",pFATAL) << "Not handling variable size bins yet";
    exit(1);
  }
  else {
    LOG("XML",pFATAL) << "Invalid bin_size attribute";
    exit(1);
  }

  return 0;
}
//____________________________________________________________________________________________________

