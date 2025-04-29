cmake_minimum_required(VERSION 3.31)

project(Engine)

include(../CMake/CommonMacros.txt)

SETUP_APP(Engine "DaisukeEngine")

target_link_libraries(Engine SharedUtils)