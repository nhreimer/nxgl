#ifndef AC6F85897BD04631A72189EA9E24083C
#define AC6F85897BD04631A72189EA9E24083C

#include "gfx/Primitives.hpp"

#include "gfx/shapes/IMVPApplicator.hpp"
#include "gfx/shapes/IGenerator.hpp"

#include "gfx/v2/NXVbo.hpp"

#include <vector>

namespace nxgl::gfx
{

/***
 * base object for any object that is drawable and can be manipulated
 */
class NXObject
{
public:

  NXObject() = default;
  virtual ~NXObject() = default;

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  GLBlend& getBlender() { return m_blender; }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  void setBlender( const GLBlend& blend ) { m_blender = blend; }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  GLModel& getModel() { return m_model; }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  void setModel( const GLModel& model ) { m_model = model; }

  ////////////////////////////////////////////////////////////////////////////////
  /// PUBLIC:
  virtual void draw( const GLCamera& camera, IMVPApplicator& mvpApplicator ) = 0;

private:

  GLBlend m_blender;
  GLModel m_model;

};

}

#endif //AC6F85897BD04631A72189EA9E24083C
