// -------------------------------------------------------------------
/// \file   VertexStats.cxx
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 24 July 2016
// -------------------------------------------------------------------

#include <TChain.h>
#include <TGeoManager.h>
#include <TGeoNode.h>
#include <TGeoVolume.h>
#include <TLorentzVector.h>

#include <Ntuple/NtpMCEventRecord.h>
#include <EVGCore/EventRecord.h>

#include <iostream>
#include <getopt.h>


namespace {
  std::string geometry_file_(""); ///< Filename of input geometry
  std::string genie_files_(""); ///< Filename of input genie files

  struct DetVol {
    std::string name;
    TVector3 position;
    int num_vertices;
  };
}


void ParseCmdLineOptions(int argc, char** argv)
{
  int c;

  while (true) {
    static struct option long_options[] = 
    {
      {"geometry",    required_argument, 0, 'g'},
      {"genie_files", required_argument, 0, 'i'},
      {0, 0, 0, 0}
    };

    int option_index = 0;

    c = getopt_long(argc, argv, "g:i:", long_options, &option_index);

    if (c == -1) break;

    switch (c) {
      case 'g':
        geometry_file_ = std::string(optarg);
        break;
      case 'i':
        genie_files_ = std::string(optarg);
        break;
      case '?':
        exit(EXIT_FAILURE);
        break;
      default:
        exit(EXIT_FAILURE);
    }
  }
}


TGeoManager* LoadGeometry(const std::string& filename)
{
  TGeoManager* gm = new TGeoManager();
  gm->Import(filename.c_str());
  if (!(gm->GetTopNode())) {
    std::cerr << "ERROR: Failed to open geometry file." << std::endl;
    exit(EXIT_FAILURE);
  }
  return gm;
}


TChain* OpenGenieFiles(const std::string& filename)
{
  TChain* chain = new TChain("gtree");
  int num_files;
  num_files = chain->Add(filename.c_str());
  return chain;
}


int main(int argc, char** argv)
{
  ParseCmdLineOptions(argc, argv);

  TGeoManager* geomgr = LoadGeometry(geometry_file_);

  TChain* chain = OpenGenieFiles(genie_files_);
  genie::NtpMCEventRecord* mcrec = 0;
  chain->SetBranchAddress("gmcrec", &mcrec);

  double total_pot = 0.;
  TFile* current_file = 0;
  std::map<TGeoNode*, DetVol> node_map;

  for (int i=0; i<chain->GetEntries(); i++) {

    // Load a new entry from the chain
    chain->GetEntry(i);
    genie::EventRecord* record = mcrec->event;

    // Check whether we are reading a new file. If so, add its weight
    // (POT) to the total for this job.
    TFile* new_file = chain->GetFile();
    if (new_file != current_file) {
      total_pot += chain->GetWeight();
      current_file = new_file;
    }

    // Locate the interaction vertex in the geometry. 
    // (Quantities stored by GENIE are expressed in the SI, while ROOT's
    // TGeoManager uses the CGS. Therefore, we multiply the vertex position
    // by a factor of 100 to transform from m to cm.)
    TGeoNode* node = geomgr->FindNode(record->Vertex()->X() * 100.,
                                      record->Vertex()->Y() * 100.,
                                      record->Vertex()->Z() * 100.);

    // The method above locates the deepest node in the geometry hierarchy.
    // In some cases (ECAL layers and TPC), however, we want to compute 
    // the statistics for a volume higher in the hierarchy.

    if (geomgr->GetLevel() > 2) {
      // GAS is the only volume of level 3 for which we want to compute
      // vertex statistics. For the rest, go one level up.
      std::string node_name(node->GetName());
      std::size_t found = node_name.find("GAS");
      if (found == std::string::npos) {
        geomgr->CdUp();
        node = geomgr->GetCurrentNode();
      } 
    }  

    auto it = node_map.find(node);
    if (it == node_map.end()) {
      DetVol dv;
      dv.name = std::string(node->GetName());
      const Double_t* xyz = node->GetMatrix()->GetTranslation();
      dv.position.SetXYZ(xyz[0], xyz[1], xyz[2]);
      dv.num_vertices = 1;
      node_map[node] = dv;
    }
    else {
      (*it).second.num_vertices++;
    }

  }

  for (auto kv: node_map) {
    std::cout << "----------" << std::endl;
    std::cout << " Name: " << kv.second.name << std::endl;
    std::cout << " Position (cm) = (" << kv.second.position.x() << ", "
	      << kv.second.position.y() << ", " << kv.second.position.z() << ")" << std::endl;
    std::cout << " Num. vertices: " << kv.second.num_vertices << std::endl;
  }

  delete geomgr;
  delete chain;

  return EXIT_SUCCESS;
}
