//____________________________________________________________________________
/*!

\class     Env

\brief     Singleton to keep information about the VALOR environment (paths etc)

\author    Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
           University of Liverpool & STFC Rutherford Appleton Laboratory

\created   June 03, 2014

\copyright (c) 2010-2014, The VALOR group.

*/
//____________________________________________________________________________

#ifndef __gastpc_env__
#define __gastpc_env__

#include <iostream>
#include <string>

using std::ostream;
using std::string;

class Env {

public:

  static Env * Instance();
  
  string TopLevelDir     (void) { return fTopLevelDir;     }
  string TopLevelDataDir (void) { return fTopLevelDataDir; }
  string TopLevelConfDir (void) { return fTopLevelConfDir; }

  void AutoSet (Option_t * valor_application);

  void SetTopLevelDir     (string dir) { fTopLevelDir     = dir; }
  void SetTopLevelDataDir (string dir) { fTopLevelDataDir = dir; }
  void SetTopLevelConfDir (string dir) { fTopLevelConfDir = dir; }

  void Print(ostream & stream) const;
  friend ostream & operator << (ostream & stream, const Env & env);
  
private:

  Env();
  Env(const Env &);
 ~Env();

  static Env * fInstance;

  string fTopLevelDir;
  string fTopLevelDataDir;
  string fTopLevelConfDir;

  struct Cleaner {
      void DummyMethodAndSilentCompiler() { }
      ~Cleaner() {
         if (Env::fInstance !=0) {
            delete Env::fInstance;
            Env::fInstance = 0;
         }
      }
  };
  friend struct Cleaner;
};

ostream & operator<< (ostream & stream, const Env & env);

#endif // __gastpc_env__
