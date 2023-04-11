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

  NXSharedShape( GLenum glPrimitive,
                 GLsizei startDraw,
                 GLsizei drawCount )
    : m_glPrimitive( glPrimitive ),
      m_startDraw( startDraw ),
      m_drawCount( drawCount )
  {}

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  void draw( const GLCamera &camera, IMVPApplicator &mvpApplicator ) override
  {
    // don't own the following, so:
    // VAO should already be bound by this point
    // VBO should be bound by this point
    getBlender().blend();
    auto mvp = getModel().getTranslation( camera );
    mvpApplicator.applyMVP( mvp );
    GLExec( glDrawArrays( m_glPrimitive, m_startDraw, m_drawCount ));
  }

private:
  GLenum m_glPrimitive;
  GLsizei m_startDraw;
  GLsizei m_drawCount;
};

}

#endif //INC_1D45A07E64694A66BE85F7C94FF5ED7C
