
include_directories( ${ASPELL_INCLUDE_DIR})


########### next target ###############

set(sonnet_aspell_PART_SRCS aspellclient.cpp aspelldict.cpp )

if(WIN32)
add_definitions(-DCMAKE_INSTALL_PREFIX="${CMAKE_INSTALL_PREFIX}")
endif()
add_library(sonnet_aspell MODULE ${sonnet_aspell_PART_SRCS})

target_link_libraries(sonnet_aspell PRIVATE KF5::SonnetCore ${ASPELL_LIBRARIES})

install(TARGETS sonnet_aspell  DESTINATION ${PLUGIN_INSTALL_DIR}/sonnet/ )
