# needs windows 8 or higher
add_definitions(-DWINVER=0x0602 -D_WIN32_WINNT=0x0602)

set(sonnet_ispellchecker_PART_SRCS
    ispellcheckerclient.cpp
    ispellcheckerdict.cpp
)
ecm_qt_declare_logging_category(sonnet_ispellchecker_PART_SRCS
    HEADER ispellcheckerdebug.h
    IDENTIFIER SONNET_ISPELLCHECKER
    CATEGORY_NAME kf.sonnet.clients.ispellchecker
    OLD_CATEGORY_NAMES sonnet.plugins.ispellchecker
    DESCRIPTION "Sonnet ISpellChecker plugin"
    EXPORT SONNET
)

add_library(sonnet_ispellchecker MODULE ${sonnet_ispellchecker_PART_SRCS})
target_link_libraries(sonnet_ispellchecker PRIVATE KF5::SonnetCore)
target_compile_definitions(sonnet_ispellchecker PRIVATE DEFINITIONS SONNET_INSTALL_PREFIX="${CMAKE_INSTALL_PREFIX}")

install(TARGETS sonnet_ispellchecker  DESTINATION ${KDE_INSTALL_PLUGINDIR}/kf${QT_MAJOR_VERSION}/sonnet/)
