project(STATISTICAL-ANALYSIS-TOOL)

# QT Imports
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(CppUnit REQUIRED)

set(CMAKE_AUTOMOC ON)

set(INCLUDE_DIRECTORIES ${Qt5Widgets_INCLUDE_DIRS} ${Qt5Core_INCLUDE_DIRS} ${Qt5Gui_INCLUDE_DIRS} ${CppUnit_INCLUDE_DIRS})
include_directories(${INCLUDE_DIRECTORIES})

set(LIBS Qt5::Widgets Qt5::Core Qt5::Gui cppunit)

SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} ${OpenMP_CXX_FLAGS} -O2 -g -std=c++11")
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${OpenMP_CXX_FLAGS} -O2 -std=c++11")
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${OpenMP_CXX_FLAGS} -O0 -g -std=c++11")
#SET(CMAKE_BUILD_TYPE DEBUG)
#SET(CMAKE_BUILD_TYPE RELEASE)
SET(CMAKE_BUILD_TYPE RELWITHDEBINFO)


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

target_link_libraries(runTests ${LIBS})

# Use the Widgets module from Qt 5.
