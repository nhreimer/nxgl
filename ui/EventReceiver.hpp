#ifndef D487FCB1799D49BD91CAE64D35DF8321
#define D487FCB1799D49BD91CAE64D35DF8321

#include "ApplicationContext.hpp"

namespace nxgl::ui
{
/***
 * no-implementation interface
 */
struct EventReceiver
{
  EventReceiver() = default;
  virtual ~EventReceiver() = default;

  ////////////////////////////////////////////////////////////////////////////////
  /// STARTUP/SHUTDOWN EVENTS
  ////////////////////////////////////////////////////////////////////////////////

  virtual void initialize( ApplicationContext& appCtx, GLFWwindow* window ) {}
  virtual void shutdown( ApplicationContext& appCtx, GLFWwindow* window ) {}

  ////////////////////////////////////////////////////////////////////////////////
  /// FRAME EVENTS
  ////////////////////////////////////////////////////////////////////////////////

  virtual void update( ApplicationContext& appCtx,
               GLFWwindow* window,
               const nxTimePoint frameDeltaInMS ) {}

  virtual void draw( ApplicationContext& appCtx, GLFWwindow* window ) {}

  ////////////////////////////////////////////////////////////////////////////////
  /// EVENTS
  ////////////////////////////////////////////////////////////////////////////////

  virtual void processKeyEvent(
    ApplicationContext& appCtx,
    GLFWwindow* window,
    int key,
    int scancode,
    int action,
    int mods ) {}

  /// MOUSE EVENTS

  virtual void processMouseClickEvent(
    ApplicationContext& appCtx,
    GLFWwindow* window,
    int button,
    int action,
    int mods ) {}

  virtual void processMouseMoveEvent( ApplicationContext& appCtx, GLFWwindow* window, double xpos, double ypos ) {}

  virtual void processMouseScrollEvent( ApplicationContext& appCtx, GLFWwindow* window, double xoffset, double yoffset ) {}

  virtual void processWindowMoveEvent( ApplicationContext& appCtx, GLFWwindow* window, double xpos, double ypos ) {}

};

}

#endif //D487FCB1799D49BD91CAE64D35DF8321
