add_library(KF6SonnetCore)
add_library(KF6::SonnetCore ALIAS KF6SonnetCore)

set_target_properties(KF6SonnetCore PROPERTIES
    VERSION     ${SONNET_VERSION}
    SOVERSION   ${SONNET_SOVERSION}
    EXPORT_NAME SonnetCore
)

target_sources(KF6SonnetCore PRIVATE
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
if (TARGET KF6::parsetrigrams)
    add_custom_command(OUTPUT "${CMAKE_BINARY_DIR}/data/trigrams.map"
                       COMMAND KF6::parsetrigrams "${CMAKE_SOURCE_DIR}/data/trigrams" > "${CMAKE_BINARY_DIR}/data/trigrams.map")
    configure_file(${CMAKE_SOURCE_DIR}/data/trigrams.qrc.in ${CMAKE_BINARY_DIR}/data/trigrams.qrc @ONLY)
    target_sources(KF6SonnetCore PRIVATE "${CMAKE_BINARY_DIR}/data/trigrams.qrc")
endif()

ecm_qt_declare_logging_category(KF6SonnetCore
    HEADER core_debug.h
    IDENTIFIER SONNET_LOG_CORE
    CATEGORY_NAME kf.sonnet.core
    OLD_CATEGORY_NAMES sonnet.core
    DESCRIPTION "Sonnet Core"
    EXPORT SONNET
)

# Dear packagers, this is just used as an extra search paths for plugins. Don't get your panties in a twist.
add_definitions(-DINSTALLATION_PLUGIN_PATH="${CMAKE_INSTALL_PREFIX}/${KDE_INSTALL_PLUGINDIR}")

ecm_generate_export_header(KF6SonnetCore
    BASE_NAME SonnetCore
    GROUP_BASE_NAME KF
    VERSION ${KF_VERSION}
    USE_VERSION_HEADER
    VERSION_BASE_NAME Sonnet
    DEPRECATED_BASE_VERSION 0
    DEPRECATION_VERSIONS
    EXCLUDE_DEPRECATED_BEFORE_AND_AT ${EXCLUDE_DEPRECATED_BEFORE_AND_AT}
)

ecm_generate_headers(SonnetCore_CamelCase_HEADERS
  HEADER_NAMES
    BackgroundChecker
    Speller
    GuessLanguage
    Settings
  PREFIX Sonnet
  REQUIRED_HEADERS SonnetCore_HEADERS
)

target_link_libraries(KF6SonnetCore PUBLIC Qt6::Core)

target_include_directories(KF6SonnetCore
    INTERFACE
        "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR_KF}/SonnetCore>"
        "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR_KF}/Sonnet>"
    PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>" # version header
)

install(TARGETS KF6SonnetCore EXPORT KF6SonnetTargets ${KF_INSTALL_TARGETS_DEFAULT_ARGS})

install(FILES ${SonnetCore_CamelCase_HEADERS} DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF}/SonnetCore/Sonnet COMPONENT Devel)

install(FILES
   ${SonnetCore_HEADERS}
   ${CMAKE_CURRENT_BINARY_DIR}/sonnetcore_export.h
   DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF}/SonnetCore/sonnet COMPONENT Devel)

if (BUILD_QCH)
    ecm_add_qch(
        KF6SonnetCore_QCH
        NAME SonnetCore
        BASE_NAME KF6SonnetCore
        VERSION ${KF_VERSION}
        ORG_DOMAIN org.kde
        SOURCES # using only public headers, to cover only public API
            ${SonnetCore_HEADERS}
        LINK_QCHS
            Qt6Core_QCH
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
ecm_generate_pri_file(
    BASE_NAME SonnetCore
    LIB_NAME KF6SonnetCore
    DEPS "core"
    FILENAME_VAR PRI_FILENAME
    INCLUDE_INSTALL_DIRS
        ${KDE_INSTALL_INCLUDEDIR_KF}/Sonnet
        ${KDE_INSTALL_INCLUDEDIR_KF}/SonnetCore
)
install(FILES ${PRI_FILENAME} DESTINATION ${ECM_MKSPECS_INSTALL_DIR})
