#ifndef INC_766C014EAECE42219FFF49CB59341F59
#define INC_766C014EAECE42219FFF49CB59341F59

#include "gfx/shapes/NXObject.hpp"
#include "gfx/shapes/IColorable.hpp"

namespace nxgl::gfx
{

////////////////////////////////////////////////////////////////////////////////

class NXSharedShape : public NXObject
{
public:

  NXSharedShape( GLenum glPrimitive,
                 GLsizei startDraw,
                 GLsizei drawCount )
    : m_glPrimitive( glPrimitive ),
      m_startDraw( startDraw ),
      m_drawCount( drawCount )
  {}

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  void draw( const GLCamera& camera, IMVPApplicator& mvpApplicator ) override
  {
    // don't own the following, so:
    // VAO should already be bound by this point
    // VBO should be bound by this point
    getBlender().blend();
    auto mvp = getModel().getTranslation( camera );
    mvpApplicator.applyMVP( mvp );
    GLExec( glDrawArrays( m_glPrimitive, m_startDraw, m_drawCount ) );
  }

private:
  GLenum m_glPrimitive;
  GLsizei m_startDraw;
  GLsizei m_drawCount;
};

////////////////////////////////////////////////////////////////////////////////

class NXSharedShapeContainer
{
public:

  explicit NXSharedShapeContainer( const std::vector< GLData >& buffer )
  {
    m_vbo.generate( GL_DYNAMIC_DRAW, buffer.size(), buffer.data() );
    GLData::setDataLayout( m_vao );
  }

  explicit NXSharedShapeContainer( NXVbo< GLData >&& vbo )
    : m_vbo( std::move( vbo ) )
  {
    m_vbo.bind();
    GLData::setDataLayout( m_vao );
  }

  inline size_t size() const { return m_shapes.size(); }

  NXSharedShape& getShape( uint32_t i )
  {
    assert( i < m_shapes.size() );
    return m_shapes[ i ];
  }

  NXSharedShape& addShape( const NXSharedShape& shape )
  {
    m_shapes.push_back( shape );
    return m_shapes[ m_shapes.size() - 1 ];
  }

  NXSharedShape& addShape( GLenum glPrimitive,
                 GLsizei startDraw,
                 GLsizei drawCount )
  {
    m_shapes.push_back( { glPrimitive, startDraw, drawCount } );
    return m_shapes[ m_shapes.size() - 1 ];
  }

  void draw( const GLCamera& camera, IMVPApplicator& mvpApplicator )
  {
    m_vao.bind();

    for ( auto& shape : m_shapes )
      shape.draw( camera, mvpApplicator );
  }

private:

  NXVbo< GLData > m_vbo;
  GLVao m_vao;

  std::vector< NXSharedShape > m_shapes;
};

////////////////////////////////////////////////////////////////////////////////

class NXShape : public NXObject
{

public:

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  NXShape( GLenum glPrimitive, const std::vector< GLData >& buffer )
    : m_glPrimitive( glPrimitive )
  {
    m_vbo.generate( GL_DYNAMIC_DRAW, buffer.size(), buffer.data() );
    GLData::setDataLayout( m_vao );
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  explicit NXShape( IGenerator& generator )
    : m_glPrimitive( generator.getPrimitiveType() )
  {
    std::vector< GLData > buffer;
    auto elementCount = generator.generate( buffer );
    m_vbo.generate( GL_DYNAMIC_DRAW, elementCount, buffer.data() );
    GLData::setDataLayout( m_vao );
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  NXShape( GLenum glPrimitive, size_t elements, const GLData * pReadBuffer )
    : m_glPrimitive( glPrimitive )
  {
    m_vbo.generate( GL_DYNAMIC_DRAW, elements, pReadBuffer );
    GLData::setDataLayout( m_vao );
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  void draw( const GLCamera& camera, IMVPApplicator& mvpApplicator ) override
  {
    m_vao.bind();
    m_vbo.bind();
    getBlender().blend();
    auto mvp = getModel().getTranslation( camera );
    mvpApplicator.applyMVP( mvp );
    GLExec( glDrawArrays( m_glPrimitive, 0, ( GLsizei )m_vbo.elementCount() ) );
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  void setColor( IColorable& colorizer )
  {
    for ( uint32_t vertexIndex = 0;
          vertexIndex < m_vbo.elementCount();
          ++vertexIndex )
    {
      auto color = colorizer( vertexIndex );
      setColor( vertexIndex, colorizer( vertexIndex ) );
    }
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  void setColor( uint32_t index, const nxgl::nxColor& color )
  {
    assert( sizeof( GLData ) * index + sizeof( nxgl::nxVec2 ) < m_vbo.size() );

    m_vbo.bind();

    GLExec( glBufferSubData(
      GL_ARRAY_BUFFER,
      ( GLintptr )( sizeof( GLData ) * index + sizeof( nxgl::nxVec2 ) ),
      sizeof( nxgl::nxColor ),
      &color ) );
  }

private:

  GLVao m_vao;
  NXVbo< GLData > m_vbo;

  GLenum m_glPrimitive;

};

}

#endif //INC_766C014EAECE42219FFF49CB59341F59
