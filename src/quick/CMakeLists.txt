qt_add_resources(sonnetquick_SRCS qmlfiles.qrc)

ecm_qt_declare_logging_category(sonnetquick_SRCS
    HEADER quick_debug.h
    IDENTIFIER SONNET_LOG_QUICK
    CATEGORY_NAME kf.sonnet.quick
    OLD_CATEGORY_NAMES sonnet.quick
    DESCRIPTION "Sonnet Quick bindings"
    EXPORT SONNET
)

add_library(sonnetquickplugin sonnetquickplugin.cpp spellcheckhighlighter.cpp ${sonnetquick_SRCS})

target_link_libraries(sonnetquickplugin
    PUBLIC Qt5::Quick
    PRIVATE KF5SonnetCore
)

set(bindest "${CMAKE_BINARY_DIR}/bin/qml/org/kde/sonnet/")
set_target_properties(sonnetquickplugin PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${bindest}")

add_custom_target(moveqmldir ALL
    COMMAND cmake -E make_directory -p ${bindest}
    COMMAND cmake -E copy ${CMAKE_CURRENT_SOURCE_DIR}/qmldir ${bindest}
)

install(TARGETS sonnetquickplugin DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/sonnet)
install(FILES qmldir DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/sonnet)

ecm_generate_qmltypes(org.kde.sonnet 1.0 DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/sonnet)
