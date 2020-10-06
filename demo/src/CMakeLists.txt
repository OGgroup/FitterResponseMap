add_library(pathbfrp OBJECT
  src/ProgressBar.cc
  src/ArgParsePB.cc
  src/AstaireDS.cc
  )

target_link_libraries(pathbfrp PUBLIC fitterresponsemap RATEvent)

target_include_directories(pathbfrp PUBLIC 
  $<BUILD_INTERFACE:${FRP_INCLUDE_DIR}>
  $<INSTALL_INTERFACE:include>)
install(TARGETS pathbfrp 
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

file(COPY include/ DESTINATION ${FRP_INCLUDE_DIR})

install(DIRECTORY ${FRP_INCLUDE_DIR}
        DESTINATION ${CMAKE_INSTALL_PREFIX})
