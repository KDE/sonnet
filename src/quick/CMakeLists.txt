
add_library(sonnetquickplugin sonnetquickplugin.cpp
    sonnetquickplugin.h
    spellcheckhighlighter.cpp
    spellcheckhighlighter.h )

ecm_qt_declare_logging_category(sonnetquickplugin
    HEADER quick_debug.h
    IDENTIFIER SONNET_LOG_QUICK
    CATEGORY_NAME kf.sonnet.quick
    OLD_CATEGORY_NAMES sonnet.quick
    DESCRIPTION "Sonnet Quick bindings"
    EXPORT SONNET
)
target_link_libraries(sonnetquickplugin
    PUBLIC Qt${QT_MAJOR_VERSION}::Quick
    PRIVATE KF5SonnetCore
)

set(bindest "${CMAKE_BINARY_DIR}/bin/qml/org/kde/sonnet/")
set_target_properties(sonnetquickplugin PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${bindest}")

install(TARGETS sonnetquickplugin DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/sonnet)
install(FILES qmldir DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/sonnet)

ecm_generate_qmltypes(org.kde.sonnet 1.0 DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/sonnet)
