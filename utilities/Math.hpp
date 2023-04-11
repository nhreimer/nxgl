#ifndef INC_97E6A2BF49EC4FDF8B0B4A07BB9D3675
#define INC_97E6A2BF49EC4FDF8B0B4A07BB9D3675

#include "gfx/shapes/NXTriangle.hpp"

namespace nxgl
{

class Math
{
public:

  static float getSign( const nxVec2& p1, const nxVec2& p2, const nxVec2& p3 )
  {
    return ( p1.x - p3.x ) * ( p2.y - p3.y ) -
           ( p2.x - p3.x ) * ( p1.y - p3.y );
  }

  // https://stackoverflow.com/questions/2049582/how-to-determine-if-a-point-is-in-a-2d-triangle
  static bool isPointInTriangle( const nxVec2& pt, const nxgl::gfx::NXTriangle& triangle )
  {
    auto d1 = getSign( pt, triangle.pointA, triangle.pointB );
    auto d2 = getSign( pt, triangle.pointB, triangle.pointC );
    auto d3 = getSign( pt, triangle.pointC, triangle.pointA );

    auto hasNeg = ( d1 < 0 ) || ( d2 < 0 ) || ( d3 < 0 );
    auto hasPos = ( d1 > 0 ) || ( d2 > 0 ) || ( d3 > 0 );

    return !( hasNeg && hasPos );
  }

  static nxVec3 getTriangleAngles( const nxgl::gfx::NXTriangle& triangleData )
  //static nxVec3 getTriangleAngles( const nxVec2& a, const nxVec2& b, const nxVec2& c )
  {
    auto lengthA = getLineLength( triangleData.pointB, triangleData.pointC );
    auto lengthB = getLineLength( triangleData.pointA, triangleData.pointC );
    auto lengthC = getLineLength( triangleData.pointA, triangleData.pointB );

    auto angleA = getAngle( lengthA, lengthB, lengthC );
    auto angleB = getAngle( lengthB, lengthC, lengthA );
    return { angleA, angleB, NX_PI - ( angleA + angleB ) };
  }

  static inline float getLineLength( const nxVec2& pointA, const nxVec2& pointB )
  {
    auto deltaX = pointA.x - pointB.x;
    auto deltaY = pointA.y - pointB.y;
    return std::sqrt( deltaX * deltaX + deltaY * deltaY );
  }

  static inline float getAngle( float lengthA, float lengthB, float lengthC )
  {
    return std::acos(
      ( lengthB * lengthB + lengthC * lengthC - lengthA * lengthA ) / ( 2 * lengthB * lengthC ) );
  }
};

}

#endif //INC_97E6A2BF49EC4FDF8B0B4A07BB9D3675
