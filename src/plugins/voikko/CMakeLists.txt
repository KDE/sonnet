include_directories(${VOIKKO_INCLUDE_DIR})


########### next target ###############

set(sonnet_voikko_PART_SRCS voikkoclient.cpp voikkodict.cpp)
ecm_qt_declare_logging_category(sonnet_voikko_PART_SRCS
    HEADER voikkodebug.h
    IDENTIFIER SONNET_VOIKKO
    CATEGORY_NAME kf.sonnet.clients.voikko
    OLD_CATEGORY_NAMES sonnet.plugins.voikko
    DESCRIPTION "Sonnet Voikko plugin"
    EXPORT SONNET
)

add_library(sonnet_voikko MODULE ${sonnet_voikko_PART_SRCS})

target_link_libraries(sonnet_voikko PRIVATE KF5::SonnetCore ${VOIKKO_LIBRARIES})

install(TARGETS sonnet_voikko  DESTINATION ${KDE_INSTALL_PLUGINDIR}/kf${QT_MAJOR_VERSION}/sonnet/)
