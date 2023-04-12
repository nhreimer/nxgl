#ifndef CC3F21C5E8404D4B956540A39128F6C7
#define CC3F21C5E8404D4B956540A39128F6C7

#include "Pch.hpp"

#include <format>
#include "Logger.hpp"

namespace nxgl::gfx
{
class OpenGLUtility
{
public:
  static void GLClearErrors();
  static bool GLLog( const char * function, const char * file, int line );
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
