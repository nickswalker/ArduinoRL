#ifndef DEBUG_H
#define DEBUG_H
#include "Strings.h"
#include "Vector.h"
extern const char spaceString[];
extern const char rightBracketString[];
extern const char leftBracketString[];

#define D_LOG(str, num) Serial.print(str);Serial.print(": ");if(isnan(num)) Serial.println("NaN");else if (isinf(num)) Serial.println("inf");else Serial.println(num);
#define D_LOG_V(str,vec, len) Serial.print(str);Serial.print(": ");logVector(vec, len);


#endif
