#pragma once

#include "../LibCoreHeaders.h"

#include "../Core/FBO.hpp"
#include "../tools/EventManager.h"
#include "../input/InputManager.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "../Timestep.h"
#include "../tools/Camera.h"

namespace libCore
{
	class Viewport
	{
	public:

		std::string viewportName = "no_name_viewport";

		Scope<libCore::Camera> camera;
		Scope<libCore::FBO> framebuffer;
		
		ImVec2 viewportSize = ImVec2(800.0f, 600.0f);
		ImVec2 viewportPos = ImVec2(0.0f, 0.0f);
		ImVec2 previousViewportSize = ImVec2(800.0f, 600.0f);
		ImVec2 previousViewportPos = ImVec2(0.0f, 0.0f);

		// Banderas de estado
		bool isResizing = false;
		bool isMoving = false;
		bool mouseInviewport = false;

		// Constructor predeterminado
		Viewport()
		{
			//--RESIZE SCENE PANEL EVENT
			EventManager::OnPanelResizedEvent().subscribe([this](const std::string name, const glm::vec2& size, const glm::vec2& position)
			{
				framebuffer->resize(size.x, size.y);
				camera->width = static_cast<int>(size.x);
				camera->height = static_cast<int>(size.y);
			});
			//---------------------------------------------------
		}

		Viewport(const std::string& name) :viewportName(name), viewportSize(0, 0), viewportPos(0, 0), previousViewportSize(0, 0), previousViewportPos(0, 0), mouseInviewport(false) {}
	};

    class GuiLayer {

    public:

        GuiLayer(GLFWwindow* window, float window_W, float window_H);
        ~GuiLayer();

        void begin();
        void end();
        void renderDockers();
        void renderMainMenuBar();


        //Viewports
        std::vector<Scope<Viewport>> viewports;
        void CreateViewport(std::string name, glm::vec3 cameraPosition);
        void DrawViewports();

		//Panels
		void DrawHierarchyPanel(const std::vector<Ref<libCore::ModelContainer>>& modelsInScene);

    private:
        bool ini_file_exists;

        float m_window_W = 0.0f;
        float m_window_H = 0.0f;

    };


	
}