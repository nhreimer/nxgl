#ifndef INC_30FA7CCE698047F2AB0AED69344FE31B
#define INC_30FA7CCE698047F2AB0AED69344FE31B

namespace nxgl
{

/// this is for testing shaders
/// \param pWindow
void runLoopShaders( GLFWwindow * pWindow )
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
  nxgl::gfx::GLShader shader;
  nxgl::gfx::GLModel model;
  nxgl::gfx::GLCamera camera;

  assert( shader.addShader( GL_VERTEX_SHADER, R"(resources/test/Vertex.glsl)" ) );
  assert( shader.addShader( GL_GEOMETRY_SHADER, R"(resources/test/Geometry.glsl)" ) );
  assert( shader.addShader( GL_FRAGMENT_SHADER, R"(resources/test/Fragment.glsl)" ) );
  assert( shader.link() );

  auto mvpAddress = shader.getUniformAddress( "uMVP" );
  auto edgesAddress = shader.getUniformAddress( "uEdges" );

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
  vao.registerVBO();

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
    shader.setUniformMatrix( mvpAddress, mvp );
    shader.setUniformUInt( edgesAddress, 3 );

    GLExec( glDrawArrays( GL_TRIANGLES, 0, 3 ) );

    // Swap front and back buffers, i.e., display
    GLExec( glfwSwapBuffers( pWindow ) );
  }
}
}

#endif //INC_30FA7CCE698047F2AB0AED69344FE31B
