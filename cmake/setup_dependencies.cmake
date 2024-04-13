include(FetchContent)
# get GLFW using the FetchContent Module
function(setup_glfw)
    FETCHCONTENT_DECLARE(
            glfw
            GIT_REPOSITORY https://github.com/glfw/glfw.git
            GIT_TAG 3.4
    )
    FETCHCONTENT_MAKEAVAILABLE(glfw)

endfunction()


function(setup_dearimgui)
    FETCHCONTENT_DECLARE(
            dearimgui
            GIT_REPOSITORY https://github.com/ocornut/imgui.git
            GIT_TAG v1.90.5
    )

    FETCHCONTENT_MAKEAVAILABLE(dearimgui)

    # dear imgui is not set up using CMake, so we have to do that by ourselves
    set(dearimgui_file_location ${PROJECT_BINARY_DIR}/_deps/dearimgui-src)

    # add all the needed sources for the library
    add_library(dearimgui
            STATIC
            ${dearimgui_file_location}/imgui.cpp
            ${dearimgui_file_location}/imgui.h
            ${dearimgui_file_location}/imconfig.h
            ${dearimgui_file_location}/imgui_demo.cpp
            ${dearimgui_file_location}/imgui_draw.cpp
            ${dearimgui_file_location}/imgui_internal.h
            ${dearimgui_file_location}/imgui_tables.cpp
            ${dearimgui_file_location}/imgui_widgets.cpp
            ${dearimgui_file_location}/imstb_rectpack.h
            ${dearimgui_file_location}/imstb_textedit.h
            ${dearimgui_file_location}/imstb_truetype.h
            # TODO: make those conditional!
            ${dearimgui_file_location}/backends/imgui_impl_glfw.cpp
            ${dearimgui_file_location}/backends/imgui_impl_opengl3.cpp
            )
    target_include_directories(dearimgui
            PUBLIC
            ${dearimgui_file_location}
            ${dearimgui_file_location}/backends/imgui_impl_glfw.h
            ${dearimgui_file_location}/backends/imgui_impl_opengl3.h
            )

    target_link_libraries(dearimgui
            PRIVATE
            glfw
            )

    if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
        target_link_libraries(dearimgui
                PRIVATE
                -lgdi32
                -lopengl32
                -limm32
                )
    endif()

    if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
        target_link_libraries(dearimgui
                PRIVATE
                -lGL
                )
    endif()


endfunction()


function(setup_plog)
    FETCHCONTENT_DECLARE(
            plog
            GIT_REPOSITORY https://github.com/SergiusTheBest/plog.git
            GIT_TAG 1.1.10
    )

    FETCHCONTENT_MAKEAVAILABLE(plog)

endfunction()