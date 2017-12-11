cmake_minimum_required(VERSION 2.8.11)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)


function(add_qtquick2_plugin module_name src_list)

  string(TOLOWER ${module_name} module_name_lower)

  set(target_name ${module_name_lower}_plugin)

  add_library(${target_name} MODULE ${src_list})

  file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/${module_name}")
  set_property(TARGET ${target_name} PROPERTY
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${module_name}")

  qt5_use_modules(${target_name} Quick Core)

  target_link_libraries(${target_name}
    ${OPENTHREADS_LIBRARIES}
    ${OSG_LIBRARIES}
    ${OSGGA_LIBRARIES}
    ${OSGDB_LIBRARIES}
    ${OSGVIEWER_LIBRARIES}
  )

  target_compile_definitions(${target_name}
    PRIVATE QT_PLUGIN $<$<CONFIG:Release>:QT_NO_DEBUG>)

  file(WRITE "${CMAKE_BINARY_DIR}/${module_name}/qmldir"
    "module ${module_name}\nplugin ${target_name}\n")

endfunction()


find_package(Qt5 COMPONENTS Quick Core)
find_package(OpenSceneGraph 3.2.0 REQUIRED
    osg
    osgGA
    osgDB
    osgViewer
    )

include_directories(
  ${OPENSCENEGRAPH_INCLUDE_DIRS}
)

set(PLUGIN_SRC_LIST osgitem.cpp osgmodelloader.cpp plugin.cpp)
add_qtquick2_plugin("OSG" "${PLUGIN_SRC_LIST}")
