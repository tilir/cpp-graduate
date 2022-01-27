//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  Simplest VK example: blank square on the screen
//  Note incredibly long time to first triangle
//  Lets for a moment close eyes for really ugly design -- it will be improved
//  really soon
//
//  Inspired by: https://vulkan-tutorial.com
//
//  cl /EHsc vk-simplest.cc /link glfw3dll.lib vulkan-1.lib
//
//-----------------------------------------------------------------------------

#include <array>
#include <cassert>
#include <fstream>
#include <iostream>
#include <memory>
#include <set>
#include <sstream>
#include <stdexcept>
#include <vector>

#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#ifndef ANALYZE
#define ANALYZE 1
#endif

#define dbgs                                                                   \
  if (!ANALYZE) {                                                              \
  } else                                                                       \
    std::cout

#define VK_CHECK_RESULT(f)                                                     \
  {                                                                            \
    VkResult Res = (f);                                                        \
    if (Res != VK_SUCCESS) {                                                   \
      std::ostringstream Os;                                                   \
      Os << "Vulkan error at " << __FILE__ << ":" << __LINE__ << "\n";         \
      throw vulkan_error(Res, Os.str());                                       \
    }                                                                          \
  }

// initial window sizes
constexpr int SZX = 600;
constexpr int SZY = 600;

// double-buffering
constexpr int MAX_FRAMES_IN_FLIGHT = 2;

const std::vector<const char *> validationLayers = {
    "VK_LAYER_KHRONOS_validation"};

// extensions list to query
const std::vector<const char *> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME};

const bool enableValidationLayers = true;

// shader names
constexpr const char *VERTNAME = "shaders/simplest-v.vert.spv";
constexpr const char *FRAGNAME = "shaders/simplest-v.frag.spv";

auto Cleanup = [](GLFWwindow *w) {
  if (w) {
    glfwDestroyWindow(w);
  }
  glfwTerminate();
};

struct Vertex {
  glm::vec2 pos;
  glm::vec3 color;

  static VkVertexInputBindingDescription getBindingDescription() {
    VkVertexInputBindingDescription bindingDescription{};
    bindingDescription.binding = 0;
    bindingDescription.stride = sizeof(Vertex);
    bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    return bindingDescription;
  }

  static std::array<VkVertexInputAttributeDescription, 2>
  getAttributeDescriptions() {
    std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};

    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[0].offset = offsetof(Vertex, pos);

    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[1].offset = offsetof(Vertex, color);

    return attributeDescriptions;
  }
};

