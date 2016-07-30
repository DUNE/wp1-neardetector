// -------------------------------------------------------------------
/// \file   RootFileWriter.h
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 12 Mar 2016
// -------------------------------------------------------------------

#ifndef ROOT_FILE_WRITER
#define ROOT_FILE_WRITER

#include <string>

class TFile;
class TTree;

namespace gastpc {

  class EventRecord;


  /// TODO: Class description

  class RootFileWriter
  {
  public:
    /// Constructor
    RootFileWriter();
    /// Destructor
    ~RootFileWriter();

    bool OpenFile(const std::string&);

    void CloseFile();

    void Write(EventRecord&);



  private:
    TFile* file_; ///< Output ROOT file
    TTree* tree_;

    EventRecord* evtrec_;
  };

} // namespace gastpc

#endif