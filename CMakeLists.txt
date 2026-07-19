# Source - https://stackoverflow.com/a/51381672
# Posted by ConsistentProgrammer
# Retrieved 2026-07-18, License - CC BY-SA 4.0

cmake_minimum_required(VERSION 3.10)

project(TickParser)
include_directories(include)
file(GLOB SOURCES "src/*.cpp")
add_executable(TickParser ${SOURCES})
