
//
#include "gfxlife.h"


// SDL2
#include <SDL.h>

#include <SDL_vulkan.h>

// Vulkan
#include <vulkan/vulkan.h>
#include <vulkan/vk_platform.h>
#include <vulkan/vk_sdk_platform.h>


#include <Windows.h>
#include <vulkan/vulkan_win32.h>



// GLM
#include <glm/glm.hpp>
#include <glm/ext.hpp>


// spux
#include <spux.h>


#ifndef VKX00_INCLUDE
#define VKX00_INCLUDE


   spux::Ref<spux::Runnable>::Type create_vkx00 (const std::vector<std::string>& args); 


#endif
