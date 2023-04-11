#ifndef E14BB060A3B642459CC0D57629B21FA9
#define E14BB060A3B642459CC0D57629B21FA9

namespace nxgl::gfx
{

template < typename TData >
class NXVbo
{
public:

  // deleted copy ctor and assignment
  NXVbo( const NXVbo& ) = delete;
  NXVbo& operator=( const NXVbo& ) = delete;

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC CTOR:
  NXVbo() = default;

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC DTOR:
  ~NXVbo()
  {
    if ( m_bufferId > 0 )
    {
      GLExec( glDeleteBuffers( 1, &m_bufferId ) );
    }
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC MOVE CTOR:
  NXVbo( NXVbo&& other ) noexcept
  {
    std::swap( m_bufferId, other.m_bufferId );
    m_bufferUsage = other.m_bufferUsage;
    m_cntBuffer = other.m_cntBuffer;
    m_szBuffer = other.m_szBuffer;
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC MOVE ASSIGNMENT:
  NXVbo& operator=( NXVbo&& other ) noexcept
  {
    std::swap( m_bufferId, other.m_bufferId );
    m_bufferUsage = other.m_bufferUsage;
    m_cntBuffer = other.m_cntBuffer;
    m_szBuffer = other.m_szBuffer;
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC CTOR:
  NXVbo( GLenum bufferUsage, uint32_t elements, const TData * pReadBuffer )
  {
    generate( bufferUsage, elements, pReadBuffer );
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  void generate( GLenum bufferUsage, uint32_t elements, const TData * pReadBuffer )
  {
    m_szBuffer = ( GLsizeiptr )( elements * sizeof( TData ) );
    m_cntBuffer = ( GLsizeiptr )elements;

    // if this is thrown, then you've already generated the data
    assert( m_bufferId <= 0 );

    // generate the id
    GLExec( glGenBuffers( 1, &m_bufferId ) );

    // bind the buffer, so we can set its data
    bind();

    // write to the GL buffer from the read buffer
    GLExec( glBufferData( GL_ARRAY_BUFFER, m_szBuffer, pReadBuffer, m_bufferUsage ) );
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  inline void bind() const
  {
    // if this assert gets thrown, then you forgot to call generate()
    assert( m_bufferId > 0 );
    GLExec( glBindBuffer( GL_ARRAY_BUFFER, m_bufferId ) );
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  inline void unbind() const
  {
    GLExec( glBindBuffer( GL_ARRAY_BUFFER, 0 ) );
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  void fill( const TData * pReadBuffer ) const
  {
    bind();
    GLExec( glBufferData( GL_ARRAY_BUFFER, m_szBuffer, pReadBuffer, m_bufferUsage ) );
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
    GLExec( glBufferSubData( GL_ARRAY_BUFFER,
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
      GL_ARRAY_BUFFER,
      ( GLintptr )posStartByte,
      ( GLsizeiptr )szBytes,
      pWriteBuffer ) );
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  void setDataRange( GLsizeiptr startIndex, const std::vector< TData >& readBuffer ) const
  {
    setDataRange( startIndex, readBuffer.size(), readBuffer.data() );
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
      GL_ARRAY_BUFFER,
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

  GLenum m_bufferUsage { GL_STATIC_DRAW }; // default to this
  GLsizeiptr m_szBuffer { 0 };  // number of bytes in the buffer
  GLsizeiptr m_cntBuffer { 0 }; // number of elements in the buffer
};

}

#endif //E14BB060A3B642459CC0D57629B21FA9
