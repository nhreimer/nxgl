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

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  /// creates a shape based on the number of edges provided.
  /// \param bufferUsage GL_DYNAMIC_DRAW, GL_STATIC_DRAW, etc
  /// \param edges sides of the polygon
  Polygon( GLenum bufferUsage, uint32_t edges )
    : m_vbo( bufferUsage, ( GLsizeiptr )( edges * 3 * 2 ), nullptr ),
      m_fillBufferStartIndex( edges * 3 ),
      m_edges( edges ),
      m_indices( edges * 3 ) // reserve the indices buffer
  {
    assert( edges > 2 );

    m_vbo.bind();
    m_vao.registerVBO();

    // create the vertices according to a circular pattern
    // NOTE: the vertices get created on initialization no matter what.
    Polygon::createVertices( m_edges );
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  /// imports the data and then doubles its size to make an outline. the
  /// outline is just a copy of the original data and then scaled down, so it might not
  /// be correct depending on certain designs. outlines are NOT drawn when the outline
  /// is set to 0.
  /// \param bufferUsage GL_DYNAMIC_DRAW, GL_STATIC_DRAW, etc
  /// \param elements number of vertices in the buffer (elements % 3 == 0)
  /// \param pReadBuffer buffer to import
  /// \param drawMode GL_TRIANGLE_FAN, GL_LINE_STRIP, etc.
//  Polygon( GLenum bufferUsage,
//           uint32_t elements,
//           const GLData * pReadBuffer,
//           GLenum drawMode = GL_TRIANGLE_FAN )
//    : m_vbo( bufferUsage, ( GLsizeiptr )( elements * 2 ), pReadBuffer ),
//      m_fillBufferStartIndex( elements ),
//      m_edges( elements / 3 ),
//      m_glDrawMode( drawMode )
//  {
//    // we're making triangles here!
//    assert( elements % 3 == 0 );
//    m_vbo.bind();
//    m_vao.registerVBO();
//
//    // copy the buffer into the outline buffer region
//    m_vbo.setDataRange( m_fillBufferStartIndex, m_fillBufferStartIndex, pReadBuffer );
//  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  void draw( const ApplicationContext& appCtx, GLFWwindow *window )
  {
    bind();               // bind ourselves
    m_blend.blend();      // apply blend

    drawPrimaryShape( appCtx );
    drawOutlineShape( appCtx );
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  GLModel& getModel() { return m_model; }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  void setModel( const GLModel& model ) { m_model = model; }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  GLBlend& getBlend() { return m_blend; }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  void setBlend( const GLBlend& blend ) { m_blend = blend; }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  /// outline rendering is skipped completely whenever its value is 0.
  /// \param percentage the percentage of the outline. range: [0 - 1]
  void setOutlineWidth( float percentage )
  {
    assert( percentage >= 0.f && percentage <= 1.f );
    m_outlinePercentage = percentage;
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  [[nodiscard]] float getOutlineWidth() const
  {
    return m_outlinePercentage;
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  void setFillColor( const IColorable& colorizer )
  {
    for ( uint32_t vertexIndex = m_fillBufferStartIndex, colorIndex = 0;
          colorIndex < m_fillBufferStartIndex;
          ++vertexIndex, ++colorIndex )
    {
      setColor( vertexIndex, colorizer( colorIndex ) );
    }
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  void setOutlineColor( const IColorable& colorizer )
  {
    for ( uint32_t vertexIndex = 0, colorIndex = 0;
          colorIndex < m_fillBufferStartIndex;
          ++vertexIndex, ++colorIndex )
    {
      setColor( vertexIndex, colorizer( colorIndex ) );
    }
  }

protected:

  ////////////////////////////////////////////////////////////////////////////////
  /// PROTECTED:
  /// gets drawn no matter what
  virtual void drawPrimaryShape( const ApplicationContext& appCtx )
  {
    // get the MVP for this object
    auto mvp = m_model.getTranslation( appCtx.camera );

    // supply the shader with the MVP (binding occurs as the shader needs)
    appCtx.mvpApplicator->applyMVP( mvp );

    // draw the outline
//    GLExec( glDrawArrays( m_glDrawMode, 0, m_fillBufferStartIndex ) );

//    GLExec( glDrawArrays( m_glDrawMode, 0, m_fillBufferStartIndex ) );
    GLExec( glDrawElements( m_glDrawMode, m_indices.size(), GL_UNSIGNED_SHORT, m_indices.data() ) );
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PROTECTED:
  /// only drawn when the outline is > 0.
  virtual void drawOutlineShape( const ApplicationContext& appCtx )
  {
    // skip the outline if it's not set
    if ( m_outlinePercentage > 0.f )
    {
      // draw the interior
      m_innerTransform = m_model;
      m_innerTransform.setScale(
        {
          ( 1.f - m_outlinePercentage ) * m_model.getScale().x,
          ( 1.f - m_outlinePercentage ) * m_model.getScale().y
        } );

      auto mvp = m_innerTransform.getTranslation( appCtx.camera );
      appCtx.mvpApplicator->applyMVP( mvp );
//      GLExec( glDrawArrays( m_glDrawMode, m_fillBufferStartIndex, m_fillBufferStartIndex ) );
      GLExec( glDrawElements( m_glDrawMode, m_indices.size(), GL_UNSIGNED_SHORT, m_indices.data() ) );
    }
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PROTECTED:
  virtual void createVertices( uint32_t edges )
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

    m_vbo.setData( 0, { pointA, white } );
    m_vbo.setData( 1, { pointB, white } );

    uint32_t posInBuffer = 2;

    for ( uint32_t i = 0, idx = 0; i < edges; ++i, idx += 3 )
    {
      if ( i + 1 < edges )
      {
        auto thirdPointAngle = ( float ) ( i + 1 ) * angle;
        nxgl::nxVec2 pointC{ std::cos( thirdPointAngle ), std::sin( thirdPointAngle ) };
        m_vbo.setData( posInBuffer, { pointC, white } );
        ++posInBuffer;
      }

      m_indices[ idx ] = 0;
      m_indices[ idx + 1 ] = i + 1;
      if ( i + 1 < edges )
        m_indices[ idx + 2 ] = i + 2;
      else
        m_indices[ idx + 2 ] = 1;
    }
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PROTECTED:
  void bind() const
  {
    m_vao.bind();
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PROTECTED:
  void unbind() const
  {
    m_vbo.unbind();
    m_vao.unbind();
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PROTECTED:
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

  ////////////////////////////////////////////////////////////////////////////////
  /// PROTECTED:
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

  ////////////////////////////////////////////////////////////////////////////////
  /// PROTECTED:

  [[nodiscard]] GLVbo< GLData >& getVBO() { return m_vbo; }

  ////////////////////////////////////////////////////////////////////////////////

private:

  ////////////////////////////////////////////////////////////////////////////////
  ///
  /// MEMBERS
  ///
  ////////////////////////////////////////////////////////////////////////////////

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

  // always default to this. overriding it is allowed when supplying a user's buffer
  GLenum m_glDrawMode { GL_TRIANGLE_FAN };

  // these are used to reduce the number of vertices by removing redundancies
  std::vector< uint16_t > m_indices;
};

}

#endif //INC_1828D8268BAA4144A7C8115DFE80A004
