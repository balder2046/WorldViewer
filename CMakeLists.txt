CMAKE_MINIMUM_REQUIRED(VERSION 3.6)
PROJECT(WorldViewer)
SET(EXECUTABLE_OUTPUT_PATH ".")

SET(VERSION_REQ_OCV "3.1")
SET(VERSION_REQ_CUDA "8.0")

IF(WIN32) # Windows
    if (CMAKE_CL_64)
        SET(ZED_INCLUDE_DIRS $ENV{ZED_INCLUDE_DIRS})
        SET(ZED_LIBRARIES $ENV{ZED_LIBRARIES_64})
        SET(ZED_LIBRARY_DIR $ENV{ZED_LIBRARY_DIR})
        SET(OPENCV_DIR $ENV{OPENCV_DIR})

        SET(GLUT_INCLUDE_DIRS $ENV{GLUT_INCLUDE_DIRS})
        SET(GLUT_LIBRARY_DIRS $ENV{GLUT_LIBRARY_DIRS})
        SET(GLUT_LIBRARY $ENV{GLUT_LIBRARY})

        SET(GLEW_INCLUDE_DIRS $ENV{GLEW_INCLUDE_DIRS})
        SET(GLEW_LIBRARY_DIRS $ENV{GLEW_LIBRARY_DIRS})
        SET(GLEW_LIBRARY $ENV{GLEW_LIBRARY})
    else()
        message(FATAL_ERROR "You've selected the 32bit version of ${CMAKE_GENERATOR}. \n Please delete the cache (file->Delete Cache) and use the 64bit version. (${CMAKE_GENERATOR} Win64)")
    endif()
ELSE() # Linux
    find_package(ZED 2.0 REQUIRED)
    find_package(GLUT REQUIRED)
    find_package(GLEW REQUIRED)
        find_package(OpenGL REQUIRED)

    ##For Jetson, OpenCV4Tegra is based on OpenCV2.4
    exec_program(uname ARGS -p OUTPUT_VARIABLE CMAKE_SYSTEM_NAME2)
    if ( CMAKE_SYSTEM_NAME2 MATCHES "aarch64" )
        SET(VERSION_REQ_OCV "2.4")
        SET(VERSION_REQ_CUDA "")
        link_directories("/usr/lib/aarch64-linux-gnu/tegra") ## on Jetson TX1 64bits, the correct libGL.so is located here (the default one will lead to linking error (undef ref))

        unset(CUDA_USE_STATIC_CUDA_RUNTIME CACHE)
        option(CUDA_USE_STATIC_CUDA_RUNTIME OFF)
        message ("CUDA_USE_STATIC_CUDA_RUNTIME : ${CUDA_USE_STATIC_CUDA_RUNTIME}")
        ##to prevent from opencv_dep_cudart dependencies error...
        ## cmake with -DCUDA_USE_STATIC_CUDA_RUNTIME=false can also be called.
    endif()

    SET(SPECIAL_OS_LIBS "pthread" "X11")
    add_definitions(-std=c++0x -O0 -pthread)
ENDIF(WIN32)
## add for glm depth range 0 to 1
add_definitions(-DGLM_FORCE_DEPTH_ZERO_TO_ONE)


set(OpenCV_DIR "/home/ad/work/opencv/build/")
find_package(OpenCV ${VERSION_REQ_OCV} REQUIRED)
find_package(CUDA ${VERSION_REQ_CUDA} REQUIRED)

find_package(OpenGL)

include_directories(${ZED_INCLUDE_DIRS})
include_directories(${GLEW_INCLUDE_DIRS})
include_directories(${GLUT_INCLUDE_DIRS})
include_directories(${CUDA_INCLUDE_DIRS})
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${OpenGL_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

link_directories(${ZED_LIBRARY_DIR})
link_directories(${GLEW_LIBRARY_DIRS})
link_directories(${GLUT_LIBRARY_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
link_directories(${CUDA_LIBRARY_DIRS})
link_directories(${OpenCV_LIBRARY_DIRS})

#FILE(GLOB_RECURSE SRC_FILES "*.cpp")
set(SRC_FILES TrackingViewer.cpp ZEDModel.cpp main.cpp ShaderProgram.cpp ShaderProgram.h FrameBuffer.cpp GLSLShader.cpp
        RenderableObject.cpp RenderTextureFBO.cpp ScreenSizeQuad.cpp GLSLException.cpp
        ShaderProgram.cpp Terrain.cpp TestTri.cpp UnitColorCube.cpp Texture.cpp Camera.cpp Camera.h RenderDevice.cpp RenderDevice.h)
add_executable(WorldViewer ${SRC_FILES})
# message(WARNING ${OpenCV_LIBRARIES})
target_link_libraries(WorldViewer ${ZED_LIBRARIES} ${OpenCV_LIBRARIES} ${GLUT_LIBRARY}
        ${GLEW_LIBRARY}
        ${SPECIAL_OS_LIBS}
        ${OPENGL_LIBRARIES}
        ${CUDA_CUDA_LIBRARY} ${CUDA_CUDART_LIBRARY} ${CUDA_npp_LIBRARY}   )