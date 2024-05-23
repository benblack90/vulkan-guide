// vulkan_guide.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <vk_types.h>
#include "vk_descriptors.h"
#include <vk_loader.h>

struct DeletionQueue
{
	std::deque<std::function<void()>> deletors;
	void push_function(std::function<void()>&& func)
	{
		deletors.push_back(func);
	}

	void flush()
	{
		//reverse-iterate the deletion queue to execute all the functions in reverse order
		for(auto it = deletors.rbegin(); it != deletors.rend(); it++)
		{
			(*it)();//call functors
		}
		deletors.clear();
	}
};

struct FrameData
{
	VkCommandPool _commandPool;
	VkCommandBuffer _mainCommandBuffer;
	VkSemaphore _swapchainSemaphore;
	VkSemaphore _renderSemaphore;
	VkFence _renderFence;
	DeletionQueue _deletionQueue;
};

struct ComputePushConstants
{
	glm::vec4 data1;
	glm::vec4 data2;
	glm::vec4 data3;
	glm::vec4 data4;
};

struct ComputeEffect
{
	const char* name;
	VkPipeline pipeline;
	VkPipelineLayout layout;
	ComputePushConstants data;
};

constexpr unsigned int FRAME_OVERLAP = 2;

class VulkanEngine {
public:

	bool _isInitialized{ false };
	int _frameNumber {0};
	bool stop_rendering{ false };
	VkExtent2D _windowExtent{ 1700 , 900 };
	VkInstance _instance;
	VkDebugUtilsMessengerEXT _debug_messenger;
	VkPhysicalDevice _chosenGPU;
	VkDevice _device;
	VkSurfaceKHR _surface;
	VkSwapchainKHR _swapchain;
	VkFormat _swapchainImageFormat;
	DescriptorAllocator _globalDesriptorAllocator;
	VkPipeline _gradientPipeline;
	VkPipelineLayout _gradientPipelineLayout;
	VkPipelineLayout _meshPipelineLayout;
	VkPipeline _meshPipeline;

	GPUMeshBuffers rectangle;

	VkDescriptorSet _drawImageDescriptors;
	VkDescriptorSetLayout _drawImageDescriptorLayout;

	//immediate submit structures
	VkFence _immFence;
	VkCommandBuffer _immCommandBuffer;
	VkCommandPool _immCommandPool;

	//handles to the actual images
	std::vector<VkImage> _swapchainImages;
	//wrappers for those images
	std::vector<VkImageView> _swapchainImageViews;
	VkExtent2D _swapchainExtent;

	FrameData _frames[FRAME_OVERLAP];
	VkQueue _graphicsQueue;
	uint32_t _graphicsQueueFamily;

	//draw resources
	AllocatedImage _drawImage;
	AllocatedImage _depthImage;
	VkExtent2D _drawExtent;
	float renderScale = 1.0f;

	std::vector<std::shared_ptr<MeshAsset>> testMeshes;

	FrameData& get_current_frame() { return _frames[_frameNumber % FRAME_OVERLAP]; };

	struct SDL_Window* _window{ nullptr };

	static VulkanEngine& Get();

	//initializes everything in the engine
	void init();

	//shuts down the engine
	void cleanup();

	//draw loop
	void draw();

	//run main loop
	void run();

	void immediate_submit(std::function<void(VkCommandBuffer cmd)>&& function);
	GPUMeshBuffers upload_mesh(std::span<uint32_t> indices, std::span<Vertex> vertices);
	bool resize_requested{ false };

private:
	DeletionQueue _mainDeletionQueue;
	VmaAllocator _allocator;
	std::vector<ComputeEffect> backgroundEffects;
	int currentBackgroundEffect{ 0 };
	void init_vulkan();
	void init_swapchain();
	void init_commands();
	void init_sync_structures();
	void init_descriptors();
	void init_pipelines();
	void init_background_pipelines();
	void init_imgui();
	void init_mesh_pipeline();
	void init_default_data();
	void draw_background(VkCommandBuffer cmd);
	void draw_imgui(VkCommandBuffer cmd, VkImageView targetImageView);
	void draw_geometry(VkCommandBuffer cmd);
	void create_swapchain(uint32_t width, uint32_t height);
	void destroy_swapchain();	
	void resize_swapchain();
	AllocatedBuffer create_buffer(size_t allocSize, VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage);
	void destroy_buffer(const AllocatedBuffer& buffer);
};
