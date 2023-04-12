#include "gfx/shapes/NXShape.hpp"

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC:
nxgl::gfx::NXShape::NXShape( GLenum glPrimitive, const std::vector< GLData > &buffer )
  : m_glPrimitive( glPrimitive )
{
  m_vbo.generate( GL_DYNAMIC_DRAW, buffer.size(), buffer.data() );
  GLData::setDataLayout( m_vao );
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC:
nxgl::gfx::NXShape::NXShape( nxgl::gfx::IGenerator &generator )
  : m_glPrimitive( generator.getPrimitiveType() )
{
  std::vector< GLData > buffer;
  auto elementCount = generator.generate( buffer );
  m_vbo.generate( GL_DYNAMIC_DRAW, elementCount, buffer.data() );
  GLData::setDataLayout( m_vao );
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC:
nxgl::gfx::NXShape::NXShape( GLenum glPrimitive, size_t elements, const nxgl::gfx::GLData *pReadBuffer )
  : m_glPrimitive( glPrimitive )
{
  m_vbo.generate( GL_DYNAMIC_DRAW, elements, pReadBuffer );
  GLData::setDataLayout( m_vao );
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC:
void nxgl::gfx::NXShape::draw( const nxgl::gfx::GLCamera &camera, nxgl::gfx::IMVPApplicator &mvpApplicator )
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
void nxgl::gfx::NXShape::setColor( nxgl::gfx::IColorable &colorizer )
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
void nxgl::gfx::NXShape::setColor( uint32_t index, const nxgl::nxColor &color )
{
  assert( sizeof( GLData ) * index + sizeof( nxgl::nxVec2 ) < m_vbo.size() );

  m_vbo.bind();

  GLExec( glBufferSubData(
    GL_ARRAY_BUFFER,
    ( GLintptr )( sizeof( GLData ) * index + sizeof( nxgl::nxVec2 ) ),
    sizeof( nxgl::nxColor ),
    &color ) );
}



