#ifndef D49F5B9FB4E9483C8DE2C0371A3E5B3D
#define D49F5B9FB4E9483C8DE2C0371A3E5B3D

#include "gfx/primitives/GLVbo.hpp"
#include "gfx/shapes/NXShape.hpp"
#include "gfx/shapes/NXSharedShapeContainer.hpp"

#include "gfx/shapes/IMVPApplicator.hpp"
#include "gfx/shapes/IColorable.hpp"

#include "gfx/shapes/generators/BarycentricGenerator.hpp"
#include "gfx/shapes/generators/PolygonGenerator.hpp"
#include "utilities/Math.hpp"

#include "ecs/Components.hpp"

namespace nxgl::ui
{

class ImguiMainWindowReceiver : public EventReceiver
{

public:

  void initialize( ApplicationContext &appCtx, GLFWwindow *window ) override
  {
    nxgl::gfx::SpectrumColorizer outerColorizer;
    outerColorizer.setColors( 4, { 1.f, 1.f, 1.f, .5f }, { 0.f, 0.f, 0.f, .5f } );

    nxgl::gfx::IntervalColorizer intervalOddColorizer;
    intervalOddColorizer.setColors(
      {
        // triangle 0
        { 0.f, 0.f, 0.f, 0.f },
        { 0.f, 1.f, 0.f, .5f },
        { 0.f, 1.f, 0.f, .5f },

        // triangle 1
        { 0.f, 0.f, 0.f, 0.f },
        { 0.f, 0.f, 1.f, .5f },
        { 0.f, 0.f, 1.f, .5f },

        // triangle 2
        { 0.f, 0.f, 0.f, 0.f },
        { 1.f, 1.f, 1.f, .5f },
        { 1.f, 1.f, 1.f, .5f }
      } );

    nxgl::gfx::IntervalColorizer intervalEvenColorizer;
    intervalEvenColorizer.setColors(
      {
        // triangle 0
        { 0.f, 0.f, 0.f, 0.f },
        { 0.f, 1.f, 0.f, .5f },
        { 0.f, 1.f, 0.f, .5f },

        // triangle 1
        { 0.f, 0.f, 0.f, 0.f },
        { 0.f, 0.f, 1.f, .5f },
        { 0.f, 0.f, 1.f, .5f },
      } );

    ////////////////////////////////////////////////////////////////////////////////

    m_bary.setColor( intervalEvenColorizer );
    m_bary.getModel().setScale( { 100.f, 100.f } );
    m_bary.getModel().setPosition( { appCtx.windowSize.x / 2.f, -appCtx.windowSize.y / 2.f } );

    m_baryBack.getModel().setScale( { 100.f, 100.f } );
    m_baryBack.getModel().setPosition( { appCtx.windowSize.x / 2.f, -appCtx.windowSize.y / 2.f } );

    auto& shape = m_shapes.addShape( GL_TRIANGLES, 0, 3 );
    shape.getModel().setScale( { 50.f, 50.f } );
    shape.getModel().setPosition( { 50.f, -50.f } );

    auto& shape2 = m_shapes.addShape( GL_TRIANGLES, 3, 3 );
    shape2.getModel().setScale( { 50.f, 50.f } );
    shape2.getModel().setPosition( { 200.f, -50.f } );

    auto& poly1 = m_polys.emplace_back( gfx::NXShape { sm_polygenerator } );
    poly1.getModel().setScale( { 75.f, 75.f } );
    poly1.getModel().setPosition( { 200.f, -150.f } );

    sm_polygenerator.setEdges( 5 );
    auto& poly2 = m_polys.emplace_back( gfx::NXShape { sm_polygenerator } );
    poly2.getModel().setScale( { 75.f, 75.f } );
    poly2.getModel().setPosition( { 325.f, -150.f } );
  }

  void update( ApplicationContext &appCtx, GLFWwindow *window, const nxTimePoint frameDeltaInMS ) override
  {
    m_timer += frameDeltaInMS;
    if ( m_timer >= 50.f )
    {
      auto& shape = m_shapes.getShape( 1 );
      shape.getModel().setAngle( shape.getModel().getAngle() + 1.f );
      m_timer = 0.f;
    }
  }

  void draw( ApplicationContext &appCtx, GLFWwindow *window ) override
  {
    m_baryBack.draw( appCtx.camera, *appCtx.mvpApplicator );
    m_bary.draw( appCtx.camera, *appCtx.mvpApplicator );
    m_shapes.draw( appCtx.camera, *appCtx.mvpApplicator );

    for ( auto& poly : m_polys )
      poly.draw( appCtx.camera, *appCtx.mvpApplicator );
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
      //nxVec2 pos { ( float )xpos, appCtx.windowSize.y - ( float )ypos };

      nxVec2 pos { ( float )xpos, ( float )-ypos };
      LOG_DEBUG( "{}, {}", pos.x, pos.y );
      if ( m_bary.getModel().getBounds().contains( pos ) )
      {
        LOG_DEBUG( "you clicked it!" );
      }
    }
  }

private:

  float m_timer { 0.f };

  gfx::NXShape m_bary { sm_generator };
  gfx::NXShape m_baryBack { GL_TRIANGLES, sm_vertices };
  gfx::NXSharedShapeContainer m_shapes { sm_vertices };
  std::vector< gfx::NXShape > m_polys;

  static inline gfx::PolygonGenerator sm_polygenerator { 4 };

  static inline gfx::BarycentricGenerator sm_generator { 2, { { -.5f, -.5f },
                                                              {  .5f, -.5f },
                                                              {  0.f,  .5f } } };

  static inline std::vector< nxgl::gfx::GLData > sm_vertices
    {
      { { -.5f, -.5f  },  { 1.f, 1.f, 1.f, 1.f } },
      { {  .5f, -.5f  },  { 1.f, 1.f, 1.f, 1.f } },
      { {  .0f,  .5f  },  { 1.f, 1.f, 1.f, 1.f } },

      { { -.5f, -.5f  },  { 1.f, 0.f, 0.f, 1.f } },
      { {  .5f, -.5f  },  { 0.f, 0.f, 1.f, 1.f } },
      { {  .0f,  .5f  },  { 0.f, 0.f, 1.f, 1.f } }
    };
};

}

#endif //D49F5B9FB4E9483C8DE2C0371A3E5B3D
