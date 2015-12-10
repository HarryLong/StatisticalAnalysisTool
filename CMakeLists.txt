cmake_minimum_required(VERSION 2.8)
project(STATISTICAL-ANALYSIS-TOOL)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
set( CMAKE_VERBOSE_MAKEFILE on )

# QT Imports
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)

set(INCLUDE_DIRECTORIES ${Qt5Widgets_INCLUDE_DIRS} ${Qt5Core_INCLUDE_DIRS} ${Qt5Gui_INCLUDE_DIRS})

include_directories(${INCLUDE_DIRECTORIES})

set(GUI_LIBS Qt5::Widgets Qt5::Core Qt5::Gui)
set(LIB_LIBS Qt5::Core Qt5::Gui)

add_subdirectory("test")

set(CMAKE_AUTOMOC ON)

SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} ${OpenMP_CXX_FLAGS} -O2 -g -std=c++11")
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${OpenMP_CXX_FLAGS} -O2 -std=c++11")
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${OpenMP_CXX_FLAGS} -O0 -g -std=c++11")
#SET(CMAKE_BUILD_TYPE DEBUG)
SET(CMAKE_BUILD_TYPE RELEASE)
#SET(CMAKE_BUILD_TYPE RELWITHDEBINFO)

SET(GUI_SRC_FILES gui/central_widget gui/dialogs gui/input_widget gui/main gui/main_window )
SET(ANALYSER_SRC_FILES analyser/analysis_configuration analyser/analyzer analyser/category_analyzer analyser/dependency_analyzer analyser/radial_distribution_analyzer)
set(UTILS_SRC_FILES utils/utils utils/dice_roller utils/file_utils utils/point_drawer)
set(REPRODUCTION_SRC_FILES reproducer/distribution_factory reproducer/point_factory reproducer/reproducer reproducer/reproduction_configuration)
set(CORE_SRC_FILES point_spatial_hashmap category_properties point_map analysis_point radial_distribution)

set(CORE_HEADER_FILES radial_distribution.h analysis_point.h category_properties.h)
set(ANALYSIS_HEADER_FILES analyser/analysis_configuration.h analyser/analyzer.h analyser/radial_distribution_analyzer.h analyser/category_analyzer.h )
set(REPRODUCER_HEADER_FILES reproducer/reproducer.h reproducer/reproduction_configuration.h )

add_executable(RadialDistributionToolGUI
${GUI_SRC_FILES}
${ANALYSER_SRC_FILES}
${UTILS_SRC_FILES}
${REPRODUCTION_SRC_FILES}
${CORE_SRC_FILES} )
target_link_libraries(RadialDistributionToolGUI ${GUI_LIBS})

add_library(RadialDistributionAnalyser SHARED ${ANALYSER_SRC_FILES} ${CORE_SRC_FILES} ${UTILS_SRC_FILES})
target_link_libraries(RadialDistributionAnalyser ${LIB_LIBS})

add_library(RadialDistributionReproducer SHARED ${REPRODUCTION_SRC_FILES} ${CORE_SRC_FILES} ${UTILS_SRC_FILES} analyser/analysis_configuration)
target_link_libraries(RadialDistributionReproducer ${LIB_LIBS})

#INSTALL EXECUTABLE
install(TARGETS RadialDistributionToolGUI
        RUNTIME DESTINATION bin
        CONFIGURATIONS RELWITHDEBINFO)

#INSTALL ANALYSIS LIB
install(TARGETS RadialDistributionAnalyser
        LIBRARY DESTINATION lib
        CONFIGURATIONS RELWITHDEBINFO)

#INSTALL REPRODUCTION LIB
install(TARGETS RadialDistributionReproducer
        LIBRARY DESTINATION lib
        CONFIGURATIONS RELWITHDEBINFO)

# INSTALL CORE HEADER FILES
install(FILES ${CORE_HEADER_FILES}
        DESTINATION include/radialDistribution)
# INSTALL ANALYSIS HEADER FILES
install(FILES ${ANALYSIS_HEADER_FILES}
        DESTINATION include/radialDistribution/analyser)
# INSTALL REPRODUCER HEADER FILES
install(FILES ${REPRODUCER_HEADER_FILES}
        DESTINATION include/radialDistribution/reproducer)
