#include <iostream>
#include <FitterResponseMap.h>

int main(int argc, char** argv)
{
  std::cout << "Hello, world" << std::endl;
  frp::FitterResponseMap ff("wbls_3pct", 0.10, 5000);
  ff.SetSeed(123);

  ff.GenerateEvent(1.0, 2.0, 3.0, 1.0, 2.0, 1.0, 32.0, -12.0);
  printf("%f %f\n", ff.position_x, ff.energy);

  ff.WhatDo();
}