const std::vector<Vertex> Vertices = {{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
                                      {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
                                      {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
                                      {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}};

const std::vector<unsigned short> Indices = {0, 1, 2, 2, 3, 0};

struct VkApp {
  std::unique_ptr<GLFWwindow, decltype(Cleanup)> Wnd;

  VkInstance Instance;
  VkPhysicalDevice PhysDevice;
  VkDevice Device;
  VkSurfaceKHR Surface;
  VkQueue GraphicsQueue;
  VkQueue PresentQueue;
  VkSwapchainKHR SwapChain;
  VkExtent2D Extent;
  VkSurfaceFormatKHR SurfaceFormat;
  std::vector<VkImage> SwapChainImages;
  VkFormat SwapChainImageFormat;
  VkExtent2D SwapChainExtent;
  std::vector<VkImageView> SwapChainImageViews;
  std::vector<VkFramebuffer> SwapChainFramebuffers;
  VkRenderPass RenderPass;
  VkPipelineLayout PipelineLayout;
  VkPipeline GraphicsPipeline;
  VkCommandPool CommandPool;
  VkBuffer VertexBuffer;
  VkDeviceMemory VertexBufferMemory;
  VkBuffer IndexBuffer;
  VkDeviceMemory IndexBufferMemory;
  std::vector<VkCommandBuffer> CommandBuffers;
  std::vector<VkSemaphore> ImageAvailableSemaphores;
  std::vector<VkSemaphore> RenderFinishedSemaphores;
  std::vector<VkFence> InFlightFences;
  std::vector<VkFence> ImagesInFlight;
  size_t CurrentFrame = 0;
  VkShaderModule StoredVertexID, StoredFragmentID;

  unsigned PresentFamily = -1u;
  unsigned GraphicsFamily = -1u;

  void initialize_window();
  void create_instance();
  void peek_device();
  void find_queues();
  void create_logical_device();

  VkShaderModule installShader(std::vector<char> ShaderCode);

  void create_swap_chain();
  void create_image_views();
  void create_render_pass();
  void create_descset_layout();
  void create_pipeline(VkShaderModule VertexID, VkShaderModule FragmentID);
  void create_frame_buffer();
  void create_command_pool();
  void create_buffers();
  void create_command_buffers();
  void create_sync_objs();

  void render_frame();
  void run();

  unsigned findMemoryType(unsigned typeFilter,
                          VkMemoryPropertyFlags properties);
  void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage,
                    VkMemoryPropertyFlags properties, VkBuffer &buffer,
                    VkDeviceMemory &bufferMemory);
  void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

  bool FramebufferResized;

  VkApp() : Wnd(nullptr, Cleanup) {}

  void update_swap_chain(); // really create from scratch

  void cleanup_swap_chain() {
    for (auto framebuffer : SwapChainFramebuffers)
      vkDestroyFramebuffer(Device, framebuffer, nullptr);

    vkFreeCommandBuffers(Device, CommandPool, CommandBuffers.size(),
                         CommandBuffers.data());

    vkDestroyPipeline(Device, GraphicsPipeline, nullptr);
    vkDestroyPipelineLayout(Device, PipelineLayout, nullptr);
    vkDestroyRenderPass(Device, RenderPass, nullptr);

    for (auto imageView : SwapChainImageViews)
      vkDestroyImageView(Device, imageView, nullptr);

    vkDestroySwapchainKHR(Device, SwapChain, nullptr);
  }

  ~VkApp() {
    cleanup_swap_chain();

    // TODO: to "unique pointers"
    vkDestroyDevice(Device, nullptr);
    vkDestroySurfaceKHR(Instance, Surface, nullptr);
    vkDestroyInstance(Instance, nullptr);
  }
};

// custom error handler class
struct glfw_error : public std::runtime_error {
  glfw_error(const char *s) : std::runtime_error(s) {}
};

// vulkan-specific error (knows error code)
struct vulkan_error : public std::runtime_error {
  VkResult Res;
  vulkan_error(VkResult R, std::string S) : std::runtime_error(S), Res(R) {}
};

// throw on errors
void error_callback(int, const char *err_str) { throw glfw_error(err_str); }

// make sure the viewport matches the new window dimensions
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  auto App = reinterpret_cast<VkApp *>(glfwGetWindowUserPointer(window));
  App->FramebufferResized = true;
}

// creating GLFW window
void VkApp::initialize_window() {
  glfwInit();
  glfwSetErrorCallback(error_callback);

  // this is interesting:
  // GLFW_NO_API required to NOT create OpenGL context
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

  auto *Window = glfwCreateWindow(SZX, SZY, "Hello, Vulkan", NULL, NULL);
  assert(Window); // error callback shall throw otherwise
  // so no need to call glfwMakeContextCurrent
  glfwSetWindowUserPointer(Window, this);
  glfwSetFramebufferSizeCallback(Window, framebuffer_size_callback);
  Wnd.reset(Window);
}

// create Vulkan Instance and Surface (requires window)
void VkApp::create_instance() {
  VkApplicationInfo appInfo{};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = "Hello, Vulkan";
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.pEngineName = "No Engine";
  appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.apiVersion = VK_API_VERSION_1_0;

  unsigned glfwExtensionCount = 0;
  const char **glfwExtensions;
  glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
  dbgs << "Enumerated: " << glfwExtensionCount << " glfw required extensions\n";

  // note: create info requires extensions list
  VkInstanceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;
  createInfo.enabledExtensionCount = glfwExtensionCount;
  createInfo.ppEnabledExtensionNames = glfwExtensions;

  // note: we may use enabledLayerCount to enable validation layers
  createInfo.enabledLayerCount = 0;

  VK_CHECK_RESULT(vkCreateInstance(&createInfo, nullptr, &Instance));

  // note: surface requires instance and window
  VK_CHECK_RESULT(
      glfwCreateWindowSurface(Instance, Wnd.get(), nullptr, &Surface));
}

// Peek Vulkan physical device (requires Instance)
void VkApp::peek_device() {
  unsigned deviceCount = 0;
  VK_CHECK_RESULT(vkEnumeratePhysicalDevices(Instance, &deviceCount, nullptr));
  if (deviceCount != 1)
    throw std::runtime_error("Multiple Vulkan devices not supported yet");
  dbgs << deviceCount << " devices enumerated\n";
  VK_CHECK_RESULT(
      vkEnumeratePhysicalDevices(Instance, &deviceCount, &PhysDevice));
}

// Query queue families (requires PhysDevice and Surface)
void VkApp::find_queues() {
  unsigned queueFamilyCount = 0;
  int i = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(PhysDevice, &queueFamilyCount,
                                           nullptr);
  dbgs << queueFamilyCount << " queue families found\n";

  std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
  vkGetPhysicalDeviceQueueFamilyProperties(PhysDevice, &queueFamilyCount,
                                           queueFamilies.data());

  for (const auto &queueFamily : queueFamilies) {
    if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
      GraphicsFamily = i;
      dbgs << "Graphics queue family: " << i << std::endl;
    }
    VkBool32 presentSupport = false;
    VK_CHECK_RESULT(vkGetPhysicalDeviceSurfaceSupportKHR(PhysDevice, i, Surface,
                                                         &presentSupport));
    if (presentSupport) {
      PresentFamily = i;
      dbgs << "Present queue family: " << i << std::endl;
    }
    if (PresentFamily != -1u && GraphicsFamily != -1u) {
      break;
    }
    i += 1;
  }

  if (PresentFamily == -1u || GraphicsFamily == -1u)
    throw std::runtime_error("Present and Graphics not found");
}

