ecm_create_qm_loader(sonnet_QM_LOADER sonnet5_qt)

set(sonnetui_SRCS
    configdialog.cpp
    configview.cpp
    configwidget.cpp
    dialog.cpp
    dictionarycombobox.cpp
    highlighter.cpp
    spellcheckdecorator.cpp
    ${sonnet_QM_LOADER}
)

ecm_qt_declare_logging_category(sonnetui_SRCS
    HEADER ui_debug.h
    IDENTIFIER SONNET_LOG_UI
    CATEGORY_NAME kf.sonnet.ui
    OLD_CATEGORY_NAMES sonnet.ui
    DESCRIPTION "Sonnet UI"
    EXPORT SONNET
)

set(sonnetui_UI
    configui.ui
    sonnetui.ui
)

ecm_generate_headers(SonnetUi_CamelCase_HEADERS
  HEADER_NAMES
    Dialog
    Highlighter
    ConfigDialog
    ConfigView
    ConfigWidget
    DictionaryComboBox
    SpellCheckDecorator
  PREFIX Sonnet
  REQUIRED_HEADERS SonnetUi_HEADERS
)

qt_wrap_ui(sonnetui_SRCS ${sonnetui_UI})

add_library(KF5SonnetUi ${sonnetui_SRCS})
ecm_generate_export_header(KF5SonnetUi
    BASE_NAME SonnetUi
    GROUP_BASE_NAME KF
    VERSION ${KF_VERSION}
    DEPRECATED_BASE_VERSION 0
    DEPRECATION_VERSIONS 5.65
    EXCLUDE_DEPRECATED_BEFORE_AND_AT ${EXCLUDE_DEPRECATED_BEFORE_AND_AT}
)

add_library(KF5::SonnetUi ALIAS KF5SonnetUi)

target_link_libraries(KF5SonnetUi
    PUBLIC Qt${QT_MAJOR_VERSION}::Widgets
    PRIVATE KF5::SonnetCore
)

set_target_properties(KF5SonnetUi PROPERTIES VERSION ${SONNET_VERSION}
                                             SOVERSION ${SONNET_SOVERSION}
                                             EXPORT_NAME SonnetUi
)


# CMAKE_CURRENT_BINARY_DIR: for camelcase headers and lowercase forwarders
target_include_directories(KF5SonnetUi INTERFACE
    "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR_KF}/SonnetUi>"
    "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR_KF}/Sonnet>"
)
target_include_directories(KF5SonnetUi PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>")

install(TARGETS KF5SonnetUi EXPORT KF5SonnetTargets ${KF5_INSTALL_TARGETS_DEFAULT_ARGS})

install(FILES ${SonnetUi_CamelCase_HEADERS} DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF}/SonnetUi/Sonnet COMPONENT Devel)

install(FILES
   ${SonnetUi_HEADERS}
   ${CMAKE_CURRENT_BINARY_DIR}/sonnetui_export.h
   DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF}/SonnetUi/sonnet COMPONENT Devel)

if(BUILD_DESIGNERPLUGIN)
    add_subdirectory(designer)
endif()

if (BUILD_QCH)
    ecm_add_qch(
        KF5SonnetUi_QCH
        NAME SonnetUi
        BASE_NAME KF5SonnetUi
        VERSION ${KF_VERSION}
        ORG_DOMAIN org.kde
        SOURCES # using only public headers, to cover only public API
            ${SonnetUi_HEADERS}
        LINK_QCHS
            Qt5Core_QCH
            Qt5Gui_QCH
            Qt5Widgets_QCH
        INCLUDE_DIRS
            ${CMAKE_CURRENT_BINARY_DIR}
        BLANK_MACROS
            SONNETUI_EXPORT
            SONNETUI_DEPRECATED_EXPORT
            SONNETUI_DEPRECATED
            "SONNETUI_DEPRECATED_VERSION(x, y, t)"
        TAGFILE_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        QCH_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        COMPONENT Devel
    )
endif()

include(ECMGeneratePriFile)
ecm_generate_pri_file(BASE_NAME SonnetUi LIB_NAME KF5SonnetUi DEPS "widgets" FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR ${KDE_INSTALL_INCLUDEDIR_KF}/SonnetUi)
install(FILES ${PRI_FILENAME} DESTINATION ${ECM_MKSPECS_INSTALL_DIR})
