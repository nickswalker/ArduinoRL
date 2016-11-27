#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H
#define EULER 2.718281828459045235360287471352f
float cubic(const float alpha, const float beta, const float gamma, const float delta, const float t);

float cubicPrime(const float alpha, const float beta, const float gamma, const float t);

float quadratic(const float a, const float b, const float c, const float t);

// Maximize the quadratic between 0 and 1
float maximizeQuadratic(const float a, const float b, const float c, float& atT);
#endif
