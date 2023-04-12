#ifndef INC_85E6BEE922664348868A60D1D3D6B200
#define INC_85E6BEE922664348868A60D1D3D6B200

#include "Pch.hpp"

#include "gfx/shapes/IMVPApplicator.hpp"
#include "gfx/primitives/GLShader.hpp"

namespace nxgl::gfx
{

class NXDefaultShader : public IMVPApplicator
{
public:

  void applyMVP( const glm::mat4 &mvp ) override;

private:

  // lazy initialization
  void initialize();

private:

  void bind();

private:

  bool m_isCreated{ false };
  GLint m_mvpAddress{ 0 };
  GLShader m_shader;
};

}

#endif //INC_85E6BEE922664348868A60D1D3D6B200
