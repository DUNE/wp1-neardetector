// -------------------------------------------------------------------
/// \file   UIBatch.cxx
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 8 Aug 2016
// -------------------------------------------------------------------

#include "UIBatch.h"

#include <G4UImanager.hh>

#include <vector>



static void Tokenize(const G4String& str, std::vector<G4String>& tokens)
{
  const char* delimiter= " ";
 
  G4String::size_type pos0= str.find_first_not_of(delimiter);
  G4String::size_type pos = str.find_first_of(delimiter, pos0);

  while (pos != G4String::npos || pos0 != G4String::npos) {
    if (str[pos0] == '\"') {
      pos = str.find_first_of("\"", pos0+1);
      if(pos != G4String::npos) pos++;
    }
    if (str[pos0] == '\'') {
      pos = str.find_first_of("\'", pos0+1);
      if(pos != G4String::npos) pos++;
    }

    tokens.push_back(str.substr(pos0, pos-pos0));
    pos0 = str.find_first_not_of(delimiter, pos);
    pos = str.find_first_of(delimiter, pos0);
  }
}


UIBatch::UIBatch(const char* filename, G4UIsession* prev_session):
  G4UIsession(), prev_session_(prev_session), opened_(false)
{
  macro_stream_.open(filename, std::ios::in);
  if (macro_stream_.fail()) {
    G4Exception("UIBatch::UIBatch()", "ERROR", FatalException,
      "Could not open macro file.");
  }
  else {
    opened_ = true;
  }

  G4UImanager::GetUIpointer()->SetSession(this);
}


UIBatch::~UIBatch()
{
  if(opened_) macro_stream_.close();
}


G4String UIBatch::ReadCommand()
{
  enum { BUFSIZE= 4096 };
  static G4ThreadLocal char *linebuf = 0; 
  if (!linebuf) linebuf = new  char [BUFSIZE];
  const char ctrM = 0x0d;

  G4String cmdtotal= "";
  G4bool qcontinued= false;
  while(macro_stream_.good()) {
    macro_stream_.getline(linebuf, BUFSIZE);

    G4String cmdline(linebuf);

    // TAB-> ' ' conversion
    str_size nb=0;
    while ((nb= cmdline.find('\t',nb)) != G4String::npos) {
      cmdline.replace(nb, 1, " ");
    }

    // strip
    cmdline= cmdline.strip(G4String::both);
    cmdline= cmdline.strip(G4String::trailing, ctrM);

    // skip null line if single line
    if(!qcontinued && cmdline.size()==0) continue;

    // '#' is treated as echoing something
    if(cmdline[(size_t)0]=='#') return cmdline;

    // tokenize...
    std::vector<G4String> tokens;
    Tokenize(cmdline, tokens);
    qcontinued= false;
    for (G4int i=0; i< G4int(tokens.size()); i++) {
      // string after '#" is ignored
      if(tokens[i][(size_t)0] == '#' ) break;
      // '\' or '_' is treated as continued line.
      if(tokens[i] == '\\' || tokens[i] == '_' ) {
        qcontinued= true;
        // check nothing after line continuation character
        if( i != G4int(tokens.size())-1) {
          G4Exception("G4UIbatch::ReadCommand","UI0003",
          JustWarning,
          "unexpected character after line continuation character");
        }
        break; // stop parsing
      }
      cmdtotal+= tokens[i];
      cmdtotal+= " ";
    }

    if(qcontinued) continue; // read the next line

    if(cmdtotal.size() != 0) break;
    if(macro_stream_.eof()) break;
  }

  // strip again
  cmdtotal= cmdtotal.strip(G4String::both);

  // finally,
  if(macro_stream_.eof() && cmdtotal.size()==0) {
    return "exit";
  }

  return cmdtotal;
}


G4int UIBatch::ExecCommand(const G4String& command)
{
  G4UImanager* UI= G4UImanager::GetUIpointer();
  G4int rc= UI-> ApplyCommand(command);

  G4String error_msg;

  switch(rc) {
    case fCommandSucceeded:
      break;
    case fCommandNotFound:
      error_msg = " Command not found: " + command;
      G4Exception("UIBatch::ExecCommand()", "ERROR", FatalException, error_msg);
      break;
    case fIllegalApplicationState:
      error_msg = " Illegal application state: " + command;
      G4Exception("UIBatch::ExecCommand()", "ERROR", FatalException, error_msg);
      break;
    default:
      //G4int pn= rc%100;
      error_msg = " Illegal parameter: " + command;
      G4Exception("UIBatch::ExecCommand()", "ERROR", FatalException, error_msg);
  }

  return rc;
}



G4UIsession* UIBatch::SessionStart()
{
  if(!opened_) return prev_session_;

  while(1) {
    G4String newCommand = ReadCommand();

    if(newCommand == "exit") {
      break;
    }

    // just echo something
    if( newCommand[(size_t)0] == '#') {
      if(G4UImanager::GetUIpointer()-> GetVerboseLevel()==2) {
        G4cout << newCommand << G4endl;
      }
      continue;
    }

    // execute command
    G4int rc= ExecCommand(newCommand);
    if(rc != fCommandSucceeded) {
      G4cerr << G4endl << "***** Batch is interrupted!! *****" << G4endl;
      break;
    }
  }

  return prev_session_;
}


void UIBatch::PauseSessionStart(const G4String& Prompt)
{
  G4cout << "Pause session <" << Prompt << "> start." << G4endl;
  SessionStart();
  G4cout << "Pause session <" << Prompt << "> Terminate." << G4endl;
}
