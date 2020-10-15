#include <MCLookup.h>
#include <sstream>
#include <cstdlib>
#include <iostream>
#include <fstream>

#ifndef DATA_DIRECTORY
  #define DATA_DIRECTORY "./"
#endif

namespace frp
{
  MCLookup::MCLookup(std::string target, std::string component ) :
    target(target), component(component)
  {
    SelectFile();
  }
  MCLookup::~MCLookup() {}

  void MCLookup::SelectFile()
  {
    std::string dict_name = DATA_DIRECTORY "montecarlo/mc_dictionary.json";
    std::ifstream file;
    file.open(dict_name);
    this->mcdict = std::unique_ptr<json::Reader>( new json::Reader(file) );
    // Top level
    json::Value toplevel;
    std::string rootfilename;
    bool filenotfound = true;
    while(mcdict->getValue(toplevel))
    {
      std::vector<std::string> keys = toplevel.getMembers();
      for(auto v : keys)
      {
        if( v == this->target )
        {
          json::Value componentdict = toplevel.getMember( v );
          std::vector<std::string> compkeys = componentdict.getMembers();
          for(auto cc : compkeys)
          {
            if( cc == this->component )
            {
              rootfilename = componentdict.getMember(cc).getString();
              filenotfound = false;
            }
          }
        }
      }
    }
    if( filenotfound )
    {
      printf("MC File for %s, %s not in dictionary\n", target.c_str(), component.c_str() );
      std::exit(1);
    }
    printf("Reading map from %s\n", rootfilename.c_str());
    //
    this->filename = DATA_DIRECTORY + rootfilename;
    printf("Opening: %s\n", this->filename.c_str());
  }
}
