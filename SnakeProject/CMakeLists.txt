cmake_minimum_required(VERSION 2.8.3)
project(snake_server CXX)

set(DUELS_ROOT "/opt/duels" CACHE STRING "Path to duels dev folder")
set(CMAKE_INSTALL_PREFIX ${DUELS_ROOT})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)
add_compile_options(-pthread)
find_package (Threads)

# find ZeroMQ
find_path ( ZMQ_INCLUDE_DIR zmq.h )
find_library ( ZMQ_LIBRARY NAMES zmq )

# find yaml-cpp
find_package(PkgConfig)
pkg_check_modules(YamlCpp yaml-cpp)

add_definitions(-DDUELS_BIN_PATH="${DUELS_ROOT}/bin")
add_definitions(-DGAME_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")
add_definitions(-DDUELS_SERVER)

add_executable(snake_server server.cpp mechanics.cpp snake_ai.cpp)
target_include_directories(snake_server PRIVATE 
    include
    ${DUELS_ROOT}/include 
    ${ZMQ_INCLUDE_DIR}
    ${YamlCpp_INCLUDE_DIRS})
target_link_libraries(snake_server ${CMAKE_THREAD_LIBS_INIT} ${ZMQ_LIBRARY} yaml-cpp)

# add whatever is needed to build display executable


# install server
install(TARGETS snake_server DESTINATION bin/snake)

# install gui and its deps
install(FILES snake_gui.py DESTINATION bin/snake
        PERMISSIONS OWNER_WRITE OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
        
# client headers and templates
install(FILES include/duels/snake/game.h include/duels/snake/msg.h include/duels/snake/msg_detail.h DESTINATION include/duels/snake)
install(FILES client_template/CMakeLists.txt DESTINATION games/snake)
install(FILES client_template/snake.cpp DESTINATION games/snake )
