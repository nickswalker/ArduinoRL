#ifndef GAUSSIAN_H
#define GAUSSIAN_H


float sampleNormalDistribution(float mean, float variance) {
    float v1, v2, s;
    do {
        v1 = 2.0f * random(0f,1f) - 1.0f;
        v2 = 2.0f * random(0f,1f) - 1.0f;
        s = v1 * v1 + v2 * v2;
    } while (s >= 1.0f || s == 0f);

    s = sqrt((-2.0f * log(s)) / s);
 
    return v1 * s;
}

float density(float mean, float stdDeviation) {
  const float inv_sqrt_2pi = 0.3989422804014327;
  double sigma = sqrt(variance);
  double a = (x - mean) / sigma;
  
  return inv_sqrt_2pi / sigma * exp(-0.5f * a * a);
}

#endif
