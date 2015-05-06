project(STATISTICAL-ANALYSIS-TOOL)

# QT Imports
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(CppUnit REQUIRED)

# Link directories
include_directories("/home/harry/workspaces/qt-workspace/spatial-hashmap/" )
link_directories("/home/harry/workspaces/qt-workspace/spatial-hashmap/" )

set(CMAKE_AUTOMOC ON)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCC_GL_DEBUG_ -DDEBUG_ON -Wall -g -std=c++11 -Wno-reorder -Wno-unused-variable") #DEBUG
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEBUG_ON -O3 -Wall -std=c++11 -Wno-reorder -Wno-unused-variable") #RELEASE

add_executable(runTests main ../dice_roller ../analysis_configuration ../utils ../point_drawer ../radial_distribution_analyzer ../point_spatial_hashmap ../radial_distribution
../distribution_factory ../analysis_point ../point_map ../category_analyzer ../category_properties ../analyzer ../reproduction_configuration ../dependency_analyzer
../reproducer ../point_factory
test_data_factory bin_test unit_tests dice_roller_test category_analyzer_test radial_distribution_analyzer_test dependency_test radial_distribution_file_test
category_properties_file_test analysis_configuration_file_test reproduction_test )

target_link_libraries(runTests cppunit Qt5::Core Qt5::Gui)

# Use the Widgets module from Qt 5.
