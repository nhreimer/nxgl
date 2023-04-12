#include "gfx/shapes/NXSharedShapeContainer.hpp"

#include "gfx/shapes/IColorable.hpp"

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC:
nxgl::gfx::NXSharedShapeContainer::NXSharedShapeContainer( const std::vector< GLData > &buffer )
{
  m_vbo.generate( GL_DYNAMIC_DRAW, buffer.size(), buffer.data());
  GLData::setDataLayout( m_vao );
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC:
nxgl::gfx::NXSharedShapeContainer::NXSharedShapeContainer( nxgl::gfx::GLVbo< nxgl::gfx::GLData > &&vbo )
  : m_vbo( std::move( vbo ))
{
  m_vbo.bind();
  GLData::setDataLayout( m_vao );
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC:
nxgl::gfx::NXSharedShape &nxgl::gfx::NXSharedShapeContainer::getShape( uint32_t i )
{
  assert( i < m_shapes.size());
  return m_shapes[ i ];
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC:
nxgl::gfx::NXSharedShape &nxgl::gfx::NXSharedShapeContainer::addShape( const nxgl::gfx::NXSharedShape &shape )
{
  m_shapes.push_back( shape );
  return m_shapes[ m_shapes.size() - 1 ];
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC:
nxgl::gfx::NXSharedShape &
nxgl::gfx::NXSharedShapeContainer::addShape( GLenum glPrimitive,
                                             GLsizei startDraw,
                                             GLsizei drawCount )
{
  return m_shapes.emplace_back( glPrimitive, startDraw, drawCount );
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC:
void nxgl::gfx::NXSharedShapeContainer::draw( const nxgl::gfx::GLCamera &camera,
                                         nxgl::gfx::IMVPApplicator &mvpApplicator )
{
  m_vao.bind();

  for ( auto &shape: m_shapes )
    shape.draw( camera, mvpApplicator );
}
