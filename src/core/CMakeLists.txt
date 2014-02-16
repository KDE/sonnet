project(sonnetcore)

set(sonnetcore_SRCS
   loader.cpp
   client.cpp
   spellerplugin.cpp
   speller.cpp
   settings.cpp
   backgroundchecker.cpp
   guesslanguage.cpp
   textbreaks.cpp
   tokenizer.cpp
   languagefilter.cpp
)


add_library(KF5SonnetCore ${sonnetcore_SRCS})
generate_export_header(KF5SonnetCore BASE_NAME SonnetCore EXPORT_FILE_NAME sonnetcore_export.h)
add_library(KF5::SonnetCore ALIAS KF5SonnetCore)

ecm_generate_headers(SonnetCore_CamelCase_HEADERS
  HEADER_NAMES
    BackgroundChecker
    Speller
    GuessLanguage
  PREFIX Sonnet
  REQUIRED_HEADERS SonnetCore_HEADERS
)

target_link_libraries(KF5SonnetCore PUBLIC Qt5::Core)

set_target_properties(KF5SonnetCore PROPERTIES VERSION ${SONNET_VERSION_STRING}
                                               SOVERSION ${SONNET_SOVERSION}
                                               EXPORT_NAME SonnetCore
)

# CMAKE_CURRENT_BINARY_DIR: for camelcase headers and lowercase forwarders
target_include_directories(KF5SonnetCore INTERFACE "$<INSTALL_INTERFACE:${INCLUDE_INSTALL_DIR}/SonnetCore>")
target_include_directories(KF5SonnetCore PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>")

install(TARGETS KF5SonnetCore EXPORT KF5SonnetTargets ${INSTALL_TARGETS_DEFAULT_ARGS})

install(FILES ${SonnetCore_CamelCase_HEADERS} DESTINATION ${INCLUDE_INSTALL_DIR}/SonnetCore/Sonnet COMPONENT Devel)

install(FILES
   ${SonnetCore_HEADERS}
   ${CMAKE_CURRENT_BINARY_DIR}/sonnetcore_export.h
   DESTINATION ${INCLUDE_INSTALL_DIR}/SonnetCore/sonnet COMPONENT Devel)

include(ECMGeneratePriFile)
ecm_generate_pri_file(BASE_NAME SonnetCore LIB_NAME KF5SonnetCore DEPS "core" FILENAME_VAR PRI_FILENAME)
install(FILES ${PRI_FILENAME} DESTINATION ${ECM_MKSPECS_INSTALL_DIR})
