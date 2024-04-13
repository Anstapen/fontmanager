cmake_minimum_required(VERSION 3.19)


project(fontmanager)

# get GLFW
include(cmake/setup_dependencies.cmake)


# Multi platform library for OpenGL Application Development
# https://github.com/glfw/glfw
setup_glfw()


# Immediate mode Graphical User Interface library
# https://github.com/ocornut/imgui
setup_dearimgui()


# Simple Logging Library
# https://github.com/SergiusTheBest/plog
setup_plog()


add_subdirectory(src)