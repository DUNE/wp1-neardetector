// -------------------------------------------------------------------
/// \file   GasTPCReco.cxx
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 9 Aug 2016
// -------------------------------------------------------------------

#include "RootFileReader.h"
#include "EventRecord.h"
#include "MCTrack.h"
#include "MCHit.h"
#include "MCParticle.h"
#include "MCGenInfo.h"
#include "DstWriter.h"
#include "DstEntry.h"
#include "Units.h"

#include <TRandom3.h>
#include <TFile.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TLorentzVector.h>

#include <Ntuple/NtpMCEventRecord.h>
#include <Interaction/Interaction.h>

#include <getopt.h>
#include <iostream>
#include <cmath>
#include <set>


namespace {

  DstWriter* dst_ = 0;
  TRandom3* rnd_  = 0;
  int rnd_seed_   = 0;          ///< Random seed
  std::string mode_("");        ///< neutrino or antineutrino
  std::string input_file_("");  ///< Name of input file
  std::string output_file_(""); ///< Name of output file

  struct ParticleContent {
    int num_muons;
    int num_protons;
    int num_pizeroes;
    int num_picharged;
    int num_electrons;
    int num_wsmuons;
    int num_wselectrons;
    int num_other;
  };

  struct TrackInfo {
    bool   is_reco;
    double pdg;
    double length;
    double length_T;
    double momentum_reco;
    double momentum;
    int    num_hits;
    int    track_id;
  };

}


void PrintUsage()
{
  std::cerr << "Usage: GasTPCReco [-r <number>] -m <string>\n"
            << "                   -i <string> -o <string>"
            << "\n" << std::endl;
  std::cerr << "   -r, --random-seed    : Random seed\n"
            << "   -m, --mode           : neutrino or antineutrino\n"
            << "   -i, --input-file     : Input file"
            << "   -o, --output-file    : Output file"
            << std::endl; 

  exit(EXIT_FAILURE);
}


void ParseCmdLineOptions(int argc, char** argv)
{
  int c;

  while (true) {

    static struct option long_options[] =
    {
      {"random-seed", required_argument, 0, 'r'},
      {"mode",        required_argument, 0, 'm'},
      {"input-file",  required_argument, 0, 'i'},
      {"output-file", required_argument, 0, 'o'},
      {0, 0, 0, 0}
    };

    int option_index = 0;
    c = getopt_long(argc, argv, "r:m:i:o:", long_options, &option_index);

    if (c == -1) break;

    switch (c) {
      case 'r':
        rnd_seed_ = std::atoi(optarg);
        break;
      case 'm':
        mode_ = std::string(optarg);
        break;
      case 'i':
        input_file_ = std::string(optarg);
        break;
      case 'o':
        output_file_ = std::string(optarg);
        break;
      case '?':
        std::exit(EXIT_FAILURE);
        break;
      default:
        std::exit(EXIT_FAILURE);
    }
  }

  if (input_file_ == "" || output_file_ == "" || mode_ == "") PrintUsage();
}


double SmearEnergyDep(double energy, double resolution=0.1)
{
  energy = energy / gastpc::GeV;
  double sigma = (resolution / std::sqrt(energy)) * energy;
  return rnd_->Gaus(energy, sigma);
}


double Distance(TLorentzVector& initial, TLorentzVector& final)
{
  double diff = (final.X() - initial.X()) * (final.X() - initial.X()) +
                (final.Y() - initial.Y()) * (final.Y() - initial.Y()) +
                (final.Z() - initial.Z()) * (final.Z() - initial.Z());

  return std::sqrt(diff);
}


double TruncatedMean(const std::multiset<double>& entries)
{
  // Calculate the mean of the smallest 70% entries
  int max_n = std::floor(entries.size() * 0.7);
  int n = 0;
  double sum = 0.;

  for (double entry: entries) {
    if (n >= max_n) break;
    sum += entry;
    n++;
  }

  return (sum / max_n);
}


double GetMomentumMag(const TLorentzVector& mom)
{
  return std::sqrt(mom.X()*mom.X() + mom.Y()*mom.Y() + mom.Z()*mom.Z());
}



