#ifndef B7A7C2C7202D4C2BAA36F9359B3BB08B
#define B7A7C2C7202D4C2BAA36F9359B3BB08B

namespace nxgl::gfx
{
// visit the following site for online testing
// https://www.andersriggelsen.dk/glblendfunc.php
struct GLBlend
{
  bool isEnabled{ false };

  // indicates to use the blendColor option
  bool useBlendColor{ false };

  GLenum srcColor{ GL_ONE };
  GLenum srcAlpha{ GL_ONE };
  GLenum destColor{ GL_ONE };
  GLenum destAlpha{ GL_ONE };

  GLenum equationColor{ GL_FUNC_ADD };
  GLenum equationAlpha{ GL_FUNC_ADD };

  nxColor blendColor { 1.f, 1.f, 1.f, 1.f };

  void blend() const
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

      if ( useBlendColor )
        glBlendColor( blendColor.r, blendColor.g, blendColor.b, blendColor.a );
    }
  }
};
}
#endif //B7A7C2C7202D4C2BAA36F9359B3BB08B
