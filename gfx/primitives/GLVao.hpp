#ifndef E4701D3F8E3B42E88AEDDF6A67AF66E4
#define E4701D3F8E3B42E88AEDDF6A67AF66E4

#include "utilities/Definitions.hpp"

namespace nxgl::gfx
{

/***
 * GLVao uses a 1-to-1 VAO-to-VBO mapping. Create a large enough VBO to hold
 * whatever content you need.
 */
class GLVao
{

public:

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  GLVao()
  {
    GLExec( glGenVertexArrays( 1, &m_vao ) );
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  ~GLVao()
  {
    GLExec( glDeleteVertexArrays( 1, &m_vao ) );
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  inline void bind() const { GLExec( glBindVertexArray( m_vao ) ); }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  inline void unbind() const { GLExec( glBindVertexArray( 0 ) ); }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  [[nodiscard]] inline GLuint id() const { return m_vao; }

  ////////////////////////////////////////////////////////////////////////////////

private:
  GLuint m_vao { 0 };             // VAO id
};

}

#endif //E4701D3F8E3B42E88AEDDF6A67AF66E4
