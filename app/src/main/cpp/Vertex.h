#pragma once
#include <glm/glm.hpp>
#include <vulkan/vulkan.h>
#include <array>

struct Vertex {
    Vertex() = default;
    Vertex(glm::vec2 pos, glm::vec3 color);
    glm::vec2 pos;
    glm::vec3 color;

    static VkVertexInputBindingDescription getBindingDescription();
    static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions();
};
