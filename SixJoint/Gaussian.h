#ifndef GAUSSIAN_H
#define GAUSSIAN_H

float randomf(const float min, const float max) {
  uint32_t intMax = ~0u;
  float normalized = random(intMax) / float(intMax);
  return min + normalized * (max - min);
}

float sampleNormalDistribution(float mean, float variance) {
    float v1, v2, s;
    do {
        v1 = 2.0f * randomf(0.f,1.f) - 1.0f;
        v2 = 2.0f * randomf(0.f,1.f) - 1.0f;
        s = v1 * v1 + v2 * v2;
    } while (s >= 1.0f || s == 0.f);

    s = sqrt((-2.0f * log(s)) / s);
    //FIXME: probably the issue
    return v1 * s;
}

float density(float x, float mean, float stdDeviation) {
  const float inv_sqrt_2pi = 0.3989422804014327;
  double sigma = stdDeviation;
  double a = (x - mean) / sigma;
  
  return inv_sqrt_2pi / sigma * exp(-0.5f * a * a);
}


#endif
