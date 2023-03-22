#ifndef INC_858B25608EBB4607B979787D881BAC21
#define INC_858B25608EBB4607B979787D881BAC21

namespace nxgl::ui
{

/***
 * call this BEFORE other imgui windows to enable window docking
 */
struct ImguiDockProviderReceiver : public EventReceiver
{
  void update( ApplicationContext& appCtx,
               GLFWwindow* window,
               const nxTimePoint frameDeltaInMS ) override
  {
    const ImGuiViewport * viewport = ImGui::GetMainViewport();
    ImGui::DockSpaceOverViewport( viewport );
  }
};

}

#endif //INC_858B25608EBB4607B979787D881BAC21
