#ifndef INC_766C014EAECE42219FFF49CB59341F59
#define INC_766C014EAECE42219FFF49CB59341F59

#include "gfx/shapes/NXObject.hpp"
#include "gfx/shapes/IColorable.hpp"

namespace nxgl::gfx
{

class NXShape : public NXObject
{

public:

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  NXShape( GLenum glPrimitive, const std::vector< GLData >& buffer )
    : m_glPrimitive( glPrimitive )
  {
    m_vbo.generate( GL_DYNAMIC_DRAW, buffer.size(), buffer.data() );
    GLData::setDataLayout( m_vao );
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  explicit NXShape( IGenerator& generator )
    : m_glPrimitive( generator.getPrimitiveType() )
  {
    std::vector< GLData > buffer;
    auto elementCount = generator.generate( buffer );
    m_vbo.generate( GL_DYNAMIC_DRAW, elementCount, buffer.data() );
    GLData::setDataLayout( m_vao );
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  NXShape( GLenum glPrimitive, size_t elements, const GLData * pReadBuffer )
    : m_glPrimitive( glPrimitive )
  {
    m_vbo.generate( GL_DYNAMIC_DRAW, elements, pReadBuffer );
    GLData::setDataLayout( m_vao );
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  void draw( const GLCamera& camera, IMVPApplicator& mvpApplicator ) override
  {
    m_vao.bind();
    m_vbo.bind();
    getBlender().blend();
    auto mvp = getModel().getTranslation( camera );
    mvpApplicator.applyMVP( mvp );
    GLExec( glDrawArrays( m_glPrimitive, 0, ( GLsizei )m_vbo.elementCount() ) );
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  void setColor( IColorable& colorizer )
  {
    for ( uint32_t vertexIndex = 0;
          vertexIndex < m_vbo.elementCount();
          ++vertexIndex )
    {
      auto color = colorizer( vertexIndex );
      setColor( vertexIndex, colorizer( vertexIndex ) );
    }
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
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

  GLenum m_glPrimitive;

};

}

#endif //INC_766C014EAECE42219FFF49CB59341F59
