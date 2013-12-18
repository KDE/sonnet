project(sonnetui)

include_directories(
  ${QT_INCLUDES}
  ${CMAKE_CURRENT_BINARY_DIR}
)

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../core
    ${CMAKE_CURRENT_BINARY_DIR}/../core
)

set(sonnetui_SRCS
    configdialog.cpp
    configwidget.cpp
    dialog.cpp
    dictionarycombobox.cpp
    highlighter.cpp
    spellcheckdecorator.cpp
)

set(sonnetui_UI
    configui.ui
    sonnetui.ui
)

set(sonnetui_HDRS
    dialog.h
    highlighter.h
    configdialog.h
    configwidget.h
    dictionarycombobox.h
    spellcheckdecorator.h
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

target_include_directories(KF5SonnetUi PUBLIC
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>" # needed?
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>" # needed?
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/..>"
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/../include>" # sonnet/*.h
)

install(TARGETS KF5SonnetUi EXPORT KF5SonnetTargets ${INSTALL_TARGETS_DEFAULT_ARGS})

install( FILES ${sonnetui_HDRS} ${CMAKE_CURRENT_BINARY_DIR}/sonnetui_export.h
         DESTINATION ${INCLUDE_INSTALL_DIR}/sonnet COMPONENT Devel)

