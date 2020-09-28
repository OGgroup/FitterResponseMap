#include <EnergyResponse.h>

// Energy Response Collection
namespace frp
{
  EnergyResponse::EnergyResponse(double energy, double mean_hits, double resolution )
  {
    conversion     = mean_hits / energy;
    minimum_energy = mean_hits - 20*resolution;
    maximum_energy = mean_hits + 20*resolution;
    mean           = mean_hits;
    resolution     = resolution;
    this->function = std::unique_ptr<TF1> (
      new TF1("ERFunction", "1/sqrt(2*pi*[0]^2)*exp(-(x-[1])^2/2/[0]^2)", minimum_energy, maximum_energy) );
    this->function->SetParameter(0, resolution);
    this->function->SetParameter(1, mean_hits);
  }

  double EnergyResponse::GetRandomEnergy(double energy)
  {
    // We choose to assume that the nhit response is locally linear.
    this->function->SetParameter(1, energy*conversion);
    double testenergy = round(this->function->GetRandom());
    testenergy = testenergy > 0 ? testenergy : 0;
    return testenergy;
  }
}
