// -------------------------------------------------------------------
/// \file   UIBatch.h
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 8 Aug 2016
// -------------------------------------------------------------------

#ifndef UI_BATCH_H
#define UI_BATCH_H

#include <G4UIsession.hh>
#include <fstream>


class UIBatch: public G4UIsession
{
public:
  /// Constructor
  UIBatch(const char* filename, G4UIsession* prev_session=0);
  /// Destructor
  virtual ~UIBatch();
  
  G4UIsession* GetPreviousSession() const;

  virtual G4UIsession* SessionStart();
  virtual void PauseSessionStart(const G4String&);

private:
  G4String ReadCommand();
  G4int ExecCommand(const G4String&);

private:
  G4UIsession* prev_session_;
  std::ifstream macro_stream_;
  G4bool opened_;
};

inline G4UIsession* UIBatch::GetPreviousSession() const { return prev_session_; }

#endif