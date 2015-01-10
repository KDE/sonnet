
########### next target ###############

include_directories( ${HUNSPELL_INCLUDE_DIR} )
set(sonnet_hunspell_PART_SRCS hunspellclient.cpp hunspelldict.cpp )


add_library(sonnet_hunspell MODULE ${sonnet_hunspell_PART_SRCS})

target_link_libraries(sonnet_hunspell PRIVATE KF5::SonnetCore ${HUNSPELL_LIBRARIES})

set_target_properties(sonnet_hunspell PROPERTIES OUTPUT_NAME "hunspell")
install(TARGETS sonnet_hunspell  DESTINATION ${KDE_INSTALL_PLUGINDIR}/kf5/sonnet/)


