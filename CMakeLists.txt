cmake_minimum_required (VERSION 3.21)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("Chappy++")

add_executable(Chappy8)
target_sources(Chappy8 PRIVATE
  src/Chappy8.cpp
  src/Chappy8.hpp
  src/chip8.cpp
  src/chip8.hpp)


target_include_directories(Chappy8 PRIVATE "${PROJECT_SOURCE_DIR}/ext/SDL2/include")
target_link_libraries(Chappy8 "${PROJECT_SOURCE_DIR}/ext/SDL2/lib/X64/SDL2.lib" "${PROJECT_SOURCE_DIR}/ext/SDL2/lib/x64/SDL2main.lib")

target_compile_features(Chappy8 PRIVATE cxx_std_20)
