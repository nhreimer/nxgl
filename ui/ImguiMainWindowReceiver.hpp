#ifndef D49F5B9FB4E9483C8DE2C0371A3E5B3D
#define D49F5B9FB4E9483C8DE2C0371A3E5B3D

#include "gfx/primitives/GLFbo.hpp"

#include "gfx/shapes/IMVPApplicator.hpp"
#include "gfx/shapes/IColorable.hpp"

namespace nxgl::ui
{

class ImguiMainWindowReceiver : public EventReceiver
{

public:

  void initialize( ApplicationContext &appCtx, GLFWwindow *window ) override
  {
    auto * pBlend = &m_polyFill.getBlender();

    nxgl::gfx::SpectrumColorizer outerColorizer;
    outerColorizer.setColors( 5, { 0.f, 1.f, 0.f, 1.f }, { 0.f, 0.f, 1.f, 1.f } );

    nxgl::gfx::SpectrumColorizer innerColorizer;
    innerColorizer.setColors( 5, { 1.f, 0.f, 0.f, 1.f }, { 0.f, 1.f, 0.f, 1.f } );

    auto size = 100.f;

    m_poly.getModel().setScale( { size, size } );
    m_poly.getModel().setPosition( { appCtx.windowSize.x / 2.f,
                                     appCtx.windowSize.y / 2.f } );
    m_poly.setColor( outerColorizer );

    m_polyFill.getModel().setScale( { size * .8f, size * .8f } );
    m_polyFill.getModel().setPosition( { appCtx.windowSize.x / 2.f,
                                         appCtx.windowSize.y / 2.f } );

    m_polyFill.setColor( innerColorizer );

    ////////////////////////////////////////////////////////////////////////////////

    pBlend->isEnabled = true;
    pBlend->srcColor = GL_ONE;
    pBlend->destColor = GL_ONE_MINUS_SRC_COLOR;
  }

  void update( ApplicationContext &appCtx, GLFWwindow *window, const nxTimePoint frameDeltaInMS ) override
  {
    m_timer += frameDeltaInMS;
    if ( m_timer >= 100.f )
    {
      m_poly.getModel().setAngle( m_poly.getModel().getAngle() - 1.f );
      m_polyFill.getModel().setAngle( m_polyFill.getModel().getAngle() - 1.f );
      m_timer = 0.f;
    }
  }

  void draw( ApplicationContext &appCtx, GLFWwindow *window ) override
  {
    m_poly.draw( appCtx.camera, *appCtx.mvpApplicator );
    m_polyFill.draw( appCtx.camera, *appCtx.mvpApplicator );
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
//      if ( m_pModel->getBounds().contains( { xpos, ypos } ) )
//      {
//        LOG_DEBUG( "MATCH" );
//      }
    }
  }

private:

  float m_timer { 0.f };
  nxgl::gfx::GLPolygon m_poly { GL_DYNAMIC_DRAW, 4 };
  nxgl::gfx::GLPolygon m_polyFill { GL_DYNAMIC_DRAW, 4 };

};

}

#endif //D49F5B9FB4E9483C8DE2C0371A3E5B3D
