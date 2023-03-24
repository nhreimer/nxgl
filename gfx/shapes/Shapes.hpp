#ifndef INC_268C0048D7E146F48CC1C8212AF82D7B
#define INC_268C0048D7E146F48CC1C8212AF82D7B

#include "gfx/shapes/Polygon.hpp"

namespace nxgl::gfx
{
class Rectangle : public Polygon
{
public:
  explicit Rectangle( GLenum bufferUsage )
    : Polygon( bufferUsage, 4 )
  {}
};

class Triangle : public Polygon
{
public:
  explicit Triangle( GLenum bufferUsage )
    : Polygon( bufferUsage, 3 )
  {}
};
}

#endif //INC_268C0048D7E146F48CC1C8212AF82D7B
