#ifndef D49F5B9FB4E9483C8DE2C0371A3E5B3D
#define D49F5B9FB4E9483C8DE2C0371A3E5B3D

#include "gfx/primitives/GLFbo.hpp"

namespace nxgl::ui
{

class ImguiMainWindowReceiver : public EventReceiver
{
public:

  void initialize( ApplicationContext &appCtx, GLFWwindow *window ) override
  {
    m_pTriangle = std::make_unique< nxgl::gfx::Triangle >( GL_DYNAMIC_DRAW );

//    m_pFBO = std::make_unique< nxgl::gfx::GLFbo >( GL_DRAW_FRAMEBUFFER, appCtx.windowSize.x, appCtx.windowSize.y );

    m_innerColorizer.setColors( 3, { 1.f, 0.f, 0.f, 1.f }, { 1.f, 0.f, 1.f, 1.f } );
    m_outerColorizer.setColors( 3, { 1.f, 0.f, 0.f, 1.f }, { 1.f, 1.f, 0.f, 1.f } );
    m_pTriangle->setFillColor( m_innerColorizer );
    m_pTriangle->setOutlineColor( m_outerColorizer );
    m_pTriangle->setOutlineWidth( .2f );
    m_pTriangle->getModel().setScale( { 100.f, 100.f } );
    m_pTriangle->getModel().setPosition( { appCtx.windowSize.x / 2.f, appCtx.windowSize.y / 2.f } );

    auto bounds = m_pTriangle->getModel().getBounds();
    LOG_DEBUG( "bounds: ({}, {}) - ({}, {})", bounds.topLeft.x, bounds.topLeft.y,
               bounds.bottomRight.x,
               bounds.bottomRight.y );
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
    m_pTriangle->draw( appCtx.camera );
//    m_pFBO->bind();
//
//    // draw as normal
//    m_pTriangle->draw( appCtx.camera );
//
//    ImGui::Begin( "Main Renderer", nullptr, ImGuiWindowFlags_AlwaysAutoResize );
//    {
//      // give ImGui the texture ID
//      ImGui::Image( ( ImTextureID )m_pFBO->getTextureID(),
//                    { appCtx.windowSize.x, appCtx.windowSize.y / 2.f } );
//
//      ImGui::End();
//    }
  }

  void
  processMouseClickEvent( ApplicationContext &appCtx,
                          GLFWwindow *window,
                          int button,
                          int action,
                          int mods ) override
  {
    if ( button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE )
    {
      double xpos, ypos;
      glfwGetCursorPos( window, &xpos, &ypos );
      // LOG_DEBUG( "CLICKED: {}, {}", xpos, ypos );
      if ( m_pTriangle->getModel().getBounds().contains( { xpos, ypos } ) )
      {
        LOG_DEBUG( "MATCH" );
      }
    }
  }

private:

  float m_timer { 0.f };

//  std::unique_ptr< nxgl::gfx::GLFbo > m_pFBO;
  std::unique_ptr< nxgl::gfx::Triangle > m_pTriangle;
  nxgl::gfx::SpectrumColorizer m_innerColorizer;
  nxgl::gfx::SpectrumColorizer m_outerColorizer;
};

}

#endif //D49F5B9FB4E9483C8DE2C0371A3E5B3D
