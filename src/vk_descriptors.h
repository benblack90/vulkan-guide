#pragma once

#include <vk_types.h>

struct DescriptorLayoutBuilder
{
	//this stores the bindings, ready to be built into actual vulkan objects. At this point, they're still config structs
	std::vector<VkDescriptorSetLayoutBinding> bindings;

	void add_binding(uint32_t binding, VkDescriptorType type);
	void clear();
	//The VkDescriptorSetLayout is the actual vulkan object we want to describe the layout of the final VkDescriptorSet.
	//This function builds one using the info structs stored in the 'bindings' vector
	VkDescriptorSetLayout build(VkDevice device, VkShaderStageFlags shaderStages, void* pNext = nullptr, VkDescriptorSetLayoutCreateFlags flags = 0);
};

//The purpose of this struct is to take a VkDescriptorSetLayout, and allocate memory, thus producing a VkDescriptorSet, the final object we need.
//Allocation happens via  VkDescriptorPool which, when reset, destroys all VkDescriptorSets allocated from it. 
struct DescriptorAllocator
{
	struct PoolSizeRatio
	{
		VkDescriptorType type;
		float ratio;
	};

	//Allocates the VkDescriptorSet
	VkDescriptorPool pool;

	void init_pool(VkDevice device, uint32_t maxSets, std::span<PoolSizeRatio> poolRatios);
	//This resets the VkDescriptorPool, which in turn borks all the VkDescriptorSets it allocated
	void clear_descriptors(VkDevice device);
	void destroy_pool(VkDevice device);

	VkDescriptorSet allocate(VkDevice device, VkDescriptorSetLayout layout);
};