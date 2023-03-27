#ifndef AEA3EE1E1A194C55A7FCA8415AB465B9
#define AEA3EE1E1A194C55A7FCA8415AB465B9

#include "gfx/shapes/IMVPApplicator.hpp"
#include "gfx/shapes/IColorable.hpp"
#include "gfx/shapes/IGenerator.hpp"

namespace nxgl::gfx
{

class GLPolygon
{

public:

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  ///
  /// \param bufferUsage GL_STATIC_DRAW, GL_DYNAMIC_DRAW, etc
  /// \param edges edges in polygon
  GLPolygon( GLenum bufferUsage, uint8_t edges )
    : m_vao( GLData::createVAO() ),
      m_vbo( bufferUsage, ( GLsizeiptr )( ( edges + 1 ) * 2 ), nullptr ),
      m_ibo( bufferUsage, ( GLsizeiptr )( edges * 3 ), nullptr ),
      m_iboFill( bufferUsage, ( GLsizeiptr )( edges * 3 ), nullptr ),
      m_edges( edges )
  {
    // the VBO contains both the fill and outline data
    // there are two IBOs: 1) for outline, 2) for fill

    // if you have more complex triangle fan-based shapes, then use a different approach
    assert( edges > 2 && edges < INT8_MAX );

    m_vbo.bind();

    createVertices();
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  ///
  /// \param bufferUsage GL_STATIC_DRAW, GL_DYNAMIC_DRAW, etc
  /// \param edges edges in polygon
  /// \param generator the vertex and index generator. do not generate indices for the outline
  GLPolygon( GLenum bufferUsage, uint8_t edges, IGenerator< GLData >& generator )
    : m_vao( GLData::createVAO() ),
      m_vbo( bufferUsage, ( GLsizeiptr )( ( edges + 1 ) * 2 ), nullptr ),
      m_ibo( bufferUsage, ( GLsizeiptr )( edges * 3 ), nullptr ),
      m_iboFill( bufferUsage, ( GLsizeiptr )( edges * 3 ), nullptr ),
      m_edges( edges ),
      m_glDrawMode( generator.getMode() )
  {
    // the VBO contains both the fill and outline data
    // there are two IBOs: 1) for outline, 2) for fill

    // if you have more complex triangle fan-based shapes, then use a different approach
    assert( edges > 2 && edges < INT8_MAX );

    m_vbo.bind();

    auto vertexBuffer = generator.generateVertices( edges );
    auto indexBuffer = generator.generateIndices( edges );

    m_vbo.setDataRange( 0, m_edges + 1, vertexBuffer.data() );
    m_vbo.setDataRange( m_edges + 1, m_edges + 1, vertexBuffer.data() );
    m_ibo.fill( indexBuffer.data() );

    std::transform(
      indexBuffer.begin(),
      indexBuffer.end(),
      indexBuffer.begin(), [ & ]( GLubyte index ) { return index + m_edges; } );

    m_iboFill.fill( indexBuffer.data() );
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  void draw( const GLCamera& camera, IMVPApplicator& mvpApplicator )
  {
    // draw: bind() -> blend() -> transform() -> draw()
    m_ibo.bind();
    m_vao.bind();
    m_blender.blend();

    auto mvp = m_model.getTranslation( camera );
    mvpApplicator.applyMVP( mvp );

    GLExec( glDrawElements( m_glDrawMode,
                            m_ibo.size(),
                            GL_UNSIGNED_BYTE,
                            nullptr ) );

    m_iboFill.bind();
    m_vao.bind();

    m_modelFill = m_model;
    m_modelFill.setScale( m_model.getScale() * .8f );
    mvp = m_modelFill.getTranslation( camera );
    mvpApplicator.applyMVP( mvp );

    GLExec( glDrawElements( m_glDrawMode, m_iboFill.size(), GL_UNSIGNED_BYTE, nullptr ) );
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  GLModel& getModel() { return m_model; }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  void setModel( const GLModel& model ) { m_model = model; }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  void setOutlineColor( IColorable& colorizer )
  {
    for ( uint32_t vertexIndex = 0;
          vertexIndex < m_edges + 1;
          ++vertexIndex )
    {
      setColor( vertexIndex, colorizer( vertexIndex ) );
    }
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  void setFillColor( IColorable& colorizer )
  {
    for ( uint32_t vertexIndex = m_edges + 1, colorIndex = 0;
          vertexIndex < ( m_edges + 1 ) * 2;
          ++vertexIndex, ++colorIndex )
    {
      setColor( vertexIndex, colorizer( colorIndex ) );
    }
  }


  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  GLBlend& getBlender() { return m_blender; }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  void setBlender( const GLBlend& blend ) { m_blender = blend; }

private:

  ////////////////////////////////////////////////////////////////////////////////
  /// PRIVATE:
  void createVertices()
  {
    // divide a circle by the number of edges
    // which is A(0, 0) -> B(0, 1) -> C(1, 1)
    // note that C is slightly less than 1, 1 however
    auto angle = nxgl::NX_TAU / ( float )m_edges;

    // move counterclockwise starting at the center (0, 0)
    // ABC
    // ACD
    // ADE ...

    nxgl::nxColor white { 1.f, 1.f, 1.f, 1.f };

    // put the data in a temporary buffer, so we can dump the data all at once
    // and this provides some better debugging opportunities
    std::vector< GLData > vertexBuffer( m_edges + 1 );
    std::vector< GLubyte > indexBuffer( m_edges * 3 );

    vertexBuffer[ 0 ] = { { 0, 0 }, white };
    vertexBuffer[ 1 ] = { { std::cos( 0 ), std::sin( 0 ) }, white };

    uint32_t posInBuffer = 2;

    for ( uint32_t edge = 0, idx = 0;
          edge < m_edges;
          ++edge, idx += 3 )
    {
      if ( edge + 1 < m_edges )
      {
        auto vertexAngle = ( float )( edge + 1 ) * angle;

        vertexBuffer[ posInBuffer ] =
          { { std::cos( vertexAngle ), std::sin( vertexAngle ) }, white };

        ++posInBuffer;
      }

      // the pattern is: (0, 1, 2); (0, 2, 3); (0, 3, 4); ... ; (0, n, 1)
      indexBuffer[ idx ] = 0;
      indexBuffer[ idx + 1 ] = edge + 1;

      if ( edge + 1 < m_edges )
      {
        indexBuffer[ idx + 2 ] = edge + 2;
      }
      else
      {
        indexBuffer[ idx + 2 ] = 1;
      }
    }


    m_vbo.setDataRange( 0, m_edges + 1, vertexBuffer.data() );
    m_vbo.setDataRange( m_edges + 1, m_edges + 1, vertexBuffer.data() );
    m_ibo.fill( indexBuffer.data() );

    std::transform(
      indexBuffer.begin(),
      indexBuffer.end(),
      indexBuffer.begin(), [ & ]( GLubyte index ) { return index + m_edges; } );

    m_iboFill.fill( indexBuffer.data() );
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PRIVATE:
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

  // VBO: outline and fill
  GLVbo< GLData > m_vbo;

  // IBO: outline
  GLIbo< GLubyte > m_ibo;

  // IBO: fill
  GLIbo< GLubyte > m_iboFill;
  GLVao m_vao;

  GLubyte m_edges { 0 };

  GLBlend m_blender;
  GLModel m_model;
  GLModel m_modelFill;

  GLenum m_glDrawMode { GL_TRIANGLE_FAN };
};

}

#endif //AEA3EE1E1A194C55A7FCA8415AB465B9
