#ifndef INC_49DC96B6E85B45568A38AF7B87B62406
#define INC_49DC96B6E85B45568A38AF7B87B62406

namespace nxgl::gfx
{

// this applies the final MVP form to an object during a rendering frame
// this is applied to the default shader used in polygons currently
struct IMVPApplicator
{
  virtual void applyMVP( const glm::mat4 & mvp ) = 0;
};
}

#endif //INC_49DC96B6E85B45568A38AF7B87B62406
