// -------------------------------------------------------------------
/// \file   EcalPid.h
/// \brief
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 21 Feb 2017
// -------------------------------------------------------------------

#ifndef ECAL_PID_H
#define ECAL_PID_H

#include <vector>

class TH1F;


/// TODO: CLASS DESCRIPTION

class EcalPid
{
public:
  /// Constructor
  EcalPid();
  /// Destructor
  ~EcalPid();

  double LLR_MIP_EM_muon() const;
  double LLR_MIP_EM_electron() const;
  double LLR_MIP_PION_muon() const;
  double LLR_MIP_PION_pion() const;
  double LLR_EM_HIP_electron() const;
  double LLR_EM_HIP_proton() const;

private:
  void FillHistogram(TH1F*, const std::vector<std::vector<double>>&);
  std::vector<std::vector<double>> Data_LLR_MIP_EM_muon();
  std::vector<std::vector<double>> Data_LLR_MIP_EM_electron();
  std::vector<std::vector<double>> Data_LLR_MIP_PION_muon();
  std::vector<std::vector<double>> Data_LLR_MIP_PION_pion();
  std::vector<std::vector<double>> Data_LLR_EM_HIP_electron();
  std::vector<std::vector<double>> Data_LLR_EM_HIP_proton();

private:
  TH1F* LLR_MIP_EM_muon_;
  TH1F* LLR_MIP_EM_electron_;
  TH1F* LLR_MIP_PION_muon_;
  TH1F* LLR_MIP_PION_pion_;
  TH1F* LLR_EM_HIP_electron_;
  TH1F* LLR_EM_HIP_proton_;
};

#endif
