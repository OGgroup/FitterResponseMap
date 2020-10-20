add_library(fitterresponsemap SHARED 
  src/FitterResponseMap.cc
  src/EnergyResponse.cc
  src/VertexResponse.cc
  src/PMTGenerator.cc
  src/MCLookup.cc
  src/json.cc
  )

target_link_libraries(fitterresponsemap PUBLIC
  ${ROOT_LIBRARIES}
  Threads::Threads)

target_include_directories(fitterresponsemap PUBLIC 
  $<BUILD_INTERFACE:${FRP_INCLUDE_DIR}>
  $<INSTALL_INTERFACE:include>)

add_library(frp OBJECT
  src/FitterResponseMap.cc
  src/EnergyResponse.cc
  src/VertexResponse.cc
  src/PMTGenerator.cc
  src/MCLookup.cc
  src/json.cc
  )

target_link_libraries(frp PUBLIC
  ${ROOT_LIBRARIES}
  Threads::Threads)

target_include_directories(frp PUBLIC 
  $<BUILD_INTERFACE:${FRP_INCLUDE_DIR}>
  $<INSTALL_INTERFACE:include>)

file(COPY include/ DESTINATION ${FRP_INCLUDE_DIR})

install(TARGETS fitterresponsemap frp
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

install(DIRECTORY ${FRP_INCLUDE_DIR}
        DESTINATION ${CMAKE_INSTALL_PREFIX})
