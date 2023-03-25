#ifndef INC_58ADFBDE4B6C4795B0BF03574BD68279
#define INC_58ADFBDE4B6C4795B0BF03574BD68279

namespace nxgl::gfx
{

template < typename TUint >
class GLIbo : public GLBuffer< GL_ELEMENT_ARRAY_BUFFER, TUint >
{
public:

  // opengl is strict about the unsigned part
  static_assert( std::is_integral< TUint >::value &&
                 std::is_unsigned< TUint >::value,
                 "TUint must be an unsigned integer!" );

  GLIbo( GLenum bufferUsage, GLsizeiptr elementCount )
    : GLBuffer< GL_ELEMENT_ARRAY_BUFFER, TUint >( bufferUsage, elementCount * sizeof( TUint ), nullptr )
  {}

  GLIbo( GLenum bufferUsage, GLsizeiptr elementCount, const TUint * pReadBuffer )
    : GLBuffer< GL_ELEMENT_ARRAY_BUFFER, TUint >( bufferUsage, elementCount * sizeof( TUint ), pReadBuffer )
  {}

private:
};

}

#endif //INC_58ADFBDE4B6C4795B0BF03574BD68279
