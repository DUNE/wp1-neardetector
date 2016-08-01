// -------------------------------------------------------------------
/// \file   RootFileReader.h
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 31 July 2016
// -------------------------------------------------------------------

#ifndef GASTPC_ROOT_FILE_READER_H
#define GASTPC_ROOT_FILE_READER_H

#include <string>

namespace gastpc { class EventRecord; }
namespace gastpc { class RootFileReader; }
class TFile;
class TTree;


/// TODO: Class description

class gastpc::RootFileReader
{
public:
  /// Constructor
  RootFileReader();
  /// Destructor
  ~RootFileReader();

  /// Open a ROOT file that contains gastpc::EventRecord objects
  bool OpenFile(const std::string& filename);
  /// Close the current file
  void CloseFile();
  /// Check whether there is an open file
  bool IsFileOpen() const;

  /// Read an entry from the file
  gastpc::EventRecord& Read(int i);
  /// Return number of entries in file
  int GetNumberOfEntries() const;

private:
  gastpc::EventRecord* evtrec_; ///< Pointer to current event record
  TFile* file_; ///< Pointer to current ROOT file
  TTree* tree_; ///< Pointer to the event record tree
};

#endif