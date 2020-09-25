#include <FitterResponseMap.h>
#include <csv.h>
#include <sstream>

#ifndef DATA_DIRECTORY
  #define DATA_DIRECTORY "./"
#endif

namespace frp
{
  FitterResponseMap::FitterResponseMap(std::string target, unsigned coverage, unsigned radius) :
    target(target), coverage(coverage), radius(radius)
  {
    this->rndm = std::shared_ptr<TRandom3>(new TRandom3());
    rndm->SetSeed();
    // Initialize the default numbers
    this->direction_x = 0;
    this->direction_y = 0;
    this->direction_z = 0;
    this->position_x  = 0;
    this->position_y  = 0;
    this->position_z  = 0;
    this->energy      = 0;
    this->time        = 0;
    // Load the CSV map
    //this->energyrc = new EnergyResponseCollection<1>(0, 100.0);
    this->energyrc = std::shared_ptr<EnergyResponseCollection<3>>(
        new EnergyResponseCollection<3>(0, 100.0));
    this->vertexrc = std::shared_ptr<VertexResponseCollection<3>>(
        new VertexResponseCollection<3>());
    //this->directionrc = new DirectionResponseCollection<1>(-1, 1); << or something like that
    SelectFile();
    ParseMapCSV();
  }
  FitterResponseMap::~FitterResponseMap() {}

  void FitterResponseMap::GenerateEvent(double x, double y, double z, 
      double u, double v, double w, double energy, double time)
  {
    // Calculate smearing here
    this->position_x  = x;
    this->position_y  = y;
    this->position_z  = z;
    this->direction_x = u;
    this->direction_y = v;
    this->direction_z = w;
    this->energy      = energy;
    this->time        = time;

    // New energy
  }
  void FitterResponseMap::WhatDo()
  {
    double seed_energy = 5.0;
    std::array<double, 3> seed_vtx = {1200.0, 100.0, 1300.0};
    printf("It do what it do\n");
    printf("Randomizing %0.2f %0.2f %0.2f %0.2f\n", seed_energy, seed_vtx[0],
        seed_vtx[1], seed_vtx[2]);
    for(int i=0; i<20; i++)
    {
      double rho = sqrt(seed_vtx[0]*seed_vtx[0] + seed_vtx[1]*seed_vtx[1]);
      double az  = abs(seed_vtx[3]);

      double ash = energyrc->GetEnergy(seed_energy, seed_energy, rho, az);
      std::array<double, 3> ember = vertexrc->GetVertex(seed_vtx, seed_energy, rho, az);

      printf("En: %0.2f %0.2f %0.2f %0.2f\n", ash, ember[0], ember[1], ember[2]);
    }
  }
  void FitterResponseMap::ParseMapCSV()
  {
    this->csvfile->read_header(io::ignore_extra_column, 
        "energy", "rho", "z", "energy_resn", "px", "py", "pz");
    double e, r, z;
    while( csvfile->read_row( e, r, z, 
           energy, position_x, position_y, position_z ) )
    {
      std::shared_ptr<EnergyResponse> ersp(new EnergyResponse(energy, -100.0, 100.0));
      energyrc->AddResponse( ersp, e, r, z );
      std::shared_ptr<VertexResponse> vrsp(new VertexResponse(position_x, position_y, position_z));
      vertexrc->AddResponse( vrsp, e, r, z );
    }
  }
  void FitterResponseMap::SelectFile()
  {
    std::stringstream ss;
    ss << "ReconstructionMap_"
       << this->target << "_"
       << this->coverage
       << this->radius << ".csv";
    this->filename = ss.str();
    // overwrite for now
    this->filename = DATA_DIRECTORY "example.csv";
    printf("Opening: %s\n", this->filename.c_str());
    this->csvfile = std::unique_ptr< io::CSVReader<7> >(new io::CSVReader<7>(filename));
  }
  double GetEnergy( double x, double y, double z,
      double u, double v, double w, double energy )
  {
    return energy;
  }
}
