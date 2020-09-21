#include <FitterResponseMap.h>
#include <csv.h>
#include <fstream>


namespace frp
{
  FitterResponseMap::FitterResponseMap()
  {
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
  }
  void ParseMapCSV(std::string mapname)
  {

  }
}
