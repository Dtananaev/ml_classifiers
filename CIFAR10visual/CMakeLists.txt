cmake_minimum_required (VERSION 2.6 FATAL_ERROR)

project      (CIFAR10visual)

# In case of Makefiles if the user does not setup CMAKE_BUILD_TYPE, assume it's Release:
if (${CMAKE_GENERATOR} MATCHES ".*Makefiles")
    if("${CMAKE_BUILD_TYPE}" STREQUAL "")
        set(CMAKE_BUILD_TYPE Release)
    endif()
endif()

find_package (Qt5Widgets REQUIRED)
find_package (Qt5Core REQUIRED)

set  (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set  (project_SOURCES main.cpp visualizer.cpp)
set  (project_HEADERS visualizer.h)
set  (project_FORMS   visualizer.ui)
set(QT_LIBRARIES Qt5::Widgets Qt5::Core)

QT5_WRAP_CPP (project_HEADERS_MOC   ${project_HEADERS})
QT5_WRAP_UI  (project_FORMS_HEADERS ${project_FORMS})

include_directories(${CMAKE_CURRENT_BINARY_DIR})



add_executable  (CIFAR10visual ${project_SOURCES}
                               ${project_FORMS_HEADERS}
                               ${project_HEADERS_MOC})

target_link_libraries (CIFAR10visual ${QT_LIBRARIES})

