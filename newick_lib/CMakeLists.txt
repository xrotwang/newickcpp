set(HEADER_FILES
        util.h
        node.h
        parser.h
        argparse.hpp
        )

set(SOURCE_FILES
        util.cpp
        node.cpp
        parser.cpp
)

add_library(newick_lib STATIC ${SOURCE_FILES} ${HEADER_FILES})
