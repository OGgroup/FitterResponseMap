#ifndef __PMTGenerator__
#define __PMTGenerator__

#include <string>
#include <map>
#include <TRandom3.h>
#include <EnergyResponse.h>
#include <VertexResponse.h>
#include <memory>
#include <csv.h>
#include <json.h>

namespace frp
{
  class PMTGenerator
  {
    public:
      PMTGenerator(double);
      ~PMTGenerator() {}

      void GenerateNewEvent();

      double direction_x;
      double direction_y;
      double direction_z;
      double position_x;
      double position_y;
      double position_z;
      double energy;
      double wall_area;
      double cap_area;
      double total_area;

    protected:
      double radius;
      std::unique_ptr<TF1> function_rho;
      std::unique_ptr<TF1> function_z;
      std::shared_ptr<TRandom3> rndm;
  };
} // Namespace frp

#endif
