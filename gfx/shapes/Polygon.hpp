#ifndef INC_1828D8268BAA4144A7C8115DFE80A004
#define INC_1828D8268BAA4144A7C8115DFE80A004

#include "ApplicationContext.hpp"

#include "gfx/shapes/IMVPApplicator.hpp"
#include "gfx/shapes/IColorable.hpp"

namespace nxgl::gfx
{

class Polygon
{

public:

  Polygon( GLenum bufferUsage, uint32_t edges )
    : m_vbo( bufferUsage, ( GLsizeiptr )( edges * 3 * 2 ), nullptr ),
      m_fillBufferStartIndex( edges * 3 ),
      m_edges( edges )
  {
    assert( edges > 2 );

    m_vao.registerVBO( m_vbo );

    // create the vertices according to a circular pattern
    createVertices( m_edges );
  }

//  Polygon( GLenum bufferUsage, uint32_t elements )
//    : m_vbo( bufferUsage, ( GLsizeiptr )( elements * 2 ), nullptr ),
//      m_fillBufferStartIndex( elements ),
//      m_edges( elements / 3 )
//  {
//    assert( elements > 2 );
//
//    m_vao.registerVBO( m_vbo );
//
//    // create the vertices according to a circular pattern
//    createVertices( m_edges );
//  }

  void draw( const ApplicationContext& appCtx, GLFWwindow *window )
  {
    bind();               // bind ourselves
    m_blend.blend();      // apply blend

    // get the MVP for this object
    auto mvp = m_model.getTranslation( appCtx.camera );

    // supply the shader with the MVP (binding occurs as the shader needs)
    appCtx.mvpApplicator->applyMVP( mvp );

    // draw the outline
    GLExec( glDrawArrays( GL_TRIANGLE_FAN, 0, m_fillBufferStartIndex ) );

    // draw the interior
    m_innerTransform = m_model;
    m_innerTransform.setScale(
      {
        ( 1.f - m_outlinePercentage ) * m_model.getScale().x,
        ( 1.f - m_outlinePercentage ) * m_model.getScale().y
      } );

    mvp = m_innerTransform.getTranslation( appCtx.camera );
    appCtx.mvpApplicator->applyMVP( mvp );
    GLExec( glDrawArrays( GL_TRIANGLE_FAN, m_fillBufferStartIndex, m_fillBufferStartIndex ) );
  }

  GLModel& getModel() { return m_model; }
  GLBlend& getBlend() { return m_blend; }

  void setOutlineWidth( float percentage )
  {
    m_outlinePercentage = percentage;
  }

  [[nodiscard]] float getOutlineWidth() const
  {
    return m_outlinePercentage;
  }

  void setFillColor( const IColorable& colorizer )
  {
    for ( uint32_t vertexIndex = m_fillBufferStartIndex, colorIndex = 0;
          colorIndex < m_fillBufferStartIndex;
          ++vertexIndex, ++colorIndex )
    {
      setColor( vertexIndex, colorizer( colorIndex ) );
    }
  }

  void setOutlineColor( const IColorable& colorizer )
  {
    for ( uint32_t vertexIndex = 0, colorIndex = 0;
          colorIndex < m_fillBufferStartIndex;
          ++vertexIndex, ++colorIndex )
    {
      setColor( vertexIndex, colorizer( colorIndex ) );
    }
  }

private:

  void createVertices( uint32_t edges )
  {
    // divide a circle by the number of edges
    // which is A(0, 0) -> B(0, 1) -> C(1, 1)
    // note that C is slightly less than 1, 1 however
    auto angle = nxgl::NX_TAU / ( float )edges;

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

    for ( uint32_t i = 0; i < edges; ++i )
    {
      auto thirdPointAngle = ( float )( i + 1 ) * angle;
      nxgl::nxVec2 pointC { std::cos( thirdPointAngle ), std::sin( thirdPointAngle ) };

      m_vbo.setData( posInBuffer + 0, { pointA, white } );
      m_vbo.setData( posInBuffer + 1, { pointB, white } );
      m_vbo.setData( posInBuffer + 2, { pointC, white } );

      m_vbo.setData( posInBuffer + m_fillBufferStartIndex, { pointA, white } );
      m_vbo.setData( posInBuffer + m_fillBufferStartIndex + 1, { pointB, white } );
      m_vbo.setData( posInBuffer + m_fillBufferStartIndex + 2, { pointC, white } );

      posInBuffer += 3;
      pointB = pointC;
    }
  }

  void bind() const
  {
    m_vao.bind();
  }

  void unbind() const
  {
    m_vbo.unbind();
    m_vao.unbind();
  }

  [[nodiscard]] nxgl::nxColor getColor( uint32_t index )
  {
    assert( sizeof( GLData ) * index + sizeof( nxgl::nxVec2 ) < m_vbo.size() );

    m_vbo.bind();
    nxColor color;

    GLExec( glGetBufferSubData(
      GL_ARRAY_BUFFER,
      ( GLintptr )( sizeof( GLData ) * index + sizeof( nxgl::nxVec2 ) ),
      sizeof( nxColor ),
      &color ) );

    return color;
  }

  void setColor( uint32_t index, const nxgl::nxColor& color )
  {
    assert( sizeof( GLData ) * index + sizeof( nxgl::nxVec2 ) < m_vbo.size() );

    m_vbo.bind();

    GLExec( glBufferSubData(
      GL_ARRAY_BUFFER,
      ( GLintptr )( sizeof( GLData ) * index + sizeof( nxgl::nxVec2 ) ),
      sizeof( nxgl::nxColor ),
      &color ) );
  }

private:

  GLVao m_vao;
  GLVbo< GLData > m_vbo;
  GLBlend m_blend;
  GLModel m_model;

  // percentage of the triangle consumed by the outline
  float m_outlinePercentage { 0.f };

  // this is the scaled down version, which must move in step with the object's
  // primary model. it copies everything from the primary over, except the scale
  nxgl::gfx::GLModel m_innerTransform;

  // the fill buffer is the 2nd one because it's the scaled down version of
  // the original
  uint32_t m_fillBufferStartIndex { 0 };

  uint32_t m_edges { 0 };
};

}

#endif //INC_1828D8268BAA4144A7C8115DFE80A004
