#include <FitterResponseMap.h>
#include <csv.h>
#include <sstream>
#include <cstdlib>
#include <iostream>
#include <fstream>

#ifndef DATA_DIRECTORY
  #define DATA_DIRECTORY "./"
#endif

namespace frp
{
  FitterResponseMap::FitterResponseMap(std::string target, double coverage, unsigned radius) :
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
    this->direction_x = u;
    this->direction_y = v;
    this->direction_z = w;
    this->time        = time;
    // New energy
    double rho   = sqrt(x*x+y*y);
    double absz  = abs(z);
    this->energy = energyrc->GetEnergy( energy, energy, rho, absz );
    // New vertex
    std::array<double, 3> fitposition = vertexrc->GetVertex({x, y, z}, energy, rho, absz);
    this->position_x = fitposition[0];
    this->position_y = fitposition[1];
    this->position_z = fitposition[2];
    // New direction
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
        "energy", "rho", "z", "emean", "eres", "px", "py", "pz", "dtheta");
    double e, r, z;
    double emean, eres, px, py, pz, dtheta;
    while( csvfile->read_row( e, r, z, 
           emean, eres, px, py, pz, dtheta ) )
    {
      std::shared_ptr<EnergyResponse> ersp(
          new EnergyResponse(e, emean, eres ));
      energyrc->AddResponse( ersp, e, r, z );
      std::shared_ptr<VertexResponse> vrsp(
          new VertexResponse(px, py, pz));
      vertexrc->AddResponse( vrsp, e, r, z );
    }
  }
  void FitterResponseMap::SelectFile()
  {
    // Default to pathfitter, but allow selection
    std::string dict_name = DATA_DIRECTORY "response/pathfitter/response_dictionary.json";
    std::ifstream file;
    file.open(dict_name);
    this->csvdict = std::unique_ptr<json::Reader>( new json::Reader(file) );
    // Top level
    json::Value toplevel;
    std::string csvfilename;
    bool filenotfound = true;
    while(csvdict->getValue(toplevel))
    {
      std::vector<std::string> keys = toplevel.getMembers();
      for(auto v : keys)
      {
        if( v == this->target )
        {
          json::Value coveragedict = toplevel.getMember( v );
          std::vector<std::string> covkeys = coveragedict.getMembers();
          for(auto cc : covkeys)
          {
            if( stod(cc) == this->coverage )
            {
              json::Value pmtdict = coveragedict.getMember(cc);
              std::vector<std::string> pmtkeys = pmtdict.getMembers();
              for(auto pp : pmtkeys)
              {
                if( stoi(pp) == this->radius )
                {
                  csvfilename = pmtdict.getMember(pp).getString();
                  filenotfound = false;
                }
              }
            }
          } // Read radii
        } // Read coverages
      } // Read targets
    } // Read dictionary end
    if( filenotfound )
    {
      printf("File for %s, %f, %i not in dictionary\n", target.c_str(), coverage, radius);
      std::exit(1);
    }
    printf("Reading map from %s\n", csvfilename.c_str());
    //
    this->filename = DATA_DIRECTORY "response/pathfitter/" + csvfilename;
    printf("Opening: %s\n", this->filename.c_str());
    this->csvfile = std::unique_ptr< io::CSVReader<9> >(new io::CSVReader<9>(filename));
  }
  double GetEnergy( double x, double y, double z,
      double u, double v, double w, double energy )
  {
    return energy;
  }
}
