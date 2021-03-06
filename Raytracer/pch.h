// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

#ifndef PCH_H
#define PCH_H

#include <math.h>
#include <assert.h>
#include <iostream>
#include <DirectXMath.h>
// TODO: add headers that you want to pre-compile here
typedef unsigned char byte;
typedef unsigned uint32;
typedef int int32;
typedef unsigned short uint16;
typedef short int16;

constexpr float DEG2RAD = DirectX::XM_PI / 180.0f;
constexpr float RAD2DEG = 180.0f * DirectX::XM_1DIVPI;

#endif //PCH_H
