#ifndef INC_55B877408AC84A008713A61519B56CC0
#define INC_55B877408AC84A008713A61519B56CC0

namespace nxgl::gfx
{

template < GLenum glBufferType, typename TData >
class GLBuffer
{
public:

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  GLBuffer( GLenum bufferUsage, GLsizeiptr szBuffer )
    : GLBuffer( bufferUsage, szBuffer, nullptr )
  {}

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  GLBuffer( GLenum bufferUsage, GLsizeiptr szBuffer, const TData * pReadBuffer )
    : m_bufferUsage( bufferUsage ),
      m_szBuffer( szBuffer ),
      m_cntBuffer( szBuffer / sizeof( TData ) )
  {
    // generate the id
    GLExec( glGenBuffers( 1, &m_bufferId ) );

    // bind the buffer, so we can set its data
    GLExec( glBindBuffer( glBufferType, m_bufferId ) );

    // write to the GL buffer from the read buffer
    GLExec( glBufferData( glBufferType, m_szBuffer, pReadBuffer, m_bufferUsage ) );
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  virtual ~GLBuffer()
  {
    GLExec( glDeleteBuffers( 1, &m_bufferId ) );
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  void bind() const
  {
    GLExec( glBindBuffer( glBufferType, m_bufferId ) );
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  void unbind() const
  {
    GLExec( glBindBuffer( glBufferType, 0 ) );
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  void fill( const TData * pReadBuffer ) const
  {
    bind();
    GLExec( glBufferData( glBufferType, m_szBuffer, pReadBuffer, m_bufferUsage ) );
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  [[nodiscard]] TData getData( GLsizeiptr index ) const
  {
    assert( index * sizeof( TData ) < m_szBuffer );

    bind();

    TData data;

    GLExec( glGetBufferSubData(
      GL_ARRAY_BUFFER,
      ( GLintptr )( sizeof( TData ) * index ),
      sizeof( TData ),
      &data ) );

    return data;
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  void setData( GLsizeiptr index, const TData& data ) const
  {
    bind();
    assert( index * sizeof( TData ) < m_szBuffer );
    GLExec( glBufferSubData( glBufferType,
                             ( GLintptr )( sizeof( TData ) * index ),
                             sizeof( TData ),
                             &data ) );
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  void getDataRange( GLsizeiptr startIndex, uint32_t count, TData * pWriteBuffer ) const
  {
    bind();
    auto posStartByte = sizeof( TData ) * startIndex;
    auto szBytes = sizeof( TData ) * count;

    assert( posStartByte + szBytes <= m_szBuffer );

    GLExec( glGetBufferSubData(
      glBufferType,
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

    assert( posStartByte + szBytes <= m_szBuffer );

    GLExec( glBufferSubData(
      glBufferType,
      ( GLintptr )posStartByte,
      ( GLsizeiptr )szBytes,
      pReadBuffer ) );
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  ///
  /// \return the OpenGL buffer ID
  [[nodiscard]] inline GLuint id() const { return m_bufferId; }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  ///
  /// \return the size of the buffer in bytes
  [[nodiscard]] inline GLsizeiptr size() const { return m_szBuffer; }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  ///
  /// \return the element count
  [[nodiscard]] inline GLsizeiptr elementCount() const { return m_cntBuffer; }

private:

  GLuint m_bufferId { 0 };

  GLenum m_bufferUsage { GL_ARRAY_BUFFER };
  GLsizeiptr m_szBuffer { 0 };  // number of bytes in the buffer
  GLsizeiptr m_cntBuffer { 0 }; // number of elements in the buffer
};

}

#endif //INC_55B877408AC84A008713A61519B56CC0
