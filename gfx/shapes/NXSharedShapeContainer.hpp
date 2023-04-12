#ifndef EE886E66331D4B5C8EDD27D166E955F8
#define EE886E66331D4B5C8EDD27D166E955F8

#include "gfx/shapes/NXSharedShape.hpp"

namespace nxgl::gfx
{

struct IColorable;

class NXSharedShapeContainer
{
public:

  explicit NXSharedShapeContainer( const std::vector< GLData > &buffer );

  explicit NXSharedShapeContainer( GLVbo< GLData > &&vbo );

  [[nodiscard]]
  inline size_t size() const  { return m_shapes.size(); }

  NXSharedShape &getShape( uint32_t i );

  NXSharedShape &addShape( const NXSharedShape &shape );

  NXSharedShape &addShape( GLenum glPrimitive,
                           GLsizei startDraw,
                           GLsizei drawCount );

  void draw( const GLCamera &camera, IMVPApplicator &mvpApplicator );

private:

  GLVbo< GLData > m_vbo;
  GLVao m_vao;

  std::vector< NXSharedShape > m_shapes;
};

}

#endif //EE886E66331D4B5C8EDD27D166E955F8
