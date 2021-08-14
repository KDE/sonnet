add_subdirectory(core)
add_subdirectory(plugins)

if(TARGET Qt5::Widgets)
    add_subdirectory(ui)
endif()

if(TARGET Qt5::Quick)
    add_subdirectory(quick)
endif()

ecm_qt_install_logging_categories(
    EXPORT SONNET
    FILE sonnet.categories
    DESTINATION ${KDE_INSTALL_LOGGINGCATEGORIESDIR}
)
