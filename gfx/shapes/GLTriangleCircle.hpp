#ifndef INC_0C476FBFB28C4E8583578F242F0CC8BE
#define INC_0C476FBFB28C4E8583578F242F0CC8BE

#include "utilities/Math.hpp"

namespace nxgl::gfx
{

class GLTriangleCircle : public GLObject
{
public:

  GLTriangleCircle( uint8_t triangles, float gap )
    : m_vbo( GL_DYNAMIC_DRAW, triangles * 3 ),
      m_vao( GLData::createVAO() ),
      m_triangles( triangles ),
      m_gap( gap )
  {
    assert( gap >= 0.f && gap <= 1.f );
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
    nxColor red { 1.f, 0.f, 0.f, 1.f };
    nxColor green { 0.f, 1.f, 0.f, 1.f };
    nxColor blue { 0.f, 0.f, 1.f, 1.f };

    auto angle = ( NX_TAU / ( float )m_triangles );

    float previousAngle = 0.f;

    float angleOffset = glm::radians( 45.f );

    for ( int i = 0, idx = 0; i < m_triangles; ++i, idx += 3 )
    {
      nxColor color = white;

      switch ( i )
      {
        case 0:
          color = red;
          break;
        case 1:
          color = white;
          break;
        case 2:
          color = green;
          break;
        case 3:
          color = blue;
          break;
        default:
          break;
      }

      // B
      vertexBuffer[ idx + 1 ] =
        {
          { std::cos( previousAngle + angleOffset ),
            std::sin( previousAngle + angleOffset ) },
          color
        };

      // C
      previousAngle = ( float )( i + 1 ) * angle;
      vertexBuffer[ idx + 2 ] =
        {
          { std::cos( previousAngle + angleOffset ),
            std::sin( previousAngle + angleOffset ) },
          color
        };

      nxVec2 centroid {
        ( ( vertexBuffer[ idx + 1 ].position.x +
          vertexBuffer[ idx + 2 ].position.x ) / 3.f ) * m_gap,
        ( ( vertexBuffer[ idx + 1 ].position.y +
          vertexBuffer[ idx + 2 ].position.y ) / 3.f ) * m_gap
      };

      // A
      vertexBuffer[ idx ] = { { centroid }, color };
      vertexBuffer[ idx + 1 ].position *= m_gap;
      vertexBuffer[ idx + 2 ].position *= m_gap;
    }

    m_vbo.fill( vertexBuffer.data() );
  }

private:

  GLVbo< GLData > m_vbo;
  GLVao m_vao;

  uint8_t m_triangles { 0 };

  float m_gap;
};

}

#endif //INC_0C476FBFB28C4E8583578F242F0CC8BE
