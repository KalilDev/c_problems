#ifndef K_CONVERSION_H
#define K_CONVERSION_H

#define FLOAT_CONVERSION_BLOCK(a, b, a_from_b_block, b_from_a_block)\
float a ## _from_ ## b(float b) a_from_b_block \
\
float b ## _from_ ## a(float a) b_from_a_block 

#define FLOAT_CONVERSION(a, b, a_from_b, b_from_a) FLOAT_CONVERSION_BLOCK(a, b, { return a_from_b; }, { return b_from_a; })
#define FLOAT_CONVERSION_SIGNATURE(a, b) FLOAT_CONVERSION_BLOCK(a, b, ;, ;)

FLOAT_CONVERSION_SIGNATURE(celsius, fahrenheit)
FLOAT_CONVERSION_SIGNATURE(kelvin, celsius)
FLOAT_CONVERSION_SIGNATURE(km_h, m_s)
FLOAT_CONVERSION_SIGNATURE(mile, km)
FLOAT_CONVERSION_SIGNATURE(rad, deg)
FLOAT_CONVERSION_SIGNATURE(pol, cm)
FLOAT_CONVERSION_SIGNATURE(lt, m_3)
FLOAT_CONVERSION_SIGNATURE(kg, lb)
FLOAT_CONVERSION_SIGNATURE(yd, m)
FLOAT_CONVERSION_SIGNATURE(m_2, ac)
FLOAT_CONVERSION_SIGNATURE(m_2, hec)

#endif
