#ifndef MYIR_H
#define MYIR_H

#include <math.h>

inline double irAnalogToCm(int analogValue)
{
  return 1619.1 * pow(analog, -0.591);
}

#endif // MYIR_H