// Create logical device and queues (requires physical device and queue
// families)
void VkApp::create_logical_device() {
  std::set<unsigned> uniqueQueueFamilies = {PresentFamily, GraphicsFamily};
  std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

  float queuePriority = 1.0f;
  for (auto queueFamily : uniqueQueueFamilies) {
    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = queueFamily;
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.pQueuePriorities = &queuePriority;
    queueCreateInfos.push_back(queueCreateInfo);
  }

  // note: we are querying no device features
  VkPhysicalDeviceFeatures deviceFeatures{};

  dbgs << deviceExtensions.size() << " device extensions to enable\n";
  VkDeviceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  createInfo.queueCreateInfoCount = queueCreateInfos.size();
  createInfo.pQueueCreateInfos = queueCreateInfos.data();
  createInfo.pEnabledFeatures = &deviceFeatures;
  createInfo.enabledExtensionCount = deviceExtensions.size();
  createInfo.ppEnabledExtensionNames = deviceExtensions.data();

  VK_CHECK_RESULT(vkCreateDevice(PhysDevice, &createInfo, nullptr, &Device));

  vkGetDeviceQueue(Device, GraphicsFamily, 0, &GraphicsQueue);
  vkGetDeviceQueue(Device, PresentFamily, 0, &PresentQueue);
}

// WA for MSVS
template <typename T> T myclamp(T x, T low, T hi) {
  if (x > hi)
    return hi;
  if (x < low)
    return low;
  return x;
}

