#ifndef INC_1D45A07E64694A66BE85F7C94FF5ED7C
#define INC_1D45A07E64694A66BE85F7C94FF5ED7C

#include "gfx/shapes/NXObject.hpp"
#include "gfx/shapes/IColorable.hpp"

namespace nxgl::gfx
{

////////////////////////////////////////////////////////////////////////////////

class NXSharedShape : public NXObject
{
public:

  NXSharedShape( GLenum glPrimitive, GLsizei startDraw, GLsizei drawCount );

  void draw( const GLCamera &camera, IMVPApplicator &mvpApplicator ) override;

private:
  GLenum m_glPrimitive;
  GLsizei m_startDraw;
  GLsizei m_drawCount;
};

}

#endif //INC_1D45A07E64694A66BE85F7C94FF5ED7C
