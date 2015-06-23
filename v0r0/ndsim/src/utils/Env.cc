//________________________________________________________________________________________
/*

Copyright (c) 2010-2014, The VALOR group. All rights reserved.

ChangeLog:

*/
//________________________________________________________________________________________

#include <TSystem.h>

#include "utils/Env.hh"

using std::endl;

//________________________________________________________________________________________
  ostream & operator<<(ostream & stream, const Env & env)
  {
    env.Print(stream);
    return stream;
  }
//________________________________________________________________________________________
Env * Env::fInstance = 0;
//________________________________________________________________________________________
Env::Env() 
{
  fTopLevelDir     = "";
  fTopLevelDataDir = "";
  fTopLevelConfDir = "";

  fInstance = 0;
}
//________________________________________________________________________________________
Env::Env(const Env &) 
{

}
//________________________________________________________________________________________
Env::~Env()
{

}
//________________________________________________________________________________________
Env * Env::Instance()
{
  if(fInstance == 0) {
    static Env::Cleaner cleaner;
    cleaner.DummyMethodAndSilentCompiler();
    fInstance = new Env;
  }
  return fInstance;
}
//________________________________________________________________________________________
void Env::AutoSet(Option_t * app)
{
  string gastpc_app = app;

  if(gastpc_app.find("GasTPC") != string::npos) {
    const char * topdir = gSystem->Getenv("GasTPC");
    if(topdir) {
       fTopLevelDir     = topdir;
       fTopLevelDataDir = fTopLevelDir + "/inputs/";
       fTopLevelConfDir = fTopLevelDir + "/config/";
    }//topdir
  }
}
//________________________________________________________________________________________
void Env::Print(ostream & stream) const
{
  stream << "[Environment]" << endl;
  stream << " - top dir      : " << fTopLevelDir     << endl;
  stream << " - top data dir : " << fTopLevelDataDir << endl;
  stream << " - top conf dir : " << fTopLevelConfDir << endl;
}
//________________________________________________________________________________________
