# Demo CMakeLists

add_executable(frpdemo frpdemo.cc)
target_link_libraries(frpdemo PUBLIC fitterresponsemap)

install(TARGETS frpdemo
  RUNTIME DESTINATION bin)