// create swapchain (requires physical device, logical device and surface)
void VkApp::create_swap_chain() {
  VkSurfaceCapabilitiesKHR capabilities;
  std::vector<VkSurfaceFormatKHR> formats;
  std::vector<VkPresentModeKHR> presentModes;

  VK_CHECK_RESULT(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(PhysDevice, Surface,
                                                            &capabilities));

  unsigned formatCount;
  VK_CHECK_RESULT(vkGetPhysicalDeviceSurfaceFormatsKHR(PhysDevice, Surface,
                                                       &formatCount, nullptr));

  dbgs << formatCount << " physical device surface formats found\n";

  if (formatCount) {
    formats.resize(formatCount);
    VK_CHECK_RESULT(vkGetPhysicalDeviceSurfaceFormatsKHR(
        PhysDevice, Surface, &formatCount, formats.data()));
  }

  SurfaceFormat = formats[0];
  auto res = std::find_if(formats.begin(), formats.end(), [](const auto &f) {
    return (f.format == VK_FORMAT_B8G8R8A8_SRGB &&
            f.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR);
  });

  if (res != formats.end()) {
    dbgs << "B8G8R8A8_SRGB available\n";
    SurfaceFormat = *res;
  }

  int w, h;
  glfwGetFramebufferSize(Wnd.get(), &w, &h);
  dbgs << "Frame buffer size: W = " << w << "; H = " << h << std::endl;
  unsigned width = w;
  unsigned height = h;
  Extent.width = myclamp(width, capabilities.minImageExtent.width,
                         capabilities.maxImageExtent.width);
  Extent.height = myclamp(height, capabilities.minImageExtent.height,
                          capabilities.maxImageExtent.height);
  dbgs << "Extent: W = " << Extent.width << "; H = " << Extent.height
       << std::endl;
  dbgs << "Image count: min = " << capabilities.minImageCount
       << "; max = " << capabilities.maxImageCount << std::endl;

  unsigned ImageCount = capabilities.minImageCount + 1;
  if (capabilities.maxImageCount > 0 && ImageCount > capabilities.maxImageCount)
    ImageCount = capabilities.maxImageCount;

  dbgs << "Capability image count: " << ImageCount << std::endl;

  VkSwapchainCreateInfoKHR createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  createInfo.surface = Surface;

  createInfo.minImageCount = ImageCount;
  createInfo.imageFormat = SurfaceFormat.format;
  createInfo.imageColorSpace = SurfaceFormat.colorSpace;
  createInfo.imageExtent = Extent;
  createInfo.imageArrayLayers = 1;
  createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

  unsigned queueFamilyIndices[] = {GraphicsFamily, PresentFamily};

  if (GraphicsFamily != PresentFamily) {
    createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
    createInfo.queueFamilyIndexCount = 2;
    createInfo.pQueueFamilyIndices = queueFamilyIndices;
    dbgs << "Graphics != Present, peeking concurrent mode\n";
  } else {
    createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    dbgs << "Graphics == Present, peeking exclusive mode\n";
  }

  // select presentation mode
  unsigned presentModeCount;
  VK_CHECK_RESULT(vkGetPhysicalDeviceSurfacePresentModesKHR(
      PhysDevice, Surface, &presentModeCount, nullptr));
  dbgs << presentModeCount << " presentation modes found\n";

  if (presentModeCount == 0)
    throw std::runtime_error("No presentation modes supported");

  presentModes.resize(presentModeCount);
  VK_CHECK_RESULT(vkGetPhysicalDeviceSurfacePresentModesKHR(
      PhysDevice, Surface, &presentModeCount, presentModes.data()));

  VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR;

  if (std::find(presentModes.begin(), presentModes.end(),
                VK_PRESENT_MODE_FIFO_KHR) != presentModes.end()) {
    dbgs << "VK_PRESENT_MODE_FIFO_KHR found\n";
  } else {
    dbgs << "VK_PRESENT_MODE_FIFO_KHR not found, found following:\n";
    for (auto m : presentModes) {
      dbgs << "\t" << m << std::endl;
      presentMode = m;
    }
    dbgs << "Using mode: " << presentMode << std::endl;
  }

  createInfo.preTransform = capabilities.currentTransform;
  createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  createInfo.presentMode = presentMode;
  createInfo.clipped = VK_TRUE;

  VK_CHECK_RESULT(
      vkCreateSwapchainKHR(Device, &createInfo, nullptr, &SwapChain));
}

// create swap chain images and views (requires logical device and swapchain)
void VkApp::create_image_views() {
  unsigned ImageCount;
  VK_CHECK_RESULT(
      vkGetSwapchainImagesKHR(Device, SwapChain, &ImageCount, nullptr));
  dbgs << "Swap chain image count " << ImageCount << std::endl;
  SwapChainImages.resize(ImageCount);
  VK_CHECK_RESULT(vkGetSwapchainImagesKHR(Device, SwapChain, &ImageCount,
                                          SwapChainImages.data()));
  SwapChainImageFormat = SurfaceFormat.format;
  SwapChainExtent = Extent;
  SwapChainImageViews.resize(ImageCount);

  for (size_t i = 0; i < ImageCount; ++i) {
    VkImageViewCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    createInfo.image = SwapChainImages[i];
    createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    createInfo.format = SwapChainImageFormat;
    createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    createInfo.subresourceRange.baseMipLevel = 0;
    createInfo.subresourceRange.levelCount = 1;
    createInfo.subresourceRange.baseArrayLayer = 0;
    createInfo.subresourceRange.layerCount = 1;
    VK_CHECK_RESULT(vkCreateImageView(Device, &createInfo, nullptr,
                                      &SwapChainImageViews[i]));
  }
}

// creating render pass (requires logical device and swap chain)
void VkApp::create_render_pass() {
  // single color buffer attachment represented by one of the images from the
  // swap chain. format equals to format of swap chain images
  VkAttachmentDescription colorAttachment{};
  colorAttachment.format = SwapChainImageFormat;
  colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
  colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

  // clear frame buffer to blank before new frame
  colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;

  // rendered contents stored in memory
  colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

  VkAttachmentReference colorAttachmentRef{};
  colorAttachmentRef.attachment = 0;
  colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  VkSubpassDescription subpass{};
  // subpass bound to graphics pipeline (alternatives: compute, etc)
  subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
  subpass.colorAttachmentCount = 1;
  subpass.pColorAttachments = &colorAttachmentRef;

  VkSubpassDependency dependency{};
  dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
  dependency.dstSubpass = 0;
  dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  dependency.srcAccessMask = 0;
  dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

  VkRenderPassCreateInfo renderPassInfo{};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  renderPassInfo.attachmentCount = 1;
  renderPassInfo.pAttachments = &colorAttachment;
  renderPassInfo.subpassCount = 1;
  renderPassInfo.pSubpasses = &subpass;
  renderPassInfo.dependencyCount = 1;
  renderPassInfo.pDependencies = &dependency;

  // Note: allocator here nullptr
  //       this means we may do all allocations by ourselves
  VK_CHECK_RESULT(
      vkCreateRenderPass(Device, &renderPassInfo, nullptr, &RenderPass));
}

