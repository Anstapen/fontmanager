//
// Created by anton on 13.04.2024.
//

#include "App.h"

/* For fprintf */
#include <cstdio>

#include "DemoWindow.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"


DemoWindow test;

App::App() : window(nullptr), io(nullptr), clear_color(ImVec4(0.45f, 0.55f, 0.60f, 1.00f)),
             fileAppender("Log.txt", 1000000, 5) {
}

App::~App() {
    // Cleanup

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    DemoWindow::terminate();

    glfwDestroyWindow(window);
    glfwTerminate();
}

bool App::StartUp() {
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
        return false;
    }

    /* Basic GLFW Setup (https://learnopengl.com/Getting-started/Hello-Window for reference) */
    const char *glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only


    // Create window with graphics context
    this->window = glfwCreateWindow(1920, 1080, "Font Manager", nullptr, nullptr);
    if (this->window == nullptr) {
        return false;
    }


    glfwMakeContextCurrent(this->window);

    /* Initialize GLAD */
    if(!gladLoadGLLoader((GLADloadproc)(glfwGetProcAddress)))
    {
        PLOG_ERROR << "Failed to initialize GLAD";
        glfwTerminate();
        return -1;
    }

    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);
    glfwMakeContextCurrent(window);

    glViewport(0, 0, bufferWidth, bufferHeight);

    DemoWindow::create_triangle();
    DemoWindow::create_shaders();
    DemoWindow::create_framebuffer();


    glfwSwapInterval(1); // Enable vsync


    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io_ref = ImGui::GetIO();
    this->io = &io_ref;
    this->io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    this->io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    //this->io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    //this->io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    ImGuiStyle &style = ImGui::GetStyle();
    style.FrameRounding = 12.0f;


    /* Connect Logging */
    this->InitLogging(plog::warning);


    return true;

}

void App::Run() {

    while (!glfwWindowShouldClose(window)) {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui::NewFrame();

        ImGui::Begin("My Scene");

        const float window_width = ImGui::GetContentRegionAvail().x;
        const float window_height = ImGui::GetContentRegionAvail().y;

        DemoWindow::rescale_framebuffer(window_width, window_height);
        glViewport(0, 0, window_width, window_height);

        ImVec2 pos = ImGui::GetCursorScreenPos();

        ImGui::GetWindowDrawList()->AddImage(
                (void *)DemoWindow::texture_id,
                ImVec2(pos.x, pos.y),
                ImVec2(pos.x + window_width, pos.y + window_height),
                ImVec2(0, 1),
                ImVec2(1, 0)
        );

        ImGui::End();

        /* Only work if we are on the "docking branch" */
#if 0
        if (this->io->ConfigFlags & ImGuiConfigFlags_DockingEnable) {
            ImGui::DockSpaceOverViewport();
        }
#endif


        static bool show_demo_window = false;

        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("Menu")) {
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Tools")) {
                ImGui::MenuItem("Show Demo Window", NULL, &show_demo_window);
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }


        bool p_open;

        if (show_demo_window) {
            ImGui::ShowDemoWindow(&p_open);
        }


        // Rendering
        ImGui::Render();

        DemoWindow::bind_framebuffer();

        glUseProgram(DemoWindow::shader);
        glBindVertexArray(DemoWindow::VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
        glUseProgram(0);

        DemoWindow::unbind_framebuffer();


        io = &ImGui::GetIO();
#if 0
        if (io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            GLFWwindow *backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
#endif
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w,
                     clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }
}

void App::glfw_error_callback(int error, const char *description) {
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

void App::InitLogging(plog::Severity max_severity) {
    /* Connect the Logging System */
    plog::init(max_severity, &fileAppender).addAppender(&consoleAppender);
}

#if 0

	while (!glfwWindowShouldClose(mainWindow))
	{
		glfwPollEvents();


		ImGui::Render();




        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

		glfwSwapBuffers(mainWindow);
	}
}
#endif
