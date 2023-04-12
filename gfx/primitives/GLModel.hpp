#ifndef INC_33748F58DD82421998FA7032A7AA1196
#define INC_33748F58DD82421998FA7032A7AA1196

#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

#include "gfx/primitives/GLData.hpp"

#include "gfx/primitives/GLCamera.hpp"
#include "gfx/primitives/GLBounds.hpp"

namespace nxgl::gfx
{

/***
 * the model performs transforms (position, rotate, scale) on an object. it requires
 * a camera and projection in order to translate to the actual screen.
 */
class GLModel
{
public:

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  void setPosition( const nxgl::nxVec2& position )
  {
    m_position = { position, 1.f };
    setBounds();
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  [[nodiscard]] nxgl::nxVec2 getPosition() const { return { m_position.x, m_position.y }; }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  void setScale( const nxgl::nxVec2& scale )
  {
    m_scale = { scale, 1.f };
    setBounds();
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  [[nodiscard]] inline nxgl::nxVec2 getScale() const { return { m_scale.x, m_scale.y }; }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  ///
  /// \param angle in degrees
  inline void setAngle( float angle ) { m_angle = angle; }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  ///
  /// \return angle in degrees
  [[nodiscard]] inline float getAngle() const { return m_angle; }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  inline void setOrigin( const nxgl::nxVec2& origin ) { m_origin = { origin, 1.f }; }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  [[nodiscard]] inline nxgl::nxVec2 getOrigin() const { return { m_origin.x, m_origin.y }; }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  [[nodiscard]] glm::mat4 getTranslation( const GLCamera& camera ) const
  {
    auto trans = glm::mat4( 1.f );
    trans = glm::translate( trans, m_position );
    trans = glm::rotate( trans, glm::radians( m_angle ), glm::vec3( 0.0f, 0.0f, 1.0f ) );
    trans = glm::scale( trans, m_scale );

    // perform P * V * M due to opengl's column-order layout rather than row-order layout
    // return camera.getProjection() * camera.getView() * trans;
    return camera.getProjection() * trans;
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  [[nodiscard]] inline nxgl::gfx::GLBounds getBounds() const { return m_bounds; }

  ////////////////////////////////////////////////////////////////////////////////

private:

  ////////////////////////////////////////////////////////////////////////////////
  /// PRIVATE:
  /// sets the bounds around the object based on the scale and position
  void setBounds()
  {
    m_bounds.topLeft = m_position - m_scale;
    m_bounds.bottomRight = m_position + m_scale;
  }

private:

  float m_angle { 0.f };  // in degrees

  nxgl::nxVec3 m_position { 0.f, 0.f, 0.f };
  nxgl::nxVec3 m_scale { 0.f, 0.f, 0.f };
  nxgl::nxVec2 m_size { 0.f, 0.f };
  nxgl::nxVec3 m_origin { 0.f, 0.f, 0.f }; // TODO: implement this
  nxgl::gfx::GLBounds m_bounds;
};


}

#endif //INC_33748F58DD82421998FA7032A7AA1196
