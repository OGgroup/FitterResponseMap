#ifndef __ProgressBar__
#define __ProgressBar__
#include <stdio.h>
#include <cmath>

class ProgressBar
{
  public:
    unsigned start;
    unsigned stop;
    int last_increment;
    ProgressBar(unsigned start, unsigned stop):
      start(start), stop(stop) {last_increment=0;}
    void update(unsigned prog);
};

#endif
