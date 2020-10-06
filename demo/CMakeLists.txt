# Demo CMakeLists

# If rat is install we can compile against it
if(DEFINED ENV{RATROOT})
  include_directories($ENV{RATROOT}/include)
  link_directories($ENV{RATROOT}/lib)

  add_subdirectory(src)

  add_executable(frprat frprat.cc)
  target_link_libraries(frprat PUBLIC fitterresponsemap RATEvent pathbfrp)
  install(TARGETS frprat RUNTIME DESTINATION bin)

  add_executable(pathb pathb.cc)
  target_link_libraries(pathb PUBLIC fitterresponsemap RATEvent pathbfrp)
  install(TARGETS pathb RUNTIME DESTINATION bin)

  add_executable(astaire astaire.cc)
  target_link_libraries(astaire PUBLIC fitterresponsemap RATEvent pathbfrp)
  install(TARGETS astaire RUNTIME DESTINATION bin)
endif()
