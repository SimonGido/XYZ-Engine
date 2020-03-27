#include <memory>
#include <glm/glm.hpp>

#include "RendererAPI.h"


namespace XYZ {
	class RenderCommand
	{
	public:
		inline static void Init()
		{
			s_RendererAPI->Init();
		}

		inline static void SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			s_RendererAPI->SetViewport(x, y, width, height);
		}
		inline static void SetClearColor(const glm::vec4& color)
		{
			s_RendererAPI->SetClearColor(color);
		}
		inline static void Clear()
		{
			s_RendererAPI->Clear();
		}
		inline static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray,uint32_t indexCount = 0)
		{
			s_RendererAPI->DrawIndexed(vertexArray, indexCount);
		}
		inline static void DrawInstanced(const std::shared_ptr<VertexArray>& vertexArray,uint32_t count)
		{
			s_RendererAPI->DrawInstanced(vertexArray, count);
		}

	private:
		static std::unique_ptr<RendererAPI> s_RendererAPI;
	};

}