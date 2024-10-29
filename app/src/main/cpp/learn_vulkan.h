#pragma once

#include "tools.h"
#include "Vertex.h"

#include <vulkan/vulkan.h>


class HelloVK
{
public:
    HelloVK();
    void initVulkan();
    void render();
    void cleanup();
    void cleanupSwapChain();
    void reset(ANativeWindow *newWindow, AAssetManager *newManager);
    bool initialized = false;

private:
    void createInstance();
    void createSurface();
    void setupDebugMessenger();
    void pickPhysicalDevice();
    void createLogicalDeviceAndQueue();
    void createSwapChain();
    void createSwapChainImageViews();

    void createRenderPass();
    void createDescriptorSetLayout();
    void createGraphicsPipeline();
    void createFramebuffers();
    void createCommandPool();
    void createCommandBuffer();
    void createSyncObjects();
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
    bool checkDeviceExtensionSupport(VkPhysicalDevice device);
    bool isDeviceSuitable(VkPhysicalDevice device);
    bool checkValidationLayerSupport();
    std::vector<const char *> getRequiredExtensions(bool enableValidation);
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

    VkShaderModule createShaderModule(const std::vector<uint8_t> &code);
    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
    void recreateSwapChain();
    void onOrientationChange();
    uint32_t findMemoryType(uint32_t typeFilter,
                            VkMemoryPropertyFlags properties);
    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage,
                      VkMemoryPropertyFlags properties, VkBuffer &buffer,
                      VkDeviceMemory &bufferMemory);
    void createUniformBuffers();
    void updateUniformBuffer(uint32_t currentImage);
    void createDescriptorPool();
    void createDescriptorSets();
    void establishDisplaySizeIdentity();
    void createVertexBuffer();
    void mapVertexBuffer();
    /*
     * In order to enable validation layer toggle this to true and
     * follow the README.md instructions concerning the validation
     * layers. You will be required to add separate vulkan validation
     * '*.so' files in order to enable this.
     *
     * The validation layers are not shipped with the APK as they are sizeable.
     */
    bool enableValidationLayers = true;

    const std::vector<const char *> validationLayers = {
            "VK_LAYER_KHRONOS_validation"};
    const std::vector<const char *> deviceExtensions =
            {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
            };
    std::unique_ptr<ANativeWindow, ANativeWindowDeleter> window;
    AAssetManager *assetManager;

    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;

    VkSurfaceKHR surface;

    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device;

    VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImages;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
    VkExtent2D displaySizeIdentity;
    std::vector<VkImageView> swapChainImageViews;
    std::vector<VkFramebuffer> swapChainFramebuffers;
    VkCommandPool commandPool;
    std::vector<VkCommandBuffer> commandBuffers;

    VkQueue graphicsQueue;
    VkQueue presentQueue;

    VkRenderPass renderPass;
    VkDescriptorSetLayout descriptorSetLayout;
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;

    std::vector<VkBuffer> uniformBuffers;
    std::vector<VkDeviceMemory> uniformBuffersMemory;

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    VkDescriptorPool descriptorPool;
    std::vector<VkDescriptorSet> descriptorSets;

    uint32_t currentFrame = 0;
    bool orientationChanged = false;
    VkSurfaceTransformFlagBitsKHR pretransformFlag;
    std::vector<Vertex> vertices;
    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
};