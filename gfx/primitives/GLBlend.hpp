#ifndef B7A7C2C7202D4C2BAA36F9359B3BB08B
#define B7A7C2C7202D4C2BAA36F9359B3BB08B

namespace nxgl::gfx
{
struct GLBlend
{
  bool isEnabled{ false };

  GLenum srcColor{ GL_ONE };
  GLenum srcAlpha{ GL_ONE };
  GLenum destColor{ GL_ONE };
  GLenum destAlpha{ GL_ONE };

  GLenum equationColor{ GL_FUNC_ADD };
  GLenum equationAlpha{ GL_FUNC_ADD };

  void blend()
  {
    if ( isEnabled )
    {
      glEnable( GL_BLEND );

      glBlendFuncSeparate( srcColor,
                           srcAlpha,
                           destColor,
                           destAlpha );

      glBlendEquationSeparate( equationColor,
                               equationAlpha );
    }
  }
};
}
#endif //B7A7C2C7202D4C2BAA36F9359B3BB08B
