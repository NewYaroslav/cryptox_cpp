# При генерации Code::Blocks проекта с MinGW можно использовать так:
# cmake -G "CodeBlocks - MinGW Makefiles" .
cmake_minimum_required(VERSION 3.18)
project(cryptox_cpp LANGUAGES CXX)

# Пути к инклудам и библиотекам
set(PROJECT_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/build-libs/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/tests
)

set(PROJECT_LIBRARY_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/build-libs/lib
    ${CMAKE_CURRENT_SOURCE_DIR}/build-libs/bin
)

set(PROJECT_DEFINES
    ASIO_STANDALONE
    SQLITE_THREADSAFE=1
)

set(PROJECT_LIBS
    ws2_32
    wsock32
    crypt32
    crypto
    ssl
    libcurl
    mdbx
	sqlite3
    zstd
    vbyte
    simdcomp
    minizip
	z
    ntdll
    bcrypt
)

# Найдём все .cpp файлы в tests/
file(GLOB TEST_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} tests/*.cpp)

# Для каждого создаём отдельную цель
foreach(test_src ${TEST_SOURCES})
    get_filename_component(test_name ${test_src} NAME_WE)

    add_executable(${test_name} ${test_src})

    target_include_directories(${test_name} PRIVATE ${PROJECT_INCLUDE_DIRS})
    target_link_directories(${test_name} PRIVATE ${PROJECT_LIBRARY_DIRS})
    target_compile_definitions(${test_name} PRIVATE ${PROJECT_DEFINES})
    target_link_libraries(${test_name} PRIVATE ${PROJECT_LIBS})
endforeach()
