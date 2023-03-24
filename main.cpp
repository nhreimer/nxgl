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

namespace nxgl::gfx
{

template < typename TData >
class _GLVbo : public GLBuffer< GL_ARRAY_BUFFER, TData >
{
public:
  _GLVbo( GLenum bufferUsage, GLsizeiptr elementCount )
    : GLBuffer< GL_ARRAY_BUFFER, TData >( bufferUsage, elementCount * sizeof( TData ), nullptr )
  {}

  _GLVbo( GLenum bufferUsage, GLsizeiptr elementCount, const TData * pReadBuffer )
    : GLBuffer< GL_ARRAY_BUFFER, TData >( bufferUsage, elementCount * sizeof( TData ), pReadBuffer )
  {}
};

template < typename TUint >
class _GLIbo : public GLBuffer< GL_ELEMENT_ARRAY_BUFFER, TUint >
{
public:

  // opengl is strict about the unsigned part
  static_assert( std::is_integral< TUint >::value &&
                 std::is_unsigned< TUint >::value,
                 "TUint must be an unsigned integer!" );

  _GLIbo( GLenum bufferUsage, GLsizeiptr elementCount )
    : GLBuffer< GL_ELEMENT_ARRAY_BUFFER, TUint >( bufferUsage, elementCount * sizeof( TUint ), nullptr )
  {}

  _GLIbo( GLenum bufferUsage, GLsizeiptr elementCount, const TUint * pReadBuffer )
  : GLBuffer< GL_ELEMENT_ARRAY_BUFFER, TUint >( bufferUsage, elementCount * sizeof( TUint ), pReadBuffer )
  {}

private:
};

}

static void runLoop( GLFWwindow * pWindow )
{
  nxgl::Clock timer;

  nxgl::gfx::GLData data[] =
  {
    //   x, y            r, g, b, a
    { {  0.f,   0.f }, { 1.f, 1.f, 1.f, 1.f } },  // A        0
    { {  1.f,   1.f }, { 1.f, 1.f, 1.f, 1.f } },  // B        1
    { { -1.f,   1.f }, { 1.f, 1.f, 1.f, 1.f } },  // C        2

//    { {  0.f,   0.f }, { 1.f, 1.f, 1.f, 1.f } },  // A      0
//    { { -1.f,   1.f }, { 1.f, 1.f, 1.f, 1.f } },  // C      2
    { { -1.f,  -1.f }, { 1.f, 1.f, 1.f, 1.f } },  // D        3

//    { {  0.f,   0.f }, { 1.f, 1.f, 1.f, 1.f } },  // A      0
//    { { -1.f,  -1.f }, { 1.f, 1.f, 1.f, 1.f } },  // D      3
    { {  1.f,  -1.f }, { 1.f, 1.f, 1.f, 1.f } }  // E         4

//    { {  0.f,   0.f }, { 1.f, 1.f, 1.f, 1.f } },  // A      0
//    { {  1.f,  -1.f }, { 1.f, 1.f, 1.f, 1.f } },  // E      4
//    { {  1.f,   1.f }, { 1.f, 1.f, 1.f, 1.f } }   // B      1
  };

  uint8_t indices[] =
  {
    0, 1, 2,
    0, 2, 3,
    0, 3, 4,
    0, 4, 1
  };

  nxgl::gfx::SpectrumColorizer colorizer;
  colorizer.setColors( 5, { 1.f, 1.f, 1.f, 1.f }, { 1.f, 0.f, 0.f, 1.f } );

  nxgl::gfx::_GLVbo< nxgl::gfx::GLData > vbo( GL_DYNAMIC_DRAW,  5, data );
//  nxgl::gfx::_GLIbo< uint8_t > ibo( GL_STATIC_DRAW, 12, indices );
  nxgl::gfx::GLVao vao;
  nxgl::gfx::ShapeShader shader;
  nxgl::gfx::GLModel model;
  nxgl::gfx::GLCamera camera;

  for ( uint32_t i = 0; i < 5; ++i )
  {
    nxgl::nxColor color = colorizer( i );
    GLExec( glBufferSubData(
      GL_ARRAY_BUFFER,
      ( GLintptr )( sizeof( nxgl::gfx::GLData ) * i + sizeof( nxgl::nxVec2 ) ),
      sizeof( nxgl::nxColor ),
      &color ) );
  }

  camera.setProjection( { 1280.f, 768.f } );
  model.setScale( { 100.f, 100.f } );
  model.setPosition( { 1280.f / 2.f, 768.f / 2.f } );

  vbo.bind();
  vao.registerVBO();

//  ibo.bind();
  vbo.unbind();

  while ( !glfwWindowShouldClose( pWindow ) )
  {
    // Poll for and process events
    glfwPollEvents();

    int width, height;
    glfwGetFramebufferSize( pWindow, &width, &height );
    glViewport( 0, 0, width, height );
    glClearColor( 0.15f, 0.15f, 0.15f, 1.f );
    glClear( GL_COLOR_BUFFER_BIT );

    vao.bind();
    auto mvp = model.getTranslation( camera );
    shader.applyMVP( mvp );

    GLExec( glDrawElements( GL_TRIANGLE_FAN, 12, GL_UNSIGNED_BYTE, indices ) );

    // Swap front and back buffers, i.e., display
    GLExec( glfwSwapBuffers( pWindow ) );
  }
}

static void createVertices( uint32_t edges )
{
  // divide a circle by the number of edges
  // which is A(0, 0) -> B(0, 1) -> C(1, 1)
  // note that C is slightly less than 1, 1 however
  auto angle = nxgl::NX_TAU / ( float )edges;

  // move counterclockwise starting at the center (0, 0)
  // ABC
  // ACD
  // ADE ...

  // get the first two points and then we only need to calculate the third
  // every loop

  nxgl::nxColor white { 1.f, 1.f, 1.f, 1.f };

  // this is fixed
  nxgl::nxVec2 pointA { 0, 0 };

  // set point B
  nxgl::nxVec2 pointB { std::cos( 0 ), std::sin( 0 ) };

  uint32_t posInBuffer = 0;

  for ( uint32_t i = 0; i < edges; ++i )
  {
    auto thirdPointAngle = ( float )( i + 1 ) * angle;
    nxgl::nxVec2 pointC { std::cos( thirdPointAngle ), std::sin( thirdPointAngle ) };

    LOG_DEBUG( "A: ({}, {})", pointA.x, pointA.y );
    LOG_DEBUG( "B: ({}, {})", pointB.x, pointB.y );
    LOG_DEBUG( "C: ({}, {})", pointC.x, pointC.y );

    posInBuffer += 3;
    pointB = pointC;
  }
}

int main()
{
  nxgl::SLog::initializeConsole();

  uint32_t width = 1280;
  uint32_t height = 768;

  auto * pWindow = createWindow( width, height, "nxgl" );
  runApplication( width, height, pWindow );
//  runLoop( pWindow );
  glfwDestroyWindow( pWindow );
  glfwTerminate();

  return 0;
}
