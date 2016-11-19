#include "TestVulkan.h"


#include <SDL.h>
#include <SDL_video.h>
#include <SDL_syswm.h>


#define APP_CLASS Vulkan_test

#include <vulkan/vulkan.h>
#include <vulkan/vk_platform.h>
#include <vulkan/vk_sdk_platform.h>

#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

template<typename Ty> 
inline void DontCare (const Ty& lol) {
   (void) lol; 

   return (void); 
}

inline int wtf () {
   int i = 0; i++; return i; 
   }

bool memory_type_from_properties (
   VkPhysicalDeviceMemoryProperties& memory_props, 
   uint32_t             typeBits,
   VkFlags              requirements_mask,
   uint32_t*            typeIndex ) 
{
    // Search memtypes to find first index with those properties
    for (uint32_t i = 0; i < memory_props.memoryTypeCount; i++) 
    {
        if ((typeBits & 1) == 1) 
        {
            // Type is available, does it match user properties?
            if ((memory_props.memoryTypes[i].propertyFlags & requirements_mask) == requirements_mask) 
            {
                *typeIndex = i;
                return true;
            }
        }
        typeBits >>= 1;
    }
    // No memory types matched, return failure
    return false;
}

// 
//
// FUNC Update_view_transform 

void Update_view_transform (
	glm::dvec3&                view_Pos, 
	glm::dvec3&                view_Rot, 
	double                     move_Rate,
	const sy::Keyboard_state&  kb,    
	const sy::Mouse_state&     ms)
{

   const double kDeg2Pi = Ma::Pi / 180.0f; 
   const double fHalfPi = Ma::HalfPi;

   static float dYdx = 0.75f; 
   static float dXdy = 0.75f; 
   
   Ma::Vec3d dir_Fwd, dir_Right, dir_Up; 

   Ma::Set (dir_Up, 0.0, 1.0, 0.0); 

   {
      if (ms.yRel || ms.yRel)
      {
         wtf (); 
      }

      view_Rot[0] -= kDeg2Pi * (ms.yRel * dYdx);
      view_Rot[1] -= kDeg2Pi * (ms.xRel * dXdy);
      view_Rot.z = 0.0; 

      Ma::Vec3f v_t;
      // (\ spherical.theta(0).phi(0)) => <1, 0, 0>
      Ma::Spherical (v_t, 0.0f, 0.0f);
      Ma::Spherical (dir_Fwd,    kDeg2Pi * view_Rot[1] - Ma::HalfPi, 0.0);
      Ma::Spherical (dir_Right,  kDeg2Pi * view_Rot[1], 0.0); 
      Ma::X (dir_Right);
   }
   
   
   
   // view movement
   {
      const glm::dvec3 kX_axis (1.0, 0.0, 0.0); 
      const glm::dvec3 kY_axis (0.0, 1.0, 0.0); 
      const glm::dvec3 kZ_axis (0.0, 0.0, 1.0); 


      glm::dvec3 dirForward  = glm::rotateY (kZ_axis, view_Rot[1] ); 
      glm::dvec3 dirRight    = glm::rotateY (kX_axis, view_Rot[1] ); 
          
      Ma::Vec3d v  ;

      //
      if (sy::Is_keydown (sy::SC_F, kb)) 
         {
         view_Pos -= move_Rate * dirForward; 
   	   }
      else 
      if (sy::Is_keydown (sy::SC_V, kb)) 
         {
         view_Pos += move_Rate * dirForward; 
   	   }
   
      //
      if (sy::Is_keydown (sy::SC_D, kb)) {
         view_Pos -= move_Rate * dirRight; 
   	   }
      else 
      if (sy::Is_keydown (sy::SC_G, kb)) {
         view_Pos += move_Rate * dirRight; 
   	   }

      //
      if (sy::Is_keydown (sy::SC_A, kb)) {
         view_Pos.y += move_Rate; 
   	   }
      else
      if (sy::Is_keydown (sy::SC_Z, kb)) {
         view_Pos.y -= move_Rate; 
   	   }


   }   

}



