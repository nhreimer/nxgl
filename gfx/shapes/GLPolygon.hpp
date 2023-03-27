#ifndef AEA3EE1E1A194C55A7FCA8415AB465B9
#define AEA3EE1E1A194C55A7FCA8415AB465B9

#include "gfx/shapes/GLObject.hpp"
#include "gfx/shapes/IMVPApplicator.hpp"
#include "gfx/shapes/IColorable.hpp"
#include "gfx/shapes/IGenerator.hpp"

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
      m_vbo( bufferUsage, ( GLsizeiptr )( ( edges + 1 ) * 2 ), nullptr ),
      m_ibo( bufferUsage, ( GLsizeiptr )( edges * 3 ), nullptr ),
      m_iboFill( bufferUsage, ( GLsizeiptr )( edges * 3 ), nullptr ),
      m_edges( edges )
  {
    // the VBO contains both the fill and outline data
    // there are two IBOs: 1) for outline, 2) for fill

    // if you have more complex triangle fan-based shapes, then use a different approach
    assert( edges > 2 && edges < INT8_MAX );

    // this works the smoothest with the SpectrumColorizer, so this is the default
    // use the other ctor to provide a custom GLData generator
    CCWBCAPolygon< GLData > polygonCreator;
    m_vbo.bind();
    setBuffers( polygonCreator );
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  ///
  /// \param bufferUsage GL_STATIC_DRAW, GL_DYNAMIC_DRAW, etc
  /// \param edges edges in polygon
  /// \param generator the vertex and index generator. do not generate indices for the outline
  GLPolygon( GLenum bufferUsage,
             uint8_t edges,
             IGenerator< GLData >& generator )
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
    setBuffers( generator );
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  void draw( const GLCamera& camera, IMVPApplicator& mvpApplicator ) override
  {
    // bind the VAO
    m_vao.bind();
    {
      // bind the first IBO
      {
        m_ibo.bind();
        getBlender().blend();
        drawFill( camera, mvpApplicator );
        m_ibo.unbind();
      }
      // bind the second IBO
      {
        m_iboFill.bind();
        drawOutline( camera, mvpApplicator );
        m_iboFill.unbind();
      }
    }
    // unbind the VAO
    m_vao.unbind();
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  void setOutlineColor( IColorable& colorizer )
  {
    for ( uint32_t vertexIndex = 0;
          vertexIndex < m_edges + 1;
          ++vertexIndex )
    {
      setColor( m_vbo, vertexIndex, colorizer( vertexIndex ) );
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

    GLExec( glDrawElements( m_glDrawMode,
                            m_ibo.size(),
                            GL_UNSIGNED_BYTE,
                            nullptr ) );
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

      GLExec( glDrawElements( m_glDrawMode, m_iboFill.size(), GL_UNSIGNED_BYTE, nullptr ) );
    }
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PRIVATE:
  void setBuffers( IGenerator< GLData >& generator )
  {
    auto vertexBuffer = generator.generateVertices( m_edges );
    auto indexBuffer = generator.generateIndices( m_edges );

    m_vbo.setDataRange( 0, m_edges + 1, vertexBuffer.data() );
    m_vbo.setDataRange( m_edges + 1, m_edges + 1, vertexBuffer.data() );
    m_ibo.fill( indexBuffer.data() );

    std::transform(
      indexBuffer.begin(),
      indexBuffer.end(),
      indexBuffer.begin(), [ & ]( GLubyte index ) { return index + m_edges; } );

    m_iboFill.fill( indexBuffer.data() );
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

  GLModel m_modelFill;

  GLenum m_glDrawMode { GL_TRIANGLE_FAN };

  float m_outlinePercentage { .2f };
};

}

#endif //AEA3EE1E1A194C55A7FCA8415AB465B9
