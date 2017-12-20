
########### next target ###############

if (EXISTS "/usr/share/hunspell/")
add_definitions(-DHUNSPELL_MAIN_DICT_PATH="/usr/share/hunspell")
elseif (EXISTS "/usr/share/myspell/")
add_definitions(-DHUNSPELL_MAIN_DICT_PATH="/usr/share/myspell")
endif (EXISTS "/usr/share/hunspell/")

include_directories( ${HUNSPELL_INCLUDE_DIRS} )
set(sonnet_hunspell_PART_SRCS
    hunspellclient.cpp
    hunspelldict.cpp
)
ecm_qt_declare_logging_category(sonnet_hunspell_PART_SRCS HEADER hunspelldebug.h IDENTIFIER SONNET_HUNSPELL CATEGORY_NAME sonnet.plugins.hunspell)

add_library(sonnet_hunspell MODULE ${sonnet_hunspell_PART_SRCS})

target_link_libraries(sonnet_hunspell PRIVATE KF5::SonnetCore ${HUNSPELL_LIBRARIES})

install(TARGETS sonnet_hunspell  DESTINATION ${KDE_INSTALL_PLUGINDIR}/kf5/sonnet/)


