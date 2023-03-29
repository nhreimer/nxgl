#ifndef AC6F85897BD04631A72189EA9E24083C
#define AC6F85897BD04631A72189EA9E24083C

namespace nxgl::gfx
{

/***
 * base object for any object that is drawable and can be manipulated
 */
class GLObject
{
public:

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  GLBlend& getBlender() { return m_blender; }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  void setBlender( const GLBlend& blend ) { m_blender = blend; }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  GLModel& getModel() { return m_model; }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  void setModel( const GLModel& model ) { m_model = model; }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  virtual void draw( const GLCamera& camera, IMVPApplicator& mvpApplicator ) = 0;

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  static void setColor( const GLVbo< GLData >& vbo, uint32_t index, const nxgl::nxColor& color )
  {
    assert( sizeof( GLData ) * index + sizeof( nxgl::nxVec2 ) < vbo.size() );

    vbo.bind();

    GLExec( glBufferSubData(
      GL_ARRAY_BUFFER,
      ( GLintptr )( sizeof( GLData ) * index + sizeof( nxgl::nxVec2 ) ),
      sizeof( nxgl::nxColor ),
      &color ) );
  }

private:

  GLBlend m_blender;
  GLModel m_model;

};

}

#endif //AC6F85897BD04631A72189EA9E24083C