// pipeline requires logical device, render pass, descriptor set layout and shader IDs
void VkApp::create_pipeline(VkShaderModule VertexID,
                            VkShaderModule FragmentID) {
  StoredVertexID = VertexID;
  StoredFragmentID = FragmentID;

  VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
  vertShaderStageInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
  vertShaderStageInfo.module = VertexID;
  vertShaderStageInfo.pName = "main";

  VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
  fragShaderStageInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
  fragShaderStageInfo.module = FragmentID;
  fragShaderStageInfo.pName = "main";

  VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo,
                                                    fragShaderStageInfo};

  VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
  vertexInputInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

  auto bindingDescription = Vertex::getBindingDescription();
  auto attributeDescriptions = Vertex::getAttributeDescriptions();

  vertexInputInfo.vertexBindingDescriptionCount = 1;
  vertexInputInfo.vertexAttributeDescriptionCount =
      attributeDescriptions.size();
  vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
  vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

  VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
  inputAssembly.sType =
      VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  inputAssembly.primitiveRestartEnable = VK_FALSE;

  VkViewport viewport{};
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width = SwapChainExtent.width;
  viewport.height = SwapChainExtent.height;
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;

  VkRect2D scissor{};
  scissor.offset = {0, 0};
  scissor.extent = SwapChainExtent;

  VkPipelineViewportStateCreateInfo viewportState{};
  viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  viewportState.viewportCount = 1;
  viewportState.pViewports = &viewport;
  viewportState.scissorCount = 1;
  viewportState.pScissors = &scissor;

  VkPipelineRasterizationStateCreateInfo rasterizer{};
  rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  rasterizer.depthClampEnable = VK_FALSE;
  rasterizer.rasterizerDiscardEnable = VK_FALSE;
  rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
  rasterizer.lineWidth = 1.0f;
  rasterizer.cullMode = VK_CULL_MODE_NONE;
  rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
  rasterizer.depthBiasEnable = VK_FALSE;

  VkPipelineMultisampleStateCreateInfo multisampling{};
  multisampling.sType =
      VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  multisampling.sampleShadingEnable = VK_FALSE;
  multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

  VkPipelineColorBlendAttachmentState colorBlendAttachment{};
  colorBlendAttachment.colorWriteMask =
      VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
      VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
  colorBlendAttachment.blendEnable = VK_FALSE;

  VkPipelineColorBlendStateCreateInfo colorBlending{};
  colorBlending.sType =
      VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
  colorBlending.logicOpEnable = VK_FALSE;
  colorBlending.logicOp = VK_LOGIC_OP_COPY;
  colorBlending.attachmentCount = 1;
  colorBlending.pAttachments = &colorBlendAttachment;
  colorBlending.blendConstants[0] = 0.0f;
  colorBlending.blendConstants[1] = 0.0f;
  colorBlending.blendConstants[2] = 0.0f;
  colorBlending.blendConstants[3] = 0.0f;

  VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
  pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  VK_CHECK_RESULT(vkCreatePipelineLayout(Device, &pipelineLayoutInfo, nullptr,
                                         &PipelineLayout));

  VkGraphicsPipelineCreateInfo pipelineInfo{};
  pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  pipelineInfo.stageCount = 2;
  pipelineInfo.pStages = shaderStages;
  pipelineInfo.pVertexInputState = &vertexInputInfo;
  pipelineInfo.pInputAssemblyState = &inputAssembly;
  pipelineInfo.pViewportState = &viewportState;
  pipelineInfo.pRasterizationState = &rasterizer;
  pipelineInfo.pMultisampleState = &multisampling;
  pipelineInfo.pColorBlendState = &colorBlending;
  pipelineInfo.layout = PipelineLayout;
  pipelineInfo.renderPass = RenderPass;
  pipelineInfo.subpass = 0;
  pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
  VK_CHECK_RESULT(vkCreateGraphicsPipelines(
      Device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &GraphicsPipeline));
}

