cmake_minimum_required(VERSION 3.25)
project(
  firefly
  VERSION 1.0.0
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  set(IS_ROOT_PROJECT ON)
else()
  set(IS_ROOT_PROJECT OFF)
endif()

# -------------------------
# Library target
# -------------------------
add_library(firefly STATIC src/logger.cpp src/log-registry.cpp)

target_compile_features(firefly PUBLIC cxx_std_20)

target_include_directories(
  firefly PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                 $<INSTALL_INTERFACE:include>)

target_precompile_headers(firefly PRIVATE include/pch.hpp)

# -------------------------
# Install Dependencies
# -------------------------

add_subdirectory(external/amanuensis)
target_link_libraries(${PROJECT_NAME} PRIVATE amanuensis)

# -------------------------
# Output dirs (optional)
# -------------------------
set_target_properties(
  firefly
  PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
             LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
             RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

foreach(OUTPUTCONFIG Debug Release RelWithDebInfo MinSizeRel)
  string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG_UPPER)
  set_target_properties(
    firefly
    PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER}
               "${CMAKE_BINARY_DIR}/lib/${OUTPUTCONFIG}"
               LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER}
               "${CMAKE_BINARY_DIR}/lib/${OUTPUTCONFIG}"
               RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER}
               "${CMAKE_BINARY_DIR}/bin/${OUTPUTCONFIG}")
endforeach()

# -------------------------
# Warnings
# -------------------------
if(MSVC)
  target_compile_options(firefly PRIVATE /W4)
else()
  target_compile_options(firefly PRIVATE -Wall -Wextra -Wpedantic)
endif()

# -------------------------
# Shared build define
# -------------------------
if(BUILD_SHARED_LIBS)
  target_compile_definitions(firefly PRIVATE FIREFLY_BUILD_DLL)
endif()

set_target_properties(
  firefly
  PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
             LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
             RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

foreach(OUTPUTCONFIG Debug Release RelWithDebInfo MinSizeRel)
  string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG_UPPER)
  set_target_properties(
    firefly
    PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER}
               "${CMAKE_BINARY_DIR}/lib/${OUTPUTCONFIG}"
               LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER}
               "${CMAKE_BINARY_DIR}/lib/${OUTPUTCONFIG}"
               RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER}
               "${CMAKE_BINARY_DIR}/bin/${OUTPUTCONFIG}")
endforeach()

if(MSVC)
  target_compile_options(firefly PRIVATE /W4 /EHsc)
else()
  target_compile_options(firefly PRIVATE -Wall -Wextra -Wpedantic)
endif()

# -------------------------
# Test executable
# -------------------------

if(IS_ROOT_PROJECT)
  add_subdirectory(external/cimmerian)
  add_subdirectory(test)
endif()

# ------------------------
# Installation
# -------------------------
include(GNUInstallDirs)

install(
  TARGETS firefly
  EXPORT fireflyTargets
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(
  EXPORT fireflyTargets
  FILE fireflyTargets.cmake
  NAMESPACE Firefly::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/firefly)

# -------------------------
# Package config (enables find_package)
# -------------------------
include(CMakePackageConfigHelpers)

configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/fireflyConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/fireflyConfig.cmake"
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/firefly)

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/fireflyConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/fireflyConfig.cmake"
              "${CMAKE_CURRENT_BINARY_DIR}/fireflyConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/firefly)
