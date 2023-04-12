#include "gfx/primitives/GLUtility.hpp"

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC:
void nxgl::gfx::OpenGLUtility::GLClearErrors()
{
  while ( glGetError() != GL_NO_ERROR );
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC:
bool nxgl::gfx::OpenGLUtility::GLLog( const char *function, const char *file, int line )
{
  GLenum error = glGetError();

  if ( error )
  {
    LOG_ERROR( "[OpenGL][{}]: {} in {}:{}", std::format( "{:x}", error ), function, file, line );
    return false;
  }

  return true;
}

