#ifndef INC_318D0B9B45B4468B88E526CEB17E3543
#define INC_318D0B9B45B4468B88E526CEB17E3543

#include "gfx/primitives/GLUtility.hpp"

namespace nxgl::gfx
{

template < typename TData >
class GLVbo
{

public:

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  /// creates a buffer and reserves the data (doesn't write to buffer)
  /// \param bufferUsage GL_DYNAMIC_DRAW, GL_STATIC_DRAW, etc.
  /// \param elementCount number of elements to reserve
  GLVbo( GLenum bufferUsage, GLsizeiptr elementCount )
    : GLVbo( bufferUsage, elementCount, nullptr )
  {}

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  /// takes ownership of a buffer that has been created
  /// \param bufferUsage GL_DYNAMIC_DRAW, GL_STATIC_DRAW, etc.
  /// \param vboId opengl-provided VBO identifier
  /// \param elementCount number of elements in buffer
  GLVbo( GLenum bufferUsage, GLuint vboId, GLsizeiptr elementCount )
    : m_bufferUsage( bufferUsage ),
      m_vbo( vboId ),
      m_size( elementCount * sizeof( TData ) )
  {}

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  /// creates a buffer and copies the data from an existing buffer
  /// \param bufferUsage GL_DYNAMIC_DRAW, GL_STATIC_DRAW, etc.
  /// \param elementCount number of elements in the buffer
  /// \param pReadBuffer the buffer to copy into the VBO
  GLVbo( GLenum bufferUsage, GLsizeiptr elementCount, const TData * pReadBuffer )
    : m_bufferUsage( bufferUsage ),
      m_size( elementCount * sizeof( TData ) )
  {
    // generate the id
    GLExec( glGenBuffers( 1, &m_vbo ) );

    // bind the buffer, so we can set its data
    GLExec( glBindBuffer( GL_ARRAY_BUFFER, m_vbo ) );

    // write to the GL buffer from the read buffer
    GLExec( glBufferData( GL_ARRAY_BUFFER, m_size, pReadBuffer, m_bufferUsage ) );

    LOG_DEBUG( "VBO allocated: {} bytes. {} elements.", m_size, elementCount );
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  ~GLVbo()
  {
    GLExec( glDeleteBuffers( 1, &m_vbo ) );
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  /// fills the entire buffer of the VBO
  /// \param pReadBuffer pointer to the buffer. the size is expected to match the size
  ///                    provided to the constructor.
  void fill( const TData * pReadBuffer ) const
  {
    bind();
    GLExec( glBufferData( GL_ARRAY_BUFFER, m_size, pReadBuffer, m_bufferUsage ) );
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  /// fills the entire buffer of the VBO
  /// \param readBuffer vector to be copied to the VBO
  void fill( const std::vector< TData >& readBuffer ) const
  {
    bind();
    GLExec( glBufferData( GL_ARRAY_BUFFER, m_size, readBuffer.data(), m_bufferUsage ) );
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  ///
  /// \param index of the element (NOT the index in bytes)
  /// \return data element at the index
  [[nodiscard]] TData getData( GLsizeiptr index ) const
  {
    bind();
    assert( index * sizeof( TData ) < m_size );

    TData layout;

    GLExec( glGetBufferSubData(
      GL_ARRAY_BUFFER,
      ( GLintptr )( sizeof( TData ) * index ),
      sizeof( TData ),
      &layout ) );

    return layout;
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  void setData( GLsizeiptr index, const TData& layout ) const
  {
    bind();
    assert( index * sizeof( TData ) < m_size );
    GLExec( glBufferSubData( GL_ARRAY_BUFFER,
                     ( GLintptr )( sizeof( TData ) * index ),
                     sizeof( TData ),
                     &layout ) );
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  void getDataRange( GLsizeiptr startIndex, uint32_t count, TData * pWriteBuffer ) const
  {
    bind();
    auto posStartByte = sizeof( TData ) * startIndex;
    auto szBytes = sizeof( TData ) * count;

    assert( posStartByte + szBytes <= m_size );

    GLExec( glGetBufferSubData(
      GL_ARRAY_BUFFER,
      ( GLintptr )posStartByte,
      ( GLsizeiptr )szBytes,
      pWriteBuffer ) );
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  void setDataRange( GLsizeiptr startIndex, uint32_t count, const TData * pReadBuffer ) const
  {
    bind();
    auto posStartByte = sizeof( TData ) * startIndex;
    auto szBytes = sizeof( TData ) * count;

    assert( posStartByte + szBytes <= m_size );

    GLExec( glBufferSubData(
      GL_ARRAY_BUFFER,
      ( GLintptr )posStartByte,
      ( GLsizeiptr )szBytes,
      pReadBuffer ) );
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  inline void bind() const
  {
    GLExec( glBindBuffer( GL_ARRAY_BUFFER, m_vbo ) );
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  inline void unbind() const
  {
    GLExec( glBindBuffer( GL_ARRAY_BUFFER, 0 ) );
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  ///
  /// \return size in bytes
  [[nodiscard]] inline GLsizeiptr size() const { return m_size; }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  ///
  /// \return number of TData elements
  [[nodiscard]] inline GLsizei count() const { return m_size / sizeof( TData ); }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  ///
  /// \return the OpenGL buffer ID
  [[nodiscard]] inline GLuint id() const { return m_vbo; }

  ////////////////////////////////////////////////////////////////////////////////

private:
  GLenum m_bufferUsage { GL_DYNAMIC_DRAW }; // GL_STATIC_DRAW, GL_DYNAMIC_DRAW, etc.
  GLsizeiptr m_size { 0 };                  // in bytes
  GLuint m_vbo { 0 };                       // opengl buffer id
};
}

#endif //INC_318D0B9B45B4468B88E526CEB17E3543
