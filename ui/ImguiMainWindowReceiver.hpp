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
    m_innerColorizer.setColors( 12, { 1.f, 0.f, 0.f, 1.f }, { 1.f, 0.f, 1.f, 1.f } );
    m_outerColorizer.setColors( 12, { 1.f, 0.f, 0.f, 1.f }, { 1.f, 1.f, 0.f, 1.f } );

    m_pRectangle = std::make_unique< nxgl::gfx::Rectangle >( GL_DYNAMIC_DRAW );
    m_pRectangle->getModel().setScale( { 100.f, 100.f } );
    m_pRectangle->getModel().setPosition( { appCtx.windowSize.x / 2.f, appCtx.windowSize.y / 2.f } );
    m_pRectangle->setOutlineColor( m_outerColorizer );
    m_pRectangle->setFillColor( m_innerColorizer );
    m_pRectangle->setOutlineWidth( .2f );

    m_innerColorizer.setIntervals( 3 );
    m_outerColorizer.setIntervals( 3 );

    m_pTriangle = std::make_unique< nxgl::gfx::Triangle >( GL_DYNAMIC_DRAW );
    m_pTriangle->setFillColor( m_innerColorizer );
    m_pTriangle->setOutlineColor( m_outerColorizer );
    m_pTriangle->setOutlineWidth( .2f );
    m_pTriangle->getModel().setScale( { 100.f, 100.f } );
    m_pTriangle->getModel().setPosition( { appCtx.windowSize.x / 4.f, appCtx.windowSize.y / 4.f } );

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
    m_pRectangle->draw( appCtx.camera );
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
      if ( m_pTriangle->getModel().getBounds().contains( { xpos, ypos } ) )
      {
        LOG_DEBUG( "MATCH" );
      }
    }
  }

private:

  float m_timer { 0.f };

  std::unique_ptr< nxgl::gfx::Triangle > m_pTriangle;
  std::unique_ptr< nxgl::gfx::Rectangle > m_pRectangle;
  nxgl::gfx::SpectrumColorizer m_innerColorizer;
  nxgl::gfx::SpectrumColorizer m_outerColorizer;
};

}

#endif //D49F5B9FB4E9483C8DE2C0371A3E5B3D
