#ifndef AEA3EE1E1A194C55A7FCA8415AB465B9
#define AEA3EE1E1A194C55A7FCA8415AB465B9

#include "gfx/shapes/IMVPApplicator.hpp"
#include "gfx/shapes/IColorable.hpp"

namespace nxgl::gfx
{

template < typename TData, typename TUint >
struct IVertexGenerator
{
  static_assert( std::is_integral< TUint >::value &&
                 std::is_unsigned< TUint >::value );

  virtual GLenum getPrimitiveMode() = 0;
  virtual void createVertices( GLVbo< TData >& vbo, GLIbo< TUint >& ibo ) = 0;
};

class GLPolygon
{

public:

  GLPolygon( GLenum bufferUsage, uint8_t edges )
    : m_vao( GLData::createVAO() ),
      m_vbo( bufferUsage, ( GLsizeiptr )( edges + 1 ), nullptr ),
      m_ibo( bufferUsage, ( GLsizeiptr )( edges * 3 ), nullptr ),
      m_edges( edges )

  {
    // vbo size = edges + 1 because we use the triangle fan, which gives a pivot point in the center
    //                      so, 4 points in a rectangle and then 1 point in the center.
    // vbo size = ( edges + 1 ) * 2 because the outline is 1st-class citizen of the buffer

    // ibo size = edges * 3 because that's how many triangles we need to create
    // ibo size = ( edges * 3 ) * 2 because of the outline

    // if you have more complex triangle fan-based shapes, then use a different approach
    assert( edges > 2 && edges < INT8_MAX );

    m_vbo.bind();

    // TODO: use IVertexGenerator
    createVertices();
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

    GLExec( glDrawElements( GL_TRIANGLE_FAN,
                            m_vbo.size(),
                            GL_UNSIGNED_BYTE,
                            nullptr ) );
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  GLModel& getModel() { return m_model; }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  void setModel( const GLModel& model ) { m_model = model; }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  void setColor( const IColorable& colorizer )
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
  GLBlend& getBlender() { return m_blender; }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  void setBlender( const GLBlend& blend ) { m_blender = blend; }

private:

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
    std::vector< GLData > vertexBuffer( ( m_edges + 1 ) * 2 );
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

    // fill the buffers
    m_vbo.fill( vertexBuffer.data() );
    m_ibo.fill( indexBuffer.data() );
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

  GLVbo< GLData > m_vbo;
  GLIbo< GLubyte > m_ibo;
  GLVao m_vao;

  GLubyte m_edges { 0 };

  GLBlend m_blender;
  GLModel m_model;

};

}

#endif //AEA3EE1E1A194C55A7FCA8415AB465B9
