#ifndef D49F5B9FB4E9483C8DE2C0371A3E5B3D
#define D49F5B9FB4E9483C8DE2C0371A3E5B3D

#include "gfx/primitives/GLFbo.hpp"

#include "gfx/shapes/IMVPApplicator.hpp"
#include "gfx/shapes/IColorable.hpp"

#include "gfx/shapes/GLPolygon.hpp"
#include "gfx/shapes/GLTriangleCircle.hpp"

namespace nxgl::ui
{

class ImguiMainWindowReceiver : public EventReceiver
{

public:

  void initialize( ApplicationContext &appCtx, GLFWwindow *window ) override
  {
    m_pModel = &m_polyBCA.getModel();

    nxgl::gfx::SpectrumColorizer outerColorizer;
    outerColorizer.setColors( 5, { 0.f, 1.f, 0.f, 1.f }, { 0.f, 1.f, 1.f, 1.f } );

    nxgl::gfx::SpectrumColorizer innerColorizer;
    innerColorizer.setColors( 5, { 1.f, 0.f, 0.f, 1.f }, { 1.f, 0.f, 1.f, 1.f } );

    auto size = 100.f;

    ////////////////////////////////////////////////////////////////////////////////

    m_polyBCA.getModel().setScale( { size, size } );
    m_polyBCA.getModel().setPosition( { appCtx.windowSize.x / 2.f,
                                        appCtx.windowSize.y / 2.f } );
    m_polyBCA.setFillColor( innerColorizer );
    m_polyBCA.setOutlineColor( outerColorizer );

    ////////////////////////////////////////////////////////////////////////////////

    m_triCirc.getModel().setPosition( { 200.f, 200.f } );
    m_triCirc.getModel().setScale( { 100.f, 100.f } );
//    m_triCirc.setColor( outerColorizer );

    ////////////////////////////////////////////////////////////////////////////////
  }

  void update( ApplicationContext &appCtx, GLFWwindow *window, const nxTimePoint frameDeltaInMS ) override
  {
    m_timer += frameDeltaInMS;
    if ( m_timer >= 100.f )
    {
      m_polyBCA.getModel().setAngle( m_polyBCA.getModel().getAngle() - 1.f );
      m_timer = 0.f;
    }
  }

  void draw( ApplicationContext &appCtx, GLFWwindow *window ) override
  {
    m_polyBCA.draw( appCtx.camera, *appCtx.mvpApplicator );
    m_triCirc.draw( appCtx.camera, *appCtx.mvpApplicator );
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

  nxgl::gfx::GLPolygon m_polyBCA { GL_DYNAMIC_DRAW, 6 };
  nxgl::gfx::GLTriangleCircle m_triCirc { 7, .5f };
  nxgl::gfx::GLModel * m_pModel { nullptr };
};

}

#endif //D49F5B9FB4E9483C8DE2C0371A3E5B3D
