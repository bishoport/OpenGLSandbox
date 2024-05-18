#include "GuiLayer.h"
#include "../tools/ShaderManager.h"

namespace libCore
{
    GuiLayer::GuiLayer(GLFWwindow* window, float window_W, float window_H)
    {
        m_window_W = window_W;
        m_window_H = window_H;

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        // Enable Docking
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags  |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
        io.ConfigFlags  |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
        //io.ConfigFlags  |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
        io.Fonts->Flags |= ImFontAtlasFlags_NoBakedLines;
        io.ConfigFlags  |= ImGuiConfigFlags_DpiEnableScaleViewports;
        io.ConfigFlags  |= ImGuiConfigFlags_DpiEnableScaleFonts;
        
        io.ConfigWindowsMoveFromTitleBarOnly = true;
        io.ConfigDragClickToInputText = true;
        io.ConfigDockingTransparentPayload = true;

        // Setup Platform/Renderer bindings
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 460");

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();

        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }


        //--ImGUI Configuration--------------------
        //std::ifstream ifile("imgui.ini");
        //ini_file_exists = ifile.is_open();
        //ifile.close();
        //
        //if (ini_file_exists)
        //    ImGui::LoadIniSettingsFromDisk("imgui.ini");

        //FONTS
        //io.Fonts->AddFontFromFileTTF("assets/default/Fonts/sourceSans/SourceSansPro-Regular.otf", 18);
        //io.Fonts->AddFontFromFileTTF("assets/default/Fonts/source-code-pro/SourceCodePro-Regular.otf", 26);

        // Despu�s de cargar las fuentes, llama a esta funci�n
        //ImGui_ImplOpenGL3_CreateFontsTexture();
    }

    GuiLayer::~GuiLayer() {
        // Cleanup
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }



    void GuiLayer::begin() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }




    void GuiLayer::renderDockers()
    {
        // Configuraci�n del Docking
        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

        renderMainMenuBar();

        // Ventana de dock
        ImGui::SetNextWindowPos(ImVec2(0, 20));
        //ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize); //Sin menu TOP
        ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y - 20));
        ImGui::Begin("Docking Example", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
    }

    void GuiLayer::renderMainMenuBar() {

        // Barra de men� principal
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Open", "Ctrl+O")) { /* Abrir archivo */ }
                if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Guardar archivo */ }
                //if (ImGui::MenuItem("Exit", "Ctrl+Q")) { glfwSetWindowShouldClose(window, true); }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Edit"))
            {
                if (ImGui::MenuItem("Undo", "Ctrl+Z")) { /* Deshacer */ }
                if (ImGui::MenuItem("Redo", "Ctrl+Y")) { /* Rehacer */ }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
    }

    void GuiLayer::CreateViewport(std::string name, glm::vec3 cameraPosition)
    {
        auto viewport = CreateScope<Viewport>();
        viewport->viewportName = name;

        auto fbo = CreateScope<FBO>();
        fbo->init(viewport->viewportSize.x, viewport->viewportSize.y);
        viewport->framebuffer = std::move(fbo);

        viewport->camera = CreateScope <libCore::Camera>(viewport->viewportSize.x, viewport->viewportSize.y, cameraPosition);

        viewports.push_back(std::move(viewport));
    }

    void GuiLayer::DrawViewports()
    {
        for (auto& viewport : viewports)
        {
            std::string windowTitle = viewport->viewportName;
            ImGui::Begin(windowTitle.c_str(), nullptr);

            // Obtener los tama�os y posiciones actuales de la ventana
            ImVec2 currentViewportSize = ImGui::GetWindowSize();
            ImVec2 currentViewportPos = ImGui::GetWindowPos();

            // Verificar si se est� arrastrando la ventana
            bool isCurrentlyDragging = ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows) && ImGui::IsMouseDragging(ImGuiMouseButton_Left);

            // Verificar si el tama�o ha cambiado
            bool isCurrentlyResizing = currentViewportSize.x != viewport->previousViewportSize.x || currentViewportSize.y != viewport->previousViewportSize.y;

            // Detectar cu�ndo se comienza a mover o redimensionar
            if (!viewport->isMoving && isCurrentlyDragging)
            {
                viewport->isMoving = true;
            }
            if (!viewport->isResizing && isCurrentlyResizing)
            {
                viewport->isResizing = true;
            }

            // Detectar cu�ndo termina el movimiento o el redimensionamiento
            if (viewport->isMoving && !isCurrentlyDragging)
            {
                viewport->isMoving = false;
                EventManager::OnPanelResizedEvent().trigger(viewport->viewportName, glm::vec2(currentViewportSize.x, currentViewportSize.y),
                    glm::vec2(currentViewportPos.x, currentViewportPos.y));
            }
            if (viewport->isResizing && !isCurrentlyResizing)
            {
                viewport->isResizing = false;
                EventManager::OnPanelResizedEvent().trigger(viewport->viewportName, glm::vec2(currentViewportSize.x, currentViewportSize.y),
                    glm::vec2(currentViewportPos.x, currentViewportPos.y));
            }

            // Actualiza los valores anteriores
            viewport->previousViewportSize = currentViewportSize;
            viewport->previousViewportPos = currentViewportPos;

            // Actualiza los valores actuales en el viewport
            viewport->viewportSize = currentViewportSize;
            viewport->viewportPos = currentViewportPos;

            // Renderiza la imagen
            ImGui::Image((void*)(intptr_t)viewport->framebuffer->getTexture("color"), ImVec2(viewport->viewportSize.x, viewport->viewportSize.y), ImVec2(0, 1), ImVec2(1, 0), ImColor(255, 255, 255, 255));
            viewport->mouseInviewport = false;
            ImGui::End();
        }
    }

    void GuiLayer::DrawHierarchyPanel(const std::vector<Ref<libCore::ModelContainer>>& modelsInScene)
    {
        ImGui::Begin("Hierarchy");
        for (auto& modelContainer : modelsInScene) {

            if (ImGui::TreeNode(modelContainer.get(), "Model: %s", modelContainer->name.c_str())) {
                for (int j = 0; j < modelContainer->models.size(); j++) {
                    if (ImGui::TreeNode(modelContainer->models[j].get(), "Child: %d", j)) {
                        // Aqu� listamos las Meshes de cada Model
                        auto& model = modelContainer->models[j];
                        for (int k = 0; k < model->meshes.size(); k++) {
                            ImGui::BulletText("Mesh: %s", model->meshes[k]->meshName.c_str());
                            ImGui::BulletText("Transform:");
                            ImGui::DragFloat3("Position", &model->transform.position[0], 0.1f);
                            ImGui::DragFloat3("Rotation", &model->transform.rotation[0], 0.01f, -3.14159f, 3.14159f, "%.3f rad");
                            ImGui::DragFloat3("Scale", &model->transform.scale[0], 0.1f, 0.01f, 100.0f, "%.3f");
                        }
                        ImGui::TreePop(); // Finaliza el nodo del Model
                    }
                }
                ImGui::TreePop(); // Finaliza el nodo del ModelContainer
            }
        }
        ImGui::End();
    }


    void GuiLayer::end()
    {
        ImGui::End(); // End of Docking Example window

        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2(m_window_W, m_window_H);
        
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
}
