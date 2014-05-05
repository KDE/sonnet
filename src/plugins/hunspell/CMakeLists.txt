
########### next target ###############

include_directories( ${HUNSPELL_INCLUDE_DIR} )
set(sonnet_hunspell_PART_SRCS hunspellclient.cpp hunspelldict.cpp )


add_library(sonnet_hunspell MODULE ${sonnet_hunspell_PART_SRCS})

target_link_libraries(sonnet_hunspell PRIVATE KF5::SonnetCore ${HUNSPELL_LIBRARIES})

install(TARGETS sonnet_hunspell  DESTINATION ${PLUGIN_INSTALL_DIR}/kf5/sonnet_clients/ )


