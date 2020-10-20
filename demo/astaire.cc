// Fred Style output
// -----------------
// This can be run in two modes depending on the background / signal
// that is being processed. The output is a set of emulated rat ntuples
#include <iostream>
#include <FitterResponseMap.h>
#include <PMTGenerator.h>
#include <MCLookup.h>
#include <ProgressBar.h>
#include <ArgParsePB.h>
#include <AstaireDS.h>

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
#include <RAT/DS/PMT.hh>
#include <RAT/DS/EV.hh>
#include <RAT/DS/MCParticle.hh>

bool boundary_check(TVector3 v, double r)
{
  double rho = sqrt(v.X()*v.X() + v.Y()*v.Y());
  double abz = abs(v.Z());
  if( (rho <= r) && (abz <= r) )
    return true;
  return false;
}

double dwall( TVector3 v, double wall );

int main(int argc, char** argv)
{
  ArgParsePB args(argc, argv);
  // Load the correct configuration csv
  string target   = args.target;
  double coverage = args.coverage;
  unsigned radius = args.radius;
  frp::FitterResponseMap ff(target, coverage, radius, false);
  // Output file
  AstaireDS fred(args.outname);

  // arbitrary at the moment
  double mc_max_rho   = radius;

  if( args.genpmts )
  {
    unsigned entries = args.pmtcount;
    frp::PMTGenerator pmt(args.radius);


    int true_entries = 0; // Pass MC boundary cuts

    ProgressBar pbar(0, entries);
    for(unsigned i=0; i<entries; i++)
    {
      // MC
      pmt.GenerateNewEvent();
      fred.mcx       = pmt.position_x;
      fred.mcy       = pmt.position_y;
      fred.mcz       = pmt.position_z;
      fred.mc_energy = pmt.energy;
      fred.mcid      = i;
      // EV
      ff.GenerateEvent(fred.mcx, fred.mcy, fred.mcz, 1.0, 0.0, 0.0, pmt.energy, 1.0);
      fred.x          = ff.position_x;
      fred.y          = ff.position_y;
      fred.z          = ff.position_z;
      fred.closestPMT = dwall( TVector3(ff.position_x, ff.position_y, ff.position_z), radius );
      // Energy estimates
      fred.inner_hit   = round(ff.energy);
      fred.n9          = round(ff.energy);
      fred.n100        = round(ff.energy);
      fred.n400        = round(ff.energy);
      fred.gtid      = i;
      // All good
      fred.FillData();
      pbar.update(i);
    }

    // Run Summary
    fred.nEvents = static_cast<int>(entries);
    fred.FillRunSummary();

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

    int true_entries = 0; // Pass MC boundary cuts
    int trigger_count = 0;
    const double biasgd = 0.30;
    const double biash = 0.15;

    unsigned entries = T->GetEntries();
    ProgressBar pbar(0, entries);
    for(unsigned i=0; i<entries; i++)
    {
      T->GetEvent(i);
      auto mc = ds->GetMC();
      // Check this event is valid for the geometry configuration
      if( !boundary_check(mc->GetMCParticle(0)->GetPosition(), mc_max_rho) ) continue;

      // Loop through sub event datastructure and fill with map
      bool pass_fv = false;
      for(unsigned sev=0; sev < ds->GetEVCount(); sev++)
      {
        auto ev            = ds->GetEV(sev);
        auto pf            = ev->GetPathFit();
        auto position      = pf->GetPosition();
        double true_energy = ev->GetTotalCharge();
        auto ctype         = pf->GetTime0();
        // Generate event
        double comp_energy = 0;
        for(unsigned npmt=0; npmt < ev->GetPMTCount(); npmt++)
        {
          auto pmt = ev->GetPMT(npmt);
          auto pdg = pmt->GetID();
          double composite_energy = pmt->GetCharge() - biash;
          if( (ctype == 1000641560) || (ctype == 1000641580) )
            composite_energy = pmt->GetCharge() - biasgd;
          ff.GenerateEvent( position.X(), position.Y(), position.Z(), 1.0,
              0.0, 0.0, composite_energy, 1.0 );
          if( composite_energy > 0.0 )
            comp_energy += ff.energy;
        }
        ff.GenerateEvent( position.X(), position.Y(), position.Z(), 1.0,
            0.0, 0.0, true_energy, 1.0 );
        // Fill
        if( (ctype == 1000641560) || (ctype == 1000641580) || (ctype == 1000010020) )
          ff.energy = comp_energy;
        // Fill
        fred.mcid        = true_entries;
        fred.mcx         = position.X();
        fred.mcy         = position.Y();
        fred.mcz         = position.Z();
        fred.mc_energy   = true_energy;
        fred.x           = ff.position_x;
        fred.y           = ff.position_y;
        fred.z           = ff.position_z;
        fred.closestPMT  = dwall( TVector3(ff.position_x, ff.position_y, ff.position_z), radius );
        fred.inner_hit   = round(ff.energy);
        fred.n9          = round(ff.energy);
        fred.n100        = round(ff.energy);
        fred.n400        = round(ff.energy);
        fred.gtid        = trigger_count;
        // Done
        trigger_count++;
        fred.FillData();
      }
      true_entries++;
      pbar.update(i);
    }
    fred.nEvents = true_entries;
    fred.FillRunSummary();
  }
  fred.Write();
}

double dwall( TVector3 v, double wall )
{
  double rho   = sqrt( v.X()*v.X() + v.Y()*v.Y() );
  double z     = v.Z();
  double dcap  = wall - abs(z);
  double dside = wall - rho;

  return min(dcap, dside);
}
