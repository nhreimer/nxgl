#include "Pch.hpp"

#include "Standards.hpp"
#include "gfx/Primitives.hpp"
#include "gfx/shapes/Triangle.hpp"
#include "gfx/shapes/Rectangle.hpp"

#include "Application.hpp"

#include "ui/ImguiDockProviderReceiver.hpp"
#include "ui/ImguiPerformanceReceiver.hpp"
#include "ui/ImguiMainWindowReceiver.hpp"
#include "ui/ImguiPropertiesReceiver.hpp"

static GLFWwindow * createWindow(
  uint32_t width,
  uint32_t height,
  const std::string& windowTitle )
{
  glfwSetErrorCallback(
    []( int error, const char* description )
    {
      LOG_ERROR( "glfw => {}: {}", error, description );
    } );

  if ( !glfwInit() )
  {
    LOG_CRITICAL( "failed to initialize glfw" );
    return nullptr;
  }
  const char* glsl_version = "#version 330";
  glfwWindowHint( GLFW_DOUBLEBUFFER, GL_TRUE );
  glfwWindowHint( GLFW_SAMPLES, 4 );
  glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
  glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
  glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

  GLFWwindow* ptrWindow = glfwCreateWindow(
    ( int )width, ( int )height, windowTitle.c_str(), nullptr, nullptr );

  if ( !ptrWindow )
  {
    LOG_CRITICAL( "failed to create window" );
    return nullptr;
  }

  // set context to the window
  glfwMakeContextCurrent( ptrWindow );

  // have glad load the opengl shit for us. if you forget this, then expect
  // access violation errors
  if ( gladLoadGL( glfwGetProcAddress ) == 0 )
  {
    LOG_CRITICAL( "failed to initialize opengl context" );
    glfwTerminate();
    return nullptr;
  }

  LOG_INFO( "opengl information: " );
  LOG_INFO( "\tversion: {}", glGetString( GL_VENDOR ) );
  LOG_INFO( "\tvendor: {}", glGetString( GL_RENDERER ) );
  LOG_INFO( "\trenderer: {}", glGetString( GL_VERSION ) );

  glfwSwapInterval( 1 ); // Enable vsync

  return ptrWindow;
}

static void runLoop( GLFWwindow * pWindow, const nxgl::gfx::GLCamera& camera )
{
  nxgl::gfx::SolidColorizer solidColorizer;
  solidColorizer.setColor( { 0.f, 1.f, 0.f, 1.f } );

  nxgl::gfx::Triangle triangle( GL_DYNAMIC_DRAW );
  triangle.setOutlineWidth( .2f );
  triangle.getModel().setPosition( { 640.f, 384.f } );
  triangle.getModel().setScale( { 100.f, 100.f } );
  triangle.setFillColor( solidColorizer );

  nxgl::gfx::SpectrumColorizer spectrumColorizer;
  spectrumColorizer.setColors( 3, { 1.f, 0.f, 0.f, 1.f }, { 0.f, 0.f, 1.f, 1.f } );

  nxgl::gfx::Triangle triangle2( GL_DYNAMIC_DRAW );
  triangle2.setOutlineWidth( .5f );
  triangle2.getModel().setPosition( { 320.f, 384.f / 2.f } );
  triangle2.getModel().setScale( { 50.f, 50.f } );
  triangle2.setFillColor( spectrumColorizer );

  nxgl::Clock timer;

  while ( !glfwWindowShouldClose( pWindow ) )
  {
    // Poll for and process events
    glfwPollEvents();

    int width, height;
    glfwGetFramebufferSize( pWindow, &width, &height );
    glViewport( 0, 0, width, height );
    glClearColor( 0.15f, 0.15f, 0.15f, 1.f );
    glClear( GL_COLOR_BUFFER_BIT );

    if ( timer.getMilliseconds() >= 500.f )
    {
      triangle.getModel().setAngle( triangle.getModel().getAngle() + 5.f );
      timer.reset();
    }

    triangle.draw( camera );
    triangle2.draw( camera );

    // Swap front and back buffers, i.e., display
    glfwSwapBuffers( pWindow );
  }
}

int main()
{
  nxgl::SLog::initializeConsole();

  std::vector< std::pair< std::string, float > > metrics;

  std::vector< nxgl::ui::EventReceiver * > distributors
    {
      // TODO: the ordering of these must change once docking is enabled
      // must appear before other windows that should be dockable
//      new nxgl::ui::ImguiDockProviderReceiver(),
      new nxgl::ui::ImguiMainWindowReceiver(),
      new nxgl::ui::ImguiPerformanceReceiver(),
      new nxgl::ui::ImguiPropertiesReceiver()
    };

  nxgl::ui::EventDistributor eventDistributor( distributors );
  nxgl::ApplicationContext appCtx;
  appCtx.windowSize = { 1280.f, 768.f };
  appCtx.camera.setProjection( appCtx.windowSize );
  appCtx.eventDistributor = &eventDistributor;

  auto * pWindow = createWindow(
    ( uint32_t )appCtx.windowSize.x,
    ( uint32_t )appCtx.windowSize.y,
    "nxgl" );

  nxgl::Application application( appCtx, pWindow );
  application.run();

  // runLoop( pWindow, camera );

  // delete BEFORE glfw terminates
  for ( auto * pEventReceiver : distributors )
    delete pEventReceiver;

  glfwTerminate();

  return 0;
}
