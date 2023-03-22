#ifndef E867CC3C3E0B4BF8A4AB42F855EE6974
#define E867CC3C3E0B4BF8A4AB42F855EE6974

#include "ApplicationContext.hpp"
#include "ui/EventReceiver.hpp"
#include "ui/EventDistributor.hpp"

namespace nxgl
{

class Application
{
public:

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  Application( ApplicationContext& appCtx,
               GLFWwindow * pWindow )
    : m_appCtx( appCtx ),
      m_pWindow( pWindow )
  {
    // set the GLFW custom data to this application
    glfwSetWindowUserPointer( m_pWindow, ( void* )this );

    // register the following events with GLFW
    glfwSetKeyCallback( m_pWindow, onGlfwKeyEvent );
    glfwSetMouseButtonCallback( m_pWindow, onGlfwMouseClick );
    glfwSetWindowSizeCallback( m_pWindow, onGlfwResize );

    // register the following event receivers
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  void run()
  {
    auto * eventDistributor = m_appCtx.eventDistributor;

    // initialize all the windows
    eventDistributor->initialize( m_appCtx, m_pWindow );
    Clock clock;

    /* Loop until the user closes the window */
    while ( !glfwWindowShouldClose( m_pWindow ) )
    {
      // Poll for and process events
      glfwPollEvents();

      eventDistributor->update( m_appCtx, m_pWindow, clock.reset() );

      // render any opengl above draw calls in the distributor
      // gets called no matter what
      renderOpenGL();

      eventDistributor->draw( m_appCtx, m_pWindow );

      // Swap front and back buffers, i.e., display
      glfwSwapBuffers( m_pWindow );
    }

    eventDistributor->shutdown( m_appCtx, m_pWindow );
    glfwTerminate();
  }

private:

  ////////////////////////////////////////////////////////////////////////////////
  /// PRIVATE:
  void renderOpenGL()
  {
    int width, height;
    glfwGetFramebufferSize( m_pWindow, &width, &height );
    glViewport( 0, 0, width, height );
    glClearColor( BG_RGB, BG_RGB, BG_RGB, 1.f );
    glClear( GL_COLOR_BUFFER_BIT );
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PRIVATE:
  void updateViewport( GLFWwindow* window, int width, int height )
  {
    m_appCtx.windowSize = { width, height };
    glViewport( 0, 0, width, height );
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// GLFW CALLBACKS
  ////////////////////////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////////////////////////
  /// PRIVATE:
  static void onGlfwKeyEvent(
    GLFWwindow* window,
    int key,
    int scancode,
    int action,
    int mods )
  {
    auto * pApplication = ( Application * )glfwGetWindowUserPointer( window );
    assert( pApplication != nullptr );

    auto * eventDistributor = pApplication->m_appCtx.eventDistributor;
    eventDistributor->processKeyEvent(
      pApplication->m_appCtx,
      window,
      key,
      scancode,
      action,
      mods );
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PRIVATE:
  static void onGlfwMouseClick( GLFWwindow* window, int button, int action, int mods )
  {
    auto * pApplication = ( Application * )glfwGetWindowUserPointer( window );
    assert( pApplication != nullptr );

    auto * eventDistributor = pApplication->m_appCtx.eventDistributor;

    eventDistributor->processMouseClickEvent(
      pApplication->m_appCtx,
      window,
      button,
      action,
      mods );
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PRIVATE:
  ///
  /// update application context when window resizes
  /// \param window
  /// \param width
  /// \param height
  static void onGlfwResize( GLFWwindow* window, int width, int height )
  {
    auto * pApplication = ( Application * )glfwGetWindowUserPointer( window );
    assert( pApplication != nullptr );

    // update the resolution in the application
    pApplication->m_appCtx.windowSize = { width, height };
  }

private:

  ApplicationContext& m_appCtx;
  GLFWwindow * m_pWindow;

  static const inline float BG_RGB = 32.f / 255.f;
};

}

#endif //E867CC3C3E0B4BF8A4AB42F855EE6974
