#ifndef D49F5B9FB4E9483C8DE2C0371A3E5B3D
#define D49F5B9FB4E9483C8DE2C0371A3E5B3D

#include "gfx/primitives/GLFbo.hpp"
#include "gfx/shapes/IMVPApplicator.hpp"
#include "gfx/shapes/Polygon.hpp"
#include "gfx/shapes/Shapes.hpp"
#include "gfx/shapes/IColorable.hpp"

namespace nxgl::ui
{

class ImguiMainWindowReceiver : public EventReceiver
{

public:

  void initialize( ApplicationContext &appCtx, GLFWwindow *window ) override
  {
    nxgl::gfx::SpectrumColorizer innerColorizer;
    innerColorizer.setColors( 12, { 1.f, 0.f, 0.f, 1.f }, { 1.f, 0.f, 1.f, 1.f } );

    nxgl::gfx::SpectrumColorizer outerColorizer;
    outerColorizer.setColors( 12, { 1.f, 0.f, 0.f, 1.f }, { 1.f, 1.f, 0.f, 1.f } );

    m_polygon.getModel().setScale( { 100.f, 100.f } );
    m_polygon.getModel().setPosition( { appCtx.windowSize.x / 2.f, appCtx.windowSize.y / 2.f } );
    m_polygon.setOutlineWidth( .2f );
    m_polygon.setFillColor( innerColorizer );
    m_polygon.setOutlineColor( outerColorizer );
  }

  void update( ApplicationContext &appCtx, GLFWwindow *window, const nxTimePoint frameDeltaInMS ) override
  {
    m_timer += frameDeltaInMS;
    if ( m_timer >= 100.f )
    {
      m_polygon.getModel().setAngle( m_polygon.getModel().getAngle() + 1.f );
      m_timer = 0.f;
    }
  }

  void draw( ApplicationContext &appCtx, GLFWwindow *window ) override
  {
    m_polygon.draw( appCtx, window );
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
      if ( m_polygon.getModel().getBounds().contains( { xpos, ypos } ) )
      {
        LOG_DEBUG( "MATCH" );
      }
    }
  }

private:

  float m_timer { 0.f };
  nxgl::gfx::Rectangle m_polygon { GL_DYNAMIC_DRAW };
//  nxgl::gfx::Triangle m_polygon { GL_DYNAMIC_DRAW };
//  nxgl::gfx::Polygon m_polygon { GL_DYNAMIC_DRAW, 12 };
//  nxgl::gfx::Rectangle m_rect { GL_DYNAMIC_DRAW };

};

}

#endif //D49F5B9FB4E9483C8DE2C0371A3E5B3D
