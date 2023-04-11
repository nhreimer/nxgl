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
    ImVec2 winPos = { ( float )xpos, ( float )ypos };

    auto *pDrawList = ImGui::GetBackgroundDrawList();
    auto winSize = appCtx.windowSize;

//    pDrawList->AddCircle( { winPos.x + winSize.x / 2.f - 150.f,
//                            winPos.y + winSize.y / 2.f },
//                          10.f,
//                          ImColor( 0.f, 1.f, 0.f, 1.f ) );
//
//    pDrawList->AddCircle( { winPos.x + winSize.x / 2.f,
//                            winPos.y + winSize.y / 2.f },
//                          10.f,
//                          ImColor( 0.f, 1.f, 0.f, 1.f ) );

    // TODO: need to set up a system of data, e.g., entt library

//    if ( appCtx.uiCtx.pShape != nullptr )
//    {
//      auto triangle = appCtx.uiCtx.pShape->getTriangle( appCtx.uiCtx.index );
//      pDrawList->AddTriangle( { winPos.x + triangle.pointA.x, winPos.y + winSize.y - triangle.pointA.y },
//                              { winPos.x + triangle.pointB.x, winPos.y + winSize.y - triangle.pointB.y },
//                              { winPos.x + triangle.pointC.x, winPos.y + winSize.y - triangle.pointC.y },
//                              ImColor( 1.f, 0.f, 0.f, 1.f ) );
//    }
  }
};

}


#endif //INC_3535B0FAC443428689D627EE41198225
