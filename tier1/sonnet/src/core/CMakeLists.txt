project(sonnetcore)

set(sonnetcore_SRCS
   loader.cpp
   client.cpp
   spellerplugin.cpp
   speller.cpp
   filter.cpp
   settings.cpp
   backgroundchecker.cpp
   backgroundengine.cpp
   globals.cpp
)
set(sonnetcore_HEADERS
    backgroundchecker.h
    speller.h
    globals.h
    ${CMAKE_CURRENT_BINARY_DIR}/sonnetcore_export.h
)


add_library(KF5SonnetCore ${sonnetcore_SRCS})
generate_export_header(KF5SonnetCore BASE_NAME SonnetCore EXPORT_FILE_NAME sonnetcore_export.h)
add_library(KF5::SonnetCore ALIAS KF5SonnetCore)

target_link_libraries(KF5SonnetCore PUBLIC Qt5::Core)

set_target_properties(KF5SonnetCore PROPERTIES VERSION ${SONNET_VERSION_STRING}
                                               SOVERSION ${SONNET_SOVERSION}
                                               EXPORT_NAME SonnetCore
)

target_include_directories(KF5SonnetCore PUBLIC
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>" # needed?
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>" # needed?
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/..>"
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/../include>" # sonnet/*.h
)

install(TARGETS KF5SonnetCore EXPORT KF5SonnetTargets ${INSTALL_TARGETS_DEFAULT_ARGS})


install( FILES ${sonnetcore_HEADERS}
         DESTINATION ${INCLUDE_INSTALL_DIR}/sonnet COMPONENT Devel)

