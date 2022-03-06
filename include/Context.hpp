#pragma once

#include <vulkan/vulkan.hpp>

#include <glm/matrix.hpp>

struct Component 
{
    glm::mat4 model;
};

namespace ARE
{

class Context
{
private:
    VkAllocationCallbacks* allocator = nullptr;
    VkInstance instance;

public:
    Context();
    ~Context();
};


}