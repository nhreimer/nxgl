#ifndef INC_7A719F49E4BB4D22AA8EC7EF36DF1838
#define INC_7A719F49E4BB4D22AA8EC7EF36DF1838

#include <glm/gtc/type_ptr.hpp>

namespace nxgl::gfx
{

class GLObject
{

public:

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  GLObject()
  {
    if ( m_shader.loadShader( GL_VERTEX_SHADER, R"(resources\DefaultVertex.glsl)" ) &&
         m_shader.loadShader( GL_FRAGMENT_SHADER, R"(resources\DefaultFragment.glsl)" ) )
    {
      LOG_DEBUG( "successfully loaded shaders" );
    }

    if ( m_shader.link() )
    {
      LOG_DEBUG( "successfully linked shaders" );
    }

    m_mvpUniformAddress = m_shader.getUniformAddress( "uMVP" );
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  virtual ~GLObject() = default;

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  [[nodiscard]] inline GLModel& getModel() { return m_transform; }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  void setModel( const GLModel& transform ) { m_transform = transform; }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  // blend accessors
  [[nodiscard]] inline GLBlend& getBlend() { return m_blend; }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  void setBlend( const GLBlend& blend ) { m_blend = blend; }

protected:

  ////////////////////////////////////////////////////////////////////////////////
  /// PROTECTED:
  void bind()
  {
    m_vao.bind();
    m_shader.bind();
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PROTECTED:
  void unbind()
  {
    m_vao.unbind();
    m_shader.unbind();
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PROTECTED:
  inline void setVBO( const GLVbo< GLData >& vbo )
  {
    m_vao.registerVBO( vbo );
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PROTECTED;
  void setMVP( const glm::mat4& mvp )
  {
    m_shader.setUniformMatrix( m_mvpUniformAddress, mvp );
  }

private:

  GLVao m_vao;
  GLBlend m_blend;
  GLShader m_shader;
  GLModel m_transform;
  GLint m_mvpUniformAddress { 0 };
};
}

#endif //INC_7A719F49E4BB4D22AA8EC7EF36DF1838
