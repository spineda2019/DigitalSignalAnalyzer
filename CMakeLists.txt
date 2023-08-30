# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project (QtCMake VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_STANDARD_REQUIRED true)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Multimedia)
qt_standard_project_setup()

qt_add_executable(QtCMake 
					"src/QtCMake.cpp" 
					"src/MainWidget.ui" 
					"src/MainWidget.cpp"
					"include/MainWidget.h")

add_subdirectory(DataframeLib)

target_link_libraries(QtCMake PRIVATE Qt6::Widgets Qt6::Multimedia)
target_link_libraries(QtCMake PUBLIC DataframeLib)

target_include_directories(QtCMake PUBLIC "${PROJECT_SOURCE_DIR}/DataframeLib")
target_include_directories(QtCMake PUBLIC "include")

set_target_properties(QtCMake PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)