double SmearPt(double Pt, double length)
{
  double sigma_x = 0.1;  // cm
  double magfield = 0.4; // tesla
  double X0 = 1300.;     // cm

  Pt = Pt / gastpc::GeV;
  length = length / gastpc::cm;

  int N = std::ceil(length);

  double sigma_1 = sigma_x * Pt * Pt * std::sqrt(720/(N+4)) / (0.3 * magfield * length * length);
  double sigma_2 = 0.05 * std::sqrt(1.43 * length / X0) / (magfield * length);

  double sigma = std::sqrt(sigma_1*sigma_1 + sigma_2*sigma_2);

  return rnd_->Gaus(Pt, sigma);
}


double SmearAngle(double angle, double P, double length)
{
  double sigma_x = 0.1; // cm
  double X0 = 1300.;    // cm

  length = length / gastpc::cm;
  P = P / gastpc::GeV;

  int N = std::ceil(length);

  double sigma = sigma_x * std::sqrt(12*(N-1)/(N*(N+1))) + 0.015 * std::sqrt(length/X0) / (std::sqrt(3.) * P);

  return rnd_->Gaus(angle, sigma);
}


double ProcessTrack(gastpc::MCTrack* track, TrackInfo& track_info)
{
  int i = 0;

  std::vector<double> prev = track->GetParticle()->GetInitialXYZT();
  std::vector<double> min = prev;
  std::vector<double> max = prev;
  std::multiset<double> dEdx_measurements;

  double edep = 0.;

  for (gastpc::MCHit* hit: track->GetHits()) {
    
    std::vector<double> curr = hit->GetXYZT();

    if (curr[0] < min[0]) min[0] = curr[0];
    if (curr[1] < min[1]) min[1] = curr[1];
    if (curr[2] < min[2]) min[2] = curr[2];
    if (curr[0] > max[0]) max[0] = curr[0];
    if (curr[1] > max[1]) max[1] = curr[1];
    if (curr[2] > max[2]) max[2] = curr[2];

    //double distance = Distance(curr, prev);
    double distance = 10.;
    double dEdx = hit->GetAmplitude() / distance;
    dEdx_measurements.insert(dEdx);

    edep += hit->GetAmplitude();

    ++i;
    prev = curr;
  }

  //track_info.dEdx = TruncatedMean(dEdx_measurements);
  //track_info.edep = edep;
  //track_info.num_hits = track->GetHits().size();

  double length_t = max[2] - min[2];
  double length_l = max[1] - min[1];

  track_info.length_T = length_t;

  TLorentzVector P4(track->GetParticle()->GetInitialMomentum()[0],
                    track->GetParticle()->GetInitialMomentum()[1],
                    track->GetParticle()->GetInitialMomentum()[2],
                    0.);

  //std::vector<double> P4 = track->GetParticle()->GetInitial4Momentum();
  double Pmod  = GetMomentumMag(P4);
  double Ptmod = P4.Perp(TVector3(0.,1.,0.));
  double angle = std::asin(Ptmod/Pmod);

  double momentum = 
    SmearPt(Pmod,length_t) / std::sin(SmearAngle(angle, Pmod, length_l));
  track_info.momentum = Pmod;
  track_info.momentum_reco = momentum;

  return momentum;
}


