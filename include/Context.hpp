#pragma once

#include <vulkan/vulkan.hpp>
#include <vector>

namespace ARE
{

struct VulkanContextCreateInfo
{
    std::vector<const char*> requiredExtensions;

    const char* appName;
    uint32_t appVersion;
};

class Context
{
private:
    VkInstance instance;

    const char* engineName = "Aphrodite's Rendering Engine";
    uint32_t engineVersion = VK_MAKE_VERSION(1, 0, 0);
    uint32_t apiVersion = VK_API_VERSION_1_0;

    VkApplicationInfo getAppInfo(const VulkanContextCreateInfo& contextCreateInfo);

public:
    Context(const VulkanContextCreateInfo& contextCreateInfo);
    ~Context();
};


}