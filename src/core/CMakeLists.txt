set(sonnetcore_SRCS
    backgroundchecker.cpp
    backgroundchecker.h
    backgroundchecker_p.h
    client.cpp
    client_p.h
    guesslanguage.cpp
    guesslanguage.h
    languagefilter.cpp
    languagefilter_p.h
    loader.cpp
    loader_p.h
    settings.cpp
    settings.h
    settingsimpl.cpp
    settingsimpl_p.h
    speller.cpp
    speller.h
    spellerplugin.cpp
    spellerplugin_p.h
    textbreaks.cpp
    textbreaks_p.h
    tokenizer.cpp
    tokenizer_p.h
)

# create trigrams file + add trigrams resource
if (TARGET KF5::parsetrigrams)
    add_custom_command(OUTPUT "${CMAKE_BINARY_DIR}/data/trigrams.map"
                       COMMAND KF5::parsetrigrams "${CMAKE_SOURCE_DIR}/data/trigrams" > "${CMAKE_BINARY_DIR}/data/trigrams.map")
    configure_file(${CMAKE_SOURCE_DIR}/data/trigrams.qrc.in ${CMAKE_BINARY_DIR}/data/trigrams.qrc @ONLY)
    qt_add_resources(sonnetcore_SRCS "${CMAKE_BINARY_DIR}/data/trigrams.qrc")
endif()

ecm_qt_declare_logging_category(sonnetcore_SRCS
    HEADER core_debug.h
    IDENTIFIER SONNET_LOG_CORE
    CATEGORY_NAME kf.sonnet.core
    OLD_CATEGORY_NAMES sonnet.core
    DESCRIPTION "Sonnet Core"
    EXPORT SONNET
)

# Dear packagers, this is just used as an extra search paths for plugins. Don't get your panties in a twist.
add_definitions(-DINSTALLATION_PLUGIN_PATH="${CMAKE_INSTALL_PREFIX}/${KDE_INSTALL_PLUGINDIR}")


add_library(KF5SonnetCore ${sonnetcore_SRCS})
ecm_generate_export_header(KF5SonnetCore
    BASE_NAME SonnetCore
    GROUP_BASE_NAME KF
    VERSION ${KF_VERSION}
    DEPRECATED_BASE_VERSION 0
    #DEPRECATION_VERSIONS 5.65
    EXCLUDE_DEPRECATED_BEFORE_AND_AT ${EXCLUDE_DEPRECATED_BEFORE_AND_AT}
)


add_library(KF5::SonnetCore ALIAS KF5SonnetCore)

ecm_generate_headers(SonnetCore_CamelCase_HEADERS
  HEADER_NAMES
    BackgroundChecker
    Speller
    GuessLanguage
    Settings
  PREFIX Sonnet
  REQUIRED_HEADERS SonnetCore_HEADERS
)

target_link_libraries(KF5SonnetCore PUBLIC Qt${QT_MAJOR_VERSION}::Core)

set_target_properties(KF5SonnetCore PROPERTIES VERSION ${SONNET_VERSION}
                                               SOVERSION ${SONNET_SOVERSION}
                                               EXPORT_NAME SonnetCore
)

# CMAKE_CURRENT_BINARY_DIR: for camelcase headers and lowercase forwarders
target_include_directories(KF5SonnetCore INTERFACE
    "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR_KF}/SonnetCore>"
    "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR_KF}/Sonnet>"
)
target_include_directories(KF5SonnetCore PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>")

install(TARGETS KF5SonnetCore EXPORT KF5SonnetTargets ${KF5_INSTALL_TARGETS_DEFAULT_ARGS})

install(FILES ${SonnetCore_CamelCase_HEADERS} DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF}/SonnetCore/Sonnet COMPONENT Devel)

install(FILES
   ${SonnetCore_HEADERS}
   ${CMAKE_CURRENT_BINARY_DIR}/sonnetcore_export.h
   DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF}/SonnetCore/sonnet COMPONENT Devel)

if (BUILD_QCH)
    ecm_add_qch(
        KF5SonnetCore_QCH
        NAME SonnetCore
        BASE_NAME KF5SonnetCore
        VERSION ${KF_VERSION}
        ORG_DOMAIN org.kde
        SOURCES # using only public headers, to cover only public API
            ${SonnetCore_HEADERS}
        LINK_QCHS
            Qt5Core_QCH
        INCLUDE_DIRS
            ${CMAKE_CURRENT_BINARY_DIR}
        BLANK_MACROS
            SONNETCORE_EXPORT
            SONNETCORE_DEPRECATED_EXPORT
            SONNETCORE_DEPRECATED
            "SONNETCORE_DEPRECATED_VERSION(x, y, t)"
        TAGFILE_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        QCH_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        COMPONENT Devel
    )
endif()

include(ECMGeneratePriFile)
ecm_generate_pri_file(BASE_NAME SonnetCore LIB_NAME KF5SonnetCore DEPS "core" FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR ${KDE_INSTALL_INCLUDEDIR_KF}/SonnetCore)
install(FILES ${PRI_FILENAME} DESTINATION ${ECM_MKSPECS_INSTALL_DIR})
