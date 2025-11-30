set(HEADER_FILES
        util.h
        node.h
        )

set(SOURCE_FILES
        util.cpp
        node.cpp
        parser.cpp
        parser.h
)

add_library(newick_lib STATIC ${SOURCE_FILES} ${HEADER_FILES})