class APP_CLASS 
   : public sy::RT_Task
   , public sy::Window_listener
   , public cx::Destructor
{
private: 

   // 
   std::shared_ptr<sy::Graphics_window>   windo; 
   bool                                   init_; 

   void init_vulkan     (sy::System_context* sys);
   void init_rendering  (); 
   void init_scene      (); 

   void setup_geometry_pass   (); 
   void draw_geometry_pass    ();    
   void setup_lighting_pass   (); 
   void draw_lighting_pass    ();    


   void update_input (sy::System_context* sys); 
   void render       (sy::System_context* sys);

public: 

   Vulkan_test (sy::System_context* sys);
   virtual ~Vulkan_test ();

   // app loop 
   virtual int Initialize	      (sy::System_context*);  
   virtual int Deinitialize      (sy::System_context*);  
   virtual int Update	         (sy::System_context*);  
   
   // window listener
   virtual void OnWindowResize   (int wd, int ht);   
   virtual void OnWindowClose    ();
   virtual void OnWindowActivate (bool activate);

   struct {
      int x;
      int y;
   } dim;

   struct {
   VkInstance                       instance; 
   } vulkan ;


   struct {
      VkSwapchainKHR                   handle;
      std::vector<VkImage>             images ; 
      std::vector<VkImageView>         imageviews ; 
      } swapchain;

   struct {
      VkImage                          handle; 
      VkImageView                      view; 
      VkDeviceMemory                   mem; 
      } depthbuffer; 
   struct  {
   VkSurfaceKHR                     handle;
   } surface; 
   VkCommandBuffer                  cmdbuf      ;
   VkCommandPool                    cmdpool     ; 

   //struct {
   //   VkDescriptorPool pool; 
   //   } descriptor;    

   struct {
      struct {
         VkDescriptorSetLayout vert;
         VkDescriptorSetLayout frag;
         VkDescriptorSetLayout teva;
         VkDescriptorSetLayout tctr;
         } layout ; 

         VkDescriptorPool pool; 

      } descriptor;
      

   struct {
      VkPipeline                 handle;                      
      VkPipelineLayout           layout; 
      } pipeline; 

   VkBuffer                         vertexbuf   ; 
   VkBuffer                         indexbuf    ; 
   std::vector<VkFramebuffer>       framebuffers;
   VkShaderModule                   vertshader  ;
   VkShaderModule                   fragshader  ;
   VkImage                          texture     ;

   struct {
      VkBuffer       buf; 
      VkDeviceMemory mem;
      } uniform;

   struct {
      VkPhysicalDevice                 phys;
      VkDevice                         logi;  
      VkPhysicalDeviceMemoryProperties memprops; 
      } device; 

   struct {
      VkQueue                       graphc; 
      VkQueue                       comput; 
      } queue;

   struct Scene_data  {
      struct { 
         glm::fmat4 model; 
         glm::fmat4 view; 
         glm::fmat4 proj; 
         } mat; 
   } scene; 
};


