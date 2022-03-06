#include "Context.hpp"

#include <iostream>

namespace ARE
{

Context::Context()
{
    VkInstanceCreateInfo createInfo{};
    VkResult result = vkCreateInstance(&createInfo, allocator, &instance);

    if (result == VK_SUCCESS)
    {
        std::cout << "VKSuccess"  << std::endl;
    }
    else 
    {
        std::cout << "VKFailure"  << std::endl;
    }
}

Context::~Context()
{
    std::cout << "vkdestroy" << std::endl;
    vkDestroyInstance(instance, allocator);
}



}