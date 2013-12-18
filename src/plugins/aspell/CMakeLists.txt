
include_directories( ${ASPELL_INCLUDE_DIR})


########### next target ###############

set(kspell_aspell_PART_SRCS kspell_aspellclient.cpp kspell_aspelldict.cpp )

if(WIN32)
add_definitions(-DCMAKE_INSTALL_PREFIX="${CMAKE_INSTALL_PREFIX}")
endif()
add_library(kspell_aspell MODULE ${kspell_aspell_PART_SRCS})

target_link_libraries(kspell_aspell PRIVATE KF5::SonnetCore ${ASPELL_LIBRARIES})

install(TARGETS kspell_aspell  DESTINATION ${PLUGIN_INSTALL_DIR}/sonnet_clients/ )
