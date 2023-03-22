#ifndef C3E093397F0D459E9EDA3D980D03D307
#define C3E093397F0D459E9EDA3D980D03D307

#include "utilities/Definitions.hpp"

namespace nxgl::gfx
{

/***
 * the bounds of an object
 */
struct GLBounds
{
  nxVec2 topLeft;
  nxVec2 bottomRight;

  [[nodiscard]] inline nxVec2 getSize() const
  {
    return { bottomRight.x - topLeft.x, bottomRight.y - topLeft.y };
  }

  [[nodiscard]] inline bool contains( const nxVec2& point ) const
  {
    return point.x >= topLeft.x && point.x <= bottomRight.x &&
           point.y >= topLeft.y && point.y <= bottomRight.y;
  }
};

}

#endif //C3E093397F0D459E9EDA3D980D03D307
