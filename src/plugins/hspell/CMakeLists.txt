find_package(ZLIB)
set_package_properties(ZLIB PROPERTIES DESCRIPTION "Support for gzip compressed files and data streams"
                       URL "http://www.zlib.net"
                       TYPE REQUIRED
                       PURPOSE "Required by the hspell sonnet plugin"
                      )

include_directories(${HSPELL_INCLUDE_DIR})


########### next target ###############

set(sonnet_hspell_PART_SRCS hspellclient.cpp hspelldict.cpp )


add_library(sonnet_hspell MODULE ${sonnet_hspell_PART_SRCS})

target_link_libraries(sonnet_hspell PRIVATE KF5::SonnetCore ${HSPELL_LIBRARIES} ${ZLIB_LIBRARY})

install(TARGETS sonnet_hspell  DESTINATION ${PLUGIN_INSTALL_DIR}/sonnet/ )

