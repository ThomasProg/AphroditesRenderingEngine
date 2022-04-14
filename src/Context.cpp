#include "Context.hpp"

#include <iostream>

namespace ARE
{

Context::Context(const VulkanContextCreateInfo& contextCreateInfo)
{
    isDebugEnabled = contextCreateInfo.debugInfo != nullptr;

    if (isDebugEnabled)
        createDebugContext(contextCreateInfo);
    else
        createReleaseContext(contextCreateInfo);
}

void Context::createDebugContext(const VulkanContextCreateInfo& contextCreateInfo)
{
    uint32_t nbLayers;
    vkEnumerateInstanceLayerProperties(&nbLayers, nullptr);
    std::vector<VkLayerProperties> availableLayers(nbLayers);
    vkEnumerateInstanceLayerProperties(&nbLayers, availableLayers.data());

    auto predicate = [](const VkLayerProperties& availableLayer) { return (strcmp(availableLayer.layerName, "VK_LAYER_KHRONOS_validation") == 0); };
    if (std::find_if(availableLayers.begin(), availableLayers.end(), predicate) == availableLayers.end())
    {
        throw std::runtime_error("Validations layers are not available for debugging.");
    }

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

    VkApplicationInfo appInfo = getAppInfo(contextCreateInfo);
    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = getDebugUtilsMessengerCreateInfo(contextCreateInfo);

    std::vector<const char*> requiredExtensions = contextCreateInfo.requiredExtensions;
    requiredExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = (uint32_t) requiredExtensions.size();
    createInfo.ppEnabledExtensionNames = requiredExtensions.data();
    createInfo.pNext = &debugCreateInfo;

    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
    {
        throw std::runtime_error("Instance could not be created.");
    }

    if (CreateDebugUtilsMessengerEXT(instance, &debugCreateInfo, nullptr, &debugMessenger) != VK_SUCCESS) 
    {
        vkDestroyInstance(instance, nullptr);

        throw std::runtime_error("Failed to create debug messenger!");
    }
}

VkDebugUtilsMessengerCreateInfoEXT Context::getDebugUtilsMessengerCreateInfo(const VulkanContextCreateInfo& contextCreateInfo)
{
    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = {};
    debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    debugCreateInfo.messageSeverity = contextCreateInfo.debugInfo->messageSeverity;
    debugCreateInfo.messageType = contextCreateInfo.debugInfo->messageType;
    debugCreateInfo.pfnUserCallback = debugCallback;
    debugCreateInfo.pUserData = this;
    return debugCreateInfo;
}


VKAPI_ATTR VkBool32 VKAPI_CALL Context::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* callbackData, void* userData) 
{
    Context* context = static_cast<Context*>(userData);

    context->printDebugMessage(callbackData->pMessage);

    return VK_FALSE;
}

void Context::printDebugMessage(const char* message)
{
    std::cout << "--- Vulkan : " << message << std::endl;
}

void Context::createReleaseContext(const VulkanContextCreateInfo& contextCreateInfo)
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
    if (isDebugEnabled) 
	    DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);

    vkDestroyInstance(instance, nullptr);
}


VkResult Context::CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) 
{
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) 
    {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } 
    else 
    {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void Context::DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) 
{
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) 
    {
        func(instance, debugMessenger, pAllocator);
    }
}

}