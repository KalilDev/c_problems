#include "conversion.h"
#include <math.h>

FLOAT_CONVERSION(celsius, fahrenheit, 5.0*((fahrenheit - 32.0)/9.0), (celsius/5.0)*9.0 + 32.0)
FLOAT_CONVERSION(kelvin, celsius, celsius+273.15, kelvin-273.15)
FLOAT_CONVERSION(km_h, m_s, m_s*3.6, km_h/3.6)
FLOAT_CONVERSION(mile, km, km/1.61, mile*1.61)
FLOAT_CONVERSION(rad, deg, (deg*M_PI)/180.0, (rad/M_PI)*180.0)
FLOAT_CONVERSION(pol, cm, cm/2.54, pol*2.54)
FLOAT_CONVERSION(lt, m_3, m_3*1000.0, lt/1000.0)
FLOAT_CONVERSION(kg, lb, lb*0.45, kg/0.45);
FLOAT_CONVERSION(yd, m, m/0.91, yd*0.91);
FLOAT_CONVERSION(m_2, ac, ac*4048.58, m_2*0.000247);
FLOAT_CONVERSION(m_2, hec, hec/10000.0, m_2*10000.0)
