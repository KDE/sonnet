find_package(ZLIB)
set_package_properties(ZLIB PROPERTIES DESCRIPTION "Support for gzip compressed files and data streams"
                       URL "http://www.zlib.net"
                       TYPE REQUIRED
                       PURPOSE "Required by the hspell sonnet plugin"
                      )

include_directories( ${HSPELL_INCLUDE_DIR}  )


########### next target ###############

set(kspell_hspell_PART_SRCS kspell_hspellclient.cpp kspell_hspelldict.cpp )


add_library(kspell_hspell MODULE ${kspell_hspell_PART_SRCS})

target_link_libraries(kspell_hspell PRIVATE KF5::SonnetCore ${HSPELL_LIBRARIES} ${ZLIB_LIBRARY})

install(TARGETS kspell_hspell  DESTINATION ${PLUGIN_INSTALL_DIR}/plugins/sonnet_clients/ )

