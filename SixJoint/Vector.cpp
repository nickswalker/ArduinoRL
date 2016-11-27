#include "Vector.h"
#include "Strings.h"
#include "Debug.h"

void logVector(const float vector[], const size_t length) {
    Serial.print(leftBracketString);
    for( uint8_t i = 0; i < length; i++) {
        Serial.print(spaceString);
        if (isnan(vector[i])) {
          Serial.print("NaN");
        } else if (isinf(vector[i])) {
          Serial.print("inf");
        }
        else {
          Serial.print(vector[i]);
        }
    }
    Serial.println(rightBracketString);  
}

void logVector(const int8_t vector[], const size_t length) {
    Serial.print(leftBracketString);
    for( uint8_t i = 0; i < length; i++) {
        Serial.print(spaceString);
        Serial.print(vector[i]);
    }
    Serial.println(rightBracketString);  
}

void logVector(const uint8_t vector[], const size_t length) {
    Serial.print(leftBracketString);
    for( uint8_t i = 0; i < length; i++) {
        Serial.print(spaceString);
        Serial.print(vector[i]);
    }
    Serial.println(rightBracketString);  
}

void copy(const float first[], float second[], const size_t length) {
  for( uint8_t i = 0; i < length; i++) {
    second[i] = first[i];
  }
}

float absMaxComponent(const float vector[], const size_t length) {
  float absMax = 0.0;
    for( uint8_t i = 0; i < length; i++) {
    absMax = fabs(vector[i]) > absMax ? fabs(vector[i]): absMax;
  }
  return absMax;
}

void norm(float vector[], const size_t length) {
  const float absMax = absMaxComponent(vector, length);
  if (absMax > 0) {
    multiply(1.0/absMax, vector, length);
    const float v_len = dot(vector,vector, length);
    const float factor = 1.0 / v_len;
    multiply(factor, vector, length);
  }

}

float dot(const uint8_t first[], const uint8_t second[], const size_t length) {
    float result = 0.0;
    for( uint8_t i = 0; i < length; i++) {
        result += float(first[i]) * float(second[i]);
    }
    return result;
}

float dot(const float first[], const float second[], const size_t length) {
    float result = 0.0;
    for( uint8_t i = 0; i < length; i++) {
        result += first[i] * second[i];

    }
            D_LOG("m", result);
    return result;
}

/// Add first and second, store result in first
void add(float first[], const float second[], const size_t length) {
    for (uint8_t i = 0; i < length; i++) {
        first[i] = first[i] + second[i];
    }
}

void add(const float first[], const float second[], float result[], const size_t length) {
     for (uint8_t i = 0; i < length; i++) {
        result[i] = first[i] + second[i];
    }  
}


void subtract(const uint8_t first[], const uint8_t second[], float result[], const size_t length) {
    for (uint8_t i = 0; i < length; i++) {
        result[i] = float(first[i]) - float(second[i]);
    }
}

/// Subtract the second from the first, storing the result in the first
void subtract(float first[], const float second[], const size_t length) {
    for (uint8_t i = 0; i < length; i++) {
        first[i] = first[i] - second[i];
    }
}

void multiply(const float scalar, float vector[], const size_t length) {
    for (uint8_t i = 0; i < length; i++) {
        vector[i] = scalar * vector[i];
    }
}

void multiply(const float scalar, const float vector[], float result[], const size_t length) {
    for (uint8_t i = 0; i < length; i++) {
        result[i] = scalar * vector[i];
    }
}
