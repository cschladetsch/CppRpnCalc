cmake_minimum_required(VERSION 3.10)
project(RPNCalculator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Option to build tests
option(BUILD_TESTS "Build unit tests" ON)

# Find packages
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

# ImGui
set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/imgui)
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

# ImPlot
set(IMPLOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/implot)
set(IMPLOT_SOURCES
    ${IMPLOT_DIR}/implot.cpp
    ${IMPLOT_DIR}/implot_items.cpp
)

# Project sources
set(PROJECT_SOURCES
    main.cpp
    src/Model/CalculatorModel.cpp
    src/Model/GraphData.cpp
    src/Model/GraphFunction.cpp
    src/Model/InfixToRPN.cpp
    src/View/CalculatorView.cpp
    src/View/GraphView.cpp
    src/Controller/CalculatorController.cpp
)

# Project headers
set(PROJECT_HEADERS
    src/Model/CalculatorModel.h
    src/Model/GraphData.h
    src/Model/GraphFunction.h
    src/Model/InfixToRPN.h
    src/View/CalculatorView.h
    src/View/GraphView.h
    src/Controller/CalculatorController.h
)

# Main executable
add_executable(rpn_calculator
    ${PROJECT_SOURCES}
    ${PROJECT_HEADERS}
    ${IMGUI_SOURCES}
    ${IMPLOT_SOURCES}
)

# Include directories
target_include_directories(rpn_calculator PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
    ${IMPLOT_DIR}
)

# Link libraries
target_link_libraries(rpn_calculator
    OpenGL::GL
    glfw
)

# Platform-specific settings
if(APPLE)
    target_link_libraries(rpn_calculator "-framework Cocoa" "-framework IOKit")
endif()

# Set output directory
set_target_properties(rpn_calculator PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

# Tests
if(BUILD_TESTS)
    # Download and configure GoogleTest
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
    )
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
    
    # Test sources
    set(TEST_SOURCES
        tests/main_test.cpp
        tests/test_calculator_model.cpp
        src/Model/CalculatorModel.cpp
    )
    
    # Test executable
    add_executable(rpn_calculator_tests ${TEST_SOURCES})
    
    # Include directories for tests
    target_include_directories(rpn_calculator_tests PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    )
    
    # Link test libraries
    target_link_libraries(rpn_calculator_tests
        GTest::gtest_main
        GTest::gtest
    )
    
    # Enable testing
    enable_testing()
    add_test(NAME rpn_calculator_tests COMMAND rpn_calculator_tests)
    
    # Set test output directory
    set_target_properties(rpn_calculator_tests PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    )
endif()