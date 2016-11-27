#include "Polynomial.h"
#include "math.h"
#include <Arduino.h>
#include "Debug.h"
float cubic(const float alpha, const float beta, const float gamma, const float delta, const float t) {
  return alpha * pow(t,3.0) + beta * pow(t,2.0) + gamma * t + delta;
}

float cubicPrime(const float alpha, const float beta, const float gamma, const float t) {
  return 3.0 * alpha * pow(t,2.0) + 2.0 * beta * t + gamma;
}

float quadratic(const float a, const float b, const float c, const float t) {
  return a * pow(t,2.0) + b * t + c;
}

// Maximize the quadratic between 0 and 1
float maximizeQuadratic(const float a, const float b, const float c, float& atT) {
  const float atZero = quadratic(a,b,c,0.0);
  const float atOne = quadratic(a,b,c,1.0);
  //D_LOG("0", atZero);
  //D_LOG("1", atOne);
  if (a == 0) {
    atT = atZero >= atOne ? 0.0: 1.0;
    return max(atZero, atOne);
  }
  float mid = -a / (2.0 * b);
  if (0.0 < mid && mid < 1.0) {
    float atMid = quadratic(a,b,c,mid);
    atT = atZero >= atOne ? 0.0: 1.0;
    atT = atMid > atT ? mid: atT;
    return max(max(atZero, atOne), atMid);
  }
   atT = atZero >= atOne ? 0.0: 1.0;
   return max(atZero, atOne);
}
