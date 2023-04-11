#ifndef AED01F84CC4543D2B49267D9DC1D6102
#define AED01F84CC4543D2B49267D9DC1D6102

#if defined(_WIN32)
#include <Windows.h>
#endif

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <cstdint>

//#include "Logger.hpp"

// glfw3 must be included AFTER Logger.hpp or APIENTRY redefinitions occur
#include <KHR/khrplatform.h>
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include <glm/glm.hpp>

#include "Logger.hpp"

#endif //AED01F84CC4543D2B49267D9DC1D6102
