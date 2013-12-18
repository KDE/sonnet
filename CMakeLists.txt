
cmake_minimum_required(VERSION 2.8.12)

project(sonnet)

find_package(ECM 0.0.9 REQUIRED NO_MODULE)

set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/cmake )

include(KDEInstallDirs)
include(KDEFrameworkCompilerSettings)
include(KDECMakeSettings)

# TODO: Remove these
remove_definitions(-DQT_NO_CAST_TO_ASCII)
remove_definitions(-DQT_NO_CAST_FROM_ASCII)
remove_definitions(-DQT_NO_CAST_FROM_BYTEARRAY)

set(REQUIRED_QT_VERSION 5.2.0)
find_package(Qt5 ${REQUIRED_QT_VERSION} CONFIG REQUIRED Widgets)

include(FeatureSummary)

include(ECMSetupVersion)

ecm_setup_version(5.0.0 VARIABLE_PREFIX SONNET
                        VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/sonnet_version.h"
                        PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KF5SonnetConfigVersion.cmake")


# Enable when kdoctools will be usable from the outside (macros are broken ATM)
# apol: they are working now, but KDocTools is in a higher tier, I'm unsure if we should enable it back
#add_subdirectory(docs)
add_subdirectory(src)
add_subdirectory(autotests)
add_subdirectory(tests)


# create a Config.cmake and a ConfigVersion.cmake file and install them
set(CMAKECONFIG_INSTALL_DIR "${CMAKECONFIG_INSTALL_PREFIX}/KF5Sonnet")

include(CMakePackageConfigHelpers)

configure_package_config_file("${CMAKE_CURRENT_SOURCE_DIR}/KF5SonnetConfig.cmake.in" "${CMAKE_CURRENT_BINARY_DIR}/KF5SonnetConfig.cmake"
                              INSTALL_DESTINATION  ${CMAKECONFIG_INSTALL_DIR}
                              )

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/KF5SonnetConfig.cmake"
              "${CMAKE_CURRENT_BINARY_DIR}/KF5SonnetConfigVersion.cmake"
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"  COMPONENT Devel )

install(EXPORT KF5SonnetTargets DESTINATION "${CMAKECONFIG_INSTALL_DIR}" FILE KF5SonnetTargets.cmake NAMESPACE KF5:: )

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/sonnet_version.h
        DESTINATION ${INCLUDE_INSTALL_DIR} COMPONENT Devel )


if ("${CMAKE_BINARY_DIR}" STREQUAL "${CMAKE_CURRENT_BINARY_DIR}")
  feature_summary(WHAT ALL   FATAL_ON_MISSING_REQUIRED_PACKAGES)
endif()
