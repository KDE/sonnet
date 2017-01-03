project(sonnetdata)

include(ECMMarkNonGuiExecutable)

add_executable(parsetrigrams parsetrigrams.cpp)
add_executable(gentrigrams gentrigrams.cpp)

# Mark it as non-gui so we won't create an app bundle on Mac OS X
ecm_mark_nongui_executable(parsetrigrams)
ecm_mark_nongui_executable(gentrigrams)

TARGET_LINK_LIBRARIES(parsetrigrams PUBLIC Qt5::Core)
TARGET_LINK_LIBRARIES(gentrigrams PUBLIC Qt5::Core)
INSTALL(TARGETS parsetrigrams EXPORT KF5SonnetTargets ${KF5_INSTALL_TARGETS_DEFAULT_ARGS})
INSTALL(TARGETS gentrigrams EXPORT KF5SonnetTargets ${KF5_INSTALL_TARGETS_DEFAULT_ARGS})

cmake_policy(SET CMP0026 OLD) # FIXME: make this work with CMP0026 set to NEW
if(CMAKE_CROSSCOMPILING AND PARSETRIGRAMS_EXECUTABLE)
    add_executable(KF5::parsetrigrams IMPORTED GLOBAL)
    set_target_properties(KF5::parsetrigrams PROPERTIES IMPORTED_LOCATION ${PARSETRIGRAMS_EXECUTABLE})
else()
    add_executable(KF5::parsetrigrams ALIAS parsetrigrams)
endif()

macro(create_trigrams_map _target_name _in_DIR _out_FILE)
    get_target_property(PARSETRIGRAMS_EXECUTABLE parsetrigrams LOCATION)
    add_custom_target(trigrams_${_target_name} ALL)
    add_custom_command(TARGET trigrams_${_target_name}
                       COMMAND $<TARGET_FILE:KF5::parsetrigrams> "${_in_DIR}" > "${_out_FILE}"
                      )

    add_dependencies(trigrams_${_target_name} parsetrigrams)
endmacro(create_trigrams_map)

create_trigrams_map(sonnet
                    ${CMAKE_SOURCE_DIR}/data/trigrams
                    ${CMAKE_CURRENT_BINARY_DIR}/trigrams.map
                   )
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/trigrams.map DESTINATION ${KDE_INSTALL_DATADIR_KF5}/sonnet/)
