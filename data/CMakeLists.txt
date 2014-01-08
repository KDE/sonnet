project(sonnetdata)

add_executable(parsetrigrams parsetrigrams.cpp)

TARGET_LINK_LIBRARIES(parsetrigrams PUBLIC Qt5::Core)
#INSTALL(TARGETS parsetrigrams DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

macro(create_trigrams_map _target_name _in_DIR _out_FILE)
    get_target_property(PARSETRIGRAMS_EXECUTABLE parsetrigrams LOCATION)
    add_custom_target(trigrams_${_target_name} ALL)
    add_custom_command(TARGET trigrams_${_target_name}
                       COMMAND "${PARSETRIGRAMS_EXECUTABLE}" "${_in_DIR}" > "${_out_FILE}"
                      )

    add_dependencies(trigrams_${_target_name} parsetrigrams)
endmacro(create_trigrams_map)

create_trigrams_map(sonnet
                    ${CMAKE_SOURCE_DIR}/data/trigrams
                    ${CMAKE_CURRENT_BINARY_DIR}/trigrams.map
                   )
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/trigrams.map DESTINATION ${DATA_INSTALL_DIR}/sonnet/)
