//____________________________________________________________________________
/*!

\class    DataCardsRecord

\brief    Class to read input cards

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva

\created  Sep 2012
\last update  Sep 2012

*/
//____________________________________________________________________________
#ifndef DATASTRUCT_DATACARDSRECORD_H
#define DATASTRUCT_DATACARDSRECORD_H 1

#include <TObject.h>

#include "DataCards.hh"

class DataCardsRecord : public TObject {

 public:
  DataCardsRecord();
  virtual ~DataCardsRecord() {}

  double           fetchValueDouble(const string& key);
  int              fetchValueInt(const string& key);
  bool             fetchValueBool(const string& key);
  TVector3         fetchValue3Vector(const string& key);
  string           fetchValueString(const string& key);
  vector<double>   fetchValueVector(const string&);

  InputDataCardsDouble   getCardsDouble()  {return cd_r;}
  InputDataCards3Vector  getCards3Vector() {return c3v_r;}
  InputDataCardsString   getCardsString()  {return cs_r;}
  InputDataCardsVector   getCardsVector()  {return cv_r;}
  InputDataCardsInt      getCardsInt()     {return ci_r;}
  InputDataCardsBool     getCardsBool()    {return cb_r;}

  void printToStream(ostream& stream);

 private:
  InputDataCardsString  cs_r;
  InputDataCardsInt     ci_r;
  InputDataCardsBool     cb_r;
  InputDataCardsDouble  cd_r;
  InputDataCards3Vector c3v_r;
  InputDataCardsVector  cv_r;

  ClassDef(DataCardsRecord, 1);
};

#endif


