
cmake_minimum_required(VERSION 2.8)
 
PROJECT(ISO)
 
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})
 
add_executable(ISO MACOSX_BUNDLE ISO)
 
if(VTK_LIBRARIES)
  target_link_libraries(ISO ${VTK_LIBRARIES})
else()
  target_link_libraries(ISO vtkHybrid vtkWidgets)
endif()

