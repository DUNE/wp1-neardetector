// -------------------------------------------------------------------
/// \file   RootFileReader.h
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 31 July 2016
// -------------------------------------------------------------------

#ifndef GASTPC_ROOT_FILE_READER_H
#define GASTPC_ROOT_FILE_READER_H

#include "EventRecord.h"

namespace gastpc { class EventRecord; }
namespace gastpc { class RootFileReader; }
class TFile;
class TTree;


/// TODO: Class description

class gastpc::RootFileReader
{
public:
  RootFileReader();
  ~RootFileReader();

  void OpenFile(const std::string& filename);
  void CloseFile();

  gastpc::EventRecord& Read(int i=0);

private:
  gastpc::EventRecord* evtrec_;  
  TFile* file_;
  TTree* tree_;
};

#endif