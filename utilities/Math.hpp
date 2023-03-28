#ifndef INC_97E6A2BF49EC4FDF8B0B4A07BB9D3675
#define INC_97E6A2BF49EC4FDF8B0B4A07BB9D3675

namespace nxgl
{

class Math
{
public:

  static nxVec3 getTriangleAngles( const nxVec2& a, const nxVec2& b, const nxVec2& c )
  {
    auto lengthA = getLineLength( b, c );
    auto lengthB = getLineLength( a, c );
    auto lengthC = getLineLength( a, b );

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
