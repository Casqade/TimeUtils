cmake_minimum_required(VERSION 3.8)

set(TARGET TimeUtils)
set(${TARGET}_VERSION 1.0.0)
project(${TARGET} VERSION ${${TARGET}_VERSION} LANGUAGES CXX)

add_library(${TARGET})
add_library(${TARGET}::${TARGET} ALIAS ${TARGET})

target_sources(${TARGET} PRIVATE
  src/Duration.cpp
  include/${TARGET}/Duration.hpp

  src/Timer.cpp
  include/${TARGET}/Timer.hpp
)

set_target_properties(${TARGET} PROPERTIES
  CXX_STANDARD_REQUIRED ON
  CXX_STANDARD 14
)

include(GenerateExportHeader)
generate_export_header(${TARGET} EXPORT_FILE_NAME ${CMAKE_BINARY_DIR}/exports/${TARGET}/${TARGET}_export.hpp)

include(GNUInstallDirs)

target_include_directories(${TARGET} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/${CMAKE_INSTALL_INCLUDEDIR}>
  $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/exports>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

if(WIN32)
  target_link_libraries(${TARGET} PUBLIC winmm)
endif()


install(TARGETS ${TARGET}
  EXPORT ${TARGET} DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(FILES ${CMAKE_BINARY_DIR}/exports/${TARGET}/${TARGET}_export.hpp
  DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${TARGET}"
)

