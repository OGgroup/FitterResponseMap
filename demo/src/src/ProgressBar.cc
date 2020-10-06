#include <ProgressBar.h>


void ProgressBar::update(unsigned prog)
{
  double fraction = static_cast<double>(prog-start)/stop;
  int increment = floor(fraction*100);
  if( increment != last_increment )
  {
    char buffer[256];
    printf(":: %i\%\r", increment);
    fflush(stdout);
    last_increment = increment;
  }
}
