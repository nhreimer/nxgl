#ifndef INC_6A261559B4B647AA89872B03F693AA58
#define INC_6A261559B4B647AA89872B03F693AA58

#include "utilities/Definitions.hpp"

#include "gfx/primitives/GLVao.hpp"

namespace nxgl::gfx
{

// gets fed into the attribute pointer in the VAO
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

struct GLData
{
  nxgl::nxVec2 position;  // x, y
  nxgl::nxColor color;    // r, g, b, a

  /// used for vertex generation. generators expect to create the data that the
  /// VAO describes
  /// \param position
  /// \return
  static GLData createData( const nxgl::nxVec2& position )
  {
    return { position, { 1.f, 1.f, 1.f, 1.f } };
  }

  static GLVao createVAO()
  {
    GLVao vao;
    setDataLayout( vao );
    return std::move( vao );
  }

  /// used for telling OpenGL how the VBO data layout is
  /// \return
  static void setDataLayout( GLVao& vao )
  {
    // if errors arise here related to 502, then see the note in GLVao ctor
    vao.bind();

    // set POSITION
    GLExec( glVertexAttribPointer(
      0,
      2,                // x, y
      GL_FLOAT,
      GL_FALSE,
      sizeof( GLData ),
      ( const GLvoid* )nullptr ) ); // no offset
    GLExec( glEnableVertexAttribArray( 0 ) );

    // set COLOR
    GLExec( glVertexAttribPointer(
      1,
      4,                // r, g, b, a
      GL_FLOAT,
      GL_FALSE,
      sizeof( GLData ),
      ( const GLvoid * )sizeof( nxgl::nxVec2 ) ) );  // offset past the x, y position
    GLExec( glEnableVertexAttribArray( 1 ) );
  }
};

}

#endif //INC_6A261559B4B647AA89872B03F693AA58
