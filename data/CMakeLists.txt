project(sonnetdata)

include(ECMMarkNonGuiExecutable)

add_executable(parsetrigrams parsetrigrams.cpp)
add_executable(gentrigrams gentrigrams.cpp)

# Mark it as non-gui so we won't create an app bundle on Mac OS X
ecm_mark_nongui_executable(parsetrigrams)
ecm_mark_nongui_executable(gentrigrams)

TARGET_LINK_LIBRARIES(parsetrigrams PUBLIC Qt5::Core)
TARGET_LINK_LIBRARIES(gentrigrams PUBLIC Qt5::Core KF5::SonnetCore)
INSTALL(TARGETS parsetrigrams ${KF5_INSTALL_TARGETS_DEFAULT_ARGS})
INSTALL(TARGETS gentrigrams ${KF5_INSTALL_TARGETS_DEFAULT_ARGS})

if(CMAKE_CROSSCOMPILING AND PARSETRIGRAMS_EXECUTABLE)
    add_executable(KF5::parsetrigrams IMPORTED GLOBAL)
    set_target_properties(KF5::parsetrigrams PROPERTIES IMPORTED_LOCATION ${PARSETRIGRAMS_EXECUTABLE})
else()
    add_executable(KF5::parsetrigrams ALIAS parsetrigrams)
endif()
