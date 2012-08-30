cmake_minimum_required (VERSION 2.8)

project (treebeard CXX)
#add_subdirectory (src)
#add_subdirectory (examples)
#add_subdirectory (test)

include_directories ("src")
include_directories ("examples")
include_directories ("test")

# -DTREEBEARD_HAVE_EDITLINE is a HACK
add_definitions(-std=c++11 -g -Wall -DTREEBEARD_HAVE_EDITLINE)

set (TREEBEARD_SRC
     src/treebeard.h
     src/treebeard/builders.h
     src/treebeard/builders.cpp
     src/treebeard/common.h
     src/treebeard/exceptions.h
     src/treebeard/input.h
     src/treebeard/input.cpp
     src/treebeard/option_description.h
     src/treebeard/option_tree.h
     src/treebeard/option_tree.cpp
     src/treebeard/parser.h
     src/treebeard/parser.cpp
    )

add_library (treebeard ${TREEBEARD_SRC})
target_link_libraries (treebeard edit curses)

add_executable(calculator examples/calculator.cpp)
target_link_libraries(calculator treebeard)

# target etags/tags
IF (UNIX)
  ADD_CUSTOM_TARGET(tags etags --language=c++ --members --declarations ${TREEBEARD_SRC})
  ADD_CUSTOM_TARGET(etags DEPENDS tags)
ENDIF (UNIX)
