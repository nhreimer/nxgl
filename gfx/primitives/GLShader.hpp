#ifndef INC_4B6A67D3562C4C0C8E779F3ECBF648BA
#define INC_4B6A67D3562C4C0C8E779F3ECBF648BA

#include <fstream>
#include <sstream>

#include <glm/gtc/type_ptr.hpp>

namespace nxgl::gfx
{

class GLShader
{
public:

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  GLShader()
  {
    m_program = GLExec( glCreateProgram() );
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  ~GLShader()
  {
    GLExec( glDeleteProgram( m_program ) );
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  [[nodiscard]] inline GLuint id() const { return m_program; }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  [[nodiscard]] inline GLint getUniformAddress( const std::string& uniform ) const
  {
    return glGetUniformLocation( m_program, uniform.c_str() );
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  [[nodiscard]] bool loadShader( GLenum shaderType, const std::string& filename )
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
  [[nodiscard]] bool addShader( GLenum shaderType, const std::string& shader )
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
    m_shaders.push_back( shaderId );
    return true;
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  bool link()
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

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  /// if this fails to bind and there were no issues compiling and linking
  /// then the problem is most likely a shader
  inline void bind() const { GLExec( glUseProgram( m_program ) ); }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  inline void unbind() const { GLExec( glUseProgram( 0 ) ); }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  inline void setUniformMatrix( GLint address, const glm::mat4 & value )
  {
    glUniformMatrix4fv(
      address,
      1,
      GL_FALSE,
      glm::value_ptr( value ) );
  }

private:

  GLuint m_program { 0 };
  bool m_isLinked { false };
  std::vector< GLuint > m_shaders;
};

}

#endif //INC_4B6A67D3562C4C0C8E779F3ECBF648BA
