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
  /// Open a ROOT file containing gastpc::EventRecord objects
  bool OpenFile(const std::string& filename, 
                const std::string& option="RECREATE");  
  /// Close the current file
  void CloseFile();
  /// Write an EventRecord in current file
  void Write(gastpc::EventRecord&);
  /// Check whether there is a file currently open
  bool IsFileOpen() const;

private:
  gastpc::EventRecord* evtrec_; ///< Pointer to transient event record
  TFile* file_; ///< Output ROOT file
  TTree* tree_; ///< Output tree
};

#endif