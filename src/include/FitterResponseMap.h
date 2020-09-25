#ifndef __FitterResponseMap__
#define __FitterResponseMap__

#include <string>
#include <map>
#include <TRandom3.h>
#include <EnergyResponse.h>
#include <VertexResponse.h>
#include <memory>
#include <csv.h>

namespace frp
{
  class FitterResponseMap
  {
    public:
      FitterResponseMap(std::string, unsigned, unsigned);
      ~FitterResponseMap();

      void GenerateEvent( double x, double y, double z,
          double u, double v, double w, double energy, double time );

      void ParseMapCSV();

      double direction_x;
      double direction_y;
      double direction_z;
      double position_x;
      double position_y;
      double position_z;
      double energy;
      double time;

      void SetSeed(unsigned _seed){
        this->rndm->SetSeed(_seed);
        if(gRandom) delete gRandom;
        gRandom = this->rndm.get();
      }
      void SelectFile();
      void WhatDo();

    protected:
      std::string target;
      std::string filename;
      std::unique_ptr< io::CSVReader<7> > csvfile;
      unsigned coverage;
      unsigned radius;
      std::shared_ptr<TRandom3> rndm;
      std::shared_ptr< EnergyResponseCollection<3> > energyrc;
      std::shared_ptr< VertexResponseCollection<3> > vertexrc;
  };
} // Namespace frp

#endif
