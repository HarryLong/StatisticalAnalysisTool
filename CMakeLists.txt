cmake_minimum_required(VERSION 2.8.11)
project(STATISTICAL-ANALYSIS-TOOL)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

# QT Imports
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)

# Link directories
include_directories("/home/harry/workspaces/qt-workspace/spatial-hashmap/")
link_directories("/home/harry/workspaces/qt-workspace/spatial-hashmap/")

add_subdirectory("test")

set(CMAKE_AUTOMOC ON)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCC_GL_DEBUG_ -DDEBUG_ON -Wall -g -std=c++11 -Wno-reorder -Wno-unused-variable") #DEBUG
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEBUG_ON -O3 -Wall -std=c++11 -Wno-reorder -Wno-unused-variable") #DEBUG
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pg -std=c++11 -pthread -Wno-reorder") # profiling
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -pthread -Wno-reorder") # base
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g -std=c++11 -pthread -Wno-reorder") #debug
#SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO ${CMAKE_CXX_FLAGS_RELWITHDEBINFO} "-O2 -g")
#SET(CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE} "-O2")
#SET(CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG} "-O0 -g")

#add_executable(RadialDistributionFileCreator point_drawer point_factory category_properties category_analyzer point_map analysis_point dialogs central_widget constants main_window input_widget spatial_hashmap dice_roller unit_tests reproducer main distribution_factory radial_distribution radial_distribution_analyzer utils)
#add_executable(RadialDistributionFileAnalyzer point_drawer point_factory category_properties category_analyzer point_map analysis_point dialogs central_widget constants main_window input_widget spatial_hashmap dice_roller unit_tests reproducer main distribution_factory radial_distribution radial_distribution_analyzer utils)
#add_executable(RadialDistributionReproducer point_drawer point_factory category_properties category_analyzer point_map analysis_point dialogs central_widget constants main_window input_widget spatial_hashmap dice_roller unit_tests reproducer main distribution_factory radial_distribution radial_distribution_analyzer utils)
add_executable(RadialDistributionToolGUI reproduction_configuration analyzer analysis_configuration point_spatial_hashmap point_drawer point_factory category_properties category_analyzer point_map analysis_point dialogs central_widget
constants main_window input_widget dice_roller reproducer main distribution_factory radial_distribution radial_distribution_analyzer utils)

add_library(StatsAnalysisisTool reproduction_configuration analyzer analysis_configuration point_spatial_hashmap point_drawer point_factory category_properties category_analyzer point_map analysis_point dialogs constants dice_roller reproducer distribution_factory radial_distribution radial_distribution_analyzer utils)

#target_link_libraries(RadialDistributionFileCreator Qt5::Widgets Qt5::Core Qt5::Gui)
#target_link_libraries(RadialDistributionFileAnalyzer Qt5::Widgets Qt5::Core Qt5::Gui)
#target_link_libraries(RadialDistributionReproducer Qt5::Widgets Qt5::Core Qt5::Gui)
target_link_libraries(RadialDistributionToolGUI Qt5::Widgets Qt5::Core Qt5::Gui)
target_link_libraries(StatsAnalysisisTool Qt5::Widgets Qt5::Core Qt5::Gui)

# Use the Widgets module from Qt 5.
