#include <iostream>
#include <FitterResponseMap.h>
#include <csv.h>

#ifndef DATA_DIRECTORY
  #define DATA_DIRECTORY "./"
#endif

int main(int argc, char** argv)
{
  std::cout << "Hello, world" << std::endl;
  frp::FitterResponseMap ff;
  ff.GenerateEvent(1.0, 2.0, 3.0, 1.0, 2.0, 1.0, 32.0, -12.0);
  printf("%f %f\n", ff.position_x, ff.energy);

  printf("Testing the CSVReader %s\n");
  io::CSVReader<2> in(DATA_DIRECTORY "test.csv");
  in.read_header(io::ignore_extra_column, "First", "3rd Row");
  double a, b;
  while(in.read_row(a, b)){
    printf("%f %f\n", a, b);
  }
}
