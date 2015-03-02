#include "LbnoProcessorManager.hh"

LbnoProcessorManager::LbnoProcessorManager() : fileSet(false),datafile_(NULL){
  this->startup();
  this->initDataCards();
}

LbnoProcessorManager::~LbnoProcessorManager(){
   if(datafile_)delete datafile_;
}

void  LbnoProcessorManager::setFile(std::string filename){
  if(fileSet)return;
  datafile_ = new TFile(filename.c_str(), "RECREATE");
  fileSet=true;
  std::cout << "\n************** Going to write to " << filename << "\n" <<std::endl;
}

void LbnoProcessorManager::initDataCards() {
  cards_ = DataCards::getInstance();
  cards_->AddDataCardInt("nIterations", 1);
  cards_->AddDataCardDouble("exposure", 1e6);
  cards_->AddDataCardInt("compression", 9);
  cards_->AddDataCardInt("verbose", 0);
  cards_->AddDataCardString("outputFileName", "nuData.root");
}

void LbnoProcessorManager::loadDataCards() {

  outputFileName_  = cards_->fetchValueString("outputFileName");
  nIterations_     = cards_->fetchValueInt("nIterations");
  exposure_        = cards_->fetchValueDouble("exposure");
  compression_     = cards_->fetchValueInt("compression");
  verbose_         = cards_->fetchValueInt("verbose");
  this->setFile(outputFileName_);
}

void LbnoProcessorManager::getRunTimeArguments(int argc, char ** argv) {
  for (int i=1;i<argc;i++)
    if (argv[i]) {
      std::string cardsfile(argv[i]);
      cards_->readKeys(cardsfile);
    }
    cards_->printToStream(std::cout);
  this->loadDataCards();
}

void LbnoProcessorManager::addProcessor(LbnoProcessor* a) {
  a->loadDataCards();
  a->SetFilePtr(datafile_);
  a->initOutDataTree();
  data_.push_back(a->getOutDataTree());
  processors_.push_back(a);
}

void LbnoProcessorManager::printToStream(ostream& stream) {
  stream << "-------- Processors -------" << std::endl;
  for(unsigned int i=0; i<processors_.size(); i++) {
    stream << processors_[i]->getName() << std::endl;
    //data_[i]->Print(); stream << std::endl;
  }
  stream << "---------------------------" << std::endl;
}

void LbnoProcessorManager::go() {
  // Check the number of the loaded processors.
  if (!processors_.size()) {
    string mesage = "No processors loaded.";
    string location = "void LbnoProcessorManager::go()";
    throw LbnoException(mesage, location, LbnoException::FATAL);
  }

  // Load the data cards needed by the Processor Manager.
  //this->loadDataCards();
/*
  for(unsigned int a=0; a<processors_.size(); a++) {
    // Load the data cards needed by this processor.
    processors_[a]->loadDataCards();
  }

  // Get the number of executions for the first processor.
  int nIterations_ = cards_->fetchValueInt("nIterations");
*/

  // Do the main loop over the loaded processors. 
  int nProcessors = processors_.size();
  int iproc = 0;

  //get flux file info from flux processor
  int fluxProcessorPosition = 0;
  int fluxFileEntries = 0;
  double fluxFileExposure = 0;

  //get the event processor information
  int eventProcessorPosition = 0;

  while (iproc<nProcessors) {
  	if (processors_[iproc]->getName() == "LoadFluxProcessor"){
		LoadFluxProcessor * fluxProcessor = (LoadFluxProcessor*)processors_[iproc];
		fluxProcessorPosition = iproc;
		fluxFileEntries = fluxProcessor->getFileEntries();
		fluxFileExposure = fluxProcessor->getFileExposure();	
		std::cout << "\n-------fluxFileEntries = " << fluxFileEntries
			  << "\n-------fluxFileExposure = " << fluxFileExposure << " p.o.t" <<std::endl;
	}
	if (processors_[iproc]->getName() == "LoadNuEventProcessor"){
		LoadNuEventProcessor * eventProcessor = (LoadNuEventProcessor*)processors_[iproc];
		eventProcessorPosition = iproc;
		fluxFileEntries = eventProcessor->getFileEntries();
		fluxFileExposure = eventProcessor->getRealExposure();
		exposure_ = fluxFileExposure;
		probScale = 1.;
		std::cout << "\n-------eventFileEntries = " << fluxFileEntries
			  << "\n-------eventFileExposure = " << fluxFileExposure << "scaled p.o.t" <<std::endl;
	}
	iproc++;
  }

  //reset processor index
  iproc = 0;

  //use exposure instead of nIterations if nIterations<=0
  //more useful to use exposure than iterations for flux
  if(nIterations_ <=0)nIterations_ = exposure_*fluxFileEntries/fluxFileExposure;
  else exposure_ = nIterations_*fluxFileExposure/fluxFileEntries;

  //loop through the processors
  while (iproc<nProcessors) {
    std::cout << "processing " <<processors_[iproc]->getName() << std::endl;
    std::cout << "number of iterations to be processed : " << nIterations_ << std::endl;
    //std::cout << "corresponds to exposure of : " << exposure_ << std::endl;

    // If this is not the very first processor, load the input data tree. 
    if (iproc > 0) {
     processors_[iproc]->setInDataTreePtr(data_[iproc-1]);
     processors_[iproc]->loadInDataTree();
    }
    //data_[iproc]->Print();
    int ientry = 0;

    //if geant is set with the particle gun then ignore loop
    if (processors_[iproc]->getName() == "Geant4TrackingProcessor") {
        if(processors_[iproc]->getGunOption()) break;
    }

    //loop over each processor
    while(ientry<nIterations_) {
      //only record the entry when the process returns true
      if ( processors_[iproc]->process() ){
	ientry++;
	if(processors_[iproc]->getVerbose()<0){
		if(ientry==0)std::cout << "\nProcessor :" << processors_[iproc]->getName() << " progress\n";
		this->progressBar(ientry,nIterations_,0,100);
	}
      }	
      //if genie processor get prob scale
      if (processors_[iproc]->getName() == "GenieNuEventProcessor") {
	GenieNuEventProcessor * genieProcessor = (GenieNuEventProcessor*)processors_[iproc];
        probScale = genieProcessor->getGlobalProbScale();
      }
      //for the first processor update the number of iterations
      //if (iproc == 0) nIterations_ = processors_[iproc]->getAlgo()->getNoIterations();
    }
    //data_[iproc]->Print();

    ///call the destructor of each processor
    delete processors_[iproc];

    // Set the number of executions for the next processor according to the number
    // of data entries generated by this processor.
    nIterations_ = data_[iproc]->GetEntries();
    std::cout << "number of data entries generated : " << nIterations_ << std::endl << std::endl;

    iproc++;
  }
}

