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

# Подключаем файлы статичных библиотек
set(PROJECT_LIBRARY_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/build-libs/lib
	${CMAKE_CURRENT_SOURCE_DIR}/build-libs/bin
)

# Подключаем dll файлы
set(PROJECT_DLL_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/build-libs/bin
)

file(GLOB DLL_FILES "${PROJECT_DLL_DIR}/*.dll")

# Устанавливаем макросы для сборки
set(PROJECT_DEFINES
    ASIO_STANDALONE
    SQLITE_THREADSAFE=1
)

# Подключаем необходимые библиотеки
set(PROJECT_LIBS
    ws2_32
    wsock32
    crypt32
    ssl
	crypto
    curl
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

# Находим все .cpp файлы в tests/
file(GLOB TEST_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} tests/*.cpp)

# Для каждого создаём отдельную цель
foreach(test_src ${TEST_SOURCES})
    get_filename_component(test_name ${test_src} NAME_WE)
    add_executable(${test_name} ${test_src})
	
	set_target_properties(${test_name} PROPERTIES
		WINDOWS_EXPORT_ALL_SYMBOLS OFF
		ENABLE_EXPORTS OFF
		PREFIX ""  # на всякий случай — отключает lib-префикс даже у dll
	)

    target_include_directories(${test_name} PRIVATE ${PROJECT_INCLUDE_DIRS})
    target_link_directories(${test_name} PRIVATE ${PROJECT_LIBRARY_DIRS})
    target_compile_definitions(${test_name} PRIVATE ${PROJECT_DEFINES})
    target_link_libraries(${test_name} PRIVATE ${PROJECT_LIBS})

	foreach(dll ${DLL_FILES})
        add_custom_command(TARGET ${test_name} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${dll}"
            "$<TARGET_FILE_DIR:${test_name}>"
        )
    endforeach()
endforeach()
