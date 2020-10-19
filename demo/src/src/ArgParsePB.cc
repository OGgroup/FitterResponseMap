#include <ArgParsePB.h>

ArgParsePB::ArgParsePB(int argc, char** argv)
{
  // defaults
  this->genpmts  = false;
  this->coverage = 0.20;
  this->radius   = 6700;
  this->target   = "wbls_3pct";
  this->signal   = "reactorSignal";
  this->filename = "";
  this->outname  = "output.root";
  this->prune    = 0;
  std::vector<std::string> arguments(argv+1, argv+argc);
  std::string iv = "";
  for(auto v : arguments)
  {
    if( iv == "-t" || iv == "--target" )
      this->target = v;
    if( iv == "-c" || iv == "--coverage" )
      this->coverage = stod(v);
    if( iv == "-r" || iv == "--radius" )
      this->radius = stoi(v);
    if( iv == "-p" || iv == "--prune" )
      this->prune = stod(v);
    if( iv == "-i" || iv == "--input" )
      this->filename = v;
    if( iv == "-o" || iv == "--output" )
      this->outname = v;
    if( iv == "-s" || iv == "--signal" )
      this->signal = v;
    if( v == "--pmt" )
      this->genpmts = true;
    if( iv == "--pmt" )
      this->pmtcount = stoi(v);
    iv = v;
  }
  //check_args();
  if( this->prune == 0 )
    this->prune = this->radius - 700;
}
