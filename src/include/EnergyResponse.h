#ifndef __EnergyResponse__
#define __EnergyResponse__

#include <map>
#include <array>
#include <limits>
#include <TF1.h>
#include <memory>
#include <string>
#include <sstream>
#include <set>

namespace frp
{
  class EnergyResponse
  {
    protected:
      double minimum_energy;
      double maximum_energy;
      double conversion;
      double mean;
      double resolution;
      std::unique_ptr<TF1> function;
    public:
      EnergyResponse(double, double, double);
      ~EnergyResponse() {}

      double GetRandomEnergy(double, bool);
  };
  // Energy Response Collection
  template<unsigned dims>
  class EnergyResponseCollection
  {
    protected:
      std::map< std::array<std::string, dims>, std::shared_ptr<EnergyResponse> > _map;
      // Energy
      double minimum_energy;
      double maximum_energy;
      bool converted;
      // Temporary container
      std::array<std::string, dims> _nextVector;
      std::array<double, dims> _nextComp;
      std::array<double, dims> _binmin;
      std::array<double, dims> _binmax;
      std::array<double, dims> _binwidth;
    public:
      EnergyResponseCollection(double minE, double maxE, bool conv) :
        minimum_energy(minE), maximum_energy(maxE), converted(conv) {}

      std::string double_as_string(double x)
      {
        std::stringstream ss;
        char buffer[255];
        sprintf(buffer, "%0.2f", x);
        ss << buffer;
        return ss.str();
      }

      template<class...Args>
      void AddResponse(std::shared_ptr<EnergyResponse> erp, double a1, Args... args)
      {
        _nextVector[dims-sizeof...(Args)-1] = double_as_string(a1);
        //printf("a1: %s\n", double_as_string(a1));
        AddResponse(erp, args...);
      }

      void AddResponse(std::shared_ptr<EnergyResponse> erp)
      {
        _map.insert( std::pair< std::array<std::string, dims>, std::shared_ptr<EnergyResponse> >(_nextVector, erp) );
        updateVariables();
      }

      template<class...Args>
      double GetEnergy(double energy, double x, Args... args)
      {
        _nextComp[dims-sizeof...(Args)-1] = x;
        return GetEnergy(energy, args...);
      }

      double GetEnergy(double energy)
      {
        // Loop through dimensions
        for(unsigned i=0; i<dims; i++)
        {
          // Loop through bins, note the values read in the table
          // are for the bin centers. So we need to look up by a
          // half binwidth
          double bwidth = _binwidth[i];
          double value  = _binmin[i] - bwidth/2.;
          int count     = 0;
          //printf("Start at: %f\n", _nextComp[i]);
          while( value <= _binmax[i] + bwidth/2. )
          {
            value = _binmin[i] - bwidth/2. + (count+1)*bwidth;
            //printf("%i > %0.3f\n", value, i);
            if( _nextComp[i] <= value ) break;
            count += 1;
          }
          _nextComp[i] = _binmin[i] + count*bwidth;
          //_nextComp[i] = count >= 0 ? _binmin[i] + count*bwidth : _binmax[i];
          if( _nextComp[i] > _binmax[i] ) _nextComp[i] = _binmax[i];
          //printf("New nv: %f\n", _nextComp[i]);
          _nextVector[i] = double_as_string(_nextComp[i]);
        }
        return (_map.at(_nextVector))->GetRandomEnergy(energy, converted);
      }

      void updateVariables()
      {
        if( _map.size() < 2 ) return;

        for(unsigned i=0; i<dims; i++)
        {
          double xmin   =  std::numeric_limits<double>::max();
          double xmax   = -std::numeric_limits<double>::max();
          std::set<double> unique_bins;
          for(auto x : _map)
          {
            double thisval = std::stod( (x.first).at(i) );
            if( thisval < xmin ) xmin = thisval;
            if( thisval > xmax ) xmax = thisval;
            unique_bins.insert(thisval);
          }
          unsigned count = unique_bins.size();
          double xwidth     = (xmax - xmin)/(count-1);
          //printf("xmin: %f\n", xmin);
          //printf("xmax: %f\n", xmax);
          //printf("xwidth: %f\n", xwidth);
          this->_binmin.at(i)   = xmin;
          this->_binmax.at(i)   = xmax;
          this->_binwidth.at(i) = xwidth;
        }
      }

  };
} // Namespace frp

#endif
