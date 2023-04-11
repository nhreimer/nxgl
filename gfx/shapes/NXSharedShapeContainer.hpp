#ifndef EE886E66331D4B5C8EDD27D166E955F8
#define EE886E66331D4B5C8EDD27D166E955F8

#include "gfx/shapes/NXObject.hpp"
#include "gfx/shapes/NXSharedShape.hpp"
#include "gfx/shapes/IColorable.hpp"

namespace nxgl::gfx
{

class NXSharedShapeContainer
{
public:

  explicit NXSharedShapeContainer( const std::vector< GLData > &buffer )
  {
    m_vbo.generate( GL_DYNAMIC_DRAW, buffer.size(), buffer.data());
    GLData::setDataLayout( m_vao );
  }

  explicit NXSharedShapeContainer( GLVbo< GLData > &&vbo )
    : m_vbo( std::move( vbo ))
  {
    m_vbo.bind();
    GLData::setDataLayout( m_vao );
  }

  [[nodiscard]]
  inline size_t size() const
  { return m_shapes.size(); }

  NXSharedShape &getShape( uint32_t i )
  {
    assert( i < m_shapes.size());
    return m_shapes[ i ];
  }

  NXSharedShape &addShape( const NXSharedShape &shape )
  {
    m_shapes.push_back( shape );
    return m_shapes[ m_shapes.size() - 1 ];
  }

  NXSharedShape &addShape( GLenum glPrimitive,
                           GLsizei startDraw,
                           GLsizei drawCount )
  {
    return m_shapes.emplace_back( glPrimitive, startDraw, drawCount );
  }

  void draw( const GLCamera &camera, IMVPApplicator &mvpApplicator )
  {
    m_vao.bind();

    for ( auto &shape: m_shapes )
      shape.draw( camera, mvpApplicator );
  }

private:

  GLVbo< GLData > m_vbo;
  GLVao m_vao;

  std::vector< NXSharedShape > m_shapes;
};

}

#endif //EE886E66331D4B5C8EDD27D166E955F8
