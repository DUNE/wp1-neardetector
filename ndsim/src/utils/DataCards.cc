//____________________________________________________________________________
/*!

\class    DataCard

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva

\created  Sep 2012
\last update Sep 2012

*/
//____________________________________________________________________________

#include "DataCards.hh"
#include "datastruct/DataCardsRecord.hh"

//ClassImp(DataCards)

unsigned int DataCards::count_ = 0;
DataCards* DataCards::instance_ = NULL;
InputDataCardsDouble  DataCards::cd_;
InputDataCards3Vector DataCards::c3v_;
InputDataCardsString  DataCards::cs_;
InputDataCardsVector  DataCards::cv_;
InputDataCardsInt     DataCards::ci_;
InputDataCardsBool    DataCards::cb_;

DataCards* DataCards::getInstance() {
  if (instance_) {
    instance_ = new DataCards();
  }

  return instance_;
}

DataCards::DataCards() {
  count_++;
  if (count_ > 1) {
    string mesage = "Trying to create a multiple instances of a static singleton class.";
    string location = "DataCards::DataCards()";
    //throw LbnoException(mesage, location, LbnoException::FATAL);
  }
}

DataCards::~DataCards() {
  count_--;
}

void DataCards::AddDataCardString(string aName, string aDefaultValue) {
  cs_[ aName ] = aDefaultValue;
}

void DataCards::AddDataCardInt(string aName, int aDefaultValue) {
  ci_[ aName ] = aDefaultValue;
}

void DataCards::AddDataCardBool(string aName, bool aDefaultValue) {
  cb_[ aName ] = aDefaultValue;
}

void DataCards::AddDataCardDouble(string aName, double aDefaultValue) {
  cd_[ aName ] = aDefaultValue;
}

void DataCards::AddDataCardDouble(string aName, double aDefaultValue, string units) {
  cd_[ aName ] = aDefaultValue * stringToUnit(units);
}

void DataCards::AddDataCardVector(string aName, vector< double > aDefaultValue) {
  cv_[ aName ] = aDefaultValue;
}

void DataCards::AddDataCardVector(string aName, const unsigned int aSize, double* aDefaultValue) {
  vector<double> dVector;
  dVector.assign( aDefaultValue, aDefaultValue + aSize );
  cv_[ aName ] = dVector;
}

double DataCards::fetchValueDouble(const string& key) {
  InputDataCardsDouble::iterator i=cd_.find(key);
  if (i == cd_.end()) {
    string mesage = "Error fetching unknown double datacard "+key+".";
    string location = "double DataCards::fetchValueDouble(const string& key)";
    throw LbnoException(mesage, location, LbnoException::FATAL);
  }
  return i->second;
}

int DataCards::fetchValueInt(const string& key) {
 InputDataCardsInt::iterator i=ci_.find(key);
 if( i == ci_.end()) {
    string mesage = "Error fetching unknown int datacard "+key+".";
    string location = "int DataCards::fetchValueInt(const string& key)";
    throw LbnoException(mesage, location, LbnoException::FATAL);
 }
   return i->second;
}

bool DataCards::fetchValueBool(const string& key) {
 InputDataCardsBool::iterator i=cb_.find(key);
 if( i == cb_.end()) {
    string mesage = "Error fetching unknown bool datacard "+key+".";
    string location = "int DataCards::fetchValueBool(const string& key)";
    throw LbnoException(mesage, location, LbnoException::FATAL);
 }
   return i->second;
}

TVector3 DataCards::fetchValue3Vector(const string& key) {
  InputDataCards3Vector::iterator i=c3v_.find(key);
  if (i == c3v_.end()) {
    string mesage = "Error fetching unknown 3Vector datacard "+key+".";
    string location = "TVector3 DataCards::fetchValue3Vector(const string& key)";
    throw LbnoException(mesage, location, LbnoException::FATAL);
  }
  return i->second;
}

string DataCards::fetchValueString(const string& key) {
  InputDataCardsString::iterator i=cs_.find(key);
  if (i == cs_.end()) {
    string mesage = "Error fetching unknown string datacard "+key+".";
    string location = "string DataCards::fetchValueString(const string& key)";
    throw LbnoException(mesage, location, LbnoException::FATAL);
  }
  return i->second;
}

vector<double> DataCards::fetchValueVector(const string& key) {
  InputDataCardsVector::iterator i=cv_.find(key);
  if (i == cv_.end()) {
    string mesage = "Error fetching unknown vector datacard. "+key+".";
    string location = "vector<double> DataCards::fetchValueVector(const string& key)";
    throw LbnoException(mesage, location, LbnoException::FATAL);
  }
  return i->second;
}

void DataCards::loadKeys(TFile* inFile) {

  DataCardsRecord* cards_rec = dynamic_cast<DataCardsRecord*>(inFile->Get("dataCards"));
  if (!cards_rec) {
    string fname(inFile->GetName());
    string mesage = "DataCardsRecord not found in file " + fname + ".";
    string location = "void DataCards::loadKeys(TFile* inFile)";
    throw LbnoException(mesage, location, LbnoException::FATAL);
  } 
  //cards_rec->printToStream(std::cout);

  cs_  = cards_rec->getCardsString();
  ci_  = cards_rec->getCardsInt();
  cb_  = cards_rec->getCardsBool();
  cd_  = cards_rec->getCardsDouble();
  c3v_ = cards_rec->getCards3Vector();
  cv_  = cards_rec->getCardsVector();
}

