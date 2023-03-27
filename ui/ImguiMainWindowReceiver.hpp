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
    nxgl::gfx::SpectrumColorizer outerColorizer;
    outerColorizer.setColors( 5, { 0.f, 1.f, 0.f, 1.f }, { 0.f, 1.f, 1.f, 1.f } );

    nxgl::gfx::SpectrumColorizer innerColorizer;
    innerColorizer.setColors( 5, { 1.f, 0.f, 0.f, 1.f }, { 1.f, 0.f, 1.f, 1.f } );

    auto size = 100.f;

    m_polyABC.getModel().setScale( { size, size } );
    m_polyABC.getModel().setPosition( { appCtx.windowSize.x / 2.f,
                                     appCtx.windowSize.y / 2.f } );
    m_polyABC.setFillColor( innerColorizer );
    m_polyABC.setOutlineColor( outerColorizer );

    ////////////////////////////////////////////////////////////////////////////////

    m_polyBCA.getModel().setScale( { size, size } );
    m_polyBCA.getModel().setPosition( { appCtx.windowSize.x / 2.f,
                                        appCtx.windowSize.y / 2.f - size * 2 } );
    m_polyBCA.setFillColor( innerColorizer );
    m_polyBCA.setOutlineColor( outerColorizer );

    ////////////////////////////////////////////////////////////////////////////////

    m_polyCBA.getModel().setScale( { size, size } );
    m_polyCBA.getModel().setPosition( { appCtx.windowSize.x / 2.f - size * 2,
                                        appCtx.windowSize.y / 2.f - size * 2 } );
    m_polyCBA.setFillColor( innerColorizer );
    m_polyCBA.setOutlineColor( outerColorizer );

    ////////////////////////////////////////////////////////////////////////////////
  }

  void update( ApplicationContext &appCtx, GLFWwindow *window, const nxTimePoint frameDeltaInMS ) override
  {
    m_timer += frameDeltaInMS;
    if ( m_timer >= 100.f )
    {
      m_polyABC.getModel().setAngle( m_polyABC.getModel().getAngle() - 1.f );
      m_polyBCA.getModel().setAngle( m_polyBCA.getModel().getAngle() - 1.f );
      m_polyCBA.getModel().setAngle( m_polyCBA.getModel().getAngle() - 1.f );

      m_timer = 0.f;
    }
  }

  void draw( ApplicationContext &appCtx, GLFWwindow *window ) override
  {
    m_polyABC.draw( appCtx.camera, *appCtx.mvpApplicator );
    m_polyBCA.draw( appCtx.camera, *appCtx.mvpApplicator );
    m_polyCBA.draw( appCtx.camera, *appCtx.mvpApplicator );
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
      if ( m_pModel->getBounds().contains( { xpos, ypos } ) )
      {
        LOG_DEBUG( "MATCH" );
      }
    }
  }

private:

  float m_timer { 0.f };

  nxgl::gfx::CCWABCPolygon< nxgl::gfx::GLData > m_ABCGen;
  nxgl::gfx::CCWBCAPolygon< nxgl::gfx::GLData > m_BCAGen;
  nxgl::gfx::CCWCBAPolygon< nxgl::gfx::GLData > m_CBAGen;

  nxgl::gfx::GLPolygon m_polyABC { GL_DYNAMIC_DRAW, 4, m_ABCGen };
  nxgl::gfx::GLPolygon m_polyBCA { GL_DYNAMIC_DRAW, 4, m_BCAGen };
  nxgl::gfx::GLPolygon m_polyCBA { GL_DYNAMIC_DRAW, 4, m_CBAGen };

  nxgl::gfx::GLModel * m_pModel { nullptr };
};

}

#endif //D49F5B9FB4E9483C8DE2C0371A3E5B3D