void APP_CLASS::init_vulkan (sy::System_context* sys)
{
   dim.x = sy::Graphics_window::kDef_windowed_width ; 
   dim.y = sy::Graphics_window::kDef_windowed_height; 

   windo.reset (sys->Create_GraphicsWindow (this, "lol volkin", dim.x, dim.y, 0x0)); 
   windo->Show (true); 


   VkResult result = VkResult (-1);

   device.logi = VK_NULL_HANDLE; 
   device.phys = VK_NULL_HANDLE; 

   VkApplicationInfo appinfo = {
      VK_STRUCTURE_TYPE_APPLICATION_INFO, 
      nullptr,
      "TestVulkan",
      1,
      "void",
      1,
      VK_API_VERSION_1_0
      }; 


   std::vector<const char*>            inst_ext_names;
   std::vector<VkExtensionProperties>  instext_props ; 
   {
      uint32_t instext_count = 0;
      result = vkEnumerateInstanceExtensionProperties (
         nullptr, 
         &instext_count, 
         nullptr
         );
      assert (result == VK_SUCCESS);

      instext_props . resize (instext_count);
      result = vkEnumerateInstanceExtensionProperties (
         nullptr, 
         &instext_count, 
         &instext_props[0]
         );

      assert (result == VK_SUCCESS);
      
      inst_ext_names.resize (instext_count); 
      for (int i = 0; i < instext_count; i++)
         inst_ext_names[i] = instext_props[i].extensionName; 
   }      

   VkInstanceCreateInfo instinfo = {
      VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO, //    VkStructureType             sType; 
      nullptr,                                //    const void*                 pNext;
      0x0,                                    //    VkInstanceCreateFlags       flags;
      &appinfo,                               //    const VkApplicationInfo*    pApplicationInfo;
      0,                                      //    uint32_t                    enabledLayerCount;
      0,                                      //    const char* const*          ppEnabledLayerNames;
      inst_ext_names.size(),                  //    uint32_t                    enabledExtensionCount;
      &inst_ext_names[0]                      //    const char* const*          ppEnabledExtensionNames
      }; 


   // create instance
   result = vkCreateInstance (
      &instinfo, 
      0, 
      &vulkan.instance
      ); 
   assert (result == VK_SUCCESS); 

   if (vulkan.instance)
   {
      VkDeviceQueueCreateInfo    queueinfo   = { VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO }; 
      VkDeviceCreateInfo         devinfo     = { VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO };

      uint32_t numdevs = 0; 

      // enumerate device
      VkResult enum_res = vkEnumeratePhysicalDevices (
         vulkan.instance, 
         &numdevs, 
         0
         ); 

      assert (VK_SUCCESS == enum_res); 
      assert (numdevs > 0); 


      // look for qualified device 
      std::vector<VkPhysicalDevice> physicaldevices (numdevs);
      vkEnumeratePhysicalDevices (vulkan.instance, &numdevs, &physicaldevices[0]); 

      int queue_family_ind = -1;

      for (int iphydev = 0; iphydev < numdevs; iphydev++)
      {
         // dev props, should we save these?
         VkPhysicalDeviceProperties devprops = {};

         vkGetPhysicalDeviceProperties (
            physicaldevices[iphydev], 
            &devprops
            ); 

         // enumerate queue props
         uint32_t propcount = 0;
         vkGetPhysicalDeviceQueueFamilyProperties (
            physicaldevices[iphydev], 
            &propcount, 
            nullptr
            ); 
         if (propcount == 0)
            continue; 

         std::vector<VkQueueFamilyProperties> queueprops (propcount); 
         vkGetPhysicalDeviceQueueFamilyProperties (
            physicaldevices[iphydev], 
            &propcount, 
            &queueprops[0]
            ); 

         // inspect props flags 
         for (size_t iq = 0; iq < queueprops.size (); iq++)
         {
            // want device that supports..
            bool req_queues = queueprops[iq].queueFlags & VK_QUEUE_GRAPHICS_BIT       
                           && queueprops[iq].queueFlags & VK_QUEUE_COMPUTE_BIT        
                           && queueprops[iq].queueFlags & VK_QUEUE_TRANSFER_BIT       
                           && queueprops[iq].queueFlags & VK_QUEUE_SPARSE_BINDING_BIT; 

            if (queueprops[iq].queueCount > 0 && req_queues) 
            {
               assert (queue_family_ind == -1); 
               queue_family_ind     = iq; 
               device.phys          = physicaldevices[iphydev];
               break;
            }
         }
      }

      assert (queue_family_ind  != -1);

      std::vector<const char*>            dev_ext_names; 
      std::vector<VkExtensionProperties>  extprops;
      VkPhysicalDeviceFeatures            physfeat = {}; 
      {
         uint32_t extpropcount;
         char     layername[256] = { 0 }; 

         VkResult result = vkEnumerateDeviceExtensionProperties (
            device.phys, 
            VK_NULL_HANDLE, 
            &extpropcount, 
            VK_NULL_HANDLE
            ); 

         extprops.resize (extpropcount);
         result = vkEnumerateDeviceExtensionProperties (
            device.phys, 
            VK_NULL_HANDLE, 
            &extpropcount, 
            &extprops[0]
            ); 

         // Device Features
         vkGetPhysicalDeviceFeatures (
            device.phys, 
            &physfeat
            ); 

         const float queue_priorities[] = { 1.0f, 0.9, 0.8, 0.7, 0.6, 0.5, 0.4, 0.3, 0.2, 0.1 }; 
         queueinfo.queueFamilyIndex = queue_family_ind;
         queueinfo.queueCount       = 1;
         queueinfo.pQueuePriorities = queue_priorities; 

         dev_ext_names . resize (extpropcount);  
         for (int i = 0; i < extprops.size (); i++)
            dev_ext_names  [i] = extprops[i].extensionName; 
      }

      devinfo.queueCreateInfoCount     = 1; 
      devinfo.pQueueCreateInfos        = &queueinfo; //VkDeviceQueueCreateInfo
      devinfo.pEnabledFeatures         = &physfeat;
      devinfo.enabledExtensionCount    = dev_ext_names.size(); 
      devinfo.ppEnabledExtensionNames  = &dev_ext_names [0]; 


      VkResult createres = vkCreateDevice (
         device.phys, 
         &devinfo, 
         VK_NULL_HANDLE, 
         &device.logi
         ); 

      assert (VK_SUCCESS == createres);

      // mem properties
      vkGetPhysicalDeviceMemoryProperties (
         device.phys, 
         &device.memprops
         );      

      vkGetDeviceQueue (
         device.logi, 
         queue_family_ind, 
         0, 
         &queue.graphc
         ); 

      surface.handle    = VK_NULL_HANDLE;
      swapchain.handle  = VK_NULL_HANDLE;

#if defined(VK_USE_PLATFORM_WIN32_KHR)
      
      VkBool32 supported = VK_FALSE; 

      VkWin32SurfaceCreateInfoKHR surfcreateinfo = { VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR};

      ::SDL_SysWMinfo info = {};
      ::SDL_Window* sdlwin = (SDL_Window*) windo->Get_WindowID ();

      SDL_version ver; 
      SDL_VERSION (&ver);
      info.version = ver; 
      ::SDL_GetWindowWMInfo (sdlwin, &info); 

      LONG_PTR hinst = GetWindowLongPtr (
         info.info.win.window, 
         GWLP_HINSTANCE
         ); 

      surfcreateinfo.pNext       = VK_NULL_HANDLE;
      surfcreateinfo.flags       = 0x0;
      surfcreateinfo.hinstance   = (HINSTANCE) hinst ;
      surfcreateinfo.hwnd        = info.info.win.window;

      assert (surfcreateinfo.hinstance); 
      assert (surfcreateinfo.hwnd); 
      
      result = vkCreateWin32SurfaceKHR (
         vulkan.instance, 
         &surfcreateinfo,
         VK_NULL_HANDLE, 
         &surface.handle
         );
      assert (result == VK_SUCCESS); 
      assert (surface.handle); 

      result = vkGetPhysicalDeviceSurfaceSupportKHR (
         device.phys,
         queue_family_ind,
         surface.handle,
         &supported 
         ); 
      assert (result == VK_SUCCESS); 
      assert (supported == VK_TRUE); 

      VkSurfaceCapabilitiesKHR surfcaps; 
      result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR ( 
         device.phys,
         surface.handle,
         &surfcaps
         ); 

      uint32_t fmtcount = 0;
      result = vkGetPhysicalDeviceSurfaceFormatsKHR (
         device.phys,
         surface.handle, 
         &fmtcount ,
         VK_NULL_HANDLE
         ); 
      assert (result == VK_SUCCESS); 

      std::vector<VkSurfaceFormatKHR> surfmt (fmtcount); 
      result = vkGetPhysicalDeviceSurfaceFormatsKHR (
         device.phys,
         surface.handle, 
         &fmtcount ,
         &surfmt[0]
         ); 
      assert (result == VK_SUCCESS); 

      int selected_surface_fmt = -1; 
      for (int ifmt = 0; ifmt < fmtcount; ifmt++)
      {
         if (surfmt[ifmt].format == VK_FORMAT_B8G8R8A8_UNORM && surfmt[ifmt].colorSpace == VK_COLORSPACE_SRGB_NONLINEAR_KHR)
         {
            selected_surface_fmt = ifmt; 
            break;
         }
      }

      assert (selected_surface_fmt != -1); 

      // create swap chain
      VkExtent2D                 imgext         = {dim.x, dim.y}; 
      VkSwapchainCreateInfoKHR   swapchain_ci   = { VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR } ; 

      swapchain_ci.pNext                  = VK_NULL_HANDLE; 
      swapchain_ci.flags                  = 0x0;
      swapchain_ci.surface                = surface.handle; 
      swapchain_ci.minImageCount          = 2; 
      swapchain_ci.imageFormat            = surfmt[selected_surface_fmt].format;
      swapchain_ci.imageColorSpace        = surfmt[selected_surface_fmt].colorSpace;  
      swapchain_ci.imageExtent            = imgext;  
      swapchain_ci.imageArrayLayers       = 1;
      swapchain_ci.imageUsage             = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
      swapchain_ci.imageSharingMode       = VK_SHARING_MODE_EXCLUSIVE;
      swapchain_ci.queueFamilyIndexCount  = 0;
      swapchain_ci.pQueueFamilyIndices    = VK_NULL_HANDLE;
      swapchain_ci.preTransform           = surfcaps.currentTransform;
      swapchain_ci.compositeAlpha         = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
      swapchain_ci.presentMode            = VK_PRESENT_MODE_FIFO_KHR;
      swapchain_ci.clipped                = VK_TRUE; 
      swapchain_ci.oldSwapchain           = VK_NULL_HANDLE;

      result = vkCreateSwapchainKHR (
         device.logi,
         &swapchain_ci,
         VK_NULL_HANDLE, 
         &swapchain.handle
         ); 

      assert (result == VK_SUCCESS); 

      uint32_t swapchain_imagecount = 0;
      vkGetSwapchainImagesKHR (
         device.logi, 
         swapchain.handle,
         &swapchain_imagecount, 
         VK_NULL_HANDLE 
         );

      swapchain.images.resize (swapchain_imagecount); 
      vkGetSwapchainImagesKHR (
         device.logi, 
         swapchain.handle,
         &swapchain_imagecount, 
         &swapchain.images[0]
         );

      swapchain.imageviews.resize (swapchain_imagecount); 
      for (uint32_t i = 0; i < swapchain_imagecount; i++) 
      {
         VkImageViewCreateInfo color_image_view = {VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
         color_image_view.pNext                          = VK_NULL_HANDLE;
         color_image_view.flags                          = 0x0;
         color_image_view.image                          = swapchain.images[i];
         color_image_view.viewType                       = VK_IMAGE_VIEW_TYPE_2D;
         color_image_view.format                         = surfmt[selected_surface_fmt].format;
         color_image_view.components.r                   = VK_COMPONENT_SWIZZLE_R;
         color_image_view.components.g                   = VK_COMPONENT_SWIZZLE_G;
         color_image_view.components.b                   = VK_COMPONENT_SWIZZLE_B;
         color_image_view.components.a                   = VK_COMPONENT_SWIZZLE_A;
         color_image_view.subresourceRange.aspectMask    = VK_IMAGE_ASPECT_COLOR_BIT;
         color_image_view.subresourceRange.baseMipLevel  = 0;
         color_image_view.subresourceRange.levelCount    = 1;
         color_image_view.subresourceRange.baseArrayLayer= 0;
         color_image_view.subresourceRange.layerCount    = 1;

         result = vkCreateImageView (
            device.logi, 
            &color_image_view, 
            VK_NULL_HANDLE,
            &swapchain.imageviews[i]
            );

         assert (result == VK_SUCCESS);
       }


#endif
      // depth buffer
      {
         VkExtent3D        imgext        = {dim.x, dim.y, 1}; 

         VkImageCreateInfo depthbuf_info = { 
            VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
            VK_NULL_HANDLE             ,
            0x0                        ,
            VK_IMAGE_TYPE_2D           ,
            VK_FORMAT_D16_UNORM        ,
            imgext                     ,
            1                          ,
            1                          ,
            VK_SAMPLE_COUNT_1_BIT      ,
            VK_IMAGE_TILING_OPTIMAL    ,
            VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
            VK_SHARING_MODE_EXCLUSIVE  ,
            0                          ,
            VK_NULL_HANDLE             ,
            VK_IMAGE_LAYOUT_UNDEFINED  ,
            }; 

         result = vkCreateImage (
            device.logi, 
            &depthbuf_info, 
            VK_NULL_HANDLE, 
            &depthbuffer.handle
            );

         assert (result == VK_SUCCESS);

         VkMemoryRequirements memreq = {};
         vkGetImageMemoryRequirements (
            device.logi, 
            depthbuffer.handle, 
            &memreq
            );


         VkMemoryAllocateInfo allocinfo = {
            VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
            VK_NULL_HANDLE,
            memreq.size, 
            };

         bool pass = memory_type_from_properties (
            device.memprops,                    //
            memreq.memoryTypeBits,       //
            0x0,                         // No Requirements 
            &allocinfo.memoryTypeIndex   //
            );
         assert (pass);

         result = vkAllocateMemory (
            device.logi, 
            &allocinfo, 
            VK_NULL_HANDLE, 
            &depthbuffer.mem
            );
         assert (result == VK_SUCCESS);

         vkBindImageMemory (
            device.logi, 
            depthbuffer.handle, 
            depthbuffer.mem, 
            0
            ); 


         VkImageViewCreateInfo depth_view_info = { VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO }; 
         depth_view_info.pNext                           = VK_NULL_HANDLE;  
         depth_view_info.image                           = depthbuffer.handle; 
         depth_view_info.format                          = VK_FORMAT_D16_UNORM;
         depth_view_info.components.r                    = VK_COMPONENT_SWIZZLE_R;
         depth_view_info.components.g                    = VK_COMPONENT_SWIZZLE_G;
         depth_view_info.components.b                    = VK_COMPONENT_SWIZZLE_B;
         depth_view_info.components.a                    = VK_COMPONENT_SWIZZLE_A;
         depth_view_info.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_DEPTH_BIT; 
         depth_view_info.subresourceRange.baseMipLevel   = 0;
         depth_view_info.subresourceRange.levelCount     = 1;
         depth_view_info.subresourceRange.baseArrayLayer = 0;
         depth_view_info.subresourceRange.layerCount     = 1;
         depth_view_info.viewType                        = VK_IMAGE_VIEW_TYPE_2D;
         depth_view_info.flags                           = 0;

         result = vkCreateImageView (
            device.logi, 
            &depth_view_info, 
            VK_NULL_HANDLE,
            &depthbuffer.view
            );         

         assert (result == VK_SUCCESS);
      }

      VkPipelineCache                  pipelinecache; 
      VkPipelineShaderStageCreateFlags _wat;


      cmdbuf = VK_NULL_HANDLE; 
      {
         VkCommandPoolCreateInfo cmdpool_create = { 
            VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,  //VkStructureType             sType;
            VK_NULL_HANDLE,                              // const void*                 pNext;
            0x0,                                         // VkCommandPoolCreateFlags    flags;
            queue_family_ind                          // uint32_t                    queueFamilyIndex;
            }; 

         result = vkCreateCommandPool (
            device.logi, 
            &cmdpool_create, 
            VK_NULL_HANDLE, 
            &cmdpool
            );
         
         assert (result == VK_SUCCESS);

         VkCommandBufferAllocateInfo cmdbuf_create = {
            VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,// VkStructureType         sType;
            VK_NULL_HANDLE,                  // const void*             pNext;
            cmdpool,                         // VkCommandPool           commandPool;
            VK_COMMAND_BUFFER_LEVEL_PRIMARY, // VkCommandBufferLevel    level;
            1                                // uint32_t                commandBufferCount;
            };

         result = vkAllocateCommandBuffers (
            device.logi, 
            &cmdbuf_create, 
            &cmdbuf
            ); 
         assert (result == VK_SUCCESS);

      }
      assert (cmdpool     != VK_NULL_HANDLE); 
      assert (cmdbuf      != VK_NULL_HANDLE); 


      uniform.buf = VK_NULL_HANDLE; 
      uniform.mem = VK_NULL_HANDLE;
      const VkDeviceSize   uniform_mem_req_size = 1024 * 1024;
      {
         VkBufferCreateInfo uniform_create = {
            VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,  // VkStructureType        sType;
            VK_NULL_HANDLE,                        // const void*            pNext;
            0x0,                                   // VkBufferCreateFlags    flags;
            sizeof Scene_data,                      // VkDeviceSize           size;
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,    // VkBufferUsageFlags     usage;
            VK_SHARING_MODE_EXCLUSIVE,             // VkSharingMode          sharingMode;
            0,                                     // uint32_t               queueFamilyIndexCount;
            VK_NULL_HANDLE                         // const uint32_t*        pQueueFamilyIndices;
            }; 

         result = vkCreateBuffer (
            device.logi, 
            &uniform_create, 
            VK_NULL_HANDLE, 
            &uniform.buf
            ); 

         assert (result == VK_SUCCESS);

         VkMemoryRequirements memreq;
         vkGetBufferMemoryRequirements (
            device.logi, 
            uniform.buf,
            &memreq
            );

         VkMemoryAllocateInfo allocinfo = {
            VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO  , // VkStructureType    sType;
            VK_NULL_HANDLE                          , // const void*        pNext;
            memreq.size                             , // VkDeviceSize       allocationSize;
            0                                         // uint32_t           memoryTypeIndex;
            };

         bool success = memory_type_from_properties (
            device.memprops, 
            memreq.memoryTypeBits,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            &allocinfo.memoryTypeIndex
            );

         assert (success);

         result = vkAllocateMemory (
            device.logi, 
            &allocinfo, 
            VK_NULL_HANDLE,
            &uniform.mem
            );

         assert (result == VK_SUCCESS);

      }
      assert (uniform.buf   != VK_NULL_HANDLE); 
      assert (uniform.mem   != VK_NULL_HANDLE); 


      // descriptor pool
      descriptor.pool         = VK_NULL_HANDLE; 
      {
      }

      // vert shader layout descriptor
      descriptor.layout.vert  = VK_NULL_HANDLE; 
      {
         VkDescriptorSetLayoutBinding vs_bind_desc[] = 
         {
            // vertex   
            {
               0,                                  // location
               VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,  // type
               1,                                  // count
               VK_SHADER_STAGE_VERTEX_BIT,         // flags;
               VK_NULL_HANDLE                      // samplers 
            }, 
         };
         


         VkDescriptorSetLayoutCreateInfo vert_descriptor_layout = {
            VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
            VK_NULL_HANDLE,
            0x0,
            El_count (vs_bind_desc),
            &vs_bind_desc[0]
            };

         result = vkCreateDescriptorSetLayout(
            device.logi,
            &vert_descriptor_layout, 
            VK_NULL_HANDLE,
            &descriptor.layout.vert
            );

         assert (result == VK_SUCCESS );
      }
      assert (descriptor.layout.vert); 

      descriptor.layout.frag  = VK_NULL_HANDLE; 
      {
      }
      //assert (descriptor.layout.frag); 

      descriptor.layout.teva  = VK_NULL_HANDLE; 
      {
      }
      //assert (descriptor.layout.teva); 
      descriptor.layout.tctr  = VK_NULL_HANDLE; 
      {
      }
      //assert (descriptor.layout.tctr); 


      pipeline.handle = VK_NULL_HANDLE; 
      {
         VkPipelineLayoutCreateFlags flags = 0x0;
         VkPipelineLayoutCreateInfo pPipelineLayoutCreateInfo = {
             VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO, // VkStructureType                 sType;
             VK_NULL_HANDLE,                 // const void*                     pNext;
             flags,                          // VkPipelineLayoutCreateFlags     flags;
             1,                              // uint32_t                        setLayoutCount;
             &descriptor.layout.vert,        // const VkDescriptorSetLayout*    pSetLayouts;
             1,                              // uint32_t                        pushConstantRangeCount;
             VK_NULL_HANDLE                  // const VkPushConstantRange*      pPushConstantRanges;
            };

         result = vkCreatePipelineLayout (
            device.logi, 
            &pPipelineLayoutCreateInfo, 
            VK_NULL_HANDLE,
            
            &pipeline.layout
            );      

         assert (result == VK_SUCCESS);
         assert (pipeline.layout != VK_NULL_HANDLE); 

      }

      VkRenderPass renderpass = VK_NULL_HANDLE;
      {
            //std::vector<VkAttachmentDescription> 
         VkFormat fmt = VK_FORMAT_R8G8B8A8_UNORM; 
         VkAttachmentDescription attachment_desc[] = {
            // color 
            {
                0x0                                      , // VkAttachmentDescriptionFlags    flags;
                fmt                                      , // VkFormat                        format;
                VK_SAMPLE_COUNT_1_BIT                    , // VkSampleCountFlagBits           samples;
                VK_ATTACHMENT_LOAD_OP_CLEAR              , // VkAttachmentLoadOp              loadOp;
                VK_ATTACHMENT_STORE_OP_STORE             , // VkAttachmentStoreOp             storeOp;
                VK_ATTACHMENT_LOAD_OP_DONT_CARE          , // VkAttachmentLoadOp              stencilLoadOp;
                VK_ATTACHMENT_STORE_OP_DONT_CARE         , // VkAttachmentStoreOp             stencilStoreOp;
                VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL , // VkImageLayout                   initialLayout;
                VK_IMAGE_LAYOUT_PRESENT_SRC_KHR            // VkImageLayout                   finalLayout;
            }, 
            // depth 
            {
               0x0                                             , // VkAttachmentDescriptionFlags    flags;
               fmt                                             , // VkFormat                        format;
               VK_SAMPLE_COUNT_1_BIT                           , // VkSampleCountFlagBits           samples;
               VK_ATTACHMENT_LOAD_OP_CLEAR                     , // VkAttachmentLoadOp              loadOp;
               VK_ATTACHMENT_STORE_OP_DONT_CARE                , // VkAttachmentStoreOp             storeOp;
               VK_ATTACHMENT_LOAD_OP_DONT_CARE                 , // VkAttachmentLoadOp              stencilLoadOp;
               VK_ATTACHMENT_STORE_OP_DONT_CARE                , // VkAttachmentStoreOp             stencilStoreOp;
               VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, // VkImageLayout                   initialLayout;
               VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL  // VkImageLayout                   finalLayout;
            }, 
         };


         VkAttachmentReference color_reference = {};
         color_reference.attachment = 0;
         color_reference.layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
         
         VkAttachmentReference depth_reference = {};
         depth_reference.attachment = 1;
         depth_reference.layout     = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

//
         VkSubpassDescription subpass_desc = {
            0x0                              , // VkSubpassDescriptionFlags       flags;
            VK_PIPELINE_BIND_POINT_GRAPHICS  , // VkPipelineBindPoint             pipelineBindPoint;
            0                                , // uint32_t                        inputAttachmentCount;
            VK_NULL_HANDLE                   , // const VkAttachmentReference*    pInputAttachments;
            1                                , // uint32_t                        colorAttachmentCount;
            &color_reference                 , // const VkAttachmentReference*    pColorAttachments;
            VK_NULL_HANDLE                   , // const VkAttachmentReference*    pResolveAttachments;
            &depth_reference                 , // const VkAttachmentReference*    pDepthStencilAttachment;
            0                                , // uint32_t                        preserveAttachmentCount;
            VK_NULL_HANDLE                     // const uint32_t*                 pPreserveAttachments;
            };


         VkRenderPassCreateFlags flags = 0x0; 
         VkRenderPassCreateInfo ci = {
            VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO , // VkStructureType                   sType;
            VK_NULL_HANDLE                            , // const void*                       pNext;
            flags                                     , // VkRenderPassCreateFlags           flags;
            El_count(attachment_desc)                 , // uint32_t                          attachmentCount;
            attachment_desc                           , // const VkAttachmentDescription*    pAttachments;
            1                                         , // uint32_t                          subpassCount;
            &subpass_desc                             , // const VkSubpassDescription*       pSubpasses;
            0                                         , // uint32_t                          dependencyCount;
            VK_NULL_HANDLE                            , // const VkSubpassDependency*        pDependencies;
            };


         result = vkCreateRenderPass (
            device.logi,
            &ci,
            VK_NULL_HANDLE, 
            &renderpass
            ); 
         assert (result == VK_SUCCESS);
      }
      assert (renderpass != VK_NULL_HANDLE); 
      
      struct 
      {} shader; 

      // shader 
      {}

      // framebuffer
      framebuffers.resize (swapchain.images.size(), VK_NULL_HANDLE); 
      {
         VkFramebufferCreateFlags flags = 0x0; 

         VkFramebufferCreateInfo ci = {
            VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO, // VkStructureType             sType;
            VK_NULL_HANDLE , // const void*                 pNext;
            flags          , // VkFramebufferCreateFlags    flags;
            renderpass     , // VkRenderPass                renderPass;
            2              , // uint32_t                    attachmentCount;
            VK_NULL_HANDLE , // const VkImageView*          pAttachments;
            dim.x          , // uint32_t                    width;
            dim.y          , // uint32_t                    height;
            1              , // uint32_t                    layers;
            }; 

         for (int iframe = 0; iframe < swapchain.images.size(); iframe++) {

            VkImageView attachments[2] = {
               swapchain.imageviews[iframe],    //
               depthbuffer.view                 //
               }; 

            ci.pAttachments = attachments; 

            result = vkCreateFramebuffer (
               device.logi, 
               &ci,
               VK_NULL_HANDLE, 
               &framebuffers[iframe]
               ); 

            assert (result == VK_SUCCESS); 
            }
      }

      for (int i = 0; i < framebuffers.size(); ++i)
         assert (framebuffers[i] != VK_NULL_HANDLE); 

      vertexbuf   = VK_NULL_HANDLE; 
      indexbuf    = VK_NULL_HANDLE; 
      vertshader  = VK_NULL_HANDLE;
      fragshader  = VK_NULL_HANDLE;
      texture     = VK_NULL_HANDLE;


      assert (descriptor.pool != VK_NULL_HANDLE); 
      assert (vertexbuf       != VK_NULL_HANDLE); 
      assert (indexbuf        != VK_NULL_HANDLE); 
      assert (vertshader      != VK_NULL_HANDLE); 
      assert (fragshader      != VK_NULL_HANDLE); 
      assert (texture         != VK_NULL_HANDLE); 

      //VkPipelineShaderStageCreateInfo 
   }
   else
   {
   wtf ();
   }


}

void APP_CLASS::init_rendering ()
{
   wtf ();
}

void APP_CLASS::init_scene ()
{
   wtf ();
} 

void APP_CLASS::setup_geometry_pass ()
{
   wtf ();
}

void APP_CLASS::draw_geometry_pass ()
{
   wtf ();
}    

void APP_CLASS::setup_lighting_pass ()
{
   wtf ();
}

void APP_CLASS::draw_lighting_pass ()
{
   wtf ();
}    

void APP_CLASS::update_input (sy::System_context* sys)
{
   wtf ();
}

void APP_CLASS::render (sy::System_context* sys)
{
   wtf ();
}


APP_CLASS::Vulkan_test (sy::System_context* sys)
{
   wtf ();
}
//
APP_CLASS::~Vulkan_test ()
{
   wtf ();
}

//
int APP_CLASS::Initialize (sy::System_context* sc) 
{ 
   init_vulkan (sc); 
   wtf ();
   return 0; 
}  

//
int APP_CLASS::Deinitialize (sy::System_context*) 
{ 

   if (vulkan.instance)
   {
      vkDestroyInstance (vulkan.instance, 0); 
   }
   wtf ();
   return 0; 
}   

//
int APP_CLASS::Update (sy::System_context*)
{ 

   wtf ();
   return 0; 
} 

// window listener
void APP_CLASS::OnWindowResize (int wd, int ht) 
{
   wtf ();

}

//
void APP_CLASS::OnWindowClose () 
{
   wtf ();
}

void APP_CLASS::OnWindowActivate (bool activate) 
{
   wtf ();
}


//
int main (int argc, char** argv)
{
   std::vector<std::string> args (argv, argv + argc); 

   std::shared_ptr<sy::System_context> sc    (sy::Create_context ()); 
   std::shared_ptr<sy::RT_Task>        app   (new APP_CLASS (sc.get())); 

   char modulename[256];

   return sy::Run_realtime_task (sc.get(), app.get());  

   return 0;
}
