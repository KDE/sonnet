include_directories(${HSPELL_INCLUDE_DIR})

find_package(ZLIB)
set_package_properties(ZLIB PROPERTIES DESCRIPTION "Support for gzip compressed files and data streams"
                       URL "http://www.zlib.net"
                       TYPE REQUIRED
                       PURPOSE "Required by the hspell sonnet plugin"
                       )


########### next target ###############

set(sonnet_hspell_PART_SRCS hspellclient.cpp hspelldict.cpp)

ecm_qt_declare_logging_category(sonnet_hspell_PART_SRCS
    HEADER hspell_debug.h
    IDENTIFIER SONNET_LOG_HSPELL
    CATEGORY_NAME kf.sonnet.clients.hspell
    OLD_CATEGORY_NAMES sonnet.plugins.hspell
    DESCRIPTION "Sonnet Hspell plugin"
    EXPORT SONNET
)

add_library(sonnet_hspell MODULE ${sonnet_hspell_PART_SRCS})

target_link_libraries(sonnet_hspell PRIVATE KF5::SonnetCore ${HSPELL_LIBRARIES} ZLIB::ZLIB)

if (TARGET Qt6::Core5Compat)
    target_link_libraries(sonnet_hspell PRIVATE Qt6::Core5Compat) # QTextCodec
endif()

install(TARGETS sonnet_hspell  DESTINATION ${KDE_INSTALL_PLUGINDIR}/kf${QT_MAJOR_VERSION}/sonnet/)

