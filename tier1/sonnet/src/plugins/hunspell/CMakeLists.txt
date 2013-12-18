
########### next target ###############

include_directories( ${HUNSPELL_INCLUDE_DIR} )
set(kspell_hunspell_PART_SRCS kspell_hunspellclient.cpp kspell_hunspelldict.cpp )


add_library(kspell_hunspell MODULE ${kspell_hunspell_PART_SRCS})

target_link_libraries(kspell_hunspell PRIVATE KF5::SonnetCore ${HUNSPELL_LIBRARIES})

install(TARGETS kspell_hunspell  DESTINATION ${PLUGIN_INSTALL_DIR}/plugins/sonnet_clients/ )


