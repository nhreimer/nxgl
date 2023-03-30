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
        // triangle 0
        { 0.f, 0.f, 0.f, 0.f },
        { 0.f, 1.f, 0.f, .5f },
        { 0.f, 1.f, 0.f, .5f },

        // triangle 1
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

      /////////////////////////////////////////////////////////
      /// ANNOYANCE                                         ///
      ///   IMPORTANT: GLFW's y is inverted since           ///
      ///              OGL's and GLFW's origins differ      ///
      /// ANNOYANCE                                         ///
      /////////////////////////////////////////////////////////
      nxVec2 pos { ( float )xpos, appCtx.windowSize.y - ( float )ypos };

      if ( m_pModel->getBounds().contains( pos ) )
      {
        LOG_DEBUG( "BB clicked!: {}, {}", pos.x, pos.y );

        for ( uint8_t i = 0; i < m_hexa.getTriangleCount(); ++i )
        {
          auto triangle = m_hexa.getTriangle( i );
          if ( Math::isPointInTriangle( pos, triangle ) )
          {
            LOG_DEBUG( "TRIANGLE {} clicked!", i );
            m_hexa.setTriangleColor( i, { 0.f, 0.f, 0.f, 0.f },
                                        { .65f, .65f, .65f, .5f },
                                        { .75f, .75f, .75f, .5f } );

//            LOG_DEBUG( "\tA: {}, {}", triangle.pointA.x, triangle.pointA.y );
//            LOG_DEBUG( "\tB: {}, {}", triangle.pointB.x, triangle.pointB.y );
//            LOG_DEBUG( "\tC: {}, {}", triangle.pointC.x, triangle.pointC.y );
          }
        }
      }
    }
  }

private:

  float m_timer { 0.f };
  nxgl::gfx::GLPolygon m_hexa { GL_DYNAMIC_DRAW, 5 };
  nxgl::gfx::GLModel * m_pModel { nullptr };
};

}

#endif //D49F5B9FB4E9483C8DE2C0371A3E5B3D
