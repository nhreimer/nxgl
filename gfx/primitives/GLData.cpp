#include "Pch.hpp"

#include "gfx/primitives/GLData.hpp"
#include "gfx/primitives/GLUtility.hpp"

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC STATIC:
nxgl::gfx::GLData nxgl::gfx::GLData::createData( const nxgl::nxVec2 &position )
{
  return { position, { 1.f, 1.f, 1.f, 1.f } };
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC STATIC:
nxgl::gfx::GLVao nxgl::gfx::GLData::createVAO()
{
  GLVao vao;
  setDataLayout( vao );
  return std::move( vao );
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC STATIC:
void nxgl::gfx::GLData::setDataLayout( nxgl::gfx::GLVao &vao )
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


