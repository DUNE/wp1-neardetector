//____________________________________________________________________________
/*!

\class    DataCard

\brief    DataCard Class

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva

\created  Sep 2012
\last update Sep 2012

*/
//____________________________________________________________________________
#ifndef UTILS_DATACARDS_H
#define UTILS_DATACARDS_H 1

#include <string>
#include <iostream>
#include <sstream>
#include <map>
#include <vector>
#include <fstream>
#include <cstdlib>

#include <TVector3.h>
#include <TFile.h>

#include "LbnoException.hh"
#include "Units.hh"

using std::string;
using std::vector;
using std::map;
using std::ostream;
using std::istream;
using std::cout;
using std::endl;

typedef map< string, double >         InputDataCardsDouble;
typedef map< string, TVector3 >       InputDataCards3Vector;
typedef map< string, string >         InputDataCardsString;
typedef map<string, vector<double> >  InputDataCardsVector;
typedef map<string, int >             InputDataCardsInt;
typedef map<string, bool >            InputDataCardsBool;

class DataCards {
 public:
  static DataCards* getInstance();
  virtual ~DataCards();

  static unsigned int count_;

  void readKeys(string fileName);
  void readKeys(istream& in);
  void loadKeys(TFile* inFile);

  bool             fetchValueBool(const string& key);
  double           fetchValueDouble(const string& key);
  int              fetchValueInt(const string& key);
  TVector3         fetchValue3Vector(const string& key);
  string           fetchValueString(const string& key);
  vector<double>   fetchValueVector(const string& key);

  //void printValue(const string& key);
  //friend ostream &operator<<(ostream &o, const DataCards& d);

  void AddDataCardString(string aName, string aDefaultValue = "");
  void AddDataCardInt(string aName, int aDefaultValue);
  void AddDataCardBool(string aName, bool aDefaultValue);
  void AddDataCardDouble(string aName, double aDefaultValue);
  void AddDataCardDouble(string aName, double aDefaultValue, string units);
  void AddDataCardVector(string aName, vector< double > aDefaultValue);
  void AddDataCardVector(string aName, const unsigned int aSize, double * aDefaultValue);

  /*
  How to use the above:

  1.) Adding string property 'foo' with empty default value.
  AddDataCard( "foo" );

  2.) Adding string property 'foo' with default value 'bar'.
  AddDataCard( "foo", "bar" );

  3.) Adding integer property 'foo' with default value 0.
  AddDataCard( "foo", 0 );

  4.) Adding double property 'foo' with default value 0.0 .
  You should explicitly appoint the type to double by adding a decimal point.
   AddDataCard( "foo", 0.0 );

  5.1.) Adding vector of double.
  vector< double > bar;
  bar.push_back( 3.14 );
  bar.push_back( -273.15 ); 
  AddDataCard( "foo", bar );

  5.2.) Adding vector of double.
  double * bar;
  bar = new double[ 2 ];
  bar[ 0 ] = 3.14;
  bar[ 1 ] = -273.15;
  AddDataCard( "foo", 2, bar );	
  */

  InputDataCardsDouble   getCardsDouble()  {return cd_;}
  InputDataCards3Vector  getCards3Vector() {return c3v_;}
  InputDataCardsString   getCardsString()  {return cs_;}
  InputDataCardsVector   getCardsVector()  {return cv_;}
  InputDataCardsInt      getCardsInt()     {return ci_;}
  InputDataCardsBool     getCardsBool()    {return cb_;}

  void printToStream(ostream& stream);

 private:
  DataCards();

  static DataCards* instance_;

  static InputDataCardsString  cs_;
  static InputDataCardsBool    cb_;
  static InputDataCardsInt     ci_;
  static InputDataCardsDouble  cd_;
  static InputDataCards3Vector c3v_;
  static InputDataCardsVector  cv_;

  //ClassDef(DataCards, 1);
};

#endif


