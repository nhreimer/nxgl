#ifndef D49F5B9FB4E9483C8DE2C0371A3E5B3D
#define D49F5B9FB4E9483C8DE2C0371A3E5B3D

#include "gfx/primitives/GLFbo.hpp"

#include "gfx/shapes/IMVPApplicator.hpp"
#include "gfx/shapes/IColorable.hpp"

#include "gfx/shapes/GLPolygon.hpp"
#include "utilities/Math.hpp"

namespace nxgl::ui
{

class ImguiMainWindowReceiver : public EventReceiver
{

public:

  void initialize( ApplicationContext &appCtx, GLFWwindow *window ) override
  {
    m_pModel = &m_hexa.getModel();

    nxgl::gfx::SpectrumColorizer outerColorizer;
    outerColorizer.setColors( 4, { 1.f, 1.f, 1.f, .5f }, { 0.f, 0.f, 0.f, .5f } );

    nxgl::gfx::IntervalColorizer intervalColorizer;
    intervalColorizer.setColors(
      {
        // triangle 1
        { 0.f, 0.f, 0.f, 0.f },
        { 0.f, 1.f, 0.f, .5f },
        { 0.f, 1.f, 0.f, .5f },

        // triangle 2
        { 0.f, 0.f, 0.f, 0.f },
        { 0.f, 0.f, 1.f, .5f },
        { 0.f, 0.f, 1.f, .5f }
      } );

    auto size = 100.f;

    ////////////////////////////////////////////////////////////////////////////////

    m_hexa.getModel().setScale( { size, size } );
    m_hexa.getModel().setPosition( { appCtx.windowSize.x / 2.f,
                                        appCtx.windowSize.y / 2.f } );
    m_hexa.setFillColor( intervalColorizer );
    m_hexa.setOutlineColor( outerColorizer );
    m_hexa.setOutlinePercentage( .1f );

    m_triangle = m_hexa.getTriangle( 0 );

    ////////////////////////////////////////////////////////////////////////////////
  }

  void update( ApplicationContext &appCtx, GLFWwindow *window, const nxTimePoint frameDeltaInMS ) override
  {
    m_timer += frameDeltaInMS;
    if ( m_timer >= 100.f )
    {
//      m_hexa.getModel().setAngle( m_hexa.getModel().getAngle() - 1.f );
      m_timer = 0.f;
    }
  }

  void draw( ApplicationContext &appCtx, GLFWwindow *window ) override
  {
    m_hexa.draw( appCtx.camera, *appCtx.mvpApplicator );
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

      nxVec2 pos { ( float )xpos, ( float )ypos };

      if ( m_pModel->getBounds().contains( pos ) )
      {
        LOG_DEBUG( "BB clicked!" );

        if ( nxgl::Math::isPointInTriangle( pos, m_triangle ) )
        {
          LOG_DEBUG( "TRIANGLE clicked!" );
        }

        LOG_DEBUG( "\tpoint: {}, {}. ", pos.x, pos.y );
        LOG_DEBUG( "\tA: {}, {}. ", m_triangle.pointA.x, m_triangle.pointA.y );
        LOG_DEBUG( "\tB: {}, {}. ", m_triangle.pointB.x, m_triangle.pointB.y );
        LOG_DEBUG( "\tC: {}, {}. ", m_triangle.pointC.x, m_triangle.pointC.y );
      }
    }
  }

private:

  float m_timer { 0.f };
  nxgl::gfx::GLPolygon m_hexa { GL_DYNAMIC_DRAW, 4 };
  nxgl::gfx::GLModel * m_pModel { nullptr };
  nxgl::gfx::TriangleData m_triangle;
};

}

#endif //D49F5B9FB4E9483C8DE2C0371A3E5B3D
