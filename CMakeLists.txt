cmake_minimum_required (VERSION 2.8)

project (treebeard CXX)
#add_subdirectory (src)
#add_subdirectory (examples)
#add_subdirectory (test)

include_directories ("src")
include_directories ("examples")
include_directories ("test")

# HACK: Do not use the outdated Boost version 1.46 that comes along
#       with Mint but try to use a more recent one.
# TODO: What is the right way?
IF (IS_DIRECTORY "/opt/boost")
  include_directories ("/opt/boost")
  link_directories("/opt/boost/lib")
ENDIF (IS_DIRECTORY "/opt/boost")

# -DTREEBEARD_HAVE_EDITLINE is a HACK
add_definitions(-std=c++11 -g -Wall -DTREEBEARD_HAVE_EDITLINE)
# TODO: Works for now but it would be nice to support to
#       link agains readline if its GPL license is not a problem.

set (TREEBEARD_SRC
     src/treebeard.h
     src/treebeard/builders.h
     src/treebeard/builders.cpp
     src/treebeard/common.h
     src/treebeard/exceptions.h
     src/treebeard/input.h
     src/treebeard/input.cpp
     src/treebeard/file_option.h
     src/treebeard/file_option.cpp
     src/treebeard/option_description.h
     src/treebeard/option_tree.h
     src/treebeard/option_tree.cpp
     src/treebeard/parser.h
     src/treebeard/parser.cpp
    )

add_library (treebeard ${TREEBEARD_SRC})
target_link_libraries (treebeard edit curses boost_filesystem boost_system)

add_executable(calculator examples/calculator.cpp)
target_link_libraries(calculator treebeard)

# target etags/tags
IF (UNIX)
  ADD_CUSTOM_TARGET(tags etags --language=c++ --members --declarations ${TREEBEARD_SRC})
  ADD_CUSTOM_TARGET(etags DEPENDS tags)
ENDIF (UNIX)
