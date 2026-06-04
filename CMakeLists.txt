cmake_minimum_required(VERSION 3.27)

project(warehouse)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)

set(OpenXLSX_DIR "C:/Program Files/OpenXLSX/lib/cmake/OpenXLSX")

find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(OpenXLSX REQUIRED)

add_executable(warehouse
        main.cpp
        models/Item.h
        models/Item.cpp
        core/Warehouse.h
        core/Warehouse.cpp
        ui/MainWindow.h
        ui/MainWindow.cpp
        file/FileManager.h
        file/FileManager.cpp
        file/ExcelExporter.h
        file/ExcelExporter.cpp
        file/ExcelImporter.h
        file/ExcelImporter.cpp
        utils/InputHelper.h
        utils/InputHelper.cpp
)

target_link_libraries(warehouse PRIVATE Qt6::Widgets)

if(OpenXLSX_FOUND)
    target_link_libraries(warehouse PRIVATE OpenXLSX::OpenXLSX)
    target_compile_definitions(warehouse PRIVATE OPENXLSX_FOUND)
endif()

enable_testing()

include(FetchContent)

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/heads/main.zip
)

FetchContent_MakeAvailable(googletest)

add_executable(warehouse_tests
        tests/WarehouseTests.cpp
        models/Item.h
        models/Item.cpp
        core/Warehouse.h
        core/Warehouse.cpp
        file/FileManager.h
        file/FileManager.cpp
)

target_link_libraries(warehouse_tests PRIVATE gtest gtest_main)

add_test(NAME WarehouseTests COMMAND warehouse_tests)