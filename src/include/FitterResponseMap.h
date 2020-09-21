#ifndef __FitterResponseMap__
#define __FitterResponseMap__

#include <string>

namespace frp
{

  class FitterResponseMap
  {
    public:
      FitterResponseMap();
      ~FitterResponseMap();

      void GenerateEvent( double x, double y, double z,
          double u, double v, double w, double energy, double time );

      void ParseMapCSV(std::string mapname);

      double direction_x;
      double direction_y;
      double direction_z;
      double position_x;
      double position_y;
      double position_z;
      double energy;
      double time;

    protected:
      double nothing;
      std::string mapname;
  };
} // Namespace frp

#endif
