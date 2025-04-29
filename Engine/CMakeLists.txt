cmake_minimum_required(VERSION 3.31)

project(Engine)

include(../CMake/CommonMacros.txt)

SETUP_APP(Daisuke_Engine "DaisukeEngine")

target_link_libraries(Daisuke_Engine SharedUtils)