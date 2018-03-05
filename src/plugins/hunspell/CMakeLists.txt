
########### next target ###############

include_directories( ${HUNSPELL_INCLUDE_DIRS} )
set(sonnet_hunspell_PART_SRCS
    hunspellclient.cpp
    hunspelldict.cpp
)
ecm_qt_declare_logging_category(sonnet_hunspell_PART_SRCS HEADER hunspelldebug.h IDENTIFIER SONNET_HUNSPELL CATEGORY_NAME sonnet.plugins.hunspell)

add_library(sonnet_hunspell MODULE ${sonnet_hunspell_PART_SRCS})

target_link_libraries(sonnet_hunspell PRIVATE KF5::SonnetCore ${HUNSPELL_LIBRARIES})
target_compile_definitions(sonnet_hunspell PRIVATE DEFINITIONS SONNET_INSTALL_PREFIX="${CMAKE_INSTALL_PREFIX}")

install(TARGETS sonnet_hunspell  DESTINATION ${KDE_INSTALL_PLUGINDIR}/kf5/sonnet/)