int AnalyzeParticleContent(const ParticleContent& pc)
{
  // std::cout << "ParticleContent: "
  //           << pc.num_muons << " "
  //           << pc.num_wsmuons << " "
  //           << pc.num_electrons << " "
  //           << pc.num_wselectrons << " "
  //           << pc.num_picharged << " "
  //           << pc.num_pizeroes << " "
  //           << pc.num_protons << " "
  //           << pc.num_other << std::endl;

  if ( (pc.num_muons == 1) && (pc.num_wsmuons == 0) &&
       (pc.num_electrons == 0) && (pc.num_wselectrons == 0) &&
       (pc.num_picharged == 0) && (pc.num_pizeroes == 0) &&
       (pc.num_protons == 0) && (pc.num_other == 0) ) return 1;

  else if ( (pc.num_muons == 1) && (pc.num_wsmuons == 0) &&
       (pc.num_electrons == 0) && (pc.num_wselectrons == 0) &&
       (pc.num_picharged == 0) && (pc.num_pizeroes == 0) &&
       (pc.num_protons == 1) && (pc.num_other == 0) ) return 2;

  else if ( (pc.num_muons == 1) && (pc.num_wsmuons == 0) &&
       (pc.num_electrons == 0) && (pc.num_wselectrons == 0) &&
       (pc.num_picharged == 0) && (pc.num_pizeroes == 0) &&
       (pc.num_protons > 1) && (pc.num_other == 0) ) return 11;

  else if ( (pc.num_muons == 1) && (pc.num_wsmuons == 0) &&
       (pc.num_electrons == 0) && (pc.num_wselectrons == 0) &&
       (pc.num_picharged == 1) && (pc.num_pizeroes == 0) &&
       (pc.num_protons >= 0) && (pc.num_other == 0) ) return 3;

  else if ( (pc.num_muons == 1) && (pc.num_wsmuons == 0) &&
       (pc.num_electrons == 0) && (pc.num_wselectrons == 0) &&
       (pc.num_picharged == 0) && (pc.num_pizeroes == 1) &&
       (pc.num_protons >= 0) && (pc.num_other == 0) ) return 5;

  else if ( (pc.num_muons == 1) && (pc.num_wsmuons == 0) &&
       (pc.num_electrons == 0) && (pc.num_wselectrons == 0) &&
       (pc.num_picharged == 1) && (pc.num_pizeroes == 1) &&
       (pc.num_protons >= 0) && (pc.num_other == 0) ) return 6;

  else if ( (pc.num_muons == 1) && (pc.num_wsmuons == 0) &&
       (pc.num_electrons == 0) && (pc.num_wselectrons == 0) ) return 7;

  else if ( (pc.num_muons == 0) && (pc.num_wsmuons == 1) &&
       (pc.num_electrons == 0) && (pc.num_wselectrons == 0) &&
       (pc.num_picharged == 0) && (pc.num_pizeroes == 0) &&
       (pc.num_protons >= 0) && (pc.num_other == 0) ) return 13;

  else if ( (pc.num_muons == 0) && (pc.num_wsmuons == 1) &&
       (pc.num_electrons == 0) && (pc.num_wselectrons == 0) &&
       (pc.num_picharged == 1) && (pc.num_pizeroes == 0) &&
       (pc.num_protons >= 0) && (pc.num_other == 0) ) return 14;

  else if ( (pc.num_muons == 0) && (pc.num_wsmuons == 1) &&
       (pc.num_electrons == 0) && (pc.num_wselectrons == 0) &&
       (pc.num_picharged == 0) && (pc.num_pizeroes == 1) &&
       (pc.num_protons >= 0) && (pc.num_other == 0) ) return 15;

  else if ( (pc.num_muons == 0) && (pc.num_wsmuons == 1) &&
       (pc.num_electrons == 0) && (pc.num_wselectrons == 0) ) return 16;

  else if ( (pc.num_muons == 0) && (pc.num_wsmuons == 0) &&
       (pc.num_electrons == 1) && (pc.num_wselectrons == 0) &&
       (pc.num_picharged == 0) && (pc.num_pizeroes == 0) &&
       (pc.num_protons == 0) && (pc.num_other == 0) ) return 25;

  else if ( (pc.num_muons == 0) && (pc.num_wsmuons == 0) &&
       (pc.num_electrons == 1) && (pc.num_wselectrons == 0) &&
       (pc.num_picharged == 0) && (pc.num_pizeroes == 0) &&
       (pc.num_protons >= 0) && (pc.num_other == 0) ) return 17;

  else if ( (pc.num_muons == 0) && (pc.num_wsmuons == 0) &&
       (pc.num_electrons == 1) && (pc.num_wselectrons == 0) &&
       (pc.num_picharged == 1) && (pc.num_pizeroes == 0) &&
       (pc.num_protons >= 0) && (pc.num_other == 0) ) return 18;

  else if ( (pc.num_muons == 0) && (pc.num_wsmuons == 0) &&
       (pc.num_electrons == 1) && (pc.num_wselectrons == 0) &&
       (pc.num_picharged == 0) && (pc.num_pizeroes == 1) &&
       (pc.num_protons >= 0) && (pc.num_other == 0) ) return 19;

  else if ( (pc.num_muons == 0) && (pc.num_wsmuons == 0) &&
       (pc.num_electrons == 1) && (pc.num_wselectrons == 0) ) return 20;

  else if ( (pc.num_muons == 0) && (pc.num_wsmuons == 0) &&
       (pc.num_electrons == 0) && (pc.num_wselectrons == 1) ) return 27;

  else if ( (pc.num_muons == 0) && (pc.num_wsmuons == 0) &&
       (pc.num_electrons == 0) && (pc.num_wselectrons == 0) &&
       (pc.num_picharged == 0) && (pc.num_pizeroes == 0) &&
       (pc.num_protons >= 1) && (pc.num_other == 0) ) return 21;

  else if ( (pc.num_muons == 0) && (pc.num_wsmuons == 0) &&
       (pc.num_electrons == 0) && (pc.num_wselectrons == 0) &&
       (pc.num_picharged == 1) && (pc.num_pizeroes == 0) &&
       (pc.num_protons >= 0) && (pc.num_other == 0) ) return 22;

  else if ( (pc.num_muons == 0) && (pc.num_wsmuons == 0) &&
       (pc.num_electrons == 0) && (pc.num_wselectrons == 0) &&
       (pc.num_picharged == 0) && (pc.num_pizeroes == 1) &&
       (pc.num_protons >= 0) && (pc.num_other == 0) ) return 23;

  // else if ( (pc.num_muons == 0) && (pc.num_wsmuons == 0) &&
  //       (pc.num_electrons == 0) && (pc.num_wselectrons == 0) &&
  //       (pc.num_picharged >= 1) && (pc.num_pizeroes = 0) &&
  //       (pc.num_protons >= 1) && (pc.num_other == 0) ) return 17;

  else return -1;
}


