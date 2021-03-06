include(ExternalProject)

execute_process(COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/download)
execute_process(COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/src)

include(${PROJECT_SOURCE_DIR}/libgramtools/lib/boost.cmake)
include(${PROJECT_SOURCE_DIR}/libgramtools/lib/sdsl.cmake)
include(${PROJECT_SOURCE_DIR}/libgramtools/lib/zlib.cmake)
include(${PROJECT_SOURCE_DIR}/libgramtools/lib/htslib.cmake)
add_dependencies(htslib zlib)

add_custom_target(py_git_version
        COMMAND make -C ${PROJECT_SOURCE_DIR}/gramtools/version)

set(CMAKE_CXX_FLAGS "-std=c++17 -march=native -lpthread -lrt -lm -llzma -msse4.2 -fopenmp -ftrapv -g -O3")

set(EXTERNAL_LIBS_DIR
        ${CMAKE_CURRENT_BINARY_DIR}/lib)
set(EXTERNAL_INCLUDE_DIR
        ${CMAKE_CURRENT_BINARY_DIR}/include)

set(SOURCE
        ${PROJECT_SOURCE_DIR}/libgramtools/src)
set(INCLUDE
        ${PROJECT_SOURCE_DIR}/libgramtools/include)

find_package(BZip2)

# In development mode, makes sure can access the dependencies
include_directories(${EXTERNAL_INCLUDE_DIR})

set(EXTERN_LIBS
        boost_filesystem
        boost_program_options
        boost_timer
        boost_system
        boost_chrono
        boost_random
        hts
        divsufsort
        divsufsort64
        z)

link_directories(${EXTERNAL_LIBS_DIR})

set(SOURCE_FILES
        ${SOURCE}/common/utils.cpp
        ${SOURCE}/common/timer_report.cpp

        ${SOURCE}/search/search.cpp
        
        ${SOURCE}/build/build.cpp
        ${SOURCE}/build/parameters.cpp

        ${SOURCE}/quasimap/quasimap.cpp
        ${SOURCE}/quasimap/parameters.cpp
        ${SOURCE}/quasimap/utils.cpp
        ${SOURCE}/quasimap/coverage/common.cpp
        ${SOURCE}/quasimap/coverage/allele_sum.cpp
        ${SOURCE}/quasimap/coverage/allele_base.cpp
        ${SOURCE}/quasimap/coverage/grouped_allele_counts.cpp

        ${SOURCE}/kmer_index/kmers.cpp
        ${SOURCE}/kmer_index/build.cpp
        ${SOURCE}/kmer_index/load.cpp
        ${SOURCE}/kmer_index/dump.cpp

        ${SOURCE}/prg/prg.cpp
        ${SOURCE}/prg/masks.cpp
        ${SOURCE}/prg/dna_ranks.cpp
        ${SOURCE}/prg/fm_index.cpp)

set(INCLUDE_FILES
        ${INCLUDE}/common/utils.hpp
        ${INCLUDE}/common/timer_report.hpp

        ${INCLUDE}/search/search.hpp
        ${INCLUDE}/search/search_types.hpp

        ${INCLUDE}/build/build.hpp
        ${INCLUDE}/build/parameters.hpp

        ${INCLUDE}/quasimap/quasimap.hpp
        ${INCLUDE}/quasimap/parameters.hpp
        ${INCLUDE}/quasimap/utils.hpp
        ${INCLUDE}/quasimap/coverage/common.hpp
        ${INCLUDE}/quasimap/coverage/allele_sum.hpp
        ${INCLUDE}/quasimap/coverage/allele_base.hpp
        ${INCLUDE}/quasimap/coverage/grouped_allele_counts.hpp
        ${INCLUDE}/quasimap/coverage/types.hpp

        ${INCLUDE}/kmer_index/kmers.hpp
        ${INCLUDE}/kmer_index/kmer_index_types.hpp
        ${INCLUDE}/kmer_index/build.hpp
        ${INCLUDE}/kmer_index/load.hpp
        ${INCLUDE}/kmer_index/dump.hpp

        ${INCLUDE}/prg/prg.hpp
        ${INCLUDE}/prg/masks.hpp
        ${INCLUDE}/prg/dna_ranks.hpp
        ${INCLUDE}/prg/fm_index.hpp)

# libgramtools
add_library(gramtools STATIC
        ${SOURCE_FILES}
        ${INCLUDE_FILES})
target_include_directories(gramtools PUBLIC
        ${INCLUDE}
        ${EXTERNAL_INCLUDE_DIR}
        ${BZIP_INCLUDE_DIRS}
        ${LZMA_INCLUDE_DIR})
target_link_libraries(gramtools LINK_PUBLIC
        ${EXTERN_LIBS}
        ${BZIP2_LIBRARIES}
        ${CMAKE_CURRENT_BINARY_DIR}/lib/libsdsl.a)
set_target_properties(gramtools
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/cmake-build-debug/lib
        LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/cmake-build-debug/lib)
add_dependencies(gramtools
        sdsl
        zlib
        htslib
        boost
        py_git_version)
add_custom_command(TARGET gramtools POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory
        ${PROJECT_SOURCE_DIR}/gramtools/lib
        COMMAND ${CMAKE_COMMAND} -E copy
        ${PROJECT_SOURCE_DIR}/cmake-build-debug/lib/libgramtools.a
        ${PROJECT_SOURCE_DIR}/gramtools/lib
        COMMAND ${CMAKE_COMMAND} -E copy
        ${PROJECT_SOURCE_DIR}/cmake-build-debug/libgramtools/lib/*
        ${PROJECT_SOURCE_DIR}/gramtools/lib)

# gram executable
add_executable(gram
        ${SOURCE}/main.cpp
        ${SOURCE}/common/timer_report.cpp)
add_dependencies(gram py_git_version)
target_include_directories(gram PUBLIC
        ${INCLUDE}
        ${EXTERNAL_INCLUDE_DIR}
        ${BZIP_INCLUDE_DIRS}
        ${LZMA_INCLUDE_DIR})
target_link_libraries(gram LINK_PUBLIC
        gramtools
        ${EXTERN_LIBS}
        ${BZIP2_LIBRARIES}
        ${CMAKE_CURRENT_BINARY_DIR}/lib/libsdsl.a)
set_target_properties(gram
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/cmake-build-debug/bin
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON)
add_custom_command(TARGET gram POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory
        ${PROJECT_SOURCE_DIR}/gramtools/bin
        COMMAND ${CMAKE_COMMAND} -E copy
        ${PROJECT_SOURCE_DIR}/cmake-build-debug/bin/gram
        ${PROJECT_SOURCE_DIR}/gramtools/bin)

# tests
enable_testing()
add_subdirectory(tests)
add_test(test_main test_main)
