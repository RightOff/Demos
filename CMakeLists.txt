cmake_minimum_required(VERSION 3.13)

project(TEST VERSION 0.0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE RelWithDebInfo)


add_executable(define_test define_test.cpp)
add_executable(enum_test enum_test.cpp)
add_executable(pragma_pack_test pragma_pack_test.cpp)
add_executable(selectBall selectBall.cpp)
add_executable(three_thread three_thread.cpp)
add_executable(produceBall produceBall.cpp)
add_executable(async_test async_test.cpp)
add_executable(circleReference circleReference.cpp)
add_executable(smart_ptr smart_ptr.cpp)
add_executable(array_test array_test.cpp)
add_executable(tuple_test tuple_test.cpp)
add_executable(vector_test vector_test.cpp)
add_executable(template_test template_test.cpp)
add_executable(function function.cpp)


find_package(Threads REQUIRED)
target_link_libraries(three_thread ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(produceBall ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(async_test ${CMAKE_THREAD_LIBS_INIT})