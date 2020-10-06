// PathB style example file
// ------------------------
// This can be run in two modes depending on the background / signal
// that is being processed. The output is a set of emulated rat MC that
// has some preliminary cuts already applied; with the overall efficiency
// of these cuts stored within the header file.
#include <iostream>
#include <FitterResponseMap.h>
#include <PMTGenerator.h>
#include <MCLookup.h>
#include <ProgressBar.h>
#include <ArgParsePB.h>
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

bool boundary_check(TVector3 v, double r)
{
  double rho = sqrt(v.X()*v.X() + v.Y()*v.Y());
  double abz = abs(v.Z());
  if( (rho <= r) && (abz <= r) )
    return true;
  return false;
}

int main(int argc, char** argv)
{
  ArgParsePB args(argc, argv);
  // Load the correct configuration csv
  string target   = args.target;
  double coverage = args.coverage;
  unsigned radius = args.radius;
  frp::FitterResponseMap ff(target, coverage, radius);


  // arbitrary at the moment
  double mc_max_rho   = radius - 150;
  double fiducial_rho = radius - 700;

  if( args.genpmts )
  {
    unsigned entries = args.pmtcount;
    frp::PMTGenerator pmt(args.radius);

    // Output file
    unique_ptr<TFile> ofile(new TFile("pmtmax.root", "recreate"));
    TTree* T = new TTree("T", "T");
    RAT::DS::Root* ds = new RAT::DS::Root();
    T->Branch("ds", &ds);
    
    // Create a simple position database
    vector<double> xdb, ydb, zdb;
    TTree* dbtree = new TTree("posdb", "posdb");
    dbtree->Branch("xdb", &xdb);
    dbtree->Branch("ydb", &ydb);
    dbtree->Branch("zdb", &zdb);

    int true_entries = 0; // Pass MC boundary cuts
    int keep_entries = 0; // Pass Recon boundary cuts

    ProgressBar pbar(0, entries);
    for(unsigned i=0; i<entries; i++)
    {
      ds->PruneMC();
      ds->PruneEV();
      // MC
      pmt.GenerateNewEvent();
      double mcx = pmt.position_x;
      double mcy = pmt.position_y;
      double mcz = pmt.position_z;
      auto mc = ds->GetMC();
      auto mcp = mc->AddNewMCParticle();
      mcp->SetPDGCode(22);
      mcp->SetPosition(TVector3(mcx, mcy, mcz));
      mcp->SetKE(pmt.energy);
      // Check this event is valid for the geometry configuration
      if( !boundary_check(mc->GetMCParticle(0)->GetPosition(), args.radius) ) continue;
      // true_entries++;
      // EV
      auto ev = ds->AddNewEV(); 
      auto pf = ev->GetPathFit();
      ff.GenerateEvent(mcx, mcy, mcz, 1.0, 0.0, 0.0, pmt.energy, 1.0);
      auto rposition = TVector3(ff.position_x, ff.position_y, ff.position_z);
      pf->SetPosition( rposition );
      ev->SetTotalCharge( pmt.energy );

      // Pruning boundary
      if( !boundary_check(rposition, fiducial_rho) ) continue;
      keep_entries++;

      // Posdb
      xdb.push_back(ff.position_x);
      ydb.push_back(ff.position_y);
      zdb.push_back(ff.position_z);
      T->Fill();
      pbar.update(i);
    }
    dbtree->Fill();

    printf("true: %i, keep %i\n", entries, keep_entries);

    // Header tree
    TTree* header = new TTree("header", "header");
    double efficiency = static_cast<double>(entries)/true_entries;
    header->Branch("efficiency", &efficiency);
    header->Fill();
    ofile->Write(0, TObject::kOverwrite);
  }
  else
  {
    std::string filename;
    if( args.filename == "" )
    {
      frp::MCLookup lookup(target, args.signal);
      filename = lookup.getFilename();
    }
    else
    {
      filename = args.filename;
    }
    printf("Filename: %s\n", filename.c_str());

    // Input files
    unique_ptr<TFile> tfile(new TFile(filename.c_str()));
    TTree* T          = (TTree*)tfile->Get("T");
    TTree* runT       = (TTree*)tfile->Get("runT");
    RAT::DS::Root* ds = new RAT::DS::Root();
    T->SetBranchAddress("ds", &ds);

    // Output file
    unique_ptr<TFile> ofile(new TFile("max.root", "recreate"));
    TTree* newT = T->CloneTree(0);
    TTree* newR = runT->CloneTree();

    // Create a simple position database
    vector<double> xdb, ydb, zdb;
    TTree* dbtree = new TTree("posdb", "posdb");
    dbtree->Branch("xdb", &xdb);
    dbtree->Branch("ydb", &ydb);
    dbtree->Branch("zdb", &zdb);

    int true_entries = 0; // Pass MC boundary cuts
    int keep_entries = 0; // Pass Recon boundary cuts

    unsigned entries = T->GetEntries();
    ProgressBar pbar(0, entries);
    for(unsigned i=0; i<entries; i++)
    {
      T->GetEvent(i);
      auto mc = ds->GetMC();
      // Check this event is valid for the geometry configuration
      if( !boundary_check(mc->GetMCParticle(0)->GetPosition(), mc_max_rho) ) continue;
      true_entries++;
      // Since the posdb only matters for single events, just keep the first
      double xfirst = -999999;
      double yfirst = -999999;
      double zfirst = -999999;

      // Loop through sub event datastructure and fill with map
      bool pass_fv = false;
      for(unsigned sev=0; sev < ds->GetEVCount(); sev++)
      {
        auto ev            = ds->GetEV(sev);
        auto pf            = ev->GetPathFit();
        auto position      = pf->GetPosition();
        double true_energy = ev->GetTotalCharge();
        // Pruning
        if( boundary_check(position, fiducial_rho) ) pass_fv = true;
        // Generate event
        ff.GenerateEvent( position.X(), position.Y(), position.Z(), 1.0,
            0.0, 0.0, true_energy, 1.0 );
        // Fill
        pf->SetPosition(TVector3(ff.position_x, ff.position_y, ff.position_z));
        ev->SetTotalCharge( ff.energy );
        // position database
        if( sev == 0 )
        {
          xfirst = position.X();
          yfirst = position.Y();
          zfirst = position.Z();
        }
      }
      if( !pass_fv ) continue;
      keep_entries++;
      xdb.push_back( xfirst );
      ydb.push_back( yfirst );
      zdb.push_back( zfirst );
      newT->Fill();
      pbar.update(i);
    }
    dbtree->Fill();

    printf("true: %i, keep %i\n", true_entries, keep_entries);

    // Header tree
    TTree* header = new TTree("header", "header");
    double efficiency = static_cast<double>(keep_entries)/true_entries;
    header->Branch("efficiency", &efficiency);
    if( tfile->GetListOfKeys()->Contains("header") )
    {
      TTree* oldheader = (TTree*)tfile->Get("header");
      efficiency = 0;
      double old_eff;
      oldheader->SetBranchAddress("efficiency", &old_eff);
      for(int i=0; i<oldheader->GetEntries(); i++)
      {
        oldheader->GetEvent(i);
        efficiency += old_eff / oldheader->GetEntries();
      }
    }
    header->Fill();

    ofile->Write(0, TObject::kOverwrite);
  }
}
