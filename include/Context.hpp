#pragma once

#include <vulkan/vulkan.hpp>
#include <vector>

namespace ARE
{

struct VulkanContextDebugCreationInfo
{
    VkDebugUtilsMessageSeverityFlagsEXT messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
                                                        // | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT
                                                        | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
                                                        | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

    VkDebugUtilsMessageTypeFlagsEXT messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT 
                                                | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT 
                                                | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
};

struct VulkanContextCreateInfo
{
    std::vector<const char*> requiredExtensions;

    const char* appName;
    uint32_t appVersion;

    VulkanContextDebugCreationInfo* debugInfo = nullptr;
};

class Context
{
private:
    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;
    VkPhysicalDevice physicalDevice;
    uint32_t allOpQueueFamily;
    VkQueue allOpQueue;
    VkDevice logicalDevice;

    bool isDebugEnabled;

    const char* engineName = "Aphrodite's Rendering Engine";
    uint32_t engineVersion = VK_MAKE_VERSION(1, 0, 0);
    uint32_t apiVersion = VK_API_VERSION_1_0;

    VkApplicationInfo getAppInfo(const VulkanContextCreateInfo& contextCreateInfo);
    VkDebugUtilsMessengerCreateInfoEXT getDebugUtilsMessengerCreateInfo(const VulkanContextCreateInfo& contextCreateInfo);

    VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, 
                                        const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, 
                                        const VkAllocationCallbacks* pAllocator, 
                                        VkDebugUtilsMessengerEXT* pDebugMessenger);

    void DestroyDebugUtilsMessengerEXT(VkInstance instance, 
                                        VkDebugUtilsMessengerEXT debugMessenger, 
                                        const VkAllocationCallbacks* pAllocator); 

    void createDebugContext(const VulkanContextCreateInfo& contextCreateInfo);
    void createReleaseContext(const VulkanContextCreateInfo& contextCreateInfo);

    void findPhysicalDevice(VkPhysicalDevice& outPhysicalDevice, uint32_t& allOpFamilyQueue);
    VkDevice createLogicalDevice();

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, 
                                                        VkDebugUtilsMessageTypeFlagsEXT messageType, 
                                                        const VkDebugUtilsMessengerCallbackDataEXT* callbackData, 
                                                        void* userData);

public:
    Context(const VulkanContextCreateInfo& contextCreateInfo);
    ~Context();

    void printDebugMessage(const char* message);
};


}