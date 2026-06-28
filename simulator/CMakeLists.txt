add_executable(simulator
    src/main.cpp
)

target_link_libraries(simulator
    PRIVATE
        common
)