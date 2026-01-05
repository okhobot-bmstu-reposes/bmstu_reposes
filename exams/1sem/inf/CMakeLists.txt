cmake_minimum_required(VERSION 3.10)
project(inf_ex_1sem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Явно указываем путь к установленной библиотеке
set(PostgreSQL_ROOT "C:/Program Files/PostgreSQL/18")
set(libpqxx_DIR "${CMAKE_SOURCE_DIR}/vcpkg/installed/x64-windows/share/libpqxx")
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")

file(GLOB SOURCES "src/*.cpp")
file(GLOB HEADERS "include/*.hpp")

find_package(libpqxx REQUIRED)
add_executable(inf_ex_1sem ${SOURCES} ${HEADERS})

target_include_directories(inf_ex_1sem PRIVATE include)
target_link_libraries(inf_ex_1sem PRIVATE libpqxx::pqxx)

set(CMAKE_CXX_STANDARD 17)


add_custom_command(TARGET inf_ex_1sem POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/vcpkg/installed/x64-windows/bin/"
    "$<TARGET_FILE_DIR:inf_ex_1sem>"
    COMMENT "Copying all DLLs from vcpkg"
)
