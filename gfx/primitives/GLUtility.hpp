#ifndef CC3F21C5E8404D4B956540A39128F6C7
#define CC3F21C5E8404D4B956540A39128F6C7

#include <format>

namespace nxgl::gfx
{
class OpenGLUtility
{
public:
  static void GLClearErrors()
  {
    while ( glGetError() != GL_NO_ERROR );
  }

  static bool GLLog( const char * function, const char * file, int line )
  {
    GLenum error = glGetError();

    if ( error )
    {
      LOG_ERROR( "[OpenGL][{}]: {} in {}:{}", std::format( "{:x}", error ), function, file, line );
      return false;
    }

    return true;
  }
};
}

#ifndef NXGL_GL_ERROR_HANDLING
#define NXGL_GL_ERROR_HANDLING

#ifdef _DEBUG

#define GL_ASSERT( x ) if ( !( x ) ) __debugbreak()

//#define GLExec( x ) nxgl::gfx::OpenGLUtility::GLClearErrors(); \
// x;                                                            \
// GL_ASSERT( nxgl::gfx::OpenGLUtility::GLLog( #x, __FILE__, __LINE__ ) )
#define GLExec( x ) x;                                            \
 GL_ASSERT( nxgl::gfx::OpenGLUtility::GLLog( #x, __FILE__, __LINE__ ) )
#else
#define GLExec( x ) x;
#endif

#endif

#endif //CC3F21C5E8404D4B956540A39128F6C7
