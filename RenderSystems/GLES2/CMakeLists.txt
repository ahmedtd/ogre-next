#-------------------------------------------------------------------
# This file is part of the CMake build system for OGRE-Next
#     (Object-oriented Graphics Rendering Engine)
# For the latest info, see http://www.ogre3d.org/
#
# The contents of this file are placed in the public domain. Feel
# free to make use of it in any way you like.
#-------------------------------------------------------------------

# Configure OpenGL ES 2.0 RenderSystem build

file(GLOB HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/include/Vao/*.h")
file(GLOB SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"  "${CMAKE_CURRENT_SOURCE_DIR}/src/Vao/*.cpp")

file(GLOB GLSLES_FILES "src/GLSLES/include/*.h" "src/GLSLES/src/*.cpp")

list(REMOVE_ITEM GLSLES_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/GLSLES/src/OgreGLSLESCgProgram.cpp")
list(REMOVE_ITEM GLSLES_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/GLSLES/src/OgreGLSLESCgProgramFactory.cpp")

if(OGRE_CONFIG_ENABLE_GLES3_SUPPORT)
  set(GLESW_HEADER_INSTALL "GLES3")
  file(GLOB GLESW_HEADERS "include/GLES3/*.h")
  list(REMOVE_ITEM SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/gles2w.cpp")
  list(APPEND HEADER_FILES
  )
  if (OGRE_BUILD_PLATFORM_APPLE_IOS)
    set_source_files_properties(src/gles3w.cpp PROPERTIES COMPILE_FLAGS "-x objective-c++")
  endif()
else()
  set(GLESW_HEADER_INSTALL "GLES2")
  file(GLOB GLESW_HEADERS "include/GLES2/*.h")
  list(REMOVE_ITEM SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/gles3w.cpp")
  if (OGRE_BUILD_PLATFORM_APPLE_IOS)
    set_source_files_properties(src/gles2w.cpp PROPERTIES COMPILE_FLAGS "-x objective-c++")
  endif()
endif()

file(GLOB KHR_HEADERS "include/KHR/*.h")

if (OGRE_CONFIG_ENABLE_GLES2_CG_SUPPORT)
  list(APPEND GLSLES_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/GLSLES/src/OgreGLSLESCgProgram.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/GLSLES/src/OgreGLSLESCgProgramFactory.cpp
  )
endif (OGRE_CONFIG_ENABLE_GLES2_CG_SUPPORT)

if(NOT OPENGLES3_FOUND)
  set(OPENGLES3_INCLUDE_DIR "")
endif()

# Add system specific settings
if (OGRE_BUILD_PLATFORM_NACL)
  file(GLOB PLATFORM_HEADERS "include/NaCl/*.h")
  file(GLOB PLATFORM_SOURCES "src/NaCl/*.cpp")

  include_directories(src/NaCl include/NaCl)
  set(PLATFORM_HEADER_INSTALL "NaCl")
elseif (WIN32)
  file(GLOB PLATFORM_HEADERS "include/EGL/WIN32/*.h" "include/EGL/*.h")
  file(GLOB PLATFORM_SOURCES "src/EGL/WIN32/*.cpp" "src/EGL/*.cpp")

  include_directories(src/EGL/WIN32 include/EGL/WIN32)
  if(OGRE_CONFIG_ENABLE_GLES3_SUPPORT)
    link_directories(${OPENGLES3_LIBRARY_PATH})
  else()
    link_directories(${OPENGLES2_LIBRARY_PATH})
  endif()
  set(PLATFORM_HEADER_INSTALL "WIN32")
elseif (ANDROID)
  file(GLOB PLATFORM_HEADERS "include/EGL/Android/*.h" "include/EGL/*.h")
  file(GLOB PLATFORM_SOURCES "src/EGL/Android/*.cpp" "src/EGL/*.cpp")

  include_directories(src/EGL/Android include/EGL/Android)
  if(OGRE_CONFIG_ENABLE_GLES3_SUPPORT)
    link_directories(${OPENGLES3_LIBRARY_PATH})
  else()
    link_directories(${OPENGLES2_LIBRARY_PATH})
  endif()
  set(PLATFORM_HEADER_INSTALL "Android")
elseif (EMSCRIPTEN)
  file(GLOB PLATFORM_HEADERS "include/EGL/Emscripten/*.h" "include/EGL/*.h")
  file(GLOB PLATFORM_SOURCES "src/EGL/Emscripten/*.cpp" "src/EGL/*.cpp")

  include_directories(src/EGL/Emscripten include/EGL/Emscripten)
  set(PLATFORM_HEADER_INSTALL "Emscripten")
elseif (APPLE)
  file(GLOB PLATFORM_HEADERS "include/EAGL/*.h")
  file(GLOB PLATFORM_SOURCES "src/EAGL/*.mm")

  include_directories(src/EAGL include/EAGL)
  link_directories(${OPENGLES2_LIBRARY_PATH})
  set(PLATFORM_HEADER_INSTALL "APPLE")
elseif (UNIX)
  file(GLOB PLATFORM_HEADERS "include/EGL/X11/*.h" "include/EGL/*.h")
  file(GLOB PLATFORM_SOURCES "src/EGL/X11/*.cpp" "src/EGL/*.cpp")

  include_directories(src/EGL/X11 include/EGL/X11)
  set(PLATFORM_HEADER_INSTALL "X11")
  set(PLATFORM_LIBS ${X11_LIBRARIES} ${X11_Xrandr_LIB})
endif ()

# Add support for the RT Shader System
add_definitions(-DINCLUDE_RTSHADER_SYSTEM)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/include/EGL
  ${CMAKE_CURRENT_SOURCE_DIR}/include/EAGL
  ${CMAKE_CURRENT_SOURCE_DIR}/src/GLSLES/include
  ${CMAKE_SOURCE_DIR}/Components/RTShaderSystem/include
  ${OPENGLES3_INCLUDE_DIR} ${OPENGLES2_INCLUDE_DIR} ${EGL_INCLUDE_DIR}
)

ogre_add_library(RenderSystem_GLES2 ${OGRE_LIB_TYPE} ${HEADER_FILES} ${GLESW_HEADERS} ${KHR_HEADERS} ${PLATFORM_HEADERS} ${SOURCE_FILES} ${PLATFORM_SOURCES} SEPARATE ${GLSLES_FILES})
target_link_libraries(RenderSystem_GLES2 OgreMain ${OPENGLES3_LIBRARIES} ${OPENGLES2_LIBRARIES} ${EGL_LIBRARIES} ${PLATFORM_LIBS})

if (NOT OGRE_STATIC)
  set_target_properties(RenderSystem_GLES2 PROPERTIES
    COMPILE_DEFINITIONS OGRE_GLES2PLUGIN_EXPORTS
  )
endif ()

if (OGRE_BUILD_PLATFORM_APPLE_IOS)
  target_link_libraries(RenderSystem_GLES2 "-framework OpenGLES")
endif()

if (OGRE_CONFIG_THREADS)
  target_link_libraries(RenderSystem_GLES2 ${OGRE_THREAD_LIBRARIES})
endif ()

ogre_config_plugin(RenderSystem_GLES2)

install(FILES ${HEADER_FILES} DESTINATION include/OGRE/RenderSystems/GLES2)
install(FILES ${PLATFORM_HEADERS} DESTINATION include/OGRE/RenderSystems/GLES2/${PLATFORM_HEADER_INSTALL})
install(FILES ${GLESW_HEADERS} DESTINATION include/OGRE/RenderSystems/GLES2/${GLESW_HEADER_INSTALL})
install(FILES ${KHR_HEADERS} DESTINATION include/OGRE/RenderSystems/GLES2/KHR)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/GLSLES/include/ DESTINATION include/OGRE/RenderSystems/GLES2/GLSLES)
