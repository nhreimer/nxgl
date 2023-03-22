#ifndef INC_8BF8F887574143C09FEC0B4499E4E454
#define INC_8BF8F887574143C09FEC0B4499E4E454

namespace nxgl::ui
{
  class EventDistributor;
}

namespace nxgl
{

/***
 * an instance of this gets pushed out with each event and frame
 */
struct ApplicationContext
{
  nxgl::nxVec2 windowSize;
  nxgl::gfx::GLCamera camera;
  nxgl::ui::EventDistributor * eventDistributor { nullptr };
};

}

#endif //INC_8BF8F887574143C09FEC0B4499E4E454
