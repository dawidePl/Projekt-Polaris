add_library(common
    src/Test.cpp
    src/LinearAlgebra.cpp
)

target_include_directories(common
    PUBLIC
        include
)