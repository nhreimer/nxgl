#ifndef D49F5B9FB4E9483C8DE2C0371A3E5B3D
#define D49F5B9FB4E9483C8DE2C0371A3E5B3D

#include "gfx/primitives/GLFbo.hpp"

namespace nxgl::ui
{

/***
 * call this BEFORE other imgui windows to enable window docking
 */
class ImguiMainWindowReceiver : public EventReceiver
{
public:

  void initialize( ApplicationContext &appCtx, GLFWwindow *window ) override
  {
    m_pTriangle = std::make_unique< nxgl::gfx::Triangle >( GL_DYNAMIC_DRAW );

    m_pFBO = std::make_unique< nxgl::gfx::GLFbo >( GL_DRAW_FRAMEBUFFER, appCtx.windowSize.x, appCtx.windowSize.y );
    m_pTriangle->getModel().setScale( { 100.f, 100.f } );
    m_pTriangle->getModel().setPosition( { appCtx.windowSize.x / 2.f, appCtx.windowSize.y / 2.f } );
  }

  void update( ApplicationContext &appCtx, GLFWwindow *window, const nxTimePoint frameDeltaInMS ) override
  {
    m_timer += frameDeltaInMS;
    if ( m_timer >= 250.f )
    {
      m_pTriangle->getModel().setAngle( m_pTriangle->getModel().getAngle() + 1.f );
      m_timer = 0.f;
    }
  }

  void draw( ApplicationContext &appCtx, GLFWwindow *window ) override
  {
    m_pFBO->bind();

    // draw as normal
    m_pTriangle->draw( appCtx.camera );

    ImGui::Begin( "Main Renderer", nullptr, ImGuiWindowFlags_AlwaysAutoResize );
    {
      // give ImGui the texture ID
      ImGui::Image( ( void* )m_pFBO->getTextureID(),
                    { appCtx.windowSize.x, appCtx.windowSize.y / 2.f } );

      ImGui::End();
    }

    // unbind fbo
    m_pFBO->unbind();
  }

private:

  float m_timer { 0.f };

  std::unique_ptr< nxgl::gfx::GLFbo > m_pFBO;
  std::unique_ptr< nxgl::gfx::Triangle > m_pTriangle;
};

}

#endif //D49F5B9FB4E9483C8DE2C0371A3E5B3D
