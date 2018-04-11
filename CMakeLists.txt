cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(gebic_gelatuca)

SET(CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}")
###### To print screen messages
#MESSAGE(STATUS " ${CMAKE_INSTALL_PREFIX}")

# ROOT
find_package(ROOT REQUIRED)
include(${ROOT_USE_FILE})

# (2)
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

# (3)
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)

# (4)
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

# (5)
add_executable(gebic_gelatuca gebic_v04_gelatuca_damic_pb_496_dl.cc ${sources} ${headers})
target_link_libraries(gebic_gelatuca ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})

# Some compilation variables
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -fPIC -Wall -Wextra -std=c++11 -m64")

## (6)
#set(EXAMPLEB1_SCRIPTS
#  exampleB1.in
#  exampleB1.out
#  init_vis.mac
#  run1.mac
#  run2.mac
#  vis.mac
#  )
#
#foreach(_script ${EXAMPLEB1_SCRIPTS})
#  configure_file(
#    ${PROJECT_SOURCE_DIR}/${_script}
#    ${PROJECT_BINARY_DIR}/${_script}
#    COPYONLY
#    )
#endforeach()

# (7)
install(TARGETS gebic_gelatuca DESTINATION bin)
