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
  /// PUBLIC:
  /// registers a VBO with the VAO. this registration happens once.
  /// \param vbo
  void registerVBO( const GLVbo< GLData >& vbo )
  {
    assert( m_arrayCount == 0 );

    // this is also included in GLData.hpp
    // attributes push data to the initial shader via Layout
    //  _________________________________________________
    // |_________________________________________________|
    // |          VERTEX BUFFER OBJECT LAYOUT            |
    // |-------------------------------------------------|
    // |       GLData 0        |        GLData 1         | ...
    // |-------------------------------------------------|
    // | POS 0 |    COLOR 0    | POS 1 |     COLOR 1     | ...
    // |-------------------------------------------------|
    // | x | y | r | g | b | a | x | y | r | g |  b |  a | ...
    // |-------------------------------------------------|

    // ensure that the VAO is selected PRIOR to adding elements to it
    bind();

    // set POSITION
    GLExec( glVertexAttribPointer(
      m_arrayCount,
      2,                // x, y
      GL_FLOAT,
      GL_FALSE,
      sizeof( GLData ),
      ( const GLvoid* )nullptr ) ); // no offset
    GLExec( glEnableVertexAttribArray( m_arrayCount ) );

    ++m_arrayCount;

    // set COLOR
    GLExec( glVertexAttribPointer(
      m_arrayCount,
      4,                // r, g, b, a
      GL_FLOAT,
      GL_FALSE,
      sizeof( GLData ),
      ( const GLvoid * )sizeof( nxgl::nxVec2 ) ) );  // offset past the x, y position
    GLExec( glEnableVertexAttribArray( m_arrayCount ) );

    ++m_arrayCount;
  }

  ////////////////////////////////////////////////////////////////////////////////

private:
  GLuint m_vao { 0 };             // VAO id
  GLuint m_arrayCount { 0 };      // the number attrib indices
};

}

#endif //E4701D3F8E3B42E88AEDDF6A67AF66E4
