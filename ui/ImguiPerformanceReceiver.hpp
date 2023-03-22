#ifndef INC_487C689D3F044ED489E76BBAFF2E171F
#define INC_487C689D3F044ED489E76BBAFF2E171F

namespace nxgl::ui
{

class ImguiPerformanceReceiver : public EventReceiver
{
public:

  void update( ApplicationContext& appCtx,
               GLFWwindow* window,
               const nxTimePoint frameDeltaInMS ) override
  {
    static float f = 0.0f;

    ImGui::Begin( "Performance" );
    {
      ImGui::Text( "Performance Metrics" );               // Display some text (you can use a format strings too)
      ImGui::Text( "Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate );
    }
    ImGui::End();
  }

private:

  void drawBoundingBox( ApplicationContext& appCtx,
                        GLFWwindow* window )
  {
    int xpos, ypos;
    glfwGetWindowPos( window, &xpos, &ypos );
    ImVec2 winPos = { ( float )xpos, ( float )ypos };

    auto * pDrawList = ImGui::GetBackgroundDrawList();
    auto winSize = appCtx.windowSize;

    pDrawList->AddRect( { winPos.x + winSize.x / 2.f, winPos.y + winSize.y / 2.f },
                        { winPos.x + winSize.x / 2.f + 100.f, winPos.y + winSize.y / 2.f + 100.f },
                        ImColor( 0.f, 1.f, 0.f, 1.f ) );
  }

};

}

#endif //INC_487C689D3F044ED489E76BBAFF2E171F
