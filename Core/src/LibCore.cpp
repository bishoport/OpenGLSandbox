#include "LibCore.h"
#include "input/InputManager.h"

namespace libCore
{
	//OPENGL VARS
	GLFWwindow* window;

	//callback func mainLoop
	MainLoopFnc		g_mainLoodFnc;
	CloseGlFnc		g_closeGlFnc;
	KeyPressedFnc	g_keyPressedFnc;
	ImGUILoopFnc	g_imGUILoopFnc;

	//Varialbles de estado
	bool running = false;
	bool useImGUI = false;

	void SetupInputCallbacks()
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
	
	bool InitializeEngine(const std::string& windowTitle, int initialWindowWidth, int initialWindowHeight,MainLoopFnc mainLoodFnc, CloseGlFnc closeGlFnc, ImGUILoopFnc imGUILoopFnc)
	{
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
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
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
			useImGUI = true;
			g_imGUILoopFnc = imGUILoopFnc;

			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO();
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
			io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

			ImGui::StyleColorsDark();
			//ImGui::StyleColorsClassic();

			ImGui_ImplGlfw_InitForOpenGL(window, true);
			ImGui_ImplOpenGL3_Init("#version 460");

			////--ImGUI Configuration--------------------
			//std::ifstream ifile("imgui.ini");
			//ini_file_exists = ifile.is_open();
			//ifile.close();

			//if (ini_file_exists)
			//	ImGui::LoadIniSettingsFromDisk("imgui.ini");
		}
		// -------------------------------------------------

		InputManager::Instance().subscribe();

		return true;
	}


	void StopMainLoop()
	{
		running = false;
	}

	void begin() 
	{
		if (useImGUI)
		{
			// Start the Dear ImGui frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
		}
	}

	void InitializeMainLoop()
	{
		Timestep lastFrameTime = glfwGetTime();

		running = true;

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window) && running)
		{
			//DELTA TIME
			Timestep currentFrameTime = glfwGetTime();
			Timestep deltaTime = currentFrameTime - lastFrameTime;
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
				g_mainLoodFnc(deltaTime);
			}
			// -------------------------------------------


			//-- ImGUI
			if (useImGUI)
			{
				ImGui_ImplOpenGL3_NewFrame();
				ImGui_ImplGlfw_NewFrame();
				ImGui::NewFrame();

				if (g_mainLoodFnc)
				{
					g_imGUILoopFnc();
				}


				ImGuiIO& io = ImGui::GetIO();
				io.DisplaySize = ImVec2((float)windowWidth, (float)windowHeight);

				ImGui::Render();
				ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

				if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
				{
					GLFWwindow* backup_current_context = glfwGetCurrentContext();
					ImGui::UpdatePlatformWindows();
					ImGui::RenderPlatformWindowsDefault();
					glfwMakeContextCurrent(backup_current_context);
				}
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

	void SetWindowResizable(bool resizable)
	{
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	}


	void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}
}