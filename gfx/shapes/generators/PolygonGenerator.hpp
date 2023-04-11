#ifndef INC_6D8CB5E60BAC478C857B424A9A313909
#define INC_6D8CB5E60BAC478C857B424A9A313909


#include "gfx/shapes/NXTriangle.hpp"
#include "gfx/shapes/IGenerator.hpp"

namespace nxgl::gfx
{

class PolygonGenerator : public IGenerator
{

public:

  PolygonGenerator() = default;

  explicit PolygonGenerator( uint8_t edges )
    : m_edges( edges )
  {}

  void setEdges( uint8_t edges )
  {
    m_edges = edges;
  }

  [[nodiscard]]
  uint8_t getEdges() const { return m_edges; }

  uint32_t generate( std::vector< GLData >& buffer ) noexcept override
  {
    auto startingPosition = buffer.size();
    // divide a circle by the number of edges
    // which is A(0, 0) -> B(0, 1) -> C(1, 1)
    // note that C is slightly less than 1, 1 however
    auto angle = nxgl::NX_TAU / ( float )m_edges;

    // move counterclockwise starting at the center (0, 0)
    // ABC
    // ACD
    // ADE ...

    // get the first two points and then we only need to calculate the third
    // every loop

    nxgl::nxColor white { 1.f, 1.f, 1.f, 1.f };

    // this is fixed
    nxgl::nxVec2 pointA { 0, 0 };

    // set point B
    nxgl::nxVec2 pointB { std::cos( 0 ), std::sin( 0 ) };

    uint32_t posInBuffer = 0;

    for ( uint32_t i = 0; i < m_edges; ++i )
    {
      auto thirdPointAngle = ( float )( i + 1 ) * angle;
      nxgl::nxVec2 pointC { std::cos( thirdPointAngle ), std::sin( thirdPointAngle ) };

      // assign the outline shape
      buffer.push_back( { pointA, white } );
      buffer.push_back( { pointB, white } );
      buffer.push_back( { pointC, white } );

      posInBuffer += 3;
      pointB = pointC;
    }

    return buffer.size() - startingPosition;
  }

  [[nodiscard]]
  inline GLenum getPrimitiveType() const override { return sm_glPrimitive; }

private:

  uint8_t m_edges { 0 };
  static const inline GLenum sm_glPrimitive { GL_TRIANGLES };

};

}

#endif //INC_6D8CB5E60BAC478C857B424A9A313909
