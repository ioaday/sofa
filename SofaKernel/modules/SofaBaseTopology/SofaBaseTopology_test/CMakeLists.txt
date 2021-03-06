cmake_minimum_required(VERSION 3.12)

project(SofaBaseTopology_test)

find_package(SofaCommon QUIET)
find_package(SofaGeneral QUIET)

set(SOURCE_FILES
    fake_TopologyScene.h
    fake_TopologyScene.cpp

    PointSetTopology_test.cpp
    EdgeSetTopology_test.cpp
    TriangleSetTopology_test.cpp
    QuadSetTopology_test.cpp
    TetrahedronSetTopology_test.cpp
    HexahedronSetTopology_test.cpp

    MeshTopology_test.cpp

    RegularGridTopology_test.cpp
    SparseGridTopology_test.cpp
    TetrahedronNumericalIntegration_test.cpp
    TriangleNumericalIntegration_test.cpp
 )
 if(SofaCommon_FOUND)
    add_definitions("-DSOFABASETOPOLOGY_TEST_WITH_COMMON_LOADERS")
 endif()
  if(SofaGeneral_FOUND)
    add_definitions("-DSOFABASETOPOLOGY_TEST_WITH_GENERAL_LOADERS")
 endif()

add_definitions("-DSOFABASETOPOLOGY_TEST_RESOURCES_DIR=\"${CMAKE_SOURCE_DIR}/share\"")
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} SofaGTestMain SofaTest)
 if(SofaCommon_FOUND)
    target_link_libraries(${PROJECT_NAME} SofaLoader)
 endif()
  if(SofaGeneral_FOUND)
    target_link_libraries(${PROJECT_NAME} SofaGeneralLoader)
 endif()


add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})
