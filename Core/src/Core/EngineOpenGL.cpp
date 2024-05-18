#include "EngineOpenGL.h"
#include "../input/InputManager.h"

#include "GuiLayer.h"

namespace libCore
{
	Scope<GuiLayer> guiLayer = nullptr; //esto se declara en el cpp porque si se pone en el h, hay errores de includes cíclicos

	


	void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		EventManager::OnWindowResizeEvent().trigger(width, height);
		glViewport(0, 0, width, height);
	}


	bool EngineOpenGL::InitializeEngine(const std::string& windowTitle,
		int initialWindowWidth,
		int initialWindowHeight,
		MainLoopFnc mainLoodFnc,
		CloseGlFnc closeGlFnc,
		ImGUILoopFnc imGUILoopFnc)
	{

		windowWidth = initialWindowWidth;
		windowHeight = initialWindowHeight;

		g_mainLoodFnc = mainLoodFnc;
		g_closeGlFnc = closeGlFnc;

		/* Initialize the library */
		if (!glfwInit())
		{
			return false;
		}

		//--GLFW: initialize and configure 
		glfwInit();
		const char* glsl_version = "#version 460";
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
		// ------------------------------


		//--GLFW window creation
		window = glfwCreateWindow(initialWindowWidth, initialWindowHeight, windowTitle.c_str(), NULL, NULL);
		if (window == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return false;
		}
		glfwMakeContextCurrent(window);
		SetupInputCallbacks();
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
		//-------------------------------------------------


		//--Glad: load all OpenGL function pointers
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			return false;
		}
		//-------------------------------------------------

		glEnable(GL_DEPTH_TEST); // Habilita el test de profundidad


		//glDepthFunc(GL_LEQUAL);
		//glDisable(GL_CULL_FACE);
		//glCullFace(GL_BACK);

		// --IMGUI
		if (imGUILoopFnc)
		{
			guiLayer = CreateScope<GuiLayer>(window, windowWidth, windowHeight);

			useImGUI = true;
			g_imGUILoopFnc = imGUILoopFnc;
		}
		// -------------------------------------------------

		InputManager::Instance().subscribe();

		// --Assets Manager & load Default Assets
		assetsManager.LoadDefaultAssets();

		return true;
	}

	void EngineOpenGL::SetupInputCallbacks()
	{
		glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			if (action == GLFW_PRESS)
			{
				InputManager::Instance().SetKeyPressed(key, true);
			}
			else if (action == GLFW_RELEASE) {
				InputManager::Instance().SetKeyPressed(key, false);
			}
			});

		glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods) {
			double x, y;
			glfwGetCursorPos(window, &x, &y);

			if (action == GLFW_PRESS) {
				InputManager::Instance().SetMousePressed(button, x, y, true);
			}
			else if (action == GLFW_RELEASE) {
				InputManager::Instance().SetMousePressed(button, x, y, false);
			}
			});

		glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos) {
			InputManager::Instance().SetMousePosition(xpos, ypos);
			});
	}

	void EngineOpenGL::StopMainLoop()
	{
		running = false;
	}

	void EngineOpenGL::begin()
	{
		if (useImGUI)
		{
			// Start the Dear ImGui frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
		}
	}

	void EngineOpenGL::InitializeMainLoop()
	{
		Timestep lastFrameTime = glfwGetTime();

		running = true;

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window) && running)
		{
			//DELTA TIME
			Timestep currentFrameTime = glfwGetTime();
			m_deltaTime = currentFrameTime - lastFrameTime;
			lastFrameTime = currentFrameTime;

			//WINDOW SIZE
			GLint windowWidth, windowHeight;
			glfwGetWindowSize(window, &windowWidth, &windowHeight);
			glViewport(0, 0, windowWidth, windowHeight);

			//START INPUT UPDATE
			InputManager::Instance().Update();

			//MAIN LOOP FUNCTION CALL
			if (g_mainLoodFnc)
			{
				g_mainLoodFnc(m_deltaTime);
			}
			// -------------------------------------------

			//-- ImGUI
			if (useImGUI)
			{
				guiLayer->begin();
				guiLayer->renderDockers();

				if (g_imGUILoopFnc)
				{
					g_imGUILoopFnc();
				}

				guiLayer->end();
			}
			// -------------------------------------------

			//END INPUT UPDATE
			InputManager::Instance().EndFrame();

			// Actualiza la ventana
			glfwPollEvents();

			// Intercambia los buffers
			glfwSwapBuffers(window);
		}

		if (g_closeGlFnc)
		{
			g_closeGlFnc();
		}

		glfwTerminate();
	}
	// -------------------------------------------------





	//--VIEWPORTS
	void EngineOpenGL::CreateViewport(std::string name, glm::vec3 cameraPosition)
	{
		guiLayer->CreateViewport(name, cameraPosition);
	}


	void EngineOpenGL::RenderViewports(const std::vector<Ref<libCore::ModelContainer>>& modelsInScene)
	{
		for (auto& viewport : guiLayer->viewports)
		{
			viewport->framebuffer->bindFBO();
			viewport->camera->Inputs(m_deltaTime);
			viewport->camera->updateMatrix(45.0f, 0.1f, 1000.0f);

			// Clear
			glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			//-- Normal Draw
			libCore::ShaderManager::Get("basic")->use();
			libCore::ShaderManager::Get("basic")->setMat4("camMatrix", viewport->camera->cameraMatrix);


			for (auto& modelContainer : modelsInScene)
			{
				modelContainer->Draw("basic");
			}
			viewport->framebuffer->unbindFBO(); // Desvincula el framebuffer para renderizar en la pantalla o en el próximo FBO
		}
	}
	void EngineOpenGL::DrawViewports()
	{
		guiLayer->DrawViewports();
	}
	// -------------------------------------------------



	//--PANELS
	void libCore::EngineOpenGL::DrawHierarchyPanel(const std::vector<Ref<libCore::ModelContainer>>& modelsInScene)
	{
		guiLayer->DrawHierarchyPanel(modelsInScene);
	}
}