void LbnoProcessorManager::write() {

  if(fileSet){
    // Open output root file
    //std::cout << "Going to write to " << outputFileName_ << std::endl;
    datafile_->SetCompressionLevel(compression_);
    datafile_->cd();
/*
  // Save random generator state to outfile
  TRandom* rand = fluxGenerator.getRand();
  rand->SetNameTitle("randomSeed", "Random seed");
  rand->Write();
*/
    // Create and set run stats
    RunStats stats;
    //add to statistics the exposure first
    stats.setStat("i   Exposure [p.o.t]", (double)exposure_);
    stats.setStat("ii  Global Prob Scale", probScale);
    stats.setStat("iii Real Exposure [p.o.t]", exposure_/probScale);

    //write to tree
    for(unsigned int d=0; d<data_.size(); d++) {
    	//getData(d)->Print();
    	TTree *tree = getData(d);
    	if (!tree->GetEntries() ) {
      		string mesage = "Data Tree " + string(tree->GetName()) + " is empty or corrupted.";
      		string location = "void LbnoProcessorManager::write()";
      		throw LbnoException(mesage, location, LbnoException::FATAL);
    	}

    	tree->Write("", TObject::kOverwrite);
    	stringstream ss;
    	ss << d << " " << tree->GetTitle();
    	//ss << d << " " << getData(d)->GetTitle();
    	stats.setStat(ss.str(), (int)tree->GetEntries());
    }
    stats.printToStream(cout);

    // Write data vards and run stats.
    DataCardsRecord cardsRec;
    datafile_->WriteTObject(&cardsRec, "dataCards");
    datafile_->WriteTObject(&stats, "runStats");

    datafile_->Close();
  }
  else std::cout << "\nERROR: No file set! " << std::endl;
}

// Process has done i out of n rounds,
// and we want a bar of width w and resolution r.
inline void LbnoProcessorManager::progressBar(int x, int n, int r, int w){
    
    //set r to zero to enable default refresh rate to every 1000th iteration
    if (r == 0) r = n*0.001;

    if (r < n) r =n;

    // Only update r times.
    if ( x % (n/r) != 0 ) return;
 
    // Calculuate the ratio of complete-to-incomplete.
    float ratio = x/(float)n;
    int   c     = ratio * w;
 
    // Show the percentage complete.
    printf("Progress: [%3f%%] [", (float)(ratio*100.) );
 
    // Show the load bar.
    for (int x=0; x<c; x++)
       printf("=");
 
    for (int x=c; x<w; x++)
       printf(" ");
 
    // ANSI Control codes to go back to the
    // previous line and clear it.
    printf("]\n\033[F\033[J");
    //printf("\r");
}

void LbnoProcessorManager::startup(){

  std::cout << "\n\t\t  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%"
 	    << "\n\t\t %%                                       		     %% %"
	    << "\n\t\t%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  %"
	    << "\n\t\t%%	____*				 		    %%  %"
	    << "\n\t\t%%	   \\____*     /**----+		 		    %%  %"
	    << "\n\t\t%%	        \\____/         	 		   	    %%  %"
	    << "\n\t\t%%	  	     \\/* 	 	 		    %%  %"
	    << "\n\t\t%%					 		    %%  %"
	    << "\n\t\t%%		  	  SoftLAND	  	  	    %%  %"
	    << "\n\t\t%%    (Simulation sOFTware for LAguna-lbno Near Detector )  %%  %"
	    << "\n\t\t%%	  	 			  		    %%  %"
	    << "\n\t\t%%					  		    %%  %"
	    << "\n\t\t%%	    Yordan Karadzhov and Tom Stainer 	  	    %%  %"
	    << "\n\t\t%%	  					 	    %%  %"
	    << "\n\t\t%%	 						    %%  %"
	    << "\n\t\t%%					 	            %% %"
	    << "\n\t\t%%					  		    %%%"
 	    << "\n\t\t%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n\n" << std::endl;

}


