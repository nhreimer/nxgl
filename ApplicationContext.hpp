#ifndef INC_8BF8F887574143C09FEC0B4499E4E454
#define INC_8BF8F887574143C09FEC0B4499E4E454

#include <entt/entt.hpp>

namespace nxgl::ui
{
  class EventDistributor;
}

namespace nxgl::gfx
{
  struct IMVPApplicator;
  class GLPolygon;
}

namespace nxgl
{

/***
 * an instance of this gets pushed out with each event and frame
 */
struct ApplicationContext
{
  // the size of the window. do not cache it because it will be updated as the window changs.
  nxgl::nxVec2 windowSize;

  // this is the project and view. it needs to be combined with a model and then
  // applied to an object via the IMVPApplicator
  nxgl::gfx::GLCamera camera;

  // this is the event distributor. it can be used to remove or add additional event receivers
  nxgl::ui::EventDistributor * eventDistributor { nullptr };

  // this applies the model, view, projection to an object.
  // this should either be a shader or transform that actually applies it when drawing
  // see ShapeShader as an example
  nxgl::gfx::IMVPApplicator * mvpApplicator { nullptr };

  // registry for current scene lookups
  entt::registry * pRegistry { nullptr };
};

}

#endif //INC_8BF8F887574143C09FEC0B4499E4E454
