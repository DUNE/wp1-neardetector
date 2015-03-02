//____________________________________________________________________________
/*!

\class    copyTree

\brief    Program to copy trees from one file to another

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\created  June 2014
\last update June 2014

*/
//____________________________________________________________________________
#include "LbnoPlotter.hh"

class copyTree : public LbnoPlotter{
  public:
    copyTree();
    virtual ~copyTree() {};

    bool copyTrees();
    void initDataCards();
    void loadDataCards();

    int getStartRunNumber() 	  const {return startRun;};
    std::string getTreeName()	  const {return treeName;};

  private:
    int startRun;
    std::string treeName;
};

copyTree::copyTree(){
  this->initDataCards();
}

void copyTree::initDataCards() {
  cards_ = DataCards::getInstance();
  cards_->AddDataCardString("dataFileName", "test");
  cards_->AddDataCardString("outFileName", "output.root");
  cards_->AddDataCardString("treeName", "NuInteractions");
  cards_->AddDataCardInt("numberOfRuns", 100);
  cards_->AddDataCardInt("startRun", 0);
}

void copyTree::loadDataCards() {
  inputFileName_  	= cards_->fetchValueString("dataFileName");
  outputFileName_  	= cards_->fetchValueString("outFileName");
  treeName	  	= cards_->fetchValueString("treeName");
  runs_		  	= cards_->fetchValueInt("numberOfRuns");
  startRun	  	= cards_->fetchValueInt("startRun");
}

bool copyTree::copyTrees(){

   //Create a new file + a clone of old tree in new file
   TFile *newfile = new TFile(outputFileName_.c_str(),"recreate");
   
   int count = 0;

   //loop over runs
   for(int i=startRun;i<(startRun+runs_);i++){
	std::stringstream ss;
	ss << i;
	std::string filename = inputFileName_ + ss.str() + ".root";
	ss.str("");

	TFile *datafile = new TFile(filename.c_str());
   	TTree *oldtree = (TTree*)datafile->Get(treeName.c_str());
	//set data addresses
    	if(!setupTrees(*datafile)) continue;

	ss << count;
   	newfile->cd();
   	TTree *newtree = oldtree->CloneTree();
	newtree->SetDirectory(0);
	std::string treename = newtree->GetName();
	treename += "_run" + ss.str();
    	newtree->Write(treename.c_str(), TObject::kOverwrite);
    
	std::string datacardName = "dataCard_run" + ss.str();
	std::string statsName = "runStats_run" + ss.str();
    	newfile->WriteTObject(dataCard_,datacardName.c_str());
    	newfile->WriteTObject(stats_, statsName.c_str());

	count++;

   	//delete from memory
   	this->deleteTrees();
	delete datafile;
   }

   //newtree->Print();
   newfile->Write();
   delete newfile;

   return true;
}

int main(int argc, char ** argv) {
  try {
    copyTree copier;
    copier.getRunTimeArguments(argc, argv);
    copier.loadDataCards();
    //if(!oldfile)return 0;
    copier.copyTrees();
    //plotter.copyTree(oldfile,"testCopy.root","NuInteractions");

  } catch (LbnoException e) {
    std::cout << e.GetLocation() << std::endl;
    std::cout << e.GetDescription() << std::endl;
    return 1;
  }

  return 0;
}

