#ifndef INC_766C014EAECE42219FFF49CB59341F59
#define INC_766C014EAECE42219FFF49CB59341F59

#include "gfx/shapes/NXObject.hpp"
#include "gfx/shapes/IColorable.hpp"

namespace nxgl::gfx
{


class NXShape : public NXObject
{

public:

  NXShape() = default;

  explicit NXShape( IGenerator& generator );
  NXShape( GLenum glPrimitive, const std::vector< GLData >& buffer );
  NXShape( GLenum glPrimitive, size_t elements, const GLData * pReadBuffer );

  void draw( const GLCamera& camera, IMVPApplicator& mvpApplicator ) override;

  void setColor( IColorable& colorizer );
  void setColor( uint32_t index, const nxgl::nxColor& color );

private:

  GLVao m_vao;
  GLVbo< GLData > m_vbo;

  GLenum m_glPrimitive;
};

}

#endif //INC_766C014EAECE42219FFF49CB59341F59
