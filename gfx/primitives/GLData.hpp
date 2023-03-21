#ifndef INC_6A261559B4B647AA89872B03F693AA58
#define INC_6A261559B4B647AA89872B03F693AA58

#include "gfx/standards/Definitions.hpp"

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
};

}

#endif //INC_6A261559B4B647AA89872B03F693AA58
