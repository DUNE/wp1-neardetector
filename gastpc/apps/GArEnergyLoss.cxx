// -------------------------------------------------------------------
/// \file   EventRecordDump.cxx
/// \brief
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 17 Feb 2017
// -------------------------------------------------------------------

#include "RootFileReader.h"
#include "EventRecord.h"
#include "MCGenInfo.h"
#include "MCParticle.h"
#include "MCTrack.h"
#include "MCHit.h"
#include "Units.h"
#include "ELossMeasurement.h"

#include <Ntuple/NtpMCEventRecord.h>
#include <EVGCore/EventRecord.h>

#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>

#include <iostream>
#include <getopt.h>


namespace {
  int event_number_ = -1;
  std::string input_filename_("");
  TH1F* dEdxMeasurements =
    new TH1F("dEdxMeasurements", "", 100, 0., 50.);
}



void PrintUsage()
{
  std::cerr << "\nUsage: EventRecordDump -i <filename> [-n <number>]\n"
            << std::endl;
  std::cerr << "   -i, --input-file    : Input file\n"
            << "   -n, --event-number  : Event to print out (all by default)"
            << std::endl;
  std::exit(EXIT_FAILURE);
}


void ParseCmdLineOptions(int argc, char** argv)
{
  int c;

  static struct option long_options[] =
  {
    {"input-file",   required_argument, 0, 'i'},
    {"event-number", required_argument, 0, 'n'},
    {0, 0, 0, 0}
  };

  while (true) {
    int option_index = 0;
    c = getopt_long(argc, argv, "i:n:", long_options, &option_index);
    if (c == -1) break;
    switch (c) {
      case 'i':
        input_filename_ = std::string(optarg);
        break;
      case 'n':
        event_number_ = std::atoi(optarg);
        break;
      case '?':
        PrintUsage();
      default:
        PrintUsage();
    }
  }

  if (input_filename_ == "") PrintUsage();
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


void ProcessTrack(gastpc::MCTrack* track)
{
  std::multiset<double> dEdx_measurements;
  double step_length = 7.5 * gastpc::mm;

  for (gastpc::MCHit* hit: track->GetMCHits()) {
    double dEdx = hit->GetAmplitude() / step_length;
    dEdxMeasurements->Fill(dEdx/(gastpc::keV/gastpc::cm));
    dEdx_measurements.insert(dEdx);
  }

  double dEdx = TruncatedMean(dEdx_measurements);
  std::cout << "dE/dx = " << dEdx / (gastpc::keV / gastpc::cm) << std::endl;
}


int main(int argc, char* argv[])
{
  ParseCmdLineOptions(argc, argv);

  gastpc::RootFileReader r;
  r.OpenFile(input_filename_);

  TFile file("plots.root", "RECREATE");

  double min = 0.;
  double max = 100. * (gastpc::keV / gastpc::cm);

  for (int i=0; i<r.GetNumberOfEntries(); ++i) {
    gastpc::EventRecord& evtrec = r.Read(i);

    for (gastpc::MCParticle* mcp: evtrec.GetMCParticles()) {

      if (mcp->GetFamilyTreeLevel() == 1) {
        ELossMeasurement eloss(mcp->GetMCTracks()[0]);
        TH1F* histo = eloss.Histogram(min, max, .7);
        //histo->Write();
      }
    }
  }

  r.CloseFile();

  file.Write();
  file.Close();



  return EXIT_SUCCESS;
}
