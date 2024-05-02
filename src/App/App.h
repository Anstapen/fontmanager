//
// Created by anton on 13.04.2024.
//

#ifndef FONTMANAGER_APP_H
#define FONTMANAGER_APP_H

#include "DemoWindow.h"

#include "GLFW/glfw3.h"
#include "imgui.h"




/**
 * @brief Includes for the Logging Utility, currently provided by the "plog" library (https://github.com/SergiusTheBest/plog.git)
 */
#include "plog/Log.h"
#include "plog/Init.h"

/* For now, we use two Logging Mechanisms, output to txt and console! */
#include "plog/Formatters/TxtFormatter.h"
#include "plog/Appenders/ColorConsoleAppender.h"
#include "plog/Appenders/RollingFileAppender.h"


class App {

public:
    App();

    virtual ~App();

    bool StartUp();

    void Run();

private:
    static void glfw_error_callback(int error, const char *description);

    void InitLogging(plog::Severity max_severity);

private:
    GLFWwindow *window;
    ImGuiIO *io;
    ImVec4 clear_color;

    /**
     * @brief private Data members for the Logging Utitlity.
     * Currently log files can be written to Console and txt File
     */
    plog::RollingFileAppender<plog::TxtFormatter> fileAppender;
    plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender;
};


#endif //FONTMANAGER_APP_H
