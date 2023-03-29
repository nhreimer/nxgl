#ifndef AEA3EE1E1A194C55A7FCA8415AB465B9
#define AEA3EE1E1A194C55A7FCA8415AB465B9

#include "gfx/shapes/GLObject.hpp"
#include "gfx/shapes/IMVPApplicator.hpp"
#include "gfx/shapes/IColorable.hpp"

namespace nxgl::gfx
{

/// generic 2D polygon with a fill and an outline
class GLPolygon : public GLObject
{

public:

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  /// Uses the CCWBCAPolygon generator
  /// \param bufferUsage GL_STATIC_DRAW, GL_DYNAMIC_DRAW, etc
  /// \param edges edges in polygon
  GLPolygon( GLenum bufferUsage, uint8_t edges )
    : m_vao( GLData::createVAO() ),
      m_vbo( bufferUsage, ( GLsizeiptr )( ( edges * 3 ) * 2 ), nullptr ),
      m_fillBufferStartIndex( edges * 3 )
  {
    // if you have more complex triangle fan-based shapes, then use a different approach
    assert( edges > 2 && edges < INT8_MAX );

    m_vbo.bind();
    createVertices( edges );
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  void draw( const GLCamera& camera, IMVPApplicator& mvpApplicator ) override
  {
    // bind the VAO
    m_vao.bind();
    {
      // bind the VBO
      m_vbo.bind();
      {
        getBlender().blend();
        drawFill( camera, mvpApplicator );
        drawOutline( camera, mvpApplicator );
      }
      // unbind the VBO
      m_vbo.unbind();
    }
    // unbind the VAO
    m_vao.unbind();
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  void setOutlineColor( IColorable& colorizer )
  {
    for ( uint32_t vertexIndex = 0;
          vertexIndex < m_fillBufferStartIndex;
          ++vertexIndex )
    {
      setColor( m_vbo, vertexIndex, colorizer( vertexIndex ) );
    }
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  void setFillColor( IColorable& colorizer )
  {
    for ( uint32_t vertexIndex = m_fillBufferStartIndex, colorIndex = 0;
          vertexIndex < m_vbo.elementCount();
          ++vertexIndex, ++colorIndex )
    {
      setColor( m_vbo, vertexIndex, colorizer( colorIndex ) );
    }
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  void setOutlinePercentage( float percentage )
  {
    assert( percentage >= 0.f && percentage <= 1.f );
    m_outlinePercentage = percentage;
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  [[nodiscard]] float getOutlinePercentage() const { return m_outlinePercentage; }

private:

  ////////////////////////////////////////////////////////////////////////////////
  /// PRIVATE:
  void drawFill( const GLCamera& camera, IMVPApplicator& mvpApplicator )
  {
    auto mvp = getModel().getTranslation( camera );
    mvpApplicator.applyMVP( mvp );

    GLExec( glDrawArrays( GL_TRIANGLE_FAN, 0, m_fillBufferStartIndex ) );
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PRIVATE:
  void drawOutline( const GLCamera& camera, IMVPApplicator& mvpApplicator )
  {
    if ( m_outlinePercentage > 0.f )
    {
      m_modelFill = getModel();
      m_modelFill.setScale( m_modelFill.getScale() * ( 1.f - m_outlinePercentage ) );
      auto mvp = m_modelFill.getTranslation( camera );
      mvpApplicator.applyMVP( mvp );
      GLExec(
          glDrawArrays( GL_TRIANGLE_FAN, m_fillBufferStartIndex, m_fillBufferStartIndex ) );
    }
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PRIVATE:
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

    std::vector< GLData > vertexBuffer( m_vbo.elementCount() );

    for ( uint32_t i = 0; i < edges; ++i )
    {
      auto thirdPointAngle = ( float )( i + 1 ) * angle;
      nxgl::nxVec2 pointC { std::cos( thirdPointAngle ), std::sin( thirdPointAngle ) };

      // assign the outline shape
      vertexBuffer[ posInBuffer + 0 ] = { pointA, white };
      vertexBuffer[ posInBuffer + 1 ] = { pointB, white };
      vertexBuffer[ posInBuffer + 2 ] = { pointC, white };

      // assign the fill shape
      vertexBuffer[ posInBuffer + m_fillBufferStartIndex ]     = { pointA, white };
      vertexBuffer[ posInBuffer + m_fillBufferStartIndex + 1 ] = { pointB, white };
      vertexBuffer[ posInBuffer + m_fillBufferStartIndex + 2 ] = { pointC, white };

      posInBuffer += 3;
      pointB = pointC;
    }

    m_vbo.fill( vertexBuffer.data() );
  }

private:

  // VBO: outline and fill
  GLVbo< GLData > m_vbo;
  GLVao m_vao;

  GLModel m_modelFill;

  float m_outlinePercentage { 0.f };

  uint32_t m_fillBufferStartIndex { 0 };
};

}

#endif //AEA3EE1E1A194C55A7FCA8415AB465B9
