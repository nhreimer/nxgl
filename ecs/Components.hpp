#ifndef D3D42F0780A442E8B4FE88346CAA5D0E
#define D3D42F0780A442E8B4FE88346CAA5D0E

#include <entt/entt.hpp>

namespace nxgl::ecs
{

struct ECSBody
{
  nxgl::gfx::GLPolygon * pBody { nullptr };
  int8_t activeWedge { -1 };
  bool isBodyActive { false };
};

struct ECSWedgeColor
{
  entt::entity wedgeEntity { entt::null };
  nxColor color;
};

struct ECSWedge
{
  entt::entity bodyEntity { entt::null };
  bool isEnabled { true };
};

class PolygonFactory
{

public:

  static entt::entity create( entt::registry& registry,
                              nxgl::gfx::GLPolygon * polygon,
                              uint8_t wedges )
  {
    const auto entity = registry.create();
    auto& bodyEntity = registry.emplace< ECSBody >( entity );
    bodyEntity.pBody = polygon;



    return entity;
  }

private:


};

}

#endif //D3D42F0780A442E8B4FE88346CAA5D0E