void DataCards::readKeys(string fileName) {

  std::cout << "Reading cards from file " << fileName << std::endl;
  std::ifstream in(fileName.c_str());
  if (!in.good()){
    string mesage = "DataCards File " + fileName + " not found.";
    string location = "void DataCards::readKeys(string fileName)";
    throw LbnoException(mesage, location, LbnoException::SERIOUS);
    exit(0);
  }
  else readKeys(in);
}

void DataCards::readKeys(std::istream& in) {
  string s;
  string t;
  string c;
  string units;

  while (in) {
    c="";
    t="";
    getline(in,c);
    std::istringstream is(c);
    is >> t;
    if (t.length() == 0) continue;
    if (t[0] == '#') continue; // Comments cards ignored
    if (t[0] == '!') continue; // MCFast style comment
    int ok = 0;

    InputDataCardsDouble::iterator i = cd_.find(t);
    if ( i != cd_.end()) {
      double d;
      units = "";
      is >> d >> units;
      cd_[t] = d;
      cd_[t] *= stringToUnit(units);
      ok = 1;
    }

    if( ok == 0)
    {
      InputDataCardsBool::iterator i1 = cb_.find(t);
      if(i1 != cb_.end()){
        bool b;
        is >> b;
        cb_[t] = b;
        ok =1;
      }
    }

    if( ok == 0)
    {
      InputDataCardsInt::iterator i1 = ci_.find(t);
      if(i1 != ci_.end()){
        int ii;
        is >> ii;
        ci_[t] = ii;
        ok =1;
      }
    }

    if (ok == 0) {
      std::istringstream is2(c);
      is2 >> t;
      InputDataCardsString::iterator i2 = cs_.find(t);
      if ( i2 != cs_.end()) {
        string s2;
        is>> s2;
        cs_[t] = s2;
        ok = 1;
      }
    }

    if (ok == 0) {
      std::istringstream iv(c);
      iv >> t;
      InputDataCards3Vector::iterator i2 = c3v_.find(t);
      if ( i2 != c3v_.end()) {
        double x, y, z;
        iv >> x >> y >> z;
        TVector3 v(x, y, z);
        c3v_[t] = v;
        ok = 1;
      }
    }

    if (ok == 0) {
      InputDataCardsVector::iterator i2 = cv_.find(t);
      if ( i2 != cv_.end()) {
        int nentries;
        is >> nentries;
        vector<double> cvec(nentries);
        for(int i=0; i<nentries; i++) {
          double vd;
          if ( is.good() ) {
            is >> vd;
          } else {
            string mesage = "Error reading key " + t + ". Bad data format.";
            string location = "void DataCards::readKeys(std::istream& in)";
            throw LbnoException(mesage, location, LbnoException::SERIOUS);
          }
          cvec[i] = vd;
        }
        cv_[t] = cvec;
        ok = 1;
      }
    }
    if (ok != 1){
      string mesage = "Failed to recognise datacard " + t + ".";
      string location = "void DataCards::readKeys(std::istream& in)";
      throw LbnoException(mesage, location, LbnoException::SERIOUS);
    }
  }
}

void DataCards::printToStream(ostream& stream) {
  stream << "-------- DataCards --------" << endl;

  stream << endl << " cards string : " << endl;
  InputDataCardsString::iterator it_cs = cs_.begin();
  for (unsigned int i=0; i < cs_.size(); i++) {
    stream << "  " << it_cs->first << " " << it_cs->second << endl;
    it_cs++;
  }

  stream << endl << " cards bool : " << endl;
  InputDataCardsBool::iterator it_cb = cb_.begin();
  for (unsigned int i=0; i < cb_.size(); i++) {
    stream << "  " << it_cb->first << " " << it_cb->second << endl;
    it_cb++;
  }

  stream << endl << " cards int : " << endl;
  InputDataCardsInt::iterator it_ci = ci_.begin();
  for (unsigned int i=0; i < ci_.size(); i++) {
    stream << "  " << it_ci->first << " " << it_ci->second << endl;
    it_ci++;
  }

  stream << endl << " cards double : " << endl;
  InputDataCardsDouble::iterator it_cd = cd_.begin();
  for (unsigned int i=0; i < cd_.size(); i++) {
    stream << "  " << it_cd->first << " " << it_cd->second << endl;
    it_cd++;
  }

  stream << endl << " cards TVector3 : " << endl;
  InputDataCards3Vector::iterator it_c3v = c3v_.begin();
  for (unsigned int i=0; i < c3v_.size(); i++) {
    stream << "  " << it_c3v->first << "  (" << it_c3v->second.x() << ", "
    << it_c3v->second.y() << ", " << it_c3v->second.z() << ")" << endl;

    it_c3v++;
  }

  stream << endl << " cards vector : " << endl;
  InputDataCardsVector::iterator it_cv = cv_.begin();
  for (unsigned int i=0; i < cv_.size(); i++) {
    stream << "  " << it_cv->first << " ( ";
    for (unsigned int j=0; j<it_cv->second.size(); j++)
      stream << it_cv->second[j] << " ";
    stream << ")" << endl;

    it_cv++;
  }
  stream << "---------------------------" << endl;
}


