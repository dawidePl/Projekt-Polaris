file(GLOB_RECURSE SIMULATOR_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_executable(simulator
    ${SIMULATOR_SOURCES}
)

target_include_directories(simulator
    PRIVATE
    src
)

target_link_libraries(simulator
    PRIVATE
    common
)

# target_compile_definitions(simulator PRIVATE DEBUG_MODE)