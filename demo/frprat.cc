#include <iostream>
#include <FitterResponseMap.h>
#include <memory>
#include <array>
#include <string>
#include <numeric>
#include <vector>
using namespace std;

// Root libraries
#include <TFile.h>
#include <TTree.h>
#include <TVector3.h>
// Rat libraries
#include <RAT/DS/Root.hh>
#include <RAT/DS/MC.hh>
#include <RAT/DS/MCParticle.hh>

int main(int argc, char** argv)
{
  frp::FitterResponseMap ff("wbls_3pct", 10, 5000);
  ff.SetSeed(123);

  // Input files
  unique_ptr<TFile> tfile(new TFile(argv[1]));
  TTree* T = (TTree*)tfile->Get("T");
  RAT::DS::Root* ds = new RAT::DS::Root();
  T->SetBranchAddress("ds", &ds);

  // Output file
  unique_ptr<TFile> ofile(new TFile("max.root", "recreate"));
  unique_ptr<TTree> output(new TTree("output", "output"));
  // I hate ttrees
  double x, y, z, u, v, w, t, energy;
  double mcx, mcy, mcz, mcu, mcv, mcw, mct, mcenergy, mcke;
  int subev, nsubev;
  output->Branch("x", &x);
  output->Branch("y", &y);
  output->Branch("z", &z);
  output->Branch("u", &u);
  output->Branch("v", &v);
  output->Branch("w", &w);
  output->Branch("t", &t);
  output->Branch("energy", &energy);
  output->Branch("mcx", &mcx);
  output->Branch("mcy", &mcy);
  output->Branch("mcz", &mcz);
  output->Branch("mcu", &mcu);
  output->Branch("mcv", &mcv);
  output->Branch("mcw", &mcw);
  output->Branch("mct", &mct);
  output->Branch("mcenergy", &mcenergy);
  output->Branch("subev", &subev);
  output->Branch("nsubev", &nsubev);
  output->Branch("mcke", &mcke);

  // Output files
  for(unsigned ev=0; ev<T->GetEntries(); ev++)
  {
    T->GetEvent(ev);
    // Grab MC Info
    std::vector<double> mcKEnergies;
    auto mc = ds->GetMC();
    int mcpcount = mc->GetMCParticleCount();
    for( int p=0; p<mcpcount; p++ )
    {
      auto particle = mc->GetMCParticle(p);
      mcKEnergies.push_back( particle->GetKE() );
    }
    mcke = accumulate(mcKEnergies.begin(), mcKEnergies.end(), 0.0);
    for(unsigned sev=0; sev<ds->GetEVCount(); sev++)
    {
      auto ev = ds->GetEV(sev);
      auto pf = ev->GetPathFit();
      // Find the values from TrueDAQ
      auto position = pf->GetPosition();
      double true_energy = ev->GetTotalCharge();
      // Generate event
      ff.GenerateEvent( position.X(), position.Y(), position.Z(), 1.0, 0.0, 0.0,
          true_energy, 1.0 );
      // Fill
      x        = ff.position_x;
      y        = ff.position_y;
      z        = ff.position_z;
      energy   = ff.energy;
      mcx      = position.X();
      mcy      = position.Y();
      mcz      = position.Z();
      mcenergy = true_energy;
      subev    = sev;
      nsubev   = ds->GetEVCount();
      //
      output->Fill();
    }
  }
  ofile->Write();
  // Read in the test file
}
