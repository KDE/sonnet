
cmake_minimum_required(VERSION 2.8.12)

project(Sonnet)

include(FeatureSummary)
find_package(ECM 5.24.0  NO_MODULE)
set_package_properties(ECM PROPERTIES TYPE REQUIRED DESCRIPTION "Extra CMake Modules." URL "https://projects.kde.org/projects/kdesupport/extra-cmake-modules")
feature_summary(WHAT REQUIRED_PACKAGES_NOT_FOUND FATAL_ON_MISSING_REQUIRED_PACKAGES)


set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/cmake )

include(KDEInstallDirs)
include(KDEFrameworkCompilerSettings NO_POLICY_SCOPE)
include(KDECMakeSettings)

set(REQUIRED_QT_VERSION 5.4.0)
find_package(Qt5 ${REQUIRED_QT_VERSION} CONFIG REQUIRED Widgets)


include(GenerateExportHeader)

include(ECMSetupVersion)
include(ECMGenerateHeaders)

include(ECMPoQmTools)

set(KF5_VERSION "5.24.0") # handled by release scripts

ecm_setup_version(${KF5_VERSION} VARIABLE_PREFIX SONNET
                        VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/sonnet_version.h"
                        PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KF5SonnetConfigVersion.cmake"
                        SOVERSION 5)

if (IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/po")
    ecm_install_po_files_as_qm(po)
endif()

add_subdirectory(src)
add_subdirectory(autotests)
add_subdirectory(data)
add_subdirectory(examples)


# create a Config.cmake and a ConfigVersion.cmake file and install them
set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/KF5Sonnet")

include(ECMPackageConfigHelpers)

ecm_configure_package_config_file("${CMAKE_CURRENT_SOURCE_DIR}/KF5SonnetConfig.cmake.in" "${CMAKE_CURRENT_BINARY_DIR}/KF5SonnetConfig.cmake"
                              INSTALL_DESTINATION  ${CMAKECONFIG_INSTALL_DIR}
                              )

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/KF5SonnetConfig.cmake"
              "${CMAKE_CURRENT_BINARY_DIR}/KF5SonnetConfigVersion.cmake"
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"  COMPONENT Devel )

install(EXPORT KF5SonnetTargets DESTINATION "${CMAKECONFIG_INSTALL_DIR}" FILE KF5SonnetTargets.cmake NAMESPACE KF5:: )

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/sonnet_version.h
        DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5} COMPONENT Devel )

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
