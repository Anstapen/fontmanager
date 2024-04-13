set(dearimgui_file_location ${PROJECT_BINARY_DIR}/_deps/dearimgui-src)

add_executable(App
        main.cpp
        main.h
        App.cpp
        App.h)


target_link_libraries(App
        PRIVATE
        dearimgui
        glfw
        plog
        )