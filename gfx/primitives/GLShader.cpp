#include "gfx/primitives/GLShader.hpp"

#include <fstream>
#include <sstream>

#include "Logger.hpp"

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC:
nxgl::gfx::GLShader::GLShader()
{
  m_program = GLExec( glCreateProgram() );
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC:
nxgl::gfx::GLShader::~GLShader()
{
  GLExec( glDeleteProgram( m_program ) );
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC:
bool nxgl::gfx::GLShader::loadShader( GLenum shaderType, const std::string &filename )
{
  std::ifstream filestream( filename );
  if ( filestream.is_open() )
  {
    std::stringstream buffer;
    buffer << filestream.rdbuf();
    return addShader( shaderType, buffer.str() );
  }

  LOG_ERROR( "unable to open shader file: {}", filename );
  return false;
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC:
bool nxgl::gfx::GLShader::addShader( GLenum shaderType, const std::string &shader )
{
  // if it's linked then it's closed
  assert( !m_isLinked );

  auto shaderId = glCreateShader( shaderType );
  const char * src = shader.c_str();
  GLExec( glShaderSource( shaderId, 1, &src, nullptr ) );
  GLExec( glCompileShader( shaderId ) );

  GLint compileResultId;
  glGetShaderiv( shaderId, GL_COMPILE_STATUS, &compileResultId );
  if ( compileResultId == GL_FALSE )
  {
    GLint msgLength = 0;
    glGetShaderiv( shaderId, GL_INFO_LOG_LENGTH, &msgLength );
    char* msg = ( char* )_malloca( msgLength * sizeof( char ) );
    glGetShaderInfoLog( shaderId, msgLength, &msgLength, msg );
    LOG_ERROR( "[OpenGL] shader compilation error" );
    LOG_DEBUG( "\n{}", shader );
    LOG_ERROR( msg );

    return false;
  }

  LOG_DEBUG( "[OpenGL] compiled shader:" );
  LOG_DEBUG( "\n{}", shader );
  GLExec( glAttachShader( m_program, shaderId ) );
  return true;
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC:
bool nxgl::gfx::GLShader::link()
{
  if ( m_isLinked ) return true;

  GLExec( glLinkProgram( m_program ) );
  GLExec( glValidateProgram( m_program ) );

  GLint result = 0;
  GLint msgLength = 0;

  // Check the program
  glGetProgramiv( m_program, GL_LINK_STATUS, &result );
  glGetProgramiv( m_program, GL_INFO_LOG_LENGTH, &msgLength );
  if ( msgLength > 0 )
  {
    char * msg = ( char* )_malloca( msgLength * sizeof( char ) );
    glGetProgramInfoLog( m_program, msgLength, nullptr, msg );
    LOG_ERROR( "[OpenGL] error linking program" );
    LOG_ERROR( msg );
    m_isLinked = false;
  }
  else
  {
    m_isLinked = true;

    // clean up after ourselves by deleting the source
//      GLExec( glDetachShader( m_program, shaderId ) );
//      GLExec( glDeleteShader( shaderId ) );
    LOG_DEBUG( "[OpenGL] linked and validated program" );
  }

  return m_isLinked;
}

