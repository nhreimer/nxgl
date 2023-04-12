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
  static GLData createData( const nxgl::nxVec2& position );

  /// creates a VAO, setting the data layout to GLData
  /// \return
  static GLVao createVAO();

  /// used for telling OpenGL how the VBO data layout is
  /// \return
  static void setDataLayout( GLVao& vao );
};

}

#endif //INC_6A261559B4B647AA89872B03F693AA58
