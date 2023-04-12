#ifndef INC_7A83510106DD4BF5A11FC674784F9806
#define INC_7A83510106DD4BF5A11FC674784F9806

#include "utilities/Definitions.hpp"

namespace nxgl::gfx
{

/***
 * the camera holds the projection and view.
 */
class GLCamera
{
public:

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  void setProjection( const nxgl::nxVec2& windowSize )
  {
    m_windowSize = windowSize;
    m_proj = glm::ortho( 0.f, windowSize.x,
                         -windowSize.y, 0.f,
                         -1.f, 1.f );
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  void setProjection( const nxgl::nxVec2& windowSize, glm::mat4 projection )
  {
    m_windowSize = windowSize;
    m_proj = projection;
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  [[nodiscard]] inline glm::mat4 getProjection() const { return m_proj; }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  [[nodiscard]] inline nxgl::nxVec2 getWindowSize() const { return m_windowSize; }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  [[nodiscard]] inline nxgl::nxVec2 getViewCoordinates() const { return m_viewSize; }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  void setViewCoordinates( const nxgl::nxVec2& viewSize )
  {
    m_viewSize = viewSize;
    m_view = glm::translate( glm::mat4 { 1.f }, { viewSize, 1.f } );
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  [[nodiscard]] inline glm::mat4 getView() const { return m_view; }

  ////////////////////////////////////////////////////////////////////////////////

private:

  nxgl::nxVec2 m_windowSize { -1.f, 1.f };
  nxgl::nxVec2 m_viewSize   { -1.f, 1.f };

  glm::mat4 m_proj{ glm::ortho( -1.f, 1.f,        // x range
                                -1.f, 1.f,        // y range
                                -1.f, 1.f ) };    // z range

  glm::mat4 m_view { glm::translate( glm::mat4( 1.f ), { 0.0f, 0.0f, 0.0f } ) };

};
}

#endif //INC_7A83510106DD4BF5A11FC674784F9806
