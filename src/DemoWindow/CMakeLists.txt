add_library(DemoWindow
        DemoWindow.h
        DemoWindow.cpp)


target_link_libraries(DemoWindow
        PRIVATE
        dearimgui
        plog
        )

target_link_libraries(DemoWindow
        PUBLIC
        glad
        glfw
        )

target_include_directories(DemoWindow
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        )