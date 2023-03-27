#ifndef INC_0C476FBFB28C4E8583578F242F0CC8BE
#define INC_0C476FBFB28C4E8583578F242F0CC8BE

namespace nxgl::gfx
{

class GLTriangleCircle : public GLObject
{
public:

  explicit GLTriangleCircle( uint8_t triangles, float radius = 50.f )
    : m_vbo( GL_DYNAMIC_DRAW, triangles * 3 ),
      m_vao( GLData::createVAO() ),
      m_triangles( triangles ),
      m_radius( 50.f )
  {
    getModel().setScale( { radius, radius } );
    setBuffer();
  }

  void draw( const GLCamera& camera, IMVPApplicator& mvpApplicator ) override
  {
    m_vbo.bind();
    m_vao.bind();

    auto mvp = getModel().getTranslation( camera );
    mvpApplicator.applyMVP( mvp );

    glDrawArrays( GL_TRIANGLES, 0, m_triangles * 3 );

    m_vbo.unbind();
    m_vao.unbind();
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  void setColor( IColorable& colorizer )
  {
    for ( uint32_t vertexIndex = 0, colorIndex = 0;
          vertexIndex < ( m_triangles * 3 );
          ++vertexIndex, ++colorIndex )
    {
      GLObject::setColor( m_vbo, vertexIndex, colorizer( colorIndex ) );
    }
  }

private:

  void setBuffer()
  {
    std::vector< GLData > vertexBuffer( m_triangles * 3 );

    nxColor white { 1.f, 1.f, 1.f, 1.f };

    auto angle = NX_TAU / ( float )m_triangles;

    float previousAngle = 0.f;

    for ( int i = 0, idx = 0; i < m_triangles; i += 3 )
    {
      // A is always at the center
      vertexBuffer[ idx ] = { { 0.f, 0.f }, white };
      vertexBuffer[ idx + 1 ] = { { std::cos( previousAngle ), std::sin( previousAngle ) }, white };

      previousAngle += angle;

      vertexBuffer[ idx + 2 ] = { { std::cos( previousAngle ), std::sin( previousAngle ) }, white };
    }

    m_vbo.fill( vertexBuffer.data() );
  }

private:

  GLVbo< GLData > m_vbo;
  GLVao m_vao;

  uint8_t m_triangles { 0 };
  float m_radius;
};

}

#endif //INC_0C476FBFB28C4E8583578F242F0CC8BE
