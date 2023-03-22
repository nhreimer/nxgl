#ifndef A92A560C01244450A53FED7D21C3A029
#define A92A560C01244450A53FED7D21C3A029

#include "gfx/shapes/Shape.hpp"

namespace nxgl::gfx
{

// TODO: should be able to set vertices around a circle now and
//       generalize it (even the triangle, bro!)
class Rectangle : public Shape
{
public:
  explicit Rectangle( GLenum drawMode )
    : Shape( drawMode, 24, ( GLData * )m_vertices )
  {}

  void draw( const GLCamera& camera )
  {
    bind();               // bind all the dependencies
    getBlend().blend();   // apply blend

    // transform & draw the OUTLINE
    auto outlineTransform = getModel().getTranslation( camera );
    setMVP( outlineTransform );
    GLExec( glDrawArrays( GL_TRIANGLE_FAN, 0, 12 ) );

    auto outlinePosition = getModel().getPosition();

    // scale down the inner shape, so it fits inside the overall size
    // we NEVER extend beyond the specified size
    m_innerTransform = getModel();
    m_innerTransform.setScale(
      {
        ( 1.f - m_outlinePercentage ) * getModel().getScale().x,
        ( 1.f - m_outlinePercentage ) * getModel().getScale().y
      } );

    // transform & draw the INTERIOR
    auto fillTransform = m_innerTransform.getTranslation( camera );
    setMVP( fillTransform );

    GLExec( glDrawArrays( GL_TRIANGLES, 12, 12 ) );
  }

  void setFillColor( IColorable& colorizer )
  {
    for ( int vertexIndex = 12, colorIndex = 0;
          colorIndex < 12;
          ++vertexIndex, ++colorIndex )
    {
      setColor( vertexIndex, colorizer( colorIndex ) );
    }
  }

  void setOutlineColor( IColorable& colorizer )
  {
    for ( int vertexIndex = 0, colorIndex = 0;
          colorIndex < 12;
          ++vertexIndex, ++colorIndex )
    {
      setColor( vertexIndex, colorizer( colorIndex ) );
    }
  }

  void setOutlineWidth( float percentage )
  {
    m_outlinePercentage = percentage;
  }

  [[nodiscard]] float getOutlineWidth() const
  {
    return m_outlinePercentage;
  }

private:

  // percentage of the triangle consumed by the outline
  float m_outlinePercentage { 0.f };

  // this is the scaled down version, which must move in step with the object's
  // primary model. it copies everything from the primary over, except the scale
  nxgl::gfx::GLModel m_innerTransform;

  static const inline GLData m_vertices[ 24 ] =
  {
    // counterclockwise, starting from bottom-left position A (right triangle)
    //   x, y            r, g, b, a
    { {  0.f,   0.f }, { 1.f, 1.f, 1.f, 1.f } },  // A
    { {  1.f,   1.f }, { 1.f, 1.f, 1.f, 1.f } },  // B
    { { -1.f,   1.f }, { 1.f, 1.f, 1.f, 1.f } },  // C

    { {  0.f,   0.f }, { 1.f, 1.f, 1.f, 1.f } },  // A
    { { -1.f,   1.f }, { 1.f, 1.f, 1.f, 1.f } },  // C
    { { -1.f,  -1.f }, { 1.f, 1.f, 1.f, 1.f } },  // D

    { {  0.f,   0.f }, { 1.f, 1.f, 1.f, 1.f } },  // A
    { { -1.f,  -1.f }, { 1.f, 1.f, 1.f, 1.f } },  // D
    { {  1.f,  -1.f }, { 1.f, 1.f, 1.f, 1.f } },  // E

    { {  0.f,   0.f }, { 1.f, 1.f, 1.f, 1.f } },  // A
    { {  1.f,  -1.f }, { 1.f, 1.f, 1.f, 1.f } },  // E
    { {  1.f,   1.f }, { 1.f, 1.f, 1.f, 1.f } },  // B

    //////////////////////////////////////////////////

    //   x, y            r, g, b, a
    { {  0.f,   0.f }, { 1.f, 1.f, 1.f, 1.f } },  // A
    { {  1.f,   1.f }, { 1.f, 1.f, 1.f, 1.f } },  // B
    { { -1.f,   1.f }, { 1.f, 1.f, 1.f, 1.f } },  // C

    { {  0.f,   0.f }, { 1.f, 1.f, 1.f, 1.f } },  // A
    { { -1.f,   1.f }, { 1.f, 1.f, 1.f, 1.f } },  // C
    { { -1.f,  -1.f }, { 1.f, 1.f, 1.f, 1.f } },  // D

    { {  0.f,   0.f }, { 1.f, 1.f, 1.f, 1.f } },  // A
    { { -1.f,  -1.f }, { 1.f, 1.f, 1.f, 1.f } },  // D
    { {  1.f,  -1.f }, { 1.f, 1.f, 1.f, 1.f } },  // E

    { {  0.f,   0.f }, { 1.f, 1.f, 1.f, 1.f } },  // A
    { {  1.f,  -1.f }, { 1.f, 1.f, 1.f, 1.f } },  // E
    { {  1.f,   1.f }, { 1.f, 1.f, 1.f, 1.f } }   // B
  };
};

}

#endif //A92A560C01244450A53FED7D21C3A029
