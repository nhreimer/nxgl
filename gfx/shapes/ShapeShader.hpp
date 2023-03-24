#ifndef INC_02A5D73CA146472D848737B73202DF3B
#define INC_02A5D73CA146472D848737B73202DF3B

#include "gfx/shapes/IMVPApplicator.hpp"

namespace nxgl::gfx
{
/***
 * this is the default shader for all Polygon-related shapes
 */
class ShapeShader : public IMVPApplicator
{
public:

  void applyMVP( const glm::mat4 &mvp ) override
  {
    if ( !m_isCreated ) { initialize(); }

    bind();
    m_shader.setUniformMatrix( m_mvpAddress, mvp );
  }

private:

  // lazy initialization
  void initialize()
  {
    LOG_DEBUG( "initializing default shaders" );

    assert( !m_isCreated );
    assert( m_shader.loadShader( GL_VERTEX_SHADER, R"(resources\DefaultVertex.glsl)" ) &&
            m_shader.loadShader( GL_FRAGMENT_SHADER, R"(resources\DefaultFragment.glsl)" ));
    assert( m_shader.link() );
    m_isCreated = true;
  }

private:

  void bind()
  {
    if ( !m_isCreated )
    { initialize(); }

    m_mvpAddress = m_shader.getUniformAddress( "uMVP" );
    m_shader.bind();
  }

  void unbind()
  {
    m_shader.unbind();
  }

private:

  bool m_isCreated{ false };
  GLint m_mvpAddress{ 0 };
  GLShader m_shader;
};
}

#endif //INC_02A5D73CA146472D848737B73202DF3B
