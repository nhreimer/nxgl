#ifndef INC_487C689D3F044ED489E76BBAFF2E171F
#define INC_487C689D3F044ED489E76BBAFF2E171F

namespace nxgl::ui
{

/***
 * call this BEFORE other imgui windows to enable window docking
 */
struct ImguiPerformanceReceiver : public EventReceiver
{
  void update( ApplicationContext& appCtx,
               GLFWwindow* window,
               const nxTimePoint frameDeltaInMS ) override
  {
    static float f = 0.0f;
    static int counter = 0;

    ImGui::Begin( "Performance" );
    {
      ImGui::Text( "This is some useful text." );               // Display some text (you can use a format strings too)

      ImGui::SliderFloat( "float", &f, 0.0f, 1.0f );            // Edit 1 float using a slider from 0.0f to 1.0f
      //ImGui::ColorEdit3( "clear color", ( float* )&clearColor ); // Edit 3 floats representing a color

      if ( ImGui::Button( "Button" ) )                            // Buttons return true when clicked (most widgets return true when edited/activated)
        counter++;
      ImGui::SameLine();
      ImGui::Text( "counter = %d", counter );

      ImGui::Text( "Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate );
    }
    ImGui::End();
  }
};

}

#endif //INC_487C689D3F044ED489E76BBAFF2E171F
