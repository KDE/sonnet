cmake_minimum_required(VERSION 3.5)

set(KF_VERSION "5.80.0") # handled by release scripts
project(Sonnet VERSION ${KF_VERSION})

include(FeatureSummary)
find_package(ECM 5.80.0  NO_MODULE)
set_package_properties(ECM PROPERTIES TYPE REQUIRED DESCRIPTION "Extra CMake Modules." URL "https://commits.kde.org/extra-cmake-modules")
feature_summary(WHAT REQUIRED_PACKAGES_NOT_FOUND FATAL_ON_MISSING_REQUIRED_PACKAGES)


set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake )

include(KDEInstallDirs)
include(KDEFrameworkCompilerSettings NO_POLICY_SCOPE)
include(KDECMakeSettings)
include(KDEGitCommitHooks)
include(ECMQtDeclareLoggingCategory)
include(ECMGenerateExportHeader)
include(ECMSetupVersion)
include(ECMGenerateHeaders)

include(ECMAddQch)
include(ECMPoQmTools)

set(EXCLUDE_DEPRECATED_BEFORE_AND_AT 0 CACHE STRING "Control the range of deprecated API excluded from the build [default=0].")

set(REQUIRED_QT_VERSION 5.14.0)
option(SONNET_USE_WIDGETS "Build components using Qt5Widgets" ON)
if(SONNET_USE_WIDGETS)
    find_package(Qt5 ${REQUIRED_QT_VERSION} CONFIG REQUIRED Widgets)
endif()
find_package(Qt5 ${REQUIRED_QT_VERSION} CONFIG REQUIRED Core)


ecm_setup_version(PROJECT VARIABLE_PREFIX SONNET
                        VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/sonnet_version.h"
                        PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KF5SonnetConfigVersion.cmake"
                        SOVERSION 5)

option(BUILD_QCH "Build API documentation in QCH format (for e.g. Qt Assistant, Qt Creator & KDevelop)" OFF)
add_feature_info(QCH ${BUILD_QCH} "API documentation in QCH format (for e.g. Qt Assistant, Qt Creator & KDevelop)")

if(SONNET_USE_WIDGETS)
    option(BUILD_DESIGNERPLUGIN "Build plugin for Qt Designer" ON)
    add_feature_info(DESIGNERPLUGIN ${BUILD_DESIGNERPLUGIN} "Build plugin for Qt Designer")
endif()

add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=0x050e00)
if (IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/po")
    ecm_install_po_files_as_qm(po)
endif()
add_definitions(-DQT_NO_FOREACH)
add_subdirectory(data)
add_subdirectory(src)
if (BUILD_TESTING)
     add_subdirectory(autotests)
endif()

if(TARGET Qt5::Widgets)
    add_subdirectory(examples)
endif()


# create a Config.cmake and a ConfigVersion.cmake file and install them
set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/KF5Sonnet")

if (BUILD_QCH)
    ecm_install_qch_export(
        TARGETS KF5SonnetCore_QCH KF5SonnetUi_QCH
        FILE KF5SonnetQchTargets.cmake
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
        COMPONENT Devel
    )
    set(PACKAGE_INCLUDE_QCHTARGETS "include(\"\${CMAKE_CURRENT_LIST_DIR}/KF5SonnetQchTargets.cmake\")")
endif()

include(CMakePackageConfigHelpers)

configure_package_config_file("${CMAKE_CURRENT_SOURCE_DIR}/KF5SonnetConfig.cmake.in" "${CMAKE_CURRENT_BINARY_DIR}/KF5SonnetConfig.cmake"
                              INSTALL_DESTINATION  ${CMAKECONFIG_INSTALL_DIR}
                              )

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/KF5SonnetConfig.cmake"
              "${CMAKE_CURRENT_BINARY_DIR}/KF5SonnetConfigVersion.cmake"
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"  COMPONENT Devel )

install(EXPORT KF5SonnetTargets DESTINATION "${CMAKECONFIG_INSTALL_DIR}" FILE KF5SonnetTargets.cmake NAMESPACE KF5:: )

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/sonnet_version.h
        DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5} COMPONENT Devel )

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)

kde_configure_git_pre_commit_hook(CHECKS CLANG_FORMAT)
