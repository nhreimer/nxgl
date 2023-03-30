#include "Pch.hpp"

#include "Standards.hpp"
#include "gfx/Primitives.hpp"

#include "Application.hpp"

#include "ui/ImguiDockProviderReceiver.hpp"
#include "ui/ImguiPerformanceReceiver.hpp"
#include "ui/ImguiMainWindowReceiver.hpp"
#include "ui/ImguiPropertiesReceiver.hpp"
#include "gfx/shapes/ShapeShader.hpp"

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

// this function scopes all GLFW and opengl to this function, so that
// cleanup occurs PRIOR to context termination
static void runApplication( uint32_t width,       // initial width
                            uint32_t height,      // initial height
                            GLFWwindow * pWindow )
{
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
  nxgl::gfx::ShapeShader defaultShader;

  nxgl::ApplicationContext appCtx;

  appCtx.windowSize = { ( float )width, ( float )height };
  appCtx.camera.setProjection( appCtx.windowSize );
  appCtx.eventDistributor = &eventDistributor;
  appCtx.mvpApplicator = ( nxgl::gfx::IMVPApplicator * )&defaultShader;

  nxgl::Application application( appCtx, pWindow );
  application.run();

  // delete BEFORE glfw terminates
  for ( auto * pEventReceiver : distributors )
    delete pEventReceiver;
}

static void generate( uint16_t granularity, float width )
{
  std::vector< nxgl::gfx::GLData > buffer;

  auto angle = nxgl::NX_TAU / ( float )granularity;

  auto vertexAngle = 0.f;

  for ( uint16_t i = 0; i < granularity; ++i )
  {
    // outer one
    buffer.push_back( { { std::cos( angle ) * width, std::sin( angle ) * width },
                        { 1.f, 1.f, 1.f, 1.f } } );

    // inner one
    buffer.push_back( { { std::cos( angle ), std::sin( angle ) },
                        { 1.f, 1.f, 1.f, 1.f } } );

    vertexAngle = angle * ( float )i;

    // 2nd outer one
    buffer.push_back( { { std::cos( angle ) * width, std::sin( angle ) * width },
                        { 1.f, 1.f, 1.f, 1.f } } );
  }
}

int main()
{
  nxgl::SLog::initializeConsole();

  uint32_t width = 1280;
  uint32_t height = 768;

  auto * pWindow = createWindow( width, height, "nxgl" );
  runApplication( width, height, pWindow );
  glfwDestroyWindow( pWindow );
  glfwTerminate();

  return 0;
}
