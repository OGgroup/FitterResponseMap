#include <AstaireDS.h>

AstaireDS::AstaireDS(std::string name)
{
  this->ofile      = std::unique_ptr<TFile>(new TFile(name.c_str(), "recreate"));
  this->data       = new TTree("data", "low-energy detector triggered events");
  this->runSummary = new TTree("runSummary", "mc run summary");
  // Branches Data
  data->Branch("gtid", &gtid);
  data->Branch("mcid", &mcid);
  data->Branch("nhit", &nhit);
  data->Branch("timestamp_s", &timestamp_s);
  data->Branch("timestamp_ns", &timestamp_ns);
  data->Branch("inner_hit", &inner_hit);
  data->Branch("id_plus_dr_hit", &id_plus_dr_hit);
  data->Branch("veto_hit", &veto_hit);
  data->Branch("veto_plus_dr_hit", &veto_plus_dr_hit);
  data->Branch("pe", &pe);
  data->Branch("n9", &n9);
  data->Branch("good_pos", &good_pos);
  data->Branch("good_dir", &good_dir);
  data->Branch("x", &x);
  data->Branch("y", &y);
  data->Branch("z", &z);
  data->Branch("u", &u);
  data->Branch("v", &v);
  data->Branch("w", &w);
  data->Branch("particleCountMC", &particleCountMC);
  data->Branch("mc_energy", &mc_energy);
  data->Branch("mcx", &mcx);
  data->Branch("mcy", &mcy);
  data->Branch("mcz", &mcz);
  data->Branch("mcu", &mcu);
  data->Branch("mcv", &mcv);
  data->Branch("mcw", &mcw);
  data->Branch("closestPMT", &closestPMT);
  data->Branch("dxPrevx", &dxPrevx);
  data->Branch("dyPrevy", &dyPrevy);
  data->Branch("dzPrevz", &dzPrevz);
  data->Branch("dxmcx", &dxmcx);
  data->Branch("dymcy", &dymcy);
  data->Branch("dzmcz", &dzmcz);
  data->Branch("dt_ev", &dt_ev);
  data->Branch("dt_prev", &dt_prev);
  data->Branch("dt0", &dt0);
  // RunSummary
  runSummary->Branch("nEvents", &nEvents);
}

void AstaireDS::FillData()
{
  data->Fill();
}

void AstaireDS::FillRunSummary()
{
  runSummary->Fill();
}

void AstaireDS::Write()
{
  ofile->Write(0, TObject::kOverwrite);
}
