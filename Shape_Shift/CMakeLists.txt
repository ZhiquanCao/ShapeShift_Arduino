add_library(Game game.cpp)
file(GLOB SOURCES "*.cpp" "*.c")
add_executable(Shape_Shift Shape_Shift.ino)

target_link_libraries(Shape_Shift Shape game)
 
