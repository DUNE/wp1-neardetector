// -------------------------------------------------------------------
/// \file   DstWriter.h
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 11 Aug 2016
// -------------------------------------------------------------------

#ifndef DST_WRITER_H
#define DST_WRITER_H

#include "DstEntry.h"
#include <string>

class TFile;
class TTree;


/// TODO: Class description

class DstWriter
{
public:
  /// Constructor
  DstWriter();
  /// Destructor
  ~DstWriter();
  
  void OpenFile(const std::string& filename, 
                const std::string& option="RECREATE");

  void CloseFile();

  void Write(DstEntry&);

  bool IsFileOpen() const;

private:
  TFile* file_;
  TTree* tree_;
  DstEntry entry_;
};

#endif