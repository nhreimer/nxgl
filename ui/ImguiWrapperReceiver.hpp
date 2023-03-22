#ifndef INC_0A08FB8480A34ABE9105EB8C918D3439
#define INC_0A08FB8480A34ABE9105EB8C918D3439

#include "ApplicationContext.hpp"
#include "EventReceiver.hpp"
#include "Logger.hpp"

namespace nxgl::ui
{

class ImguiWrapperReceiver : public EventReceiver
{
public:

  // call order: BEFORE other imgui windows
  void initialize( ApplicationContext& appCtx, GLFWwindow* window ) override
  {
    LOG_DEBUG( "checking imgui version and creating imgui context" );
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    m_pIO = &ImGui::GetIO(); //( void )io;
    m_pIO->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    m_pIO->ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking
    m_pIO->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // When viewports are enabled we tweak WindowRounding/WindowBg
    // so platform windows can look identical to regular ones.
    LOG_DEBUG( "setting up imgui viewports" );
    ImGuiStyle& style = ImGui::GetStyle();
    if ( m_pIO->ConfigFlags & ImGuiConfigFlags_ViewportsEnable )
    {
      style.WindowRounding = 0.0f;
      style.Colors[ ImGuiCol_WindowBg ].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    LOG_DEBUG( "setting up imgui opengl3 renderer" );
    ImGui_ImplGlfw_InitForOpenGL( window, true );
    ImGui_ImplOpenGL3_Init( GLSL_VERSION.c_str() );
  }


  // call order: AFTER other imgui windows
  void shutdown( ApplicationContext& appCtx, GLFWwindow* window ) override
  {
    LOG_DEBUG( "shutting down imgui context" );

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
  }

  // call order: BEFORE other imgui windows
  void update( ApplicationContext& appCtx, GLFWwindow* window, const nxTimePoint frameDeltaInMS ) override
  {
    // Start the Dear ImGui frame before any updates are made
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
  }

  // call order: AFTER other imgui windows
  void draw( ApplicationContext& appCtx, GLFWwindow* window ) override
  {
    // render this at the end of update but before draw
    ImGui::Render();

    // imgui children have no purpose for drawing

    ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData() );

    // Update and Render additional Platform Windows
    // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
    //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
    if ( m_pIO->ConfigFlags & ImGuiConfigFlags_ViewportsEnable )
    {
      GLFWwindow* backup_current_context = glfwGetCurrentContext();
      ImGui::UpdatePlatformWindows();
      ImGui::RenderPlatformWindowsDefault();
      glfwMakeContextCurrent( backup_current_context );
    }
  }

private:

  static inline std::string GLSL_VERSION { "#version 330" };

  ImGuiIO * m_pIO { nullptr };

};
}

#endif //INC_0A08FB8480A34ABE9105EB8C918D3439
