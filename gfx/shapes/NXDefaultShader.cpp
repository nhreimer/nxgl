#include <glm/ext/matrix_float4x4.hpp>
#include "gfx/shapes/NXDefaultShader.hpp"

#include "Logger.hpp"

void nxgl::gfx::NXDefaultShader::applyMVP( const glm::mat4 &mvp )
{
  if ( !m_isCreated ) { initialize(); }

  bind();
  m_shader.setUniformMatrix( m_mvpAddress, mvp );
}

void nxgl::gfx::NXDefaultShader::initialize()
{
  LOG_DEBUG( "initializing default shaders" );

  assert( !m_isCreated );
  assert( m_shader.loadShader( GL_VERTEX_SHADER, R"(resources\DefaultVertex.glsl)" ) &&
          m_shader.loadShader( GL_FRAGMENT_SHADER, R"(resources\DefaultFragment.glsl)" ));
  assert( m_shader.link() );
  m_isCreated = true;
}

void nxgl::gfx::NXDefaultShader::bind()
{
  if ( !m_isCreated )
  { initialize(); }

  m_mvpAddress = m_shader.getUniformAddress( "uMVP" );
  m_shader.bind();
}
