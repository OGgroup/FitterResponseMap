#include <EnergyResponse.h>

// Energy Response Collection
namespace frp
{
  EnergyResponse::EnergyResponse(double resolution, double min_energy, double max_energy) :
    resolution(resolution), minimum_energy(min_energy), maximum_energy(max_energy)
  {
    this->function = std::unique_ptr<TF1> (
      new TF1("ERFunction", "1/sqrt(2*pi*[0]^2)*exp(-(x-[1])^2/2/[0]^2)", min_energy, max_energy) );
    this->function->SetParameter(0, resolution);
    this->function->SetParameter(1, 0);
  }

  double EnergyResponse::GetRandomEnergy(double energy)
  {
    this->function->SetParameter(1, energy);
    return this->function->GetRandom();
  }
}
