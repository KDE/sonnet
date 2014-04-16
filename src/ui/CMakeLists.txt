project(sonnetui)

include_directories(
  ${QT_INCLUDES}
  ${CMAKE_CURRENT_BINARY_DIR}
)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../core
    ${CMAKE_CURRENT_BINARY_DIR}/../core
)

ecm_create_qm_loader(sonnet_QM_LOADER sonnet5)

set(sonnetui_SRCS
    configdialog.cpp
    configwidget.cpp
    dialog.cpp
    dictionarycombobox.cpp
    highlighter.cpp
    spellcheckdecorator.cpp
    ${sonnet_QM_LOADER}
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
    ConfigWidget
    DictionaryComboBox
    SpellCheckDecorator
  PREFIX Sonnet
  REQUIRED_HEADERS SonnetUi_HEADERS
)

qt5_wrap_ui(sonnetui_SRCS ${sonnetui_UI})

add_library(KF5SonnetUi ${sonnetui_SRCS})
generate_export_header(KF5SonnetUi BASE_NAME SonnetUi EXPORT_FILE_NAME sonnetui_export.h)
add_library(KF5::SonnetUi ALIAS KF5SonnetUi)

target_link_libraries(KF5SonnetUi
    PUBLIC Qt5::Widgets
    PRIVATE KF5::SonnetCore
)

set_target_properties(KF5SonnetUi PROPERTIES VERSION ${SONNET_VERSION_STRING}
                                             SOVERSION ${SONNET_SOVERSION}
                                             EXPORT_NAME SonnetUi
)


# CMAKE_CURRENT_BINARY_DIR: for camelcase headers and lowercase forwarders
target_include_directories(KF5SonnetUi INTERFACE "$<INSTALL_INTERFACE:${INCLUDE_INSTALL_DIR}/SonnetUi>")
target_include_directories(KF5SonnetUi PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>")

install(TARGETS KF5SonnetUi EXPORT KF5SonnetTargets ${INSTALL_TARGETS_DEFAULT_ARGS})

install(FILES ${SonnetUi_CamelCase_HEADERS} DESTINATION ${INCLUDE_INSTALL_DIR}/SonnetUi/Sonnet COMPONENT Devel)

install(FILES
   ${SonnetUi_HEADERS}
   ${CMAKE_CURRENT_BINARY_DIR}/sonnetui_export.h
   DESTINATION ${INCLUDE_INSTALL_DIR}/SonnetUi/sonnet COMPONENT Devel)

include(ECMGeneratePriFile)
ecm_generate_pri_file(BASE_NAME SonnetUi LIB_NAME KF5SonnetUi DEPS "widgets" FILENAME_VAR PRI_FILENAME)
install(FILES ${PRI_FILENAME} DESTINATION ${ECM_MKSPECS_INSTALL_DIR})
