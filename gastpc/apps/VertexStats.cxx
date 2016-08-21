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
#include <fstream>
#include <getopt.h>


namespace {
  std::string geometry_file_("");         ///< Filename of input geometry
  std::string genie_files_("");           ///< Filename of input genie files
  std::string output_file_("output.txt"); ///< Filename of output file
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
      {"output_file", required_argument, 0, 'o'},
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
      case 'o':
        output_file_ = std::string(optarg);
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
  int num_files = chain->Add(filename.c_str());
  std::cout << "Added " << num_files << " to the chain." << std::endl;
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
  int total_num_vertices = 0;
  TFile* current_file = 0;
  std::map<TGeoNode*, DetVol> node_map;
  std::map<int, int> target_map;

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

    // Create an entry for this geometry node if we have not seen it before.
    // Update the vertex statistics otherwise.
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

    // Store now statistics about the target nucleus
    genie::Interaction* interaction = record->Summary();
    const genie::Target& tgt = interaction->InitState().Tgt();
    target_map[tgt.Z()] += 1;

    // Compute total statistics
    ++total_num_vertices;
  }

  // Write results in the output file

  std::ofstream ofile;
  ofile.open(output_file_, std::ios::trunc);

  ofile << "Total POT analyzed: " << total_pot << std::endl;

  ofile << "Total num. vertices: " << total_num_vertices << std::endl; 

  ofile << "\nVertex distribution per geometry volumes:" << std::endl;

  ofile << "\n----------" << std::endl;

  for (auto kv: node_map) {
    ofile << " Name: " << kv.second.name << std::endl;
    ofile << " Position (cm) = (" << kv.second.position.x() << ", "
	                                    << kv.second.position.y() << ", " 
                                      << kv.second.position.z() << ")" 
                                      << std::endl;
    ofile << " Num. vertices: " << kv.second.num_vertices << std::endl;
    ofile << "----------" << std::endl;
  }

  ofile << "\n\nVertex distribution per target nucleus:\n" << std::endl;
  ofile << "Z\t" << "Num. interactions" << std::endl;

  for (auto kv: target_map) {
    ofile << kv.first << "\t" << kv.second << std::endl;
  }

  ofile.close();

  delete geomgr;
  delete chain;

  return EXIT_SUCCESS;
}