// creating frame buffer: requires logical device, render pass and image views
void VkApp::create_frame_buffer() {
  unsigned nFrameBuffers = SwapChainImageViews.size();
  dbgs << "Initializing " << nFrameBuffers << " frame buffers\n";
  SwapChainFramebuffers.resize(nFrameBuffers);
  for (size_t i = 0; i < nFrameBuffers; i++) {
    VkImageView attachments[] = {SwapChainImageViews[i]};
    VkFramebufferCreateInfo framebufferInfo{};
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.renderPass = RenderPass;
    framebufferInfo.attachmentCount = 1;
    framebufferInfo.pAttachments = attachments;
    framebufferInfo.width = SwapChainExtent.width;
    framebufferInfo.height = SwapChainExtent.height;
    framebufferInfo.layers = 1;
    VK_CHECK_RESULT(vkCreateFramebuffer(Device, &framebufferInfo, nullptr,
                                        &SwapChainFramebuffers[i]));
  }
}

// requires logical device and queue families
void VkApp::create_command_pool() {
  VkCommandPoolCreateInfo poolInfo{};
  poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  poolInfo.queueFamilyIndex = GraphicsFamily;
  VK_CHECK_RESULT(
      vkCreateCommandPool(Device, &poolInfo, nullptr, &CommandPool));
}

template <typename T> size_t vectorsizeof(const std::vector<T> &vec) {
  return sizeof(T) * vec.size();
}

// TODO: maybe
unsigned VkApp::findMemoryType(unsigned typeFilter,
                               VkMemoryPropertyFlags properties) {
  VkPhysicalDeviceMemoryProperties memProperties;
  vkGetPhysicalDeviceMemoryProperties(PhysDevice, &memProperties);
  dbgs << memProperties.memoryTypeCount << " memory types found\n";
  for (unsigned i = 0; i < memProperties.memoryTypeCount; ++i)
    if ((typeFilter & (1 << i)) &&
        (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
      return i;
  throw std::runtime_error("failed to find suitable memory type!");
}

// TODO: buffer ctor?
void VkApp::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage,
                         VkMemoryPropertyFlags properties, VkBuffer &buffer,
                         VkDeviceMemory &bufferMemory) {
  VkBufferCreateInfo bufferInfo{};
  bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  bufferInfo.size = size;
  bufferInfo.usage = usage;
  bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  VK_CHECK_RESULT(vkCreateBuffer(Device, &bufferInfo, nullptr, &buffer));

  VkMemoryRequirements memRequirements;
  vkGetBufferMemoryRequirements(Device, buffer, &memRequirements);
  VkMemoryAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  allocInfo.allocationSize = memRequirements.size;
  allocInfo.memoryTypeIndex =
      findMemoryType(memRequirements.memoryTypeBits, properties);

  VK_CHECK_RESULT(vkAllocateMemory(Device, &allocInfo, nullptr, &bufferMemory));
  vkBindBufferMemory(Device, buffer, bufferMemory, 0);
}

// TODO: buffer copy ctor
void VkApp::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer,
                       VkDeviceSize size) {
  VkCommandBufferAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandPool = CommandPool;
  allocInfo.commandBufferCount = 1;

  VkCommandBuffer commandBuffer;
  vkAllocateCommandBuffers(Device, &allocInfo, &commandBuffer);

  VkCommandBufferBeginInfo beginInfo{};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

  vkBeginCommandBuffer(commandBuffer, &beginInfo);

  VkBufferCopy copyRegion{};
  copyRegion.size = size;
  vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

  vkEndCommandBuffer(commandBuffer);

  VkSubmitInfo submitInfo{};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &commandBuffer;

  vkQueueSubmit(GraphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
  vkQueueWaitIdle(GraphicsQueue);

  vkFreeCommandBuffers(Device, CommandPool, 1, &commandBuffer);
}

void VkApp::create_buffers() {
  // vertices and indices

  {
    VkDeviceSize bufferSize = vectorsizeof(Vertices);
    dbgs << "Vertex buffer size: " << bufferSize << std::endl;
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                     VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                 stagingBuffer, stagingBufferMemory);

    void *data;
    vkMapMemory(Device, stagingBufferMemory, 0, bufferSize, 0, &data);
    std::copy(Vertices.begin(), Vertices.end(), static_cast<Vertex *>(data));
    vkUnmapMemory(Device, stagingBufferMemory);

    createBuffer(
        bufferSize,
        VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VertexBuffer, VertexBufferMemory);

    copyBuffer(stagingBuffer, VertexBuffer, bufferSize);

    vkDestroyBuffer(Device, stagingBuffer, nullptr);
    vkFreeMemory(Device, stagingBufferMemory, nullptr);
  }

  {
    VkDeviceSize bufferSize = vectorsizeof(Indices);
    dbgs << "Index buffer size: " << bufferSize << std::endl;
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                     VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                 stagingBuffer, stagingBufferMemory);

    void *data;
    vkMapMemory(Device, stagingBufferMemory, 0, bufferSize, 0, &data);
    std::copy(Indices.begin(), Indices.end(),
              static_cast<unsigned short *>(data));
    vkUnmapMemory(Device, stagingBufferMemory);

    createBuffer(
        bufferSize,
        VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, IndexBuffer, IndexBufferMemory);

    copyBuffer(stagingBuffer, IndexBuffer, bufferSize);

    vkDestroyBuffer(Device, stagingBuffer, nullptr);
    vkFreeMemory(Device, stagingBufferMemory, nullptr);
  }
}

