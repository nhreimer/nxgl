#include "gfx/shapes/NXSharedShape.hpp"

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC:
nxgl::gfx::NXSharedShape::NXSharedShape( GLenum glPrimitive, GLsizei startDraw, GLsizei drawCount )
  : m_glPrimitive( glPrimitive ),
    m_startDraw( startDraw ),
    m_drawCount( drawCount )
{}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC:
void nxgl::gfx::NXSharedShape::draw( const nxgl::gfx::GLCamera &camera, nxgl::gfx::IMVPApplicator &mvpApplicator )
{
  // don't own the following, so:
  // VAO should already be bound by this point
  // VBO should be bound by this point
  getBlender().blend();
  auto mvp = getModel().getTranslation( camera );
  mvpApplicator.applyMVP( mvp );
  GLExec( glDrawArrays( m_glPrimitive, m_startDraw, m_drawCount ));
}


