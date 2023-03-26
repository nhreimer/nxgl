#ifndef INC_788A7B75CB8046A8AD3C78B09F5B6F79
#define INC_788A7B75CB8046A8AD3C78B09F5B6F79

namespace nxgl
{

/// this is for testing primitives
/// \param pWindow
static void runLoop( GLFWwindow * pWindow )
{
  nxgl::Clock timer;

  nxgl::gfx::SpectrumColorizer colorizer;
  colorizer.setColors( 5, { 1.f, 1.f, 1.f, 1.f }, { 1.f, 0.f, 0.f, 1.f } );

  nxgl::gfx::GLData data[] =
    {
      { {  0.f, 0.f }, { 1.f, 1.f, 1.f, 1.f } },
      { {  1.f, 0.f }, { 1.f, 1.f, 1.f, 1.f } },
      { {  .5f, .5f }, { 1.f, 1.f, 1.f, 1.f } }
    };

  nxgl::gfx::GLVbo< nxgl::gfx::GLData > vbo( GL_DYNAMIC_DRAW, 3, data );
  nxgl::gfx::GLVao vao;
  nxgl::gfx::ShapeShader shader;
  nxgl::gfx::GLModel model;
  nxgl::gfx::GLCamera camera;

  for ( uint32_t i = 0; i < 3; ++i )
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
//  vao.registerVBO();

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

    GLExec( glDrawArrays( GL_TRIANGLES, 0, 3 ) );

    // Swap front and back buffers, i.e., display
    GLExec( glfwSwapBuffers( pWindow ) );
  }
}

}

#endif //INC_788A7B75CB8046A8AD3C78B09F5B6F79