// requires: render pass, pipeline, swap chain, vertex buffers
void VkApp::create_command_buffers() {
  unsigned NFrameBufs = SwapChainFramebuffers.size();
  dbgs << "Number of command buffers: " << NFrameBufs << std::endl;
  CommandBuffers.resize(NFrameBufs);

  VkCommandBufferAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.commandPool = CommandPool;
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandBufferCount = NFrameBufs;

  VK_CHECK_RESULT(
      vkAllocateCommandBuffers(Device, &allocInfo, CommandBuffers.data()));

  for (unsigned i = 0; i < CommandBuffers.size(); ++i) {
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    VK_CHECK_RESULT(vkBeginCommandBuffer(CommandBuffers[i], &beginInfo));

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = RenderPass;
    renderPassInfo.framebuffer = SwapChainFramebuffers[i];
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = SwapChainExtent;

    VkClearValue clearColor = {{{0.2f, 0.3f, 0.3f, 1.0f}}};
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;

    vkCmdBeginRenderPass(CommandBuffers[i], &renderPassInfo,
                         VK_SUBPASS_CONTENTS_INLINE);
    vkCmdBindPipeline(CommandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS,
                      GraphicsPipeline);
    VkBuffer vertexBuffers[] = {VertexBuffer};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(CommandBuffers[i], 0, 1, vertexBuffers, offsets);
    vkCmdBindIndexBuffer(CommandBuffers[i], IndexBuffer, 0,
                         VK_INDEX_TYPE_UINT16);
    vkCmdDrawIndexed(CommandBuffers[i], Indices.size(), 1, 0, 0, 0);
    vkCmdEndRenderPass(CommandBuffers[i]);
    VK_CHECK_RESULT(vkEndCommandBuffer(CommandBuffers[i]));
  }
}

void VkApp::create_sync_objs() {
  ImageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
  RenderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
  InFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
  ImagesInFlight.resize(SwapChainImages.size(), VK_NULL_HANDLE);

  VkSemaphoreCreateInfo semaphoreInfo{};
  semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

  VkFenceCreateInfo fenceInfo{};
  fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

  for (unsigned i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
    VK_CHECK_RESULT(vkCreateSemaphore(Device, &semaphoreInfo, nullptr,
                                      &ImageAvailableSemaphores[i]));
    VK_CHECK_RESULT(vkCreateSemaphore(Device, &semaphoreInfo, nullptr,
                                      &RenderFinishedSemaphores[i]));
    VK_CHECK_RESULT(
        vkCreateFence(Device, &fenceInfo, nullptr, &InFlightFences[i]));
  }
}

void VkApp::update_swap_chain() {
  int width = 0, height = 0;
  glfwGetFramebufferSize(Wnd.get(), &width, &height);
  while (width == 0 || height == 0) {
    glfwGetFramebufferSize(Wnd.get(), &width, &height);
    glfwWaitEvents();
  }

  vkDeviceWaitIdle(Device);

  cleanup_swap_chain();

  create_swap_chain();
  create_image_views();
  create_render_pass();
  create_pipeline(StoredVertexID, StoredFragmentID);
  create_frame_buffer();
  create_command_buffers();

  ImagesInFlight.resize(SwapChainImages.size(), VK_NULL_HANDLE);
}

