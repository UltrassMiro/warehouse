cmake_minimum_required(VERSION 3.27)
project(warehouse)

set(CMAKE_CXX_STANDARD 17)

add_executable(warehouse main.cpp
        models/Item.h
        models/Item.cpp
        core/Warehouse.h
        core/Warehouse.cpp
        ui/Interface.h
        ui/Interface.cpp
        file/FileManager.h
        file/FileManager.cpp
        file/ExcelExporter.h
        file/ExcelExporter.cpp
        utils/InputHelper.h
        utils/InputHelper.cpp)

# Excel бібліотека
find_package(OpenXLSX QUIET)
if(OpenXLSX_FOUND)
    target_link_libraries(warehouse OpenXLSX::OpenXLSX)
endif()