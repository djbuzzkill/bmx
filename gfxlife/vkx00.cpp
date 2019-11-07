
#include "vkx00.h"


using namespace spux; 

//
//
void wtf () { assert (false); }

//
//
bool memory_type_from_properties (
   const VkPhysicalDeviceMemoryProperties& memory_props, 
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


template<typename Ty>
struct rectangle 
{
   tuplet<Ty> pos;
   tuplet<Ty> dim;
};


namespace vkx
{
   struct Instance 
   {
      VkInstance        handle; 
      VkApplicationInfo appinfo; 
   };

   struct Surface
   {
      VkSurfaceKHR                     handle; 
      VkSurfaceCapabilitiesKHR         caps;
      std::vector<VkSurfaceFormatKHR>  formats; 
   };

   struct Swapchain 
   {
      VkSwapchainKHR                   handle;
      std::vector<VkImage>             images ; 
      std::vector<VkImageView>         imageviews ; 
   };

   struct DepthBuffer
   {
      VkImage                          handle; 
      VkImageView                      view; 
      VkDeviceMemory                   mem; 
   }; 

   struct PipelineLayout 
   {      
      VkPipelineLayout handle; 
   };

   struct Pipeline 
   {
      VkPipeline        handle;                      
      PipelineLayout    layout; 
   }; 




   struct Queue
   { 
      VkQueue handle; 
   };      

   struct CommandBuffer
   {
      VkCommandBuffer handle;
   };
   
   struct CommandPool 
   {
      VkCommandPool handle; 
   };


   struct Device
   {
      //
      struct 
      {
         VkPhysicalDevice                 handle;     // VkPhysicalDevice                 
         VkPhysicalDeviceMemoryProperties memprops;   // selected queue family b
         VkPhysicalDeviceFeatures         features ; 

      } physical;

      //
      struct 
      {
         VkDevice       handle;     // VkDevice                         
         Queue          queue;      // []Queue                            
         CommandBuffer  commandbuffer;
         CommandPool    commandpool;
      } logical;       

      std::vector<const char*>            extension_names; 
   };

   struct RenderPass 
   {
      VkRenderPass handle ;
   }; 

   struct Buffer 
   {
      VkBuffer handle;
   };
 
  struct DeviceMemory 
   {
      VkDeviceMemory handle;
   };

   struct DescriptorSetLayout
   {
      VkDescriptorSetLayout handle;
   };

   struct DescriptorSet 
   { 
      VkDescriptorSet handle;
   };

   struct DescriptorPool     
   { 
      VkDescriptorPool handle;
   };

   struct DescriptorGroup
   {
      enum { vert = 0, frag, teva, tctr, MaxCount };

      vkx::DescriptorSetLayout   layout ;
      vkx::DescriptorSet         set; 
   } ;

   //
   //
   VkDescriptorSetLayout create_descriptor_set_layout (
      VkDescriptorSetLayout*                             out, 
      const std::vector<VkDescriptorSetLayoutBinding>&   bindingarray,
      VkDevice                                           device); 



}

//
VkDescriptorSetLayout vkx::create_descriptor_set_layout (
   VkDescriptorSetLayout*                             out, 
   const std::vector<VkDescriptorSetLayoutBinding>&   bindingarray,
   VkDevice                                           device)
{

   VkDescriptorSetLayoutCreateFlags flags = 0x0;

   VkDescriptorSetLayoutCreateInfo ci = {
      VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,  // VkStructureType                        sType;
      VK_NULL_HANDLE,                                       // const void*                            pNext;
      flags,                                                // VkDescriptorSetLayoutCreateFlags       flags;
      (uint32_t) bindingarray.size(),                       // uint32_t                               bindingCount;
      std::data(bindingarray),                              // const VkDescriptorSetLayoutBinding*    pBindings;
      };

   VkResult res = vkCreateDescriptorSetLayout (
      device, 
      &ci,
      VK_NULL_HANDLE, 
      out
      );
   // vkCreateDescriptorSetLayout
   // vkDestroyDescriptorSetLayout
   assert(res == VK_SUCCESS);  
   return (res == VK_SUCCESS ? *out : nullptr);
}

//
//
struct VulkanState
{      
   rectangle<uint32_t>  window;

   struct 
   {
      vkx::Instance  instance; 
      vkx::Surface   surface;
   } vulkan;

   vkx::Swapchain       swapchain;
   vkx::DepthBuffer     depthbuffer; 
   vkx::Device          device; 
   vkx::Pipeline        pipeline;
   vkx::RenderPass      renderpass; 


   struct {
      uint32_t family_queue; 
      uint32_t surface_format;
   } selected_index;




   struct {

      vkx::DescriptorPool  pool; 

      // struct 
      // {
      //    vkx::DescriptorSetLayout vert;
      //    vkx::DescriptorSetLayout frag;
      //    vkx::DescriptorSetLayout teva;
      //    vkx::DescriptorSetLayout tctr;
      // } layout ; 
      std::array<vkx::DescriptorGroup, vkx::DescriptorGroup::MaxCount> grp;


      //vkx::DescriptorSet            set; 

      } descriptor;


   VkBuffer                         vertexbuf   ; 
   VkBuffer                         indexbuf    ; 
   std::vector<VkFramebuffer>       framebuffers;
   VkShaderModule                   vertshader  ;
   VkShaderModule                   fragshader  ;
   VkImage                          texture     ;

   struct {
      vkx::DeviceMemory devicemem;  
      vkx::Buffer       buffer;
      } uniform;

   struct {
      bool        initialized; 
      SDL_Window* window;
      } SDL;

   std::string apptitle; 
};


bool init_VK_instance         (Ref<System>::Type sys, VulkanState& glob);
bool init_VK_device           (Ref<System>::Type sys, VulkanState& glob);
bool init_VK_commandbuffer    (Ref<System>::Type sys, VulkanState& glob);
bool init_VK_swapchain        (Ref<System>::Type sys, VulkanState& glob);
bool init_VK_depthbuffer      (Ref<System>::Type sys, VulkanState& glob);
bool init_VK_uniformbuffer    (Ref<System>::Type sys, VulkanState& glob);


// DESCRIPTOR SET LAYOUT
bool init_VK_DSLO_vertex      (Ref<System>::Type sys, VulkanState& glob);
bool init_VK_DSLO_tesselator  (Ref<System>::Type sys, VulkanState& glob);
bool init_VK_DSLO_fragment    (Ref<System>::Type sys, VulkanState& glob);

bool init_VK_descriptor_pool  (Ref<System>::Type sys, VulkanState& glob);
bool init_VK_descriptors       (Ref<System>::Type sys, VulkanState& glob);
bool init_VK_pipeline_layout  (Ref<System>::Type sys, VulkanState& glob);

bool init_VK_renderpass       (Ref<System>::Type sys, VulkanState& glob);
bool init_VK_shaders          (Ref<System>::Type sys, VulkanState& glob);
bool init_VK_framebuffer      (Ref<System>::Type sys, VulkanState& glob);
bool init_VK_vertexbuffers    (Ref<System>::Type sys, VulkanState& glob);

bool init_display_surface     (Ref<System>::Type sys, VulkanState& glob);
bool init_SDL2_instance       (Ref<System>::Type sys, VulkanState& glob);
bool init_SDL2_window         (Ref<System>::Type sys, VulkanState& glob);


//
//
struct vkx00 : public Runnable, public WindowListener, public Destructor 
{
   vkx00 () 
   {
   }

   virtual ~vkx00 () 
   {}

   // Runnable
   virtual int Run           (Ref<System>::Type, double); 
	virtual int Initialize    (Ref<System>::Type)			; 
   virtual int Deinitialize  (Ref<System>::Type)        ; 

   // WindowListener
   virtual void OnWindowResize	(int wd, int ht); 
   virtual void OnWindowClose		(); 
   virtual void OnWindowActivate	(bool activate); 

   VulkanState glob;

}; 


int vkx00::Initialize (Ref<System>::Type sys)		    
{

   glob.apptitle = "vkx00";

   glob.SDL.window      = nullptr; 
   glob.SDL.initialized = false;

   glob.window.pos      = { 128, 128 };
   glob.window.dim      = { 800, 600 };

   glob.vulkan.instance.handle = VK_NULL_HANDLE;
   glob.vulkan.instance.appinfo = {
      VK_STRUCTURE_TYPE_APPLICATION_INFO, 
      nullptr,
      "vkx00",
      1,
      "void",
      1,
      VK_API_VERSION_1_0
      }; 

   glob.vulkan.surface.handle                = VK_NULL_HANDLE;
   // glob.vulkan.surface.caps
   // glob.vulkan.surface.formats

   glob.swapchain.handle                     = VK_NULL_HANDLE;

   glob.device.physical.handle               = VK_NULL_HANDLE;
   glob.device.physical.features             = {};
   glob.device.physical.memprops             = {};

   glob.device.logical.handle                = VK_NULL_HANDLE; 
   glob.device.logical.queue.handle          = VK_NULL_HANDLE; 
   glob.device.logical.commandbuffer.handle  = VK_NULL_HANDLE; 
   glob.device.logical.commandpool.handle    = VK_NULL_HANDLE; 

   glob.pipeline.handle                      = VK_NULL_HANDLE; 
   glob.pipeline.layout.handle               = VK_NULL_HANDLE; 

   glob.descriptor.pool.handle               = VK_NULL_HANDLE; 

   for (size_t i = 0; i < vkx::DescriptorGroup::MaxCount; ++i)
   {
      glob.descriptor.grp[i].set.handle        = VK_NULL_HANDLE; 
      glob.descriptor.grp[i].layout.handle     = VK_NULL_HANDLE; 
   }


   glob.selected_index.family_queue          = -1;
   glob.selected_index.surface_format        = -1;

   glob.uniform.buffer.handle    = VK_NULL_HANDLE; 
   glob.uniform.devicemem.handle = VK_NULL_HANDLE;
   glob.renderpass.handle        = VK_NULL_HANDLE;

   glob.vertexbuf                = VK_NULL_HANDLE; 
   glob.indexbuf                 = VK_NULL_HANDLE; 
   glob.vertshader               = VK_NULL_HANDLE;
   glob.fragshader               = VK_NULL_HANDLE;
   glob.texture                  = VK_NULL_HANDLE;

   //assert (glob.descriptor.pool != VK_NULL_HANDLE); 
   //assert (glob.vertexbuf       != VK_NULL_HANDLE); 
   //assert (glob.indexbuf        != VK_NULL_HANDLE); 
   //assert (glob.vertshader      != VK_NULL_HANDLE); 
   //assert (glob.fragshader      != VK_NULL_HANDLE); 
   //assert (glob.texture         != VK_NULL_HANDLE); 



   std::map<std::string, VkBool32> initres; 

   initres["init_VK_instance"]         = init_VK_instance         (sys, glob) ? VK_TRUE : VK_FALSE;
   initres["init_SDL2_instance"]       = init_SDL2_instance       (sys, glob) ? VK_TRUE : VK_FALSE;
   initres["init_SDL2_window"]         = init_SDL2_window         (sys, glob) ? VK_TRUE : VK_FALSE;
   initres["init_VK_device"]           = init_VK_device           (sys, glob) ? VK_TRUE : VK_FALSE; 
   initres["init_display_surface"]     = init_display_surface     (sys, glob) ? VK_TRUE : VK_FALSE;
   initres["init_VK_commandbuffer"]    = init_VK_commandbuffer    (sys, glob) ? VK_TRUE : VK_FALSE;
   initres["init_VK_swapchain"]        = init_VK_swapchain        (sys, glob) ? VK_TRUE : VK_FALSE; 
   initres["init_VK_depthbuffer"]      = init_VK_depthbuffer      (sys, glob) ? VK_TRUE : VK_FALSE; 
   initres["init_VK_uniformbuffer"]    = init_VK_uniformbuffer    (sys, glob) ? VK_TRUE : VK_FALSE;
   initres["init_VK_DSLO_vertex"]      = init_VK_DSLO_vertex      (sys, glob) ? VK_TRUE : VK_FALSE;
   initres["init_VK_DSLO_tesselator"]  = init_VK_DSLO_tesselator  (sys, glob) ? VK_TRUE : VK_FALSE;
   initres["init_VK_DSLO_fragment"]    = init_VK_DSLO_fragment    (sys, glob) ? VK_TRUE : VK_FALSE;
   initres["init_VK_descriptor_pool"]  = init_VK_descriptor_pool  (sys, glob) ? VK_TRUE : VK_FALSE;
   initres["init_VK_descriptors"]      = init_VK_descriptors      (sys, glob) ? VK_TRUE : VK_FALSE;
   initres["init_VK_pipeline_layout"]  = init_VK_pipeline_layout  (sys, glob) ? VK_TRUE : VK_FALSE;
   initres["init_VK_renderpass"]       = init_VK_renderpass       (sys, glob) ? VK_TRUE : VK_FALSE;
   initres["init_VK_shaders"]          = init_VK_shaders          (sys, glob) ? VK_TRUE : VK_FALSE;
   initres["init_VK_framebuffer"]      = init_VK_framebuffer      (sys, glob) ? VK_TRUE : VK_FALSE;
   initres["init_VK_vertexbuffers"]    = init_VK_vertexbuffers    (sys, glob) ? VK_TRUE : VK_FALSE;

   return 0; 


}
 
int vkx00::Run (Ref<System>::Type sys, double dt) 
{
   //int run (const std::vector<std::string>& args)
   //{
      return 0; 
   //}
} 

int vkx00::Deinitialize  (Ref<System>::Type sys) 
{
   return 0; 
} 

void vkx00::OnWindowResize	(int wd, int ht)
{}

void vkx00::OnWindowClose		()
{}

void vkx00::OnWindowActivate	(bool activate)
{}

   //
   //
bool init_SDL2_instance (Ref<System>::Type sys, VulkanState& glob)
{
   VkBool32 supported = VK_FALSE; 
   const Uint32 kGFXLIFE_SDL_INIT_FLAGS = SDL_INIT_TIMER          
                                       | SDL_INIT_AUDIO          
                                       | SDL_INIT_VIDEO          
                                       | SDL_INIT_EVENTS         ;

   glob.SDL.initialized = (SDL_Init (kGFXLIFE_SDL_INIT_FLAGS) == 0 ? true : false);


   assert(glob.SDL.initialized);
   return glob.SDL.initialized ;
}


//
//
bool init_SDL2_window (Ref<System>::Type sys, VulkanState& glob)
{
   if (glob.SDL.initialized)
   {
      uint32_t windowflags =  SDL_WINDOW_VULKAN;

      //
      glob.SDL.window = SDL_CreateWindow (
         glob.apptitle.c_str (), 
         glob.window.pos.x, glob.window.pos.y, 
         glob.window.dim.x, glob.window.dim.y, 
         windowflags 
         );

      return (glob.SDL.window ? true : false);
   }

   assert(false);
   return false;
}

bool init_display_surface (Ref<System>::Type sys, VulkanState& glob)
{
   if (glob.vulkan.instance.handle && glob.device.physical.handle && glob.SDL.window)
   {
      //
      if (SDL_TRUE == SDL_Vulkan_CreateSurface (
         glob.SDL.window, glob.vulkan.instance.handle, 
         &glob.vulkan.surface.handle)) 
      {
         assert (glob.vulkan.surface.handle); 



         VkBool32 supported = VK_FALSE; 
         VkResult result    = VK_SUCCESS;

         result = vkGetPhysicalDeviceSurfaceSupportKHR (
            glob.device.physical.handle,
            glob.selected_index.family_queue,
            glob.vulkan.surface.handle,
            &supported 
            ); 
         assert (result == VK_SUCCESS); 
         assert (supported == VK_TRUE); 


         result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR ( 
            glob.device.physical.handle,
            glob.vulkan.surface.handle,
            &glob.vulkan.surface.caps
            ); 


         uint32_t formatcount = 0;
         result = vkGetPhysicalDeviceSurfaceFormatsKHR (
            glob.device.physical.handle,
            glob.vulkan.surface.handle, 
            &formatcount ,
            VK_NULL_HANDLE
            ); 
         assert (result == VK_SUCCESS); 

         glob.vulkan.surface.formats.resize (formatcount );

         result = vkGetPhysicalDeviceSurfaceFormatsKHR (
            glob.device.physical.handle,
            glob.vulkan.surface.handle, 
            &formatcount,
            std::data(glob.vulkan.surface.formats)
            ); 
         assert (result == VK_SUCCESS); 

         for (uint32_t  ifmt = 0; ifmt < formatcount; ifmt++)
         {
            const VkSurfaceFormatKHR& surfmt =  glob.vulkan.surface.formats[ifmt];
            if (surfmt.format == VK_FORMAT_B8G8R8A8_UNORM && surfmt.colorSpace == VK_COLORSPACE_SRGB_NONLINEAR_KHR)
            {
               glob.selected_index.surface_format = ifmt;
               break;
            }
         }

         assert (glob.selected_index.surface_format != -1); 
         return true;
      }
   }

   assert (false); 
   return false;
}


bool init_VK_device (Ref<System>::Type sys, VulkanState& glob) 
{
   VkResult result ; 

   assert (glob.vulkan.instance.handle);
   if (!glob.vulkan.instance.handle)
      return false;

   uint32_t numdevs = 0; 

   // enumerate device
   VkResult enum_res = vkEnumeratePhysicalDevices (
      glob.vulkan.instance.handle, 
      &numdevs, 
      0
      ); 

   assert (VK_SUCCESS == enum_res); 
   assert (numdevs > 0); 

   // look for qualified device 
   std::vector<VkPhysicalDevice> physicaldevices (numdevs);
   vkEnumeratePhysicalDevices (glob.vulkan.instance.handle, &numdevs, &physicaldevices[0]); 

   for (uint32_t idev = 0; idev < numdevs; idev++)
   {
      // dev props, should we save these?
      Ref<VkPhysicalDeviceProperties>::Type devprops (new VkPhysicalDeviceProperties); 

      vkGetPhysicalDeviceProperties (
         physicaldevices[idev], 
         devprops.get()
         ); 

      // enumerate queue props
      // vkGetPhysicalDeviceQueueFamilyProperties
      uint32_t propcount = 0;
      vkGetPhysicalDeviceQueueFamilyProperties (
         physicaldevices[idev], 
         &propcount, 
         nullptr
         ); 
      if (propcount == 0)
         continue; 

      std::vector<VkQueueFamilyProperties> queueprops (propcount); 
      vkGetPhysicalDeviceQueueFamilyProperties (
         physicaldevices[idev], 
         &propcount, 
         &queueprops[0]
         ); 

      // inspect props flags 
      for (uint32_t iq = 0; iq < propcount; iq++)
      {
         // want device that supports..
         bool req_queues = queueprops[iq].queueFlags & VK_QUEUE_GRAPHICS_BIT       
                        && queueprops[iq].queueFlags & VK_QUEUE_COMPUTE_BIT        
                        && queueprops[iq].queueFlags & VK_QUEUE_TRANSFER_BIT       
                        && queueprops[iq].queueFlags & VK_QUEUE_SPARSE_BINDING_BIT; 

         if (queueprops[iq].queueCount > 0 && req_queues) 
         {
            assert (glob.selected_index.family_queue  == -1); 
            assert (glob.device.physical.handle       == VK_NULL_HANDLE); 

            glob.selected_index.family_queue = iq; 
            glob.device.physical.handle      = physicaldevices[idev];
            break;
         }
      }
   }

   assert (glob.selected_index.family_queue != -1);
   assert (glob.device.physical.handle);

   // mem properties
   vkGetPhysicalDeviceMemoryProperties (
      glob.device.physical.handle, 
      &glob.device.physical.memprops
      );      

   Ref<VkDeviceQueueCreateInfo>::Type queueinfo   (new VkDeviceQueueCreateInfo); 
   Ref<VkDeviceCreateInfo>::Type      devinfo     (new VkDeviceCreateInfo);

   *queueinfo = { VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO};
   *devinfo   = { VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO      };

   std::vector<VkExtensionProperties>  extprops;
  {
      uint32_t extpropcount = 0;
      char     layername[256] = { 0 }; 

      result = vkEnumerateDeviceExtensionProperties (
         glob.device.physical.handle, 
         VK_NULL_HANDLE, 
         &extpropcount, 
         VK_NULL_HANDLE
         ); 

      extprops.resize (extpropcount);
      result = vkEnumerateDeviceExtensionProperties (
         glob.device.physical.handle, 
         VK_NULL_HANDLE, 
         &extpropcount, 
         &extprops[0]
         ); 

      // Device Features
      vkGetPhysicalDeviceFeatures (
         glob.device.physical.handle, 
         &glob.device.physical.features
         ); 

      const float queue_priorities[] = { 1.0f, 0.9, 0.8, 0.7, 0.6, 0.5, 0.4, 0.3, 0.2, 0.1 }; 
      queueinfo->queueFamilyIndex = glob.selected_index.family_queue;
      queueinfo->queueCount       = 1;
      queueinfo->pQueuePriorities = queue_priorities; 

      glob.device.extension_names.resize (extpropcount);  
      for (int i = 0; i < extprops.size (); i++)
         glob.device.extension_names[i] = extprops[i].extensionName; 
   }

   devinfo->queueCreateInfoCount     = 1; 
   devinfo->pQueueCreateInfos        = queueinfo.get(); //VkDeviceQueueCreateInfo
   devinfo->pEnabledFeatures         = &glob.device.physical.features;
   devinfo->enabledExtensionCount    = (uint32_t) glob.device.extension_names.size(); 
   devinfo->ppEnabledExtensionNames  = std::data (glob.device.extension_names); 

   result = vkCreateDevice (
      glob.device.physical.handle, 
      devinfo.get(), 
      VK_NULL_HANDLE, 
      &glob.device.logical.handle
      ); 

   assert (VK_SUCCESS == result);

   vkGetDeviceQueue (
      glob.device.logical.handle, 
      glob.selected_index.family_queue, 
      0, 
      &glob.device.logical.queue.handle
      ); 

   //glob.device.extension_names.size (); 
   return (glob.device.physical.handle ? true : false);                 

}

//
//
bool init_VK_swapchain (Ref<System>::Type sys, VulkanState& glob)
{
   if (glob.device.logical.handle && glob.vulkan.surface.handle) 
   {
      // VkResult result;   

      // create swap chain
      const VkSurfaceFormatKHR& surfmt = glob.vulkan.surface.formats[glob.selected_index.surface_format];
   
      VkSwapchainCreateInfoKHR  ci;
      ci.sType                  = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR ;
      ci.pNext                  = VK_NULL_HANDLE; 
      ci.flags                  = 0x0;
      ci.surface                = glob.vulkan.surface.handle; 
      ci.minImageCount          = 2; 
      ci.imageFormat            = surfmt.format;
      ci.imageColorSpace        = surfmt.colorSpace;  
      ci.imageExtent            = { glob.window.dim.x, glob.window.dim.y }; 
      ci.imageArrayLayers       = 1;
      ci.imageUsage             = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
      ci.imageSharingMode       = VK_SHARING_MODE_EXCLUSIVE;
      ci.queueFamilyIndexCount  = 0;
      ci.pQueueFamilyIndices    = VK_NULL_HANDLE;
      ci.preTransform           = glob.vulkan.surface.caps.currentTransform;
      ci.compositeAlpha         = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
      ci.presentMode            = VK_PRESENT_MODE_FIFO_KHR;
      ci.clipped                = VK_TRUE; 
      ci.oldSwapchain           = VK_NULL_HANDLE;

      VkResult result = vkCreateSwapchainKHR (
         glob.device.logical.handle,
         &ci,
         VK_NULL_HANDLE, 
         &glob.swapchain.handle);

      if (result !=  VK_SUCCESS)
      {
         assert (false); 
         return false;
      } 
   
   
      uint32_t swapchain_imagecount = 0;

      vkGetSwapchainImagesKHR (
         glob.device.logical.handle, 
         glob.swapchain.handle,
         &swapchain_imagecount, 
         VK_NULL_HANDLE 
         );
   
      glob.swapchain.images.resize (swapchain_imagecount); 

      if (VK_SUCCESS != vkGetSwapchainImagesKHR (
         glob.device.logical.handle, 
         glob.swapchain.handle,
         &swapchain_imagecount, 
         std::data (glob.swapchain.images) ))
      {
         assert (false); 
         return false;
      }

   
      glob.swapchain.imageviews.resize (swapchain_imagecount); 

      for (uint32_t i = 0; i < swapchain_imagecount; i++) 
      {
         VkImageViewCreateInfo color_image_view = {VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
         color_image_view.pNext                          = VK_NULL_HANDLE;
         color_image_view.flags                          = 0x0;
         color_image_view.image                          = glob.swapchain.images[i];
         color_image_view.viewType                       = VK_IMAGE_VIEW_TYPE_2D;
         color_image_view.format                         = glob.vulkan.surface.formats[glob.selected_index.surface_format].format;
         color_image_view.components.r                   = VK_COMPONENT_SWIZZLE_R;
         color_image_view.components.g                   = VK_COMPONENT_SWIZZLE_G;
         color_image_view.components.b                   = VK_COMPONENT_SWIZZLE_B;
         color_image_view.components.a                   = VK_COMPONENT_SWIZZLE_A;
         color_image_view.subresourceRange.aspectMask    = VK_IMAGE_ASPECT_COLOR_BIT;
         color_image_view.subresourceRange.baseMipLevel  = 0;
         color_image_view.subresourceRange.levelCount    = 1;
         color_image_view.subresourceRange.baseArrayLayer= 0;
         color_image_view.subresourceRange.layerCount    = 1;
   
         if (VK_SUCCESS != vkCreateImageView (
            glob.device.logical.handle, 
            &color_image_view, 
            VK_NULL_HANDLE,
            &glob.swapchain.imageviews[i] ))
         {
            assert (false);
            return false;
         }

       }

      return true;
   }

   return false;
}


bool init_VK_depthbuffer  (Ref<System>::Type sys, VulkanState& glob)
{

   if (glob.device.logical.handle)
   {
      VkExtent3D        imgext        = {glob.window.dim.x, glob.window.dim.y, 1}; 

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

      if (VK_SUCCESS != vkCreateImage (
         glob.device.logical.handle, 
         &depthbuf_info, 
         VK_NULL_HANDLE, 
         &glob.depthbuffer.handle))
      {
         return false;
      }


      VkMemoryRequirements memreq = {};
      vkGetImageMemoryRequirements (
         glob.device.logical.handle, 
         glob.depthbuffer.handle, 
         &memreq
         );

      VkMemoryAllocateInfo allocinfo = {
         VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
         VK_NULL_HANDLE,
         memreq.size, 
         0
         };

      bool pass = memory_type_from_properties (
         glob.device.physical.memprops,                    //
         memreq.memoryTypeBits,       //
         0x0,                         // No Requirements 
         &allocinfo.memoryTypeIndex //
         );

      assert (pass);


      if (VK_SUCCESS != vkAllocateMemory (
         glob.device.logical.handle, 
         &allocinfo, 
         VK_NULL_HANDLE, 
         &glob.depthbuffer.mem)) 
      {
         return false;
      }

      if (VK_SUCCESS != vkBindImageMemory (
         glob.device.logical.handle, 
         glob.depthbuffer.handle, 
         glob.depthbuffer.mem, 
         0))
      {
         return false;
      } 


      VkImageViewCreateInfo depth_view_info ; 
      depth_view_info.sType                           = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO ;
      depth_view_info.pNext                           = VK_NULL_HANDLE;  
      depth_view_info.image                           = glob.depthbuffer.handle; 
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

      if (VK_SUCCESS != vkCreateImageView (
         glob.device.logical.handle, 
         &depth_view_info, 
         VK_NULL_HANDLE,
         &glob.depthbuffer.view))
      {
         return false;
      }         

      return true;
   }

   return false;
}

bool init_VK_commandbuffer_pool (Ref<System>::Type sys, VulkanState& glob)
{
   assert (glob.device.logical.handle); 
   if (glob.device.logical.handle) 
   {
      assert (glob.device.logical.commandpool.handle == VK_NULL_HANDLE); 

      VkCommandPoolCreateFlags flags = 0x0;
      //typedef enum VkCommandPoolCreateFlagBits {
      //    VK_COMMAND_POOL_CREATE_TRANSIENT_BIT = 0x00000001,
      //    VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT = 0x00000002,
      //    VK_COMMAND_POOL_CREATE_PROTECTED_BIT = 0x00000004,
      //    VK_COMMAND_POOL_CREATE_FLAG_BITS_MAX_ENUM = 0x7FFFFFFF
      //} VkCommandPoolCreateFlagBits;
      //typedef VkFlags VkCommandPoolCreateFlags;

      VkCommandPoolCreateInfo ci = 
      { 
         VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,  //VkStructureType             sType;
         VK_NULL_HANDLE,                              // const void*                 pNext;
         flags,                                         // VkCommandPoolCreateFlags    flags;
         glob.selected_index.family_queue             // uint32_t                    queueFamilyIndex;
      }; 

      if (VK_SUCCESS != vkCreateCommandPool (
         glob.device.logical.handle, 
         &ci, 
         VK_NULL_HANDLE, 
         &glob.device.logical.commandpool.handle))
      {
         return false;
      }

      return true; 
   }

   return false; 
}
//
bool init_VK_commandbuffer (Ref<System>::Type sys, VulkanState& glob)
{
      
   assert (glob.device.logical.commandbuffer.handle == VK_NULL_HANDLE); 

   if (glob.device.logical.handle && glob.device.logical.commandpool.handle) 
   {

      
      Ref<VkCommandBufferAllocateInfo>::Type cmdbuf_ci (new VkCommandBufferAllocateInfo);  
      *cmdbuf_ci = {
         VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO, // VkStructureType         sType;
         VK_NULL_HANDLE,                                 // const void*             pNext;
         glob.device.logical.commandpool.handle,         // VkCommandPool           commandPool;
         VK_COMMAND_BUFFER_LEVEL_PRIMARY,                // VkCommandBufferLevel    level;
         1                                               // uint32_t                commandBufferCount;
         };

      if (VK_SUCCESS != vkAllocateCommandBuffers (
         glob.device.logical.handle, 
         cmdbuf_ci.get(), 
         &glob.device.logical.commandbuffer.handle))
      {
         return false;
      }
   
      assert (glob.device.logical.commandpool.handle   ); 
      assert (glob.device.logical.commandbuffer.handle ); 

      return true;
   }

   return false;
}

bool init_VK_framebuffer (Ref<System>::Type sys, VulkanState& glob)
{ 
   if (glob.device.logical.handle && glob.renderpass.handle) 
   {
      VkResult result; 

      // framebuffer
      glob.framebuffers.resize (glob.swapchain.images.size(), VK_NULL_HANDLE); 
      {
         VkFramebufferCreateFlags flags = 0x0; 

         VkFramebufferCreateInfo ci = {
            VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO, // VkStructureType             sType;
            VK_NULL_HANDLE             , // const void*                 pNext;
            flags                      , // VkFramebufferCreateFlags    flags;
            glob.renderpass.handle     , // VkRenderPass                renderPass;
            2                          , // uint32_t                    attachmentCount;
            VK_NULL_HANDLE             , // const VkImageView*          pAttachments;
            glob.window.dim.x          , // uint32_t                    width;
            glob.window.dim.y          , // uint32_t                    height;
            1                          , // uint32_t                    layers;
            }; 

         for (int iframe = 0; iframe < glob.swapchain.images.size(); iframe++) 
         {
            VkImageView attachments[2] = {
               glob.swapchain.imageviews[iframe],    //
               glob.depthbuffer.view                 //
               }; 

            ci.pAttachments = attachments; 

            result = vkCreateFramebuffer (
               glob.device.logical.handle, 
               &ci,
               VK_NULL_HANDLE, 
               &glob.framebuffers[iframe]
               ); 

            assert (result == VK_SUCCESS); 
            }
      }

      for (int i = 0; i < glob.framebuffers.size(); ++i)
         assert (glob.framebuffers[i] != VK_NULL_HANDLE); 


   }

   assert (false); 
   return false; 

}
bool init_VK_shaders (Ref<System>::Type sys, VulkanState& glob) 
{ 
   return false; 
}

bool init_VK_vertexbuffers (Ref<System>::Type sys, VulkanState& glob) 
{ 
   return false; 
}


bool init_VK_uniformbuffer (Ref<System>::Type sys, VulkanState& glob)
{
   if (glob.device.logical.handle)
   {

      const VkDeviceSize   uniform_mem_req_size = 1024 * 1024;

      VkBufferCreateInfo uniform_create = {
         VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,  // VkStructureType        sType;
         VK_NULL_HANDLE,                        // const void*            pNext;
         0x0,                                   // VkBufferCreateFlags    flags;
         sizeof VulkanState,                           // VkDeviceSize           size;
         VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,    // VkBufferUsageFlags     usage;
         VK_SHARING_MODE_EXCLUSIVE,             // VkSharingMode          sharingMode;
         0,                                     // uint32_t               queueFamilyIndexCount;
         VK_NULL_HANDLE                         // const uint32_t*        pQueueFamilyIndices;
         }; 

      if (VK_SUCCESS != vkCreateBuffer (
         glob.device.logical.handle, 
         &uniform_create, 
         VK_NULL_HANDLE, 
         &glob.uniform.buffer.handle))
      {
         return false;
      } 

      VkMemoryRequirements memreq;
      vkGetBufferMemoryRequirements (
         glob.device.logical.handle, 
         glob.uniform.buffer.handle,
         &memreq
         );

      VkMemoryAllocateInfo allocinfo = {
         VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO  , // VkStructureType    sType;
         VK_NULL_HANDLE                          , // const void*        pNext;
         memreq.size                             , // VkDeviceSize       allocationSize;
         0                                         // uint32_t           memoryTypeIndex;
         };

      bool success = memory_type_from_properties (
         glob.device.physical.memprops, 
         memreq.memoryTypeBits,
         VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
         &allocinfo.memoryTypeIndex
         );

      assert (success);

      if (VK_SUCCESS != vkAllocateMemory (
         glob.device.logical.handle, 
         &allocinfo, 
         VK_NULL_HANDLE,
         &glob.uniform.devicemem.handle))
      {
         return false;
      }

      assert (glob.uniform.buffer.handle     != VK_NULL_HANDLE); 
      assert (glob.uniform.devicemem.handle  != VK_NULL_HANDLE); 

      return true;
   }

   return false;
}

bool init_VK_descriptors (Ref<System>::Type sys, VulkanState& glob) 
{ 
   assert (glob.descriptor.pool.handle);
   assert (glob.device.logical.handle);

   for (size_t i = 0; i < vkx::DescriptorGroup::MaxCount; ++i)
   {
      assert (VK_NULL_HANDLE == glob.descriptor.grp[i].set.handle );
   }

//   
//   if (glob.device.logical.handle)
//   {
//      VkResult result ; 
//
//      VkPipelineCache                  pipelinecache; 
//      VkPipelineShaderStageCreateFlags pipelinecreateflags;
//
//
//      // descriptor pool
//      {
//      }
//
//
//      {
//         VkDescriptorSetLayoutBinding vs_bind_desc[] = 
//         {
//            // vertex   
//            {
//               0,                                  // location
//               VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,  // type
//               1,                                  // count
//               VK_SHADER_STAGE_VERTEX_BIT,         // flags;
//               VK_NULL_HANDLE                      // samplers 
//            }, 
//         };
//
//
//    
//    
//         VkDescriptorSetLayoutCreateFlags flags = 0x0;
//
//         VkDescriptorSetLayoutCreateInfo ci = 
//         {
//            VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,  // VkStructureType                        sType;
//            VK_NULL_HANDLE,                                       // const void*                            pNext;
//            flags,                                                // VkDescriptorSetLayoutCreateFlags       flags;
//            1, //El_count (vs_bind_desc),                         // uint32_t                               bindingCount;
//            &vs_bind_desc[0]                                      // const VkDescriptorSetLayoutBinding*    pBindings;
//         };
//
//
//assert (false); 
//// this is wrong
//
//         result = vkCreateDescriptorSetLayout (
//            glob.device.logical.handle,
//            vert_descriptor_layout.get(), 
//            VK_NULL_HANDLE,
//            &glob.descriptor.grp[vkx::DescriptorGroup::vert].layout.handle
//            );
//
//         assert (result == VK_SUCCESS);
//      }
//
//      assert (glob.descriptor.grp[vkx::DescriptorGroup::vert].layout.handle); 
//
//
//      // vert shader layout descriptor
//      {
//      }
//      //assert (descriptor.layout.frag); 
//
//      {
//      }
//      //assert (descriptor.layout.teva); 
//      {
//      }
//      //assert (descriptor.layout.tctr); 
//      return true; 
//   }
//
//   assert (false); 

   if (glob.device.logical.handle && glob.descriptor.pool.handle) 
   {
      VkDevice          device = glob.device.logical.handle;
      VkDescriptorPool  pool   = glob.descriptor.pool.handle;

      const std::array<VkDescriptorSetLayout, vkx::DescriptorGroup::MaxCount> desclayouts =  
      {
         glob.descriptor.grp[vkx::DescriptorGroup::vert].layout.handle, 
         glob.descriptor.grp[vkx::DescriptorGroup::tctr].layout.handle, 
         glob.descriptor.grp[vkx::DescriptorGroup::teva].layout.handle, 
         glob.descriptor.grp[vkx::DescriptorGroup::frag].layout.handle, 
      };

      const VkDescriptorSetAllocateInfo allocinfo = 
      {
         VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO, // VkStructureType                 sType;
         VK_NULL_HANDLE,                                 // const void*                     pNext;
         pool,                                           // VkDescriptorPool                descriptorPool;
         (uint32_t) std::size(desclayouts),              // uint32_t                        descriptorSetCount;
         std::data(desclayouts)                          // const VkDescriptorSetLayout*    pSetLayouts;
      };

   
      std::array<VkDescriptorSet, vkx::DescriptorGroup::MaxCount> descriptorsets = { VK_NULL_HANDLE }; 
      VkResult result = vkAllocateDescriptorSets (
         device,                    //     VkDevice                                    device,
         &allocinfo,                //     const VkDescriptorSetAllocateInfo*          pAllocateInfo,
         std::data(descriptorsets)  //     VkDescriptorSet*                            pDescriptorSets);
         ); 

      // VkDescriptorSet
      if (VK_SUCCESS == result)
      {
         for (size_t i = 0; i < vkx::DescriptorGroup::MaxCount; ++i)
         {
            glob.descriptor.grp[i].set.handle = descriptorsets[i];
         }

         return true ; 
      }

      return true; // ?
   }


   return false; 
}



bool init_VK_descriptor_pool (Ref<System>::Type sys, VulkanState& glob)
{
   //vkCreateCommandPool (device, cmdpoolci, alloccbs, &commandpool);

   //vkCreateCommandPool
   //vkCreateDescriptorPool
   // 

   assert (VK_NULL_HANDLE == glob.descriptor.pool.handle); 

   if (glob.device.logical.handle)
   {
      const uint32_t kMaxAllocFromPool        = 8; 
      const size_t num_descriptor_pool_types  = 4;
      const uint32_t num_elements_per_pool    = 32; 

      //typedef struct VkDescriptorPoolSize {
      //    VkDescriptorType    type;
      //    uint32_t            descriptorCount;
      //} VkDescriptorPoolSize;

      std::array<VkDescriptorPoolSize, num_descriptor_pool_types> poolsizes = 
      {
         VkDescriptorPoolSize { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, num_elements_per_pool }, 
         VkDescriptorPoolSize { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, num_elements_per_pool }, 
         VkDescriptorPoolSize { VK_DESCRIPTOR_TYPE_SAMPLER       , num_elements_per_pool },
         VkDescriptorPoolSize { VK_DESCRIPTOR_TYPE_SAMPLER       , num_elements_per_pool },
      };

      // VkDescriptorType {
      //     VK_DESCRIPTOR_TYPE_SAMPLER = 0,
      //     VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER = 1,
      //     VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE = 2,
      //     VK_DESCRIPTOR_TYPE_STORAGE_IMAGE = 3,
      //     VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER = 4,
      //     VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER = 5,
      //     VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER = 6,
      //     VK_DESCRIPTOR_TYPE_STORAGE_BUFFER = 7,
      //     VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC = 8,
      //     VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC = 9,
      //     VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT = 10,
      //     VK_DESCRIPTOR_TYPE_INLINE_UNIFORM_BLOCK_EXT = 1000138000,
      //     VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_NV = 1000165000,
      //     VK_DESCRIPTOR_TYPE_BEGIN_RANGE = VK_DESCRIPTOR_TYPE_SAMPLER,
      //     VK_DESCRIPTOR_TYPE_END_RANGE = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT,
      //     VK_DESCRIPTOR_TYPE_RANGE_SIZE = (VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT - VK_DESCRIPTOR_TYPE_SAMPLER + 1),
      //     VK_DESCRIPTOR_TYPE_MAX_ENUM = 0x7FFFFFFF
      // } VkDescriptorType;


      // poolsizes[?] = { ??,  ?? };


      //typedef enum VkDescriptorPoolCreateFlagBits {
      //    VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT = 0x00000001,
      //    VK_DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT_EXT = 0x00000002,
      VkDescriptorPoolCreateFlags flags = 0x0; 

      VkDescriptorPoolCreateInfo ci = 
      {
         VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,  // VkStructureType               sType;
         VK_NULL_HANDLE,                                 // const void*                   pNext;
         flags,                                          // VkDescriptorPoolCreateFlags   flags;
         kMaxAllocFromPool,                              // uint32_t                      maxSets;
         (uint32_t) std::size(poolsizes),                // uint32_t                      poolSizeCount;
         std::data(poolsizes)                            // const VkDescriptorPoolSize*   pPoolSizes;
      }; 

      //
      VkResult result = vkCreateDescriptorPool (
         glob.device.logical.handle,   // VkDevice                            device,
         &ci,                          // const VkDescriptorPoolCreateInfo*   pCreateInfo,
         VK_NULL_HANDLE,               // const VkAllocationCallbacks*        pAllocator,
         &glob.descriptor.pool.handle  // VkDescriptorPool*                   pDescriptorPool);
         );

      assert (VK_SUCCESS == result);
      assert (glob.descriptor.pool.handle);
      return true;
   }

   assert (false);
   return false;
}


 
//  
bool init_VK_DSLO_vertex (Ref<System>::Type sys, VulkanState& glob)
{ 

   assert (glob.device.logical.handle);

   if (glob.device.logical.handle) 
   {
      assert (VK_NULL_HANDLE == glob.descriptor.grp[vkx::DescriptorGroup::vert].layout.handle);

      const VkShaderStageFlags vertstageflags = VK_SHADER_STAGE_VERTEX_BIT;
                                             // | VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT
                                             // | VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT
                                             // | VK_SHADER_STAGE_FRAGMENT_BIT;
      //
      //
      const VkDescriptorSetLayoutBinding vertstagedesc[] = 
      {
         {
            0,                                  // uint32_t              binding;
            VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,  // VkDescriptorType      descriptorType;
            1,                                  // uint32_t              descriptorCount;
            VK_SHADER_STAGE_VERTEX_BIT,         // VkShaderStageFlags    stageFlags;
            nullptr,                            // const VkSampler*      pImmutableSamplers;
         }, 
      };
 
      
      VkDescriptorSetLayout*                    dslo     = &glob.descriptor.grp[vkx::DescriptorGroup::vert].layout.handle;
      std::vector<VkDescriptorSetLayoutBinding> bindings (vertstagedesc, vertstagedesc + ARRAY_COUNT (vertstagedesc));
      VkDevice                                  device   = glob.device.logical.handle;

      VkDescriptorSetLayout desclayout = vkx::create_descriptor_set_layout (
         dslo,
         bindings,
         device
         ); 

      assert (desclayout); 
      return (desclayout ? true : false);
   }

   return false; 
}

//init_VK_descriptor 
//glob.descriptor.set.handle                
//
bool init_VK_DSLO_tesselator (Ref<System>::Type sys, VulkanState& glob)
{
   assert (glob.device.logical.handle);
   assert (glob.descriptor.grp[vkx::DescriptorGroup::tctr].layout.handle);
   assert (glob.descriptor.grp[vkx::DescriptorGroup::teva].layout.handle);

   if (glob.device.logical.handle) 
   {

      // tessellation control      
      const VkDescriptorSetLayoutBinding tctr_desc_set_arr[] = 
      {
         {
            0,                                        // uint32_t              binding;
            VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,        // VkDescriptorType      descriptorType;
            1,                                        // uint32_t              descriptorCount;
            VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT, // VkShaderStageFlags    stageFlags;
            nullptr,                                  // const VkSampler*      pImmutableSamplers;
         }, 

         {
            1,                                        // uint32_t              binding;
            VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,        // VkDescriptorType      descriptorType;
            1,                                        // uint32_t              descriptorCount;
            VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT, // VkShaderStageFlags    stageFlags;
            nullptr,                                  // const VkSampler*      pImmutableSamplers;
         }, 
      }; 

      // tessellation eval 
      VkDescriptorSetLayout tctrdesc = vkx::create_descriptor_set_layout (
         &glob.descriptor.grp[vkx::DescriptorGroup::tctr].layout.handle,
         std::vector<VkDescriptorSetLayoutBinding> (tctr_desc_set_arr, tctr_desc_set_arr + ARRAY_COUNT (tctr_desc_set_arr)),
         glob.device.logical.handle
         ); 

      assert (tctrdesc); 
      if (!tctrdesc)
         return false;


      //
      const VkDescriptorSetLayoutBinding teva_desc_set_arr[] = 
      {
         {      
            1,                                        // uint32_t              binding;
            VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,        // VkDescriptorType      descriptorType;
            1,                                        // uint32_t              descriptorCount;
            VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT, // VkShaderStageFlags    stageFlags;
            nullptr,                                  // const VkSampler*      pImmutableSamplers;
         }, 

         {      
            1,                                        // uint32_t              binding;
            VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,        // VkDescriptorType      descriptorType;
            1,                                        // uint32_t              descriptorCount;
            VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT, // VkShaderStageFlags    stageFlags;
            nullptr,                                  // const VkSampler*      pImmutableSamplers;
         }, 
      };

      // 
      VkDescriptorSetLayout tevadesc = vkx::create_descriptor_set_layout (
         &glob.descriptor.grp[vkx::DescriptorGroup::teva].layout.handle,
         std::vector<VkDescriptorSetLayoutBinding> (teva_desc_set_arr, teva_desc_set_arr + ARRAY_COUNT (teva_desc_set_arr)),
         glob.device.logical.handle
         ); 

      assert (tevadesc); 
      if (tevadesc)
         return true;
   }

   
   return false; 
}

// gazebo 


//
bool init_VK_DSLO_fragment (Ref<System>::Type sys, VulkanState& glob)
{
   assert (glob.device.logical.handle);
   assert (glob.descriptor.grp[vkx::DescriptorGroup::frag].layout.handle);

   if (glob.device.logical.handle) 
   {
      const VkDescriptorSetLayoutBinding frag_desc_set_arr[] = 
      {
         {
            0,                                  // uint32_t              binding;
            VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,  // VkDescriptorType      descriptorType;
            1,                                  // uint32_t              descriptorCount;
            VK_SHADER_STAGE_FRAGMENT_BIT,         // VkShaderStageFlags    stageFlags;
            nullptr,                            // const VkSampler*      pImmutableSamplers;
         }, 

         {
            1,                                  // uint32_t              binding;
            VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,  // VkDescriptorType      descriptorType;
            1,                                  // uint32_t              descriptorCount;
            VK_SHADER_STAGE_FRAGMENT_BIT,         // VkShaderStageFlags    stageFlags;
            nullptr,                            // const VkSampler*      pImmutableSamplers;
         }, 

      };

      // VK_DESCRIPTOR_TYPE_UNIFORM_
      VkDescriptorSetLayout fragdesc = vkx::create_descriptor_set_layout (
         &glob.descriptor.grp[vkx::DescriptorGroup::frag].layout.handle,
         std::vector<VkDescriptorSetLayoutBinding> (frag_desc_set_arr, frag_desc_set_arr + ARRAY_COUNT (frag_desc_set_arr)),
         glob.device.logical.handle
         ); 

      assert (fragdesc); 
      return (fragdesc ? true : false);

   }

   return false; 
}


// VKAPI_ATTR VkResult VKAPI_CALL vkCreatePipelineLayout(
//     VkDevice                                    device,
//     const VkPipelineLayoutCreateInfo*           pCreateInfo,
//     const VkAllocationCallbacks*                pAllocator,
//     VkPipelineLayout*                           pPipelineLayout);
//
//bool init_pipeline_layout (
//   VkPipelineLayout*          layout,
//   VkDescriptorSetLayout*     dslo, 
//
//   //
//   const std::vector<VkDescriptorSetLayoutBinding>& layoutbindings,
//   VkPipelineLayoutCreateFlags               flags, 
//   VkAllocationCallbacks*                    allocatorCBs,
//   VkDevice                                  device)
//{
//
//   VkDescriptorSetLayoutCreateFlags dslo_flags = 0;
//   // 
//   //VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO = 0;
//
//   // VkDescriptorSetLayoutCreateInfo {
//   //
//   //    VkStructureType                        sType;
//   //    const void*                            pNext;
//   //    VkDescriptorSetLayoutCreateFlags       flags;
//   //    uint32_t                               bindingCount;
//   //    const VkDescriptorSetLayoutBinding*    pBindings;
//   //
//   // } VkDescriptorSetLayoutCreateInfo;
//
//   const VkDescriptorSetLayoutCreateInfo dslo_ci = 
//   {
//      VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
//      VK_NULL_HANDLE,
//      dslo_flags,
//      layoutbindings.size() ,  
//      std::data(layoutbindings)
//   }; 
//
//
//   //VkDescriptorSetLayout dslo = VK_NULL_HANDLE;  
//   VkPipelineLayoutCreateFlags plo_flags = 0x0;
//   vkCreateDescriptorSetLayout (device, &dslo_ci, allocatorCBs, dslo) ;
//
//
//   const VkPipelineLayoutCreateInfo createinfo =
//   {
//      VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO, // VkStructureType                 sType;
//      VK_NULL_HANDLE,                 // const void*                     pNext;
//      flags,                          // VkPipelineLayoutCreateFlags     flags;
//      0,                           // uint32_t                        setLayoutCount;
//      0,        // const VkDescriptorSetLayout*    pSetLayouts;
//      0,                              // uint32_t                        pushConstantRangeCount;
//      VK_NULL_HANDLE                  // const VkPushConstantRange*      pPushConstantRanges;
//   };
//
//   // VkDescriptorSetLayout*
//   VkResult res = vkCreatePipelineLayout ( 
//         device, 
//         &createinfo,      
//         VK_NULL_HANDLE, //allocator ,
//         layout
//         );
//
//
//   if (VK_SUCCESS != res )
//   {
//      
//   }
//   
//   return true; 
//}
 
bool init_VK_pipeline_layout (Ref<System>::Type sys, VulkanState& glob)
{ 
   assert (false); 

   std::array<VkDescriptorSetLayout, 4> layout_arr = 
   {
      glob.descriptor.grp[vkx::DescriptorGroup::vert].layout.handle, 
      glob.descriptor.grp[vkx::DescriptorGroup::tctr].layout.handle, 
      glob.descriptor.grp[vkx::DescriptorGroup::teva].layout.handle,
      glob.descriptor.grp[vkx::DescriptorGroup::frag].layout.handle
   };                             

   //VkPipelineLayoutCreateInfo
   VkPipelineLayoutCreateFlags flags = 0x0;
   VkPipelineLayoutCreateInfo ci = {
      VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO, // VkStructureType                 sType;
      VK_NULL_HANDLE,                     // const void*                     pNext;
      flags,                              // VkPipelineLayoutCreateFlags     flags;
      (uint32_t) std::size(layout_arr),   // uint32_t                        setLayoutCount;
      std::data(layout_arr),              // const VkDescriptorSetLayout*    pSetLayouts;
      0,                                  // uint32_t                        pushConstantRangeCount;
      VK_NULL_HANDLE                      // const VkPushConstantRange*      pPushConstantRanges;
      };

   //VKAPI_ATTR VkResult VKAPI_CALL vkCreatePipelineLayout(
   if (VK_SUCCESS != vkCreatePipelineLayout (
      glob.device.logical.handle,      // VkDevice                                    device, 
      &ci,                             // const VkPipelineLayoutCreateInfo*           pCreateInfo,
      VK_NULL_HANDLE,                  // const VkAllocationCallbacks*                pAllocator,
      &glob.pipeline.layout.handle))   // VkPipelineLayout*                           pPipelineLayout);
   {
      assert (false);
      return false;
   }      


   assert (glob.pipeline.layout.handle != VK_NULL_HANDLE); 

   return false; 
}

bool init_VK_renderpass (Ref<System>::Type sys, VulkanState& glob)
{
   if (glob.device.logical.handle)
   {
      VkResult result; 
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
            2, //El_count(attachment_desc)                 , // uint32_t                          attachmentCount;
            attachment_desc                           , // const VkAttachmentDescription*    pAttachments;
            1                                         , // uint32_t                          subpassCount;
            &subpass_desc                             , // const VkSubpassDescription*       pSubpasses;
            0                                         , // uint32_t                          dependencyCount;
            VK_NULL_HANDLE                            , // const VkSubpassDependency*        pDependencies;
            };


         result = vkCreateRenderPass (
            glob.device.logical.handle,
            &ci,
            VK_NULL_HANDLE, 
            &glob.renderpass.handle
            ); 

         assert (result == VK_SUCCESS);
      }

      assert (glob.renderpass.handle != VK_NULL_HANDLE); 
      return true; 
   }
   assert (false); 
   return false; 
}


//
//
bool init_VK_instance (Ref<System>::Type sys, VulkanState& glob)
{

//   VkWin32SurfaceCreateInfoKHR surfcreateinfo = 
//   { 
//      VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR
//   };
//   surfcreateinfo.sType       = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR; 
//   surfcreateinfo.pNext       = nullptr;
//   surfcreateinfo.flags       = 0;
//   surfcreateinfo.hwnd        = 0;
//   surfcreateinfo.hinstance   = 0; 
//
//
//   VkResult surf_res = vkCreateWin32SurfaceKHR (glob.vulkan.instance, &surfcreateinfo, VK_NULL_HANDLE , &glob.surface.handle); 
//
//   glob.dim.x = 800 ; 
//   glob.dim.y = 600 ; 


   VkResult result = VkResult (-1);

   std::vector<const char*>            instance_ext_names;
   std::vector<VkExtensionProperties>  inst_ext_props; 

   {
      uint32_t instance_ext_count = 0;
      result = vkEnumerateInstanceExtensionProperties (
         nullptr, 
         &instance_ext_count, 
         nullptr
         );
      assert (result == VK_SUCCESS);

      inst_ext_props . resize (instance_ext_count);
      result = vkEnumerateInstanceExtensionProperties (
         nullptr, 
         &instance_ext_count, 
         &inst_ext_props[0]
         );

      assert (result == VK_SUCCESS);
   
      instance_ext_names.resize (instance_ext_count); 
      for (uint32_t i = 0; i < instance_ext_count; i++)
         instance_ext_names[i] = inst_ext_props[i].extensionName; 


//VK_KHR_WIN32_SURFACE_EXTENSION_NAME
//VK_KHR_SWAPCHAIN_EXTENSION_NAME 
//VK_KHR_SURFACE_EXTENSION_NAME

      DontCare (instance_ext_names.size()); 
   }      

   VkInstanceCreateInfo instinfo = 
      {
      VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,   //    VkStructureType             sType; 
      nullptr,                                  //    const void*                 pNext;
      0x0,                                      //    VkInstanceCreateFlags       flags;
      &glob.vulkan.instance.appinfo,            //    const VkApplicationInfo*    pApplicationInfo;
      0,                                        //    uint32_t                    enabledLayerCount;
      0,                                        //    const char* const*          ppEnabledLayerNames;
      (uint32_t) instance_ext_names.size(),     //    uint32_t                    enabledExtensionCount;
      &instance_ext_names[0]                    //    const char* const*          ppEnabledExtensionNames
      }; 


   // create instance
   result = vkCreateInstance (
      &instinfo, 
      0, 
      &glob.vulkan.instance.handle
      ); 


   assert (result == VK_SUCCESS); 
   return (result == VK_SUCCESS ? true : false);
}



//
//
Ref<Runnable>::Type create_vkx00 (const std::vector<std::string>& args)
{
   return Ref<Runnable>::Type (new vkx00);
} 
