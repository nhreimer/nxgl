#ifndef INC_26E05C3E8DE947A29CC05F205010C862
#define INC_26E05C3E8DE947A29CC05F205010C862

#include <glm/ext/matrix_transform.hpp>

#include "gfx/Primitives.hpp"
#include "gfx/shapes/IColorable.hpp"

namespace nxgl::gfx
{

class Shape : public GLObject
{
public:

  Shape() = delete;

  Shape( GLenum drawMode, GLsizeiptr vertices, GLData * ptrData )
    : GLObject(),
      m_vbo( drawMode, vertices, ptrData )
  {
    // indicate
    setVBO( m_vbo );
  }

  ~Shape() override = default;

protected:

  [[nodiscard]] GLVbo< GLData > getVBO() const { return m_vbo; }

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

};

}

#endif //INC_26E05C3E8DE947A29CC05F205010C862
