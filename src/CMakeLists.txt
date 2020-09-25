add_library(fitterresponsemap OBJECT
  src/FitterResponseMap.cc
  src/EnergyResponse.cc
  src/VertexResponse.cc
  )

target_link_libraries(fitterresponsemap PUBLIC
  ${ROOT_LIBRARIES}
  Threads::Threads)

target_include_directories(fitterresponsemap PUBLIC 
  $<BUILD_INTERFACE:${FRP_INCLUDE_DIR}>
  $<INSTALL_INTERFACE:include>)

file(COPY include/ DESTINATION ${FRP_INCLUDE_DIR})

install(TARGETS fitterresponsemap
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

install(DIRECTORY ${FRP_INCLUDE_DIR}
        DESTINATION ${CMAKE_INSTALL_PREFIX})
