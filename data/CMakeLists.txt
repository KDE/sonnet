include(ECMMarkNonGuiExecutable)

if(NOT CMAKE_CROSSCOMPILING)
    add_executable(parsetrigrams parsetrigrams.cpp)
    add_executable(gentrigrams gentrigrams.cpp)

    # Mark it as non-gui so we won't create an app bundle on Mac OS X
    ecm_mark_nongui_executable(parsetrigrams)
    ecm_mark_nongui_executable(gentrigrams)

    TARGET_LINK_LIBRARIES(parsetrigrams PUBLIC Qt${QT_MAJOR_VERSION}::Core)
    TARGET_LINK_LIBRARIES(gentrigrams PUBLIC Qt${QT_MAJOR_VERSION}::Core KF5::SonnetCore)
    INSTALL(TARGETS parsetrigrams ${KF5_INSTALL_TARGETS_DEFAULT_ARGS})
    INSTALL(TARGETS gentrigrams ${KF5_INSTALL_TARGETS_DEFAULT_ARGS})

    add_executable(KF5::parsetrigrams ALIAS parsetrigrams)
else()
    if (NOT PARSETRIGRAMS_EXECUTABLE AND KF5_HOST_TOOLING)
        find_program(PARSETRIGRAMS_EXECUTABLE parsetrigrams PATHS ${KF5_HOST_TOOLING}/${KDE_INSTALL_BINDIR} NO_DEFAULT_PATH)
    endif()
    if (PARSETRIGRAMS_EXECUTABLE)
        add_executable(KF5::parsetrigrams IMPORTED GLOBAL)
        set_target_properties(KF5::parsetrigrams PROPERTIES IMPORTED_LOCATION ${PARSETRIGRAMS_EXECUTABLE})
    endif()
endif()
