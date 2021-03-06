cmake_minimum_required (VERSION 2.6 FATAL_ERROR)

project(knn)

# In case of Makefiles if the user does not setup CMAKE_BUILD_TYPE, assume it's Release:
if (${CMAKE_GENERATOR} MATCHES ".*Makefiles")
    if("${CMAKE_BUILD_TYPE}" STREQUAL "")
        set(CMAKE_BUILD_TYPE Release)
    endif()
endif()

find_package (Qt5Widgets REQUIRED)
find_package (Qt5Core REQUIRED)
find_package(OpenMP)


set  (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set  (project_SOURCES main.cpp knn.cpp)
set  (project_HEADERS knn.h)
set  (project_FORMS   knn.ui)
set(QT_LIBRARIES Qt5::Widgets Qt5::Core)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

QT5_WRAP_CPP (project_HEADERS_MOC   ${project_HEADERS})
QT5_WRAP_UI  (project_FORMS_HEADERS ${project_FORMS})

include_directories(${CMAKE_CURRENT_BINARY_DIR})

#include         (${QT_USE_FILE})
#add_definitions (${QT_DEFINITIONS})

add_executable  (knn ${project_SOURCES}
                               ${project_FORMS_HEADERS}
                               ${project_HEADERS_MOC})

target_link_libraries (knn ${QT_LIBRARIES})