void VkApp::render_frame() {
  vkWaitForFences(Device, 1, &InFlightFences[CurrentFrame], VK_TRUE,
                  UINT64_MAX);

  unsigned imageIndex;
  VkResult result = vkAcquireNextImageKHR(
      Device, SwapChain, UINT64_MAX, ImageAvailableSemaphores[CurrentFrame],
      VK_NULL_HANDLE, &imageIndex);

  if (result == VK_ERROR_OUT_OF_DATE_KHR) {
    update_swap_chain();
    return;
  } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
    throw std::runtime_error("failed to acquire swap chain image!");
  }

  if (ImagesInFlight[imageIndex] != VK_NULL_HANDLE) {
    vkWaitForFences(Device, 1, &ImagesInFlight[imageIndex], VK_TRUE,
                    UINT64_MAX);
  }
  ImagesInFlight[imageIndex] = InFlightFences[CurrentFrame];

  VkSubmitInfo submitInfo{};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

  VkSemaphore waitSemaphores[] = {ImageAvailableSemaphores[CurrentFrame]};
  VkPipelineStageFlags waitStages[] = {
      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
  submitInfo.waitSemaphoreCount = 1;
  submitInfo.pWaitSemaphores = waitSemaphores;
  submitInfo.pWaitDstStageMask = waitStages;

  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &CommandBuffers[imageIndex];

  VkSemaphore signalSemaphores[] = {RenderFinishedSemaphores[CurrentFrame]};
  submitInfo.signalSemaphoreCount = 1;
  submitInfo.pSignalSemaphores = signalSemaphores;

  vkResetFences(Device, 1, &InFlightFences[CurrentFrame]);

  VK_CHECK_RESULT(vkQueueSubmit(GraphicsQueue, 1, &submitInfo,
                                InFlightFences[CurrentFrame]));

  VkPresentInfoKHR presentInfo{};
  presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

  presentInfo.waitSemaphoreCount = 1;
  presentInfo.pWaitSemaphores = signalSemaphores;

  VkSwapchainKHR swapChains[] = {SwapChain};
  presentInfo.swapchainCount = 1;
  presentInfo.pSwapchains = swapChains;

  presentInfo.pImageIndices = &imageIndex;

  result = vkQueuePresentKHR(PresentQueue, &presentInfo);

  if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
      FramebufferResized) {
    FramebufferResized = false;
    update_swap_chain();
  } else if (result != VK_SUCCESS) {
    throw std::runtime_error("failed to present swap chain image!");
  }

  CurrentFrame = (CurrentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void VkApp::run() {
  while (!glfwWindowShouldClose(Wnd.get())) {
    glfwPollEvents();
    render_frame();
  }
  vkDeviceWaitIdle(Device);
}

// read program code from file
std::vector<char> readFile(const char *Path) {
  std::ifstream ShaderFile;
  ShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  ShaderFile.open(Path, std::ios::binary);
  std::istreambuf_iterator<char> Start(ShaderFile), Fin;
  return std::vector<char>(Start, Fin);
}

// requires logical device
VkShaderModule VkApp::installShader(std::vector<char> ShaderCode) {
  VkShaderModuleCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  createInfo.codeSize = ShaderCode.size();
  createInfo.pCode = reinterpret_cast<const unsigned *>(ShaderCode.data());

  VkShaderModule ShaderID;
  VK_CHECK_RESULT(
      vkCreateShaderModule(Device, &createInfo, nullptr, &ShaderID));
  return ShaderID;
}

// entry point
int main() try {
  VkApp app;
  app.initialize_window();
  app.create_instance();
  app.peek_device();
  app.find_queues();
  app.create_logical_device();

  VkShaderModule VertexID = app.installShader(readFile(VERTNAME));
  VkShaderModule FragmentID = app.installShader(readFile(FRAGNAME));

  app.create_swap_chain();
  app.create_image_views();
  app.create_render_pass();
  app.create_pipeline(VertexID, FragmentID);
  app.create_frame_buffer();
  app.create_command_pool();
  app.create_buffers();
  app.create_command_buffers();
  app.create_sync_objs();
  app.run();

} catch (vulkan_error &E) {
  std::cerr << "Vulkan error: " << E.Res << " " << E.what() << std::endl;
  switch (E.Res) {
  case VK_ERROR_OUT_OF_HOST_MEMORY:
    std::cerr << "VK_ERROR_OUT_OF_HOST_MEMORY\n";
    break;
  case VK_ERROR_OUT_OF_DEVICE_MEMORY:
    std::cerr << "VK_ERROR_OUT_OF_DEVICE_MEMORY\n";
    break;
  case VK_ERROR_DEVICE_LOST:
    std::cerr << "VK_ERROR_DEVICE_LOST\n";
    break;
  }
} catch (glfw_error &E) {
  std::cerr << "GLFW error: " << E.what() << std::endl;
} catch (std::exception &E) {
  std::cerr << "Standard error: " << E.what() << std::endl;
} catch (...) {
  std::cerr << "Unknown error\n";
}
