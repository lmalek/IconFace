cmake_minimum_required(VERSION 2.6)
message(STATUS "[  ----     IconFace     ----  ]")
project(IconFace)
#

# search for allegro library - needed for testing
execute_process(COMMAND allegro-config --libs RESULT_VARIABLE ALLEGRO_NOTFOUND 
  OUTPUT_VARIABLE ALLEGRO_LIBRARIES
  ERROR_QUIET 
  OUTPUT_STRIP_TRAILING_WHITESPACE
  ERROR_STRIP_TRAILING_WHITESPACE
)
if (ALLEGRO_NOTFOUND)
  message(FATAL_ERROR "Can't find Allegro!")
endif(ALLEGRO_NOTFOUND)

add_executable(IconFace src/IconFace.cpp)
# include directories for IconFace and Allegro
include_directories(include ${OPENCV_INCLUDE_DIRS} )
# linking
target_link_libraries(IconFace ${ALLEGRO_LIBRARIES})
