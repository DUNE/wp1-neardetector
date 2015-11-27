//____________________________________________________________________________
/*

 Copyright (c) 2003-2014, GENIE Neutrino MC Generator Collaboration
 For the full text of the license visit http://copyright.genie-mc.org
 or see $GENIE/LICENSE

*/
//____________________________________________________________________________

#include <iostream>
#include <vector>
#include <iomanip>

#include "log4cpp/SimpleLayout.hh"
#include "log4cpp/CategoryStream.hh"

#include <TSystem.h>
#include <TXMLEngine.h>

#include "Messenger.hh"

using std::setw;
using std::setfill;
using std::cout;
using std::endl;
using std::vector;

//____________________________________________________________________________
Messenger * Messenger::fInstance = 0;
//____________________________________________________________________________
Messenger::Messenger()
{
  fInstance =  0;
}
//____________________________________________________________________________
Messenger::~Messenger()
{
  fInstance = 0;
}
//____________________________________________________________________________
Messenger * Messenger::Instance()
{
  if(fInstance == 0) {	
    static Messenger::Cleaner cleaner;
    cleaner.DummyMethodAndSilentCompiler();

    fInstance = new Messenger;
    log4cpp::Appender * appender;
    appender = new log4cpp::OstreamAppender("default", &cout);
    const char* layoutenv = gSystem->Getenv("GasTPC_MSG_LAYOUT");
    std::string layoutstr = (layoutenv) ? string(layoutenv) : "BASIC";
    if ( layoutstr == "SIMPLE" ) {
      appender->setLayout(new log4cpp::SimpleLayout());
    } else {
      appender->setLayout(new log4cpp::BasicLayout());
    }

    log4cpp::Category & MSG = log4cpp::Category::getRoot();
    MSG.setAdditivity(false);
    MSG.addAppender(appender);
  }
  return fInstance;
}
//____________________________________________________________________________
log4cpp::Category & Messenger::operator () (const char * stream)
{
  log4cpp::Category & MSG = log4cpp::Category::getInstance(stream);
  return MSG;
}
//____________________________________________________________________________
void Messenger::SetPriorityLevel(
   const char * stream, log4cpp::Priority::Value priority)
{
  log4cpp::Category & MSG = log4cpp::Category::getInstance(stream);
  MSG.setPriority(priority);
}
//____________________________________________________________________________
bool Messenger::Configure(string filename)
{
// Set priority levels and set them.

  TXMLEngine* xml = new TXMLEngine;   
  XMLDocPointer_t xmldoc = xml->ParseFile(filename.c_str());
  if (xmldoc==0) {
    LOG("Messenger",pERROR) << "Can not parse input XML file [" << filename << "]";
    delete xml;
    return false;  
  }
  XMLNodePointer_t root_node = xml->DocGetRootElement(xmldoc);

  XMLNodePointer_t curr_node = xml->GetChild(root_node);
  while(curr_node) {
    string curr_node_name = xml->GetNodeName(curr_node);
    if(curr_node_name == "priority") {
      string stream   = xml->GetAttr(curr_node,"msgstream");
      string priority = xml->GetNodeContent(curr_node);

      log4cpp::Priority::Value pv = this->PriorityFromString(priority);
      this->SetPriorityLevel(stream.c_str(), pv);

      LOG("Messenger",pINFO) 
        << "Setting priority for message stream: " 
        << stream << " --> " << priority;
    }
    curr_node = xml->GetNext(curr_node);
  }

  return true;
}
//____________________________________________________________________________
log4cpp::Priority::Value Messenger::PriorityFromString(string p)
{
  if ( p.find("DEBUG")  != string::npos ) return log4cpp::Priority::DEBUG;
  if ( p.find("INFO")   != string::npos ) return log4cpp::Priority::INFO;
  if ( p.find("NOTICE") != string::npos ) return log4cpp::Priority::NOTICE;
  if ( p.find("WARN")   != string::npos ) return log4cpp::Priority::WARN;
  if ( p.find("ERROR")  != string::npos ) return log4cpp::Priority::ERROR;
  if ( p.find("CRIT")   != string::npos ) return log4cpp::Priority::CRIT;
  if ( p.find("ALERT")  != string::npos ) return log4cpp::Priority::ALERT;
  if ( p.find("FATAL")  != string::npos ) return log4cpp::Priority::FATAL;

  LOG("Messenger", pWARN)
    << "Unknown priority = " << p << " - Setting to INFO";
  return log4cpp::Priority::INFO;
}
//____________________________________________________________________________
