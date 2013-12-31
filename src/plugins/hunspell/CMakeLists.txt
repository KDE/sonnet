
########### next target ###############

include_directories( ${HUNSPELL_INCLUDE_DIR} )
set(kspell_hunspell_PART_SRCS kspell_hunspellclient.cpp kspell_hunspelldict.cpp )


add_library(sonnet_hunspell MODULE ${kspell_hunspell_PART_SRCS})

target_link_libraries(sonnet_hunspell PRIVATE KF5::SonnetCore ${HUNSPELL_LIBRARIES})

install(TARGETS sonnet_hunspell  DESTINATION ${PLUGIN_INSTALL_DIR}/sonnet_clients/ )