int main(int argc, char* argv[])
{
  if (argc < 4) PrintUsage();
  ParseCmdLineOptions(argc, argv);

  rnd_ = new TRandom3(rnd_seed_);

  TFile* file = new TFile(input_file_.c_str());
  TTreeReader rd("EventRecord", file);
  TTreeReaderValue<gastpc::EventRecord> rv(rd, "EventRecord");

  dst_ = new DstWriter();
  dst_->OpenFile(output_file_);

  int pdg_muon, pdg_electron;
  if (mode_=="neutrino") {
    pdg_muon = 13;
    pdg_electron = 11;
  }
  else {
    pdg_muon = -13;
    pdg_electron = -11;
  }

  // Loop through the simulated spills/events

  while (rd.Next()) {

    // Loop through the primary interactions simulated in this spill

    for (gastpc::MCGenInfo* nuint: rv->GetMCGenInfo()) {

      // Locate argon interactions
      genie::NtpMCEventRecord* gmcrec = nuint->GetGeneratorRecord();
      genie::Interaction* interaction = (gmcrec->event)->Summary();
      const genie::Target& tgt = interaction->InitState().Tgt();
      if (tgt.Z() != 18) continue;

      DstEntry entry;

      double energy_reco = 0.;
      double energy_nu = 
        (interaction->InitState().ProbeE(genie::kRfLab)) * gastpc::GeV;
      double Y = -1.;
      double Y_reco = 0.;
      std::vector<TrackInfo> trackinfo_v;
      ParticleContent pc{ 0, 0, 0, 0, 0, 0, 0, 0 };

      entry.RunID   = rv->GetRunID();
      entry.EventID = rv->GetEventID();

      TLorentzVector* vertex = gmcrec->event->Vertex();
      entry.VertexPosition[0] = vertex->X();
      entry.VertexPosition[1] = vertex->Y();
      entry.VertexPosition[2] = vertex->Z();
      entry.VertexPosition[3] = vertex->T();

      genie::NtpMCEventRecord* gmcrec_copy = new genie::NtpMCEventRecord();
      gmcrec_copy->Copy(*gmcrec);
      entry.gmcrec = gmcrec_copy;
      //entry.gmcrec = gmcrec;

      // Loop through the primary particles
      for (gastpc::MCParticle* mcp: nuint->GetParticles()) {

        int pdg = mcp->GetPDGCode();
        int trackid = mcp->GetMCID();

        // MUONS ///////////////////////////////////////////
        if (std::abs(pdg) == 13) {

          for (gastpc::MCTrack* mct: mcp->GetTracks()) {

            if (mct->GetLabel() != "TPC") continue;

            TrackInfo ti{false, 0., 0., 0., 0., 0., 0, 0 };
            ti.pdg = pdg;
            ti.track_id = trackid;

            double measured_mom = ProcessTrack(mct, ti) * gastpc::GeV;

            if (ti.length_T < (4.*gastpc::cm)) continue;

            if (pdg == pdg_muon) pc.num_muons += 1;
            else pc.num_wsmuons += 1;

            ti.is_reco = true;

            double mass_muon = 105.6583715 * gastpc::MeV;
            double energy = 
              std::sqrt( measured_mom * measured_mom + mass_muon * mass_muon);

            energy_reco += energy;
            Y_reco = energy;
            //Y = mct->GetParticle()->GetInitial4Momentum().E();
            Y = std::sqrt(ti.momentum*ti.momentum + mass_muon*mass_muon);
            trackinfo_v.push_back(ti);
          }
        }
        // PROTONS /////////////////////////////////////////
        else if (pdg == 2212) {

          for (gastpc::MCTrack* mct: mcp->GetTracks()) {

            if (mct->GetLabel() != "TPC") continue;

            TrackInfo ti{false, 0., 0., 0., 0., 0., 0, 0 };
            double measured_mom = ProcessTrack(mct, ti) * gastpc::GeV;

            if (ti.length_T < (4.*gastpc::cm)) continue;

            pc.num_protons += 1;            
            ti.is_reco = true;
            ti.pdg = pdg;
            ti.track_id = trackid;
            
            energy_reco += measured_mom;
            trackinfo_v.push_back(ti);
          }
        }
        // ELECTRONS ///////////////////////////////////////
        else if (std::abs(pdg) == 11) {

          for (gastpc::MCTrack* mct: mcp->GetTracks()) {

            if (mct->GetLabel() != "TPC") continue;

            TrackInfo ti{false, 0., 0., 0., 0., 0., 0, 0 };
            double measured_mom = ProcessTrack(mct, ti) * gastpc::GeV;

            if (ti.length_T < (4.*gastpc::cm)) continue;

            if (pdg == pdg_electron) pc.num_electrons += 1;
            else pc.num_wselectrons += 1;

            ti.is_reco = true;
            ti.pdg = pdg;
            ti.track_id = trackid;
            
            double mass_electron = 0.511 * gastpc::MeV;
            double energy = 
              std::sqrt( measured_mom * measured_mom + mass_electron * mass_electron);

            energy_reco += energy;
            Y_reco = energy;
            //Y = mct->GetParticle()->GetInitial4Momentum().E();
            Y = std::sqrt(ti.momentum*ti.momentum + mass_electron*mass_electron);
            trackinfo_v.push_back(ti);
          }
        }
        // CHARGED PIONS ///////////////////////////////////
        else if (std::abs(pdg) == 211) {

          for (gastpc::MCTrack* mct: mcp->GetTracks()) {

            if (mct->GetLabel() != "TPC") continue;

            TrackInfo ti{false, 0., 0., 0., 0., 0., 0, 0 };
            double measured_mom = ProcessTrack(mct, ti) * gastpc::GeV;

            if (ti.length_T < (4.*gastpc::cm)) continue;

            pc.num_picharged += 1;
            ti.is_reco = true;
            ti.pdg = pdg;
            ti.track_id = trackid;

            double mass_pion = 139.57018 * gastpc::MeV;
            double energy = 
              std::sqrt( measured_mom * measured_mom + mass_pion * mass_pion);

            energy_reco += energy;
            trackinfo_v.push_back(ti);
          }
        }
        // PI ZEROES ///////////////////////////////////////
        else if (pdg == 111) {

          pc.num_pizeroes += 1;

          TrackInfo ti{true, 0., 0., 0., 0., 0., 0, 0 };
          ti.pdg = pdg;
          ti.track_id = trackid;

          //double energy = mcp->GetInitial4Momentum().E();
          double energy = 1.;
          energy = energy * gastpc::GeV;
          energy_reco += SmearEnergyDep(energy);
          trackinfo_v.push_back(ti);
        }        
      } // for (gastpc::MCParticle* mcp: nuint->GetParticles())

      entry.Sample  = AnalyzeParticleContent(pc);
      entry.Ev_reco = energy_reco;
      entry.Ev      = energy_nu;
      entry.Y       = 1. - Y / energy_nu;
      entry.Y_reco  = 1. - Y_reco / energy_reco;

      int num_tracks = trackinfo_v.size();
      entry.NTracks = num_tracks;

      for (int i=0; i<num_tracks; ++i) {
        entry.TrackID[i]       = trackinfo_v[i].track_id;
        entry.Pdg[i]           = trackinfo_v[i].pdg;
        entry.Momentum_reco[i] = trackinfo_v[i].momentum_reco;
        entry.Momentum[i]      = trackinfo_v[i].momentum;
        entry.RecoTrack[i]     = trackinfo_v[i].is_reco;
      }

      dst_->Write(entry);
      trackinfo_v.clear();
      entry.gmcrec->Clear();

    } // for (gastpc::NuInteraction* nuint: rv->GetNuInteractions())
  } // while (rd.Next())

  dst_->CloseFile();


  return 0;
}