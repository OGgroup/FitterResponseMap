#ifndef __AstaireDS__
#define __AstaireDS__

#include <string>
#include <memory>
#include <TFile.h>
#include <TTree.h>

class AstaireDS
{
  private:
    std::unique_ptr<TFile> ofile;
    TTree* data;
    TTree* runSummary;
  public:
    AstaireDS(std::string);
    // Functions
    void FillData();
    void FillRunSummary();
    void Write();
    // Data members
    int gtid = 0;
    int mcid = 0;
    int nhit = 0;
    int timestamp_s = 0;
    int timestamp_ns = 0;
    int inner_hit = 0;
    int id_plus_dr_hit = 0;
    int veto_hit = 0;
    int veto_plus_dr_hit = 0;
    double pe = 0;
    int n9   = 0;
    int n400 = 0;
    int n100 = 0;
    double good_pos = 0;
    double good_dir = 0;
    double x = 0;
    double y = 0;
    double z = 0;
    double u = 0;
    double v = 0;
    double w = 0;
    int particleCountMC = 0;
    double mc_energy = 0;
    double mcx = 0;
    double mcy = 0;
    double mcz = 0;
    double mct = 0;
    double mcu = 0;
    double mcv = 0;
    double mcw = 0;
    double closestPMT = 0;
    double dxPrevx = 0;
    double dyPrevy = 0;
    double dzPrevz = 0;
    double dxmcx = 0;
    double dymcy = 0;
    double dzmcz = 0;
    double dt_ev = 0;
    double dt_prev = 0;
    double dt0 = 0;
    // Run summary
    int nEvents = 0;
};

#endif
