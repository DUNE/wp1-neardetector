// -------------------------------------------------------------------
/// \file   RootFileWriter.h
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 12 Mar 2016
// -------------------------------------------------------------------

#ifndef GASTPC_ROOT_FILE_WRITER_H
#define GASTPC_ROOT_FILE_WRITER_H

#include <string>

namespace gastpc { class EventRecord; }
namespace gastpc { class RootFileWriter; }

class TFile;
class TTree;


/// TODO: Class description

class gastpc::RootFileWriter
{
public:
  /// Constructor
  RootFileWriter();
  /// Destructor
  ~RootFileWriter();

  void OpenFile(const std::string&);

  void CloseFile();

  void Write(gastpc::EventRecord&);

private:
  gastpc::EventRecord* evtrec_; ///< Pointer to transient event record
  TFile* file_; ///< Output ROOT file
  TTree* tree_; ///< Output tree
};

#endif