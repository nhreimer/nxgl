#ifndef BA20117FF36244C2AB46800B9E0E0E2C
#define BA20117FF36244C2AB46800B9E0E0E2C

#include "gfx/shapes/Shape.hpp"

namespace nxgl::gfx
{

/***
 * outlining works according to a percentage of the overall size of the object.
 * outlines do not extrude, they eat into the existing size, which is why
 * outlines use a percentage.
 */
class Triangle : public Shape
{
public:
  explicit Triangle( GLenum drawMode )
    : Shape( drawMode, 6, ( GLData * )m_vertices )
  {}

  void draw( const GLCamera& camera )
  {
    bind();               // bind all the dependencies
    getBlend().blend();   // apply blend

    // transform & draw the OUTLINE
    auto outlineTransform = getModel().getTranslation( camera );
    setMVP( outlineTransform );
    GLExec( glDrawArrays( GL_TRIANGLES, 0, 3 ) );

    auto outlinePosition = getModel().getPosition();

    // scale down the inner shape
    m_innerTransform = getModel();
    m_innerTransform.setScale(
      {
        ( 1.f - m_outlinePercentage ) * getModel().getScale().x,
        ( 1.f - m_outlinePercentage ) * getModel().getScale().y
      } );

    // transform & draw the INTERIOR
    auto fillTransform = m_innerTransform.getTranslation( camera );
    setMVP( fillTransform );

    GLExec( glDrawArrays( GL_TRIANGLES, 3, 3 ) );
  }

  void setFillColor( IColorable& colorizer )
  {
    setColor( 3, colorizer( 0 ) );
    setColor( 4, colorizer( 1 ) );
    setColor( 5, colorizer( 2 ) );
  }

  void setOutlineColor( IColorable& colorizer )
  {
    setColor( 0, colorizer( 0 ) );
    setColor( 1, colorizer( 1 ) );
    setColor( 2, colorizer( 2 ) );
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

  static const inline GLData m_vertices[ 6 ] =
    {
      // x, y                   r, g, b, a
      { { -1.f,      -1.f  }, { 1.f, 1.f, 1.f, 1.f } },
      { {  1.f,      -1.f  }, { 1.f, 1.f, 1.f, 1.f } },
      { {  0.f,       1.f  }, { 1.f, 1.f, 1.f, 1.f } },

      // x, y                   r, g, b, a
      { { -1.f,      -1.f  }, { 1.f, 1.f, 1.f, 1.f } },
      { {  1.f,      -1.f  }, { 1.f, 1.f, 1.f, 1.f } },
      { {  0.f,       1.f  }, { 1.f, 1.f, 1.f, 1.f } }
    };
};
}

#endif //BA20117FF36244C2AB46800B9E0E0E2C
