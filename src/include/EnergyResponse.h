#ifndef __EnergyResponse__
#define __EnergyResponse__

#include <map>
#include <array>
#include <limits>

namespace frp
{
  class EnergyResponse
  {
    protected:
      double minimum_energy;
      double maximum_energy;
    public:
      EnergyResponse(){}
      ~EnergyResponse(){}
  };
  // Energy Response Collection
  template<unsigned dims>
  class EnergyResponseCollection
  {
    protected:
      std::map< std::array<double, dims>, EnergyResponse > _map;
      // Energy
      double minimum_energy;
      double maximum_energy;
      // Temporary container
      std::array<double, dims> _nextVector;
      std::array<double, dims> _binmin;
      std::array<double, dims> _binmax;
      std::array<double, dims> _binwidth;
    public:
      EnergyResponseCollection(int ndims, double minE, double maxE) :
        minimum_energy(minE), maximum_energy(maxE) {}

      template<typename...Args>
      void AddResponse(EnergyResponse erp, Args... args)
      {
        _nextVector = {args...};
        _map[ _nextVector ] = erp;
        updateVariables();
      }

      void updateVariables()
      {
        if( _map.size() < 2 ) return;

        unsigned ndims = _nextVector.size();
        for(unsigned i=0; i<ndims; i++)
        {
          double xmin   =  std::numeric_limits<double>::max();
          double xmax   = -std::numeric_limits<double>::max();
          double xcount = 0;
          for(auto x : _map)
          {
            double thisval = (x.first).at(i);
            if( thisval < xmin ) xmin = thisval;
            if( thisval > xmax ) xmax = thisval;
            xcount += 1;
          }
          double xwidth     = (xmax - xmin)/xcount;
          this->_binmin.at(i)   = xmin;
          this->_binmax.at(i)   = xmax;
          this->_binwidth.at(i) = xwidth;
        }
      }
  };
} // Namespace frp

#endif
