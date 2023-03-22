#ifndef INC_5CB07868677B43FA85DA098D43DA93EB
#define INC_5CB07868677B43FA85DA098D43DA93EB

#include "ApplicationContext.hpp"
#include "ImguiWrapperReceiver.hpp"

namespace nxgl::ui
{

//struct EventReceiverComparer
//{
//  bool operator()( EventReceiver * lhs, EventReceiver * rhs ) const
//  {
//    return lhs < rhs;
//  }
//};

class EventDistributor : public EventReceiver
{

public:

  EventDistributor() = default;

  explicit EventDistributor( const std::vector< EventReceiver * > & eventReceivers )
    : m_eventReceivers( eventReceivers )
  {}

  EventDistributor( uint32_t eventReceiverCount, EventReceiver* pEventReceivers )
  {
    for ( uint32_t i = 0; i < eventReceiverCount; ++i )
      m_eventReceivers.push_back( &pEventReceivers[ i ] );
  }

  void addReceiver( EventReceiver& eventReceiver )
  {
    m_eventReceivers.push_back( &eventReceiver );
  }

  void removeReceiver( EventReceiver& eventReceiver )
  {
    auto * pComparisonReceiver = &eventReceiver;

    for ( auto it = m_eventReceivers.begin();
          it != m_eventReceivers.end();
          ++it )
    {
      auto * pReceiver = ( * it );

      if ( pReceiver == pComparisonReceiver )
      {
        m_eventReceivers.erase( it );
        return;
      }
    }
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// STARTUP/SHUTDOWN EVENTS
  ////////////////////////////////////////////////////////////////////////////////

  void initialize( ApplicationContext& appCtx, GLFWwindow* window ) override
  {
    // BEFORE
    m_imguiWrapperReceiver.initialize( appCtx, window );

    for ( auto& eventReceivers : m_eventReceivers )
      eventReceivers->initialize( appCtx, window );
  }

  void shutdown( ApplicationContext& appCtx, GLFWwindow* window ) override
  {
    for ( auto& eventReceivers : m_eventReceivers )
      eventReceivers->shutdown( appCtx, window );


    // AFTER
    m_imguiWrapperReceiver.shutdown( appCtx, window );
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// FRAME EVENTS
  ////////////////////////////////////////////////////////////////////////////////

  void update( ApplicationContext& appCtx,
               GLFWwindow* window,
               const nxTimePoint frameDeltaInMS ) override
  {
    // BEFORE
    m_imguiWrapperReceiver.update( appCtx, window, frameDeltaInMS );

    for ( auto& eventReceivers : m_eventReceivers )
      eventReceivers->update( appCtx, window, frameDeltaInMS );
  }

  void draw( ApplicationContext& appCtx, GLFWwindow* window ) override
  {
    for ( auto& eventReceivers : m_eventReceivers )
      eventReceivers->draw( appCtx, window );

    // AFTER
    m_imguiWrapperReceiver.draw( appCtx, window );
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// EVENTS
  ////////////////////////////////////////////////////////////////////////////////

  void processKeyEvent(
    ApplicationContext& appCtx,
    GLFWwindow* window,
    int key,
    int scancode,
    int action,
    int mods ) override
  {
    for ( auto& eventReceivers : m_eventReceivers )
      eventReceivers->processKeyEvent( appCtx, window, key, scancode, action, mods );
  }

  /// MOUSE EVENTS

  void processMouseClickEvent(
    ApplicationContext& appCtx,
    GLFWwindow* window,
    int button,
    int action,
    int mods ) override
  {
    for ( auto& eventReceivers : m_eventReceivers )
      eventReceivers->processMouseClickEvent( appCtx, window, button, action, mods );
  }

  void processMouseMoveEvent( ApplicationContext& appCtx, GLFWwindow* window, double xpos, double ypos ) override
  {}

  void processMouseScrollEvent( ApplicationContext& appCtx, GLFWwindow* window, double xoffset, double yoffset ) override
  {}

  void processWindowMoveEvent( ApplicationContext& appCtx, GLFWwindow* window, double xpos, double ypos ) override
  {
    for ( auto& eventReceivers : m_eventReceivers )
      eventReceivers->processWindowMoveEvent( appCtx, window, xpos, ypos );
  }

private:
  ImguiWrapperReceiver m_imguiWrapperReceiver;
  std::vector< EventReceiver* > m_eventReceivers;
};

}

#endif //INC_5CB07868677B43FA85DA098D43DA93EB
