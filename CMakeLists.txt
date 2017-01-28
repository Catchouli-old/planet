cmake_minimum_required(VERSION 2.8)

INCLUDE(FindPkgConfig)

# Main project
project(planet)

set(SOURCES
  src/window.c
  src/maths.c
  src/mesh.c
)

add_executable(${PROJECT_NAME} src/main.c ${SOURCES})

PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(GL REQUIRED gl)
PKG_SEARCH_MODULE(GLEW REQUIRED glew)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${SDL2_LIBRARIES} ${GL_LIBRARIES} ${GLEW_LIBRARIES})

add_custom_command(
  TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ln -sf ${CMAKE_SOURCE_DIR}/data ${CMAKE_BINARY_DIR}/data
  )

# Tests
project(tests)

set(TEST_SOURCES
  src/maths.c
  tests/main.cpp
  tests/math_tests.cpp
)

add_executable(${PROJECT_NAME} ${TEST_SOURCES})

TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${SDL2_LIBRARIES} ${GL_LIBRARIES} ${GLEW_LIBRARIES})

add_custom_command(
  TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ln -sf ${CMAKE_SOURCE_DIR}/test_data ${CMAKE_BINARY_DIR}/test_data
  )
