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

set(ANALYSER_SRC_FILES ../analyser/analysis_configuration ../analyser/radial_distribution_analyzer ../analyser/category_analyzer ../analyser/analyzer
../analyser/dependency_analyzer)
set(REPRODUCTION_SRC_FILES ../reproducer/distribution_factory ../reproducer/point_factory ../reproducer/reproducer ../reproducer/reproduction_configuration)
set(UTILS_SRC_FILES ../utils/dice_roller ../utils/utils ../utils/file_utils ../utils/point_drawer)


add_executable(runTests
${ANALYSER_SRC_FILES}
${REPRODUCTION_SRC_FILES}
${UTILS_SRC_FILES}
main ../point_spatial_hashmap ../radial_distribution ../analysis_point ../point_map ../category_properties
test_data_factory bin_test unit_tests dice_roller_test category_analyzer_test radial_distribution_analyzer_test dependency_test radial_distribution_file_test
category_properties_file_test analysis_configuration_file_test reproduction_test )

target_link_libraries(runTests cppunit Qt5::Core Qt5::Gui)

# Use the Widgets module from Qt 5.
