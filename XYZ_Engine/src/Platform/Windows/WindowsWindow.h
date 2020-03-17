#pragma once
#include "XYZ/Renderer/APIContext.h"
#include "XYZ/Core/Window.h"

#include <GLFW/glfw3.h>
namespace XYZ {
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProperties& props);
		virtual ~WindowsWindow();

		virtual void Update() override;
		virtual void SetVSync(bool enabled) override;
		virtual bool IsClosed() override;

		inline void SetEventCallback(const std::function<void(Event&)>& callback) override { m_Data.EventCallback = callback; }
		inline virtual void* GetNativeWindow() const { return m_Window; }

		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }
	private:
		virtual void Destroy();

		GLFWwindow* m_Window;
		std::unique_ptr<APIContext> m_Context;

		struct WindowData
		{
			std::string Title;
			unsigned int Width = 0;
			unsigned int Height = 0;
			bool VSync = false;

			std::function<void(Event&)> EventCallback;
		};

		WindowData m_Data;
	};
}