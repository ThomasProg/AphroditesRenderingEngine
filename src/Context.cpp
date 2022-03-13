#include "Context.hpp"

#include <iostream>

namespace ARE
{

Context::Context(const VulkanContextCreateInfo& contextCreateInfo)
{
    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

    VkApplicationInfo appInfo = getAppInfo(contextCreateInfo);

    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = (uint32_t) contextCreateInfo.requiredExtensions.size();
    createInfo.ppEnabledExtensionNames = contextCreateInfo.requiredExtensions.data();

    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
    {
        throw std::runtime_error("Instance could not be created.");
    }
}

VkApplicationInfo Context::getAppInfo(const VulkanContextCreateInfo& contextCreateInfo)
{
    VkApplicationInfo appInfo;
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = contextCreateInfo.appName;
    appInfo.applicationVersion = contextCreateInfo.appVersion;
    appInfo.pEngineName = engineName;
    appInfo.engineVersion = engineVersion;
    appInfo.apiVersion = apiVersion;
    return appInfo;
}

Context::~Context()
{
    vkDestroyInstance(instance, nullptr);
}



}