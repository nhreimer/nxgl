#ifndef A8B2D764596C4059B7B2C9AA507BC264
#define A8B2D764596C4059B7B2C9AA507BC264

namespace nxgl::gfx
{

struct IGenerator
{
  virtual uint32_t generate( std::vector< GLData > &buffer ) = 0;

  [[nodiscard]]
  virtual GLenum getPrimitiveType() const = 0;
};

}

#endif //A8B2D764596C4059B7B2C9AA507BC264
