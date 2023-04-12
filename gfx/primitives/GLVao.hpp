#ifndef E4701D3F8E3B42E88AEDDF6A67AF66E4
#define E4701D3F8E3B42E88AEDDF6A67AF66E4

#include "Pch.hpp"

#include "utilities/Definitions.hpp"
#include "gfx/primitives/GLUtility.hpp"

namespace nxgl::gfx
{

/***
 * GLVao uses a 1-to-1 VAO-to-VBO mapping. Create a large enough VBO to hold
 * whatever content you need.
 */
class GLVao
{

public:

  GLVao( const GLVao& other ) = delete;
  GLVao& operator=( const GLVao& other ) noexcept = delete;

  GLVao( GLVao&& other )
  {
    std::swap( m_vao, other.m_vao );
  }

  GLVao& operator=( GLVao&& other ) noexcept
  {
    std::swap( m_vao, other.m_vao );
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  GLVao()
  {
    // NOTE: VAO can be generated, but 502 errors may occur whenever
    //       the VAO is bound PRIOR to VBO's data being set
    GLExec( glGenVertexArrays( 1, &m_vao ) );
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  ~GLVao()
  {
    // if it's 0 then it didn't bind or the object's been moved. either way,
    // it'll create an opengl error if you try to delete an invalid VAO ID
    if ( m_vao > 0 )
    {
      GLExec( glDeleteVertexArrays( 1, &m_vao ) );
    }
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  /// see ctor note if you get 502 errors on bind
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
