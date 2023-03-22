#ifndef DF61045CC5844B0CB85814A109AFB9FE
#define DF61045CC5844B0CB85814A109AFB9FE

namespace nxgl::gfx
{
class GLFbo
{

  // simple container for keeping track of all the IDs related to setting up the FBO
  struct FramebufferID
  {
    uint32_t fbo { 0 };
    uint32_t tcbo { 0 };
    uint32_t rbo { 0 };
  };

public:

  GLFbo( GLenum bufferTarget, GLsizei width, GLsizei height )
    : m_bufferTarget( bufferTarget ),
      m_width( width ),
      m_height( height )
  {
    glGenFramebuffers( 1, &m_bufferID.fbo );

    createColorAttachment();
    createRenderBuffer();
    attachBuffers();

    assert( glCheckFramebufferStatus( GL_FRAMEBUFFER ) == GL_FRAMEBUFFER_COMPLETE );
  }

  inline uint32_t getTextureID() const { return m_bufferID.tcbo; }

  inline void bind() const { GLExec( glBindFramebuffer( GL_FRAMEBUFFER, m_bufferID.fbo ) ); }
  inline void unbind() const { GLExec( glBindFramebuffer( GL_FRAMEBUFFER, 0 ) ); }

  ~GLFbo()
  {
    GLExec( glDeleteFramebuffers( 1, &m_bufferID.fbo ) );
    GLExec( glDeleteRenderbuffers( 1, &m_bufferID.rbo ) );
    GLExec( glDeleteTextures( 1, &m_bufferID.tcbo ) );
  }

private:

  void createColorAttachment()
  {
    GLExec( glGenTextures( 1, &m_bufferID.tcbo ) );
    GLExec( glBindTexture( GL_TEXTURE_2D, m_bufferID.tcbo ) );
    GLExec( glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr ) );
    GLExec( glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR ) );
    GLExec( glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR ) );
  }

  void createRenderBuffer()
  {
    GLExec( glGenRenderbuffers( 1, &m_bufferID.rbo ) );
    GLExec( glBindRenderbuffer( GL_RENDERBUFFER, m_bufferID.rbo ) );
    GLExec( glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height ) );
  }

  void attachBuffers() const
  {
    GLExec( glBindFramebuffer( m_bufferTarget, m_bufferID.fbo ) );
    GLExec( glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_bufferID.tcbo, 0 ) );
    GLExec( glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_bufferID.rbo ) );
  }

private:
  GLsizei m_width;
  GLsizei m_height;
  FramebufferID m_bufferID;
  GLenum m_bufferTarget;

};
}

#endif //DF61045CC5844B0CB85814A109AFB9FE
