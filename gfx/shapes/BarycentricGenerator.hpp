#ifndef INC_2525E54DF6544F5B805F620445B345A6
#define INC_2525E54DF6544F5B805F620445B345A6

#include "gfx/shapes/NXTriangle.hpp"
#include "gfx/shapes/IGenerator.hpp"

namespace nxgl::gfx
{

class BarycentricGenerator : public IGenerator
{

public:

  BarycentricGenerator( uint8_t depth,
                        NXTriangle triangle,
                        nxgl::nxColor color = { 1.f, 1.f, 1.f, 1.f } )
    : m_initialTriangle( triangle ),
      m_depth( depth ),
      m_color( color )
  {}

 uint32_t generate( std::vector< GLData >& buffer ) noexcept override
  {
    auto startingPosition = buffer.size();
    generate( buffer, m_initialTriangle, m_depth );
    return buffer.size() - startingPosition;
  }

  [[nodiscard]]
  inline GLenum getPrimitiveType() const override { return sm_glPrimitive; }

private:

  void generate( std::vector< GLData >& buffer,
                 const NXTriangle& triangle,
                 uint8_t currentDepth )
  {
    buffer.push_back( { triangle.pointA, m_color } );
    buffer.push_back( { triangle.pointB, m_color } );
    buffer.push_back( { triangle.pointC, m_color } );

    if ( currentDepth == 0 ) return;

    nxgl::nxVec2 centroid = { ( triangle.pointA.x + triangle.pointB.x + triangle.pointC.x ) / 3.f,
                              ( triangle.pointA.y + triangle.pointB.y + triangle.pointC.y ) / 3.f };

    generate( buffer, { triangle.pointA, triangle.pointB, centroid }, currentDepth - 1 );
    generate( buffer, { triangle.pointA, triangle.pointC, centroid }, currentDepth - 1 );
    generate( buffer, { triangle.pointB, triangle.pointC, centroid }, currentDepth - 1 );
  }

private:

  NXTriangle m_initialTriangle;
  uint8_t m_depth { 0 };
  nxColor m_color;
  static const inline GLenum sm_glPrimitive { GL_LINE_LOOP };

};

}

#endif //INC_2525E54DF6544F5B805F620445B345A6
