#pragma once 
#include <vk_types.h>

namespace vkutil {
	//In vulkan pipelines, shaders are set by building a VkShaderModule. This function does that. 
	bool load_shader_module(const char* filePath, VkDevice device, VkShaderModule* outShaderModule);

};