#ifndef INC_4B6A67D3562C4C0C8E779F3ECBF648BA
#define INC_4B6A67D3562C4C0C8E779F3ECBF648BA

#include "Pch.hpp"

#include <glm/gtc/type_ptr.hpp>

#include "gfx/primitives/GLUtility.hpp"

namespace nxgl::gfx
{

class GLShader
{
public:


  GLShader();
  ~GLShader();

  [[nodiscard]] inline GLuint id() const { return m_program; }

  [[nodiscard]] inline GLint getUniformAddress( const std::string& uniform ) const
  {
    return glGetUniformLocation( m_program, uniform.c_str() );
  }

  [[nodiscard]] bool loadShader( GLenum shaderType, const std::string& filename );

  [[nodiscard]] bool addShader( GLenum shaderType, const std::string& shader );

  bool link();

  ////////////////////////////////////////////////////////////////////////////////
  /// if this fails to bind and there were no issues compiling and linking
  /// then the problem is most likely a shader
  inline void bind() const { GLExec( glUseProgram( m_program ) ); }

  inline void setUniformMatrix( GLint address, const glm::mat4 & value ) const
  {
    glUniformMatrix4fv(
      address,
      1,
      GL_FALSE,
      glm::value_ptr( value ) );
  }

  inline void setUniformUInt( GLint address, GLuint value ) const
  {
    glUniform1ui( address, value );
  }

private:

  GLuint m_program { 0 };
  bool m_isLinked { false };

};

}

#endif //INC_4B6A67D3562C4C0C8E779F3ECBF648BA
