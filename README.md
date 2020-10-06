Fitter Response Map
===================

[![][travis-img]][travis-url]

Using an included set of MC, Fitter Response Map applies an additional
smearing to given event coordinates based on a set of input parameters
and response map files. This requires a large set of general MC that is
then placed into a fake detector to generate rat-like MC.

## Install
A simple `Makefile` exists that installs the executables and the library
into a directory within the source `./local`. Begin by downloading and
linking to the data directory where the montecarlo is stored.
```shell
ln -s $(realpath /path/to/truedaq/) data/montecarlo
```
This will allow the full path to insteall with the system share path.

The library can be installed as a system library:
```shell
cmake . -Bbuild
cmake --build build --target install
```
or into the users local install
```shell
cmake . -Bbuild -DCMAKE_INSTALL_PREFIX=~/.local
cmake --build build --target install
```
or into any other path. An example would be to link to local in the
local directory from wmutils.

## Usage
The core utilities do not need to know about the underlying datastructure,
that is for the user to provide.
```c++
#include <FitterResponseMap.h>
#include <PMTGenerator.h>
// Define a detector
std::string target = "wbls_3%";
double coverage = 0.20;
unsigned radius = 5700;
// Read the detector response
FitterResponseMap frp(target, coverage, radius);
// Apply to events / input files
for(auto event : event_list) {
  ff.GenerateEvent( mcx, mcy, mcz, mcu, mcw, mcv, energy, time );
  // Retrieve the fitted information
  double x = ff.position_x;
  double y = ff.position_y;
  double z = ff.position_z;
  double u = ff.direction_x;
  double v = ff.direction_y;
  double w = ff.direction_z;
  double energy = ff.energy;
}
// Or use the pmt generator
frp::PMTGenerator pmt(radius);
pmt.GenerateNewEvent();
double mcx    = pmt.position_x;
double mcy    = pmt.position_y;
double mcz    = pmt.position_z;
double mcu    = pmt.direction_x;
double mcv    = pmt.direction_y;
double mcw    = pmt.direction_z;
double energy = pmt.energy;
// And smear as above
```

---

[travis-img]: https://travis-ci.org/OGgroup/FitterResponseMap.svg?branch-master
[travis-url]: https://travis-ci.org/OGgroup/FitterResponseMap
