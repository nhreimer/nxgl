#ifndef INC_318D0B9B45B4468B88E526CEB17E3543
#define INC_318D0B9B45B4468B88E526CEB17E3543

#include "gfx/primitives/GLUtility.hpp"
#include "gfx/primitives/GLBuffer.hpp"

namespace nxgl::gfx
{

template < typename TData >
class GLVbo : public GLBuffer< GL_ARRAY_BUFFER, TData >
{
public:
  GLVbo( GLenum bufferUsage, GLsizeiptr elementCount )
    : GLBuffer< GL_ARRAY_BUFFER, TData >( bufferUsage, elementCount * sizeof( TData ), nullptr )
  {}

  GLVbo( GLenum bufferUsage, GLsizeiptr elementCount, const TData * pReadBuffer )
    : GLBuffer< GL_ARRAY_BUFFER, TData >( bufferUsage, elementCount * sizeof( TData ), pReadBuffer )
  {}
};

}

#endif //INC_318D0B9B45B4468B88E526CEB17E3543
