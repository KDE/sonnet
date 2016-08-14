find_package(ZLIB)
set_package_properties(ZLIB PROPERTIES DESCRIPTION "Support for gzip compressed files and data streams"
                       URL "http://www.zlib.net"
                       TYPE REQUIRED
                       PURPOSE "Required by the hspell sonnet plugin"
                      )

include_directories(${HSPELL_INCLUDE_DIR})


########### next target ###############

set(sonnet_hspell_PART_SRCS hspellclient.cpp hspelldict.cpp hspell_debug.cpp)


add_library(sonnet_hspell MODULE ${sonnet_hspell_PART_SRCS})

target_link_libraries(sonnet_hspell PRIVATE KF5::SonnetCore ${HSPELL_LIBRARIES} ${ZLIB_LIBRARY})

set_target_properties(sonnet_hspell PROPERTIES OUTPUT_NAME "hspell")
install(TARGETS sonnet_hspell  DESTINATION ${KDE_INSTALL_PLUGINDIR}/kf5/sonnet/)

