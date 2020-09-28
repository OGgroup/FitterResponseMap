# Demo CMakeLists

# If rat is install we can compile against it
if(DEFINED ENV{RATROOT})
  include_directories($ENV{RATROOT}/include)
  link_directories($ENV{RATROOT}/lib)

  add_executable(frprat frprat.cc)
  target_link_libraries(frprat PUBLIC fitterresponsemap RATEvent)
  install(TARGETS frprat RUNTIME DESTINATION bin)
endif()

add_executable(frpdemo frpdemo.cc)
target_link_libraries(frpdemo PUBLIC fitterresponsemap)

install(TARGETS frpdemo
  RUNTIME DESTINATION bin)
