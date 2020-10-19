#ifndef __ArgParsePB__
#define __ArgParsePB__

#include <vector>
#include <string>

class ArgParsePB
{
  public:
    std::string filename;
    std::string outname;
    std::string target;
    std::string signal;
    double coverage;
    unsigned radius;
    unsigned pmtcount;
    double prune;
    bool genpmts;
    ArgParsePB(int argc, char** argv);
  private:
    //void check_args()
    //{
    //  if( (this->filename == "") && !this->genpmts )
    //  {
    //    printf("Either specify an input file with -i or generate "
    //        "pmt events with --pmt. Exiting.\n");
    //    exit(1);
    //  }
    //}
};

#endif
