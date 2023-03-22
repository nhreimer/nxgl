#ifndef INC_3535B0FAC443428689D627EE41198225
#define INC_3535B0FAC443428689D627EE41198225

namespace nxgl::ui
{

class ImguiPropertiesReceiver : public EventReceiver
{
public:

  void update( ApplicationContext &appCtx,
               GLFWwindow *window,
               const nxTimePoint frameDeltaInMS ) override
  {
    int xpos, ypos;
    glfwGetWindowPos( window, &xpos, &ypos );
    ImVec2 winPos = { ( float ) xpos, ( float ) ypos };

    auto *pDrawList = ImGui::GetBackgroundDrawList();
    auto winSize = appCtx.windowSize;

    pDrawList->AddRect( { winPos.x + winSize.x / 2.f, winPos.y + winSize.y / 2.f },
                        { winPos.x + winSize.x / 2.f + 100.f, winPos.y + winSize.y / 2.f + 100.f },
                        ImColor( 0.f, 1.f, 0.f, 1.f ));
  }
};

}


#endif //INC_3535B0FAC443428689D627EE41198225