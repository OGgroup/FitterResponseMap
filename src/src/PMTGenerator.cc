#include <PMTGenerator.h>
#include <cmath>
#include <TVector3.h>

// Energy Response Collection
namespace frp
{
  // Produce unsmeared PMT events
  PMTGenerator::PMTGenerator(double radius) :
    radius(radius)
  {
    this->function_rho = std::unique_ptr<TF1> (
        new TF1("PMTposRHO", "[0]*exp(-x/[2])+[1]*exp(-x/[3])", 0, radius) );
    this->function_z = std::unique_ptr<TF1> (
        new TF1("PMTposRHO", "[0]*exp(-x/[2])+[1]*exp(-x/[3])", 0, radius) );
    this->function_rho->SetParameters(0.2033, 0.1218, 68.85, 225.64);
    this->function_z->SetParameters(0.2206, 0.1372, 54.85, 212.29);
    this->position_x = 0;
    this->position_y = 0;
    this->position_z = 0;
    this->direction_x = 0;
    this->direction_y = 0;
    this->direction_z = 0;
    rndm = std::shared_ptr<TRandom3>(new TRandom3());
    // Energy?
    this->energy = 2.6;
    // precalculate constants
    wall_area = 2 * M_PI * radius * 2 * radius;
    cap_area = M_PI * radius * radius;
    total_area = wall_area + 2*cap_area;
  }
  void PMTGenerator::GenerateNewEvent()
  {
    // First pick a point uniformly on the cylindrical shell
    double wallx, wally, wallz;
    bool wall_not_cap = rndm->Rndm() < wall_area/total_area ? true : false;
    if( wall_not_cap )
    {
      double random_phi = rndm->Rndm()*2*M_PI;
      wallx = radius * cos( random_phi );
      wally = radius * sin( random_phi );
      wallz = (rndm->Rndm()-0.5)*2*radius;
    }
    else
    {
      int sign = rndm->Rndm() < 0.5 ? -1 : 1;
      wallz = sign*radius;
      double random_phi = rndm->Rndm()*2*M_PI;
      double random_r = sqrt(rndm->Rndm())*radius;
      wallx = random_r * cos( random_phi );
      wally = random_r * sin( random_phi );
    }
    // Now create a MC position and direction
    TVector3 wall_pos( wallx, wally, wallz );
    int rhosign     = rndm->Rndm() < 0.5 ? -1 : 1;
    double deltarho = rhosign*this->function_rho->GetRandom();
    int zsign       = rndm->Rndm() < 0.5 ? -1 : 1;
    double deltaz   = zsign*this->function_z->GetRandom();
    double deltaphi = rndm->Rndm()*2*M_PI;

    TVector3 randomizer( deltarho*cos(deltaphi), deltarho*sin(deltaphi), deltaz );

    TVector3 mcvector = wall_pos + randomizer;
    this->position_x = mcvector.X();
    this->position_y = mcvector.Y();
    this->position_z = mcvector.Z();
    // Direction
    this->direction_x = randomizer.X();
    this->direction_y = randomizer.Y();
    this->direction_z = randomizer.Z();
  }
}
