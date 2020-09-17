Fitter Response Map
===================

Using an included set of MC, Fitter Response Map applies an additional
smearing to given event coordinates based on a set of input parameters
and response map files.

## Install

## Usage
The core utilities do not need to know about the underlying datastructure,
that is for the user to provide.
```
FitterResponseMap frp(detector_configuration);
for(auto event : event_list) {
  frp.ApplyResponse( event.x, event.y, event.z, event.energy );
}
```

## Examples
