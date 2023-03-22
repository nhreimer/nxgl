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
    : Shape( drawMode, 12, ( GLData * )m_vertices )
  {}

  void draw( const GLCamera& camera )
  {
    bind();               // bind all the dependencies
    getBlend().blend();   // apply blend

    // transform & draw the OUTLINE
    auto outlineTransform = getModel().getTranslation( camera );
    setMVP( outlineTransform );
    GLExec( glDrawArrays( GL_TRIANGLES, 0, 3 ) );
    GLExec( glDrawArrays( GL_TRIANGLES, 3, 3 ) );

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

    GLExec( glDrawArrays( GL_TRIANGLES, 6, 3 ) );
    GLExec( glDrawArrays( GL_TRIANGLES, 9, 3 ) );
  }

  void setFillColor( IColorable& colorizer )
  {
    setColor( 6, colorizer( 0 ) );
    setColor( 7, colorizer( 1 ) );
    setColor( 8, colorizer( 2 ) );

    setColor( 9, colorizer( 3 ) );
    setColor( 10, colorizer( 4 ) );
    setColor( 11, colorizer( 5 ) );
  }

  void setOutlineColor( IColorable& colorizer )
  {
    setColor( 0, colorizer( 0 ) );
    setColor( 1, colorizer( 1 ) );
    setColor( 2, colorizer( 2 ) );

    setColor( 3, colorizer( 3 ) );
    setColor( 4, colorizer( 4 ) );
    setColor( 5, colorizer( 5 ) );
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

  static const inline GLData m_vertices[ 12 ] =
  {
    // counterclockwise, starting from bottom-left position A (right triangle)
    // x, y                   r, g, b, a
    { { -1.f,      -1.f  }, { 1.f, 1.f, 1.f, 1.f } }, // A (bottom left)
    { {  1.f,      -1.f  }, { 1.f, 1.f, 1.f, 1.f } }, // B (bottom right)
    { { -1.f,       1.f  }, { 1.f, 1.f, 1.f, 1.f } }, // C (top left)

    { {  1.f,      -1.f  }, { 1.f, 1.f, 1.f, 1.f } }, // B (bottom right)
    { {  1.f,       1.f  }, { 1.f, 1.f, 1.f, 1.f } }, // D (top right)
    { { -1.f,       1.f  }, { 1.f, 1.f, 1.f, 1.f } }, // C (top left)

    // x, y                   r, g, b, a
    { { -1.f,      -1.f  }, { 1.f, 1.f, 1.f, 1.f } }, // A (bottom left)
    { {  1.f,      -1.f  }, { 1.f, 1.f, 1.f, 1.f } }, // B (bottom right)
    { { -1.f,       1.f  }, { 1.f, 1.f, 1.f, 1.f } }, // C (top left)

    { {  1.f,      -1.f  }, { 1.f, 1.f, 1.f, 1.f } }, // B (bottom right)
    { {  1.f,       1.f  }, { 1.f, 1.f, 1.f, 1.f } }, // D (top right)
    { { -1.f,       1.f  }, { 1.f, 1.f, 1.f, 1.f } }  // C (top left)
  };
};

}

#endif //A92A560C01244450A53FED7D21C3A029
