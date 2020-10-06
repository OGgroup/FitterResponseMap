#ifndef __MCLookup__
#define __MCLookup__

#include <string>
#include <memory>
#include <json.h>

namespace frp
{
  class MCLookup
  {
    public:
      MCLookup(std::string, std::string);
      ~MCLookup();

      void SelectFile();
      std::string getFilename(){return this->filename;}

    protected:
      std::string target;
      std::string component;
      std::string filename;
      std::unique_ptr< json::Reader > mcdict;
  };
} // Namespace frp

#endif
