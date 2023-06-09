#include "Pch.hpp"

#include "Standards.hpp"
#include "gfx/Primitives.hpp"

#include "Application.hpp"

#include "ui/ImguiDockProviderReceiver.hpp"
#include "ui/ImguiPerformanceReceiver.hpp"
#include "ui/ImguiMainWindowReceiver.hpp"
#include "ui/ImguiPropertiesReceiver.hpp"
#include "gfx/shapes/NXDefaultShader.hpp"

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
  glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
  glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 6 );
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
  //if ( gladLoadGL( glfwGetProcAddress ) == 0 )
  if ( gladLoadGL() == 0 )
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
  nxgl::gfx::NXDefaultShader defaultShader;

  entt::registry registry;

  nxgl::ApplicationContext appCtx;

  appCtx.windowSize = { ( float )width, ( float )height };
  appCtx.camera.setProjection( appCtx.windowSize );
  appCtx.eventDistributor = &eventDistributor;
  appCtx.mvpApplicator = ( nxgl::gfx::IMVPApplicator * )&defaultShader;
  appCtx.pRegistry = &registry;

  nxgl::Application application( appCtx, pWindow );
  application.run();

  // delete BEFORE glfw terminates
  for ( auto * pEventReceiver : distributors )
    delete pEventReceiver;
}

static void runTest(  uint32_t winWidth,       // initial width
                      uint32_t winHeight,      // initial height
                      GLFWwindow * pWindow )
{
  std::vector< nxgl::gfx::GLData > vertices
    {
      { { -.5f, -.5f  },  { 1.f, 1.f, 1.f, 1.f } },
      { {  .5f, -.5f  },  { 1.f, 1.f, 1.f, 1.f } },
      { {  .0f,  .5f  },  { 1.f, 1.f, 1.f, 1.f } },

      { { -.5f, -.5f  },  { 1.f, 0.f, 0.f, 1.f } },
      { {  .5f, -.5f  },  { 1.f, 1.f, 1.f, 1.f } },
      { {  .0f,  .5f  },  { 1.f, 0.f, 1.f, 1.f } }
    };

  nxgl::gfx::GLVao vao;
  nxgl::gfx::GLVbo< nxgl::gfx::GLData > vbo;
  vbo.generate( GL_STATIC_DRAW, vertices.size(), vertices.data() );
  nxgl::gfx::GLData::setDataLayout( vao );
  nxgl::gfx::GLShader shader;
  assert( shader.loadShader( GL_VERTEX_SHADER  , "resources/DefaultVertex.glsl"   ) );
  assert( shader.loadShader( GL_FRAGMENT_SHADER, "resources/DefaultFragment.glsl" ) );
  assert( shader.link() );

  nxgl::gfx::GLCamera camera;
  nxgl::gfx::GLModel objectModel1;
  nxgl::gfx::GLModel objectModel2;

  objectModel1.setScale( { 100.f, 100.f } );
  objectModel1.setPosition( { ( float )winWidth / 2.f, ( float )winHeight / 2.f } );

  objectModel2.setScale( { 100.f, 100.f } );
  objectModel2.setPosition( { ( float )winWidth / 2.f - 200.f, ( float )winHeight / 2.f } );

  camera.setProjection( { winWidth, winHeight } );

  vao.bind();
  shader.bind();

  auto mvpAddress = shader.getUniformAddress( "uMVP" );

  nxgl::Clock clock;
  while ( !glfwWindowShouldClose( pWindow ) )
  {
    glfwPollEvents();

    int width, height;
    glfwGetFramebufferSize( pWindow, &width, &height );
    glViewport( 0, 0, width, height );
    glClearColor( .15f, .15f, .15f, 1.f );
    glClear( GL_COLOR_BUFFER_BIT );

    auto mvp = objectModel1.getTranslation( camera );
    shader.setUniformMatrix( mvpAddress, mvp );
    GLExec( glDrawArrays( GL_TRIANGLES, 0, 3 ) );

    if ( clock.getMilliseconds() >= 75.f )
    {
      objectModel2.setAngle( objectModel2.getAngle() - 1.f );
      clock.reset();
    }

    mvp = objectModel2.getTranslation( camera );
    shader.setUniformMatrix( mvpAddress, mvp );
    GLExec( glDrawArrays( GL_TRIANGLES, 3, 3 ) );

//    model.setScale( { 80, 80 } );
//    mvp = model.getTranslation( camera );
//    shader.setUniformMatrix( mvpAddress, mvp );
//    GLExec( glDrawArrays( GL_TRIANGLES, 3, 3 ) );

    GLExec( glfwSwapBuffers( pWindow ) );
  }
}

int main()
{
  nxgl::SLog::initializeConsole();

  uint32_t width = 1280;
  uint32_t height = 768;

  auto * pWindow = createWindow( width, height, "nxgl" );
  runApplication( width, height, pWindow );
//  runTest( width, height, pWindow );
  glfwDestroyWindow( pWindow );
  glfwTerminate();

  return 0;
}
