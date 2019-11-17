# needs windows 8 or higher
add_definitions(-DWINVER=0x0602 -D_WIN32_WINNT=0x0602)

set(sonnet_ispellchecker_PART_SRCS
    ispellcheckerclient.cpp
    ispellcheckerdict.cpp
)
ecm_qt_declare_logging_category(sonnet_ispellchecker_PART_SRCS HEADER ispellcheckerdebug.h IDENTIFIER SONNET_ISPELLCHECKER CATEGORY_NAME sonnet.plugins.ispellchecker)

add_library(sonnet_ispellchecker MODULE ${sonnet_ispellchecker_PART_SRCS})
target_include_directories(sonnet_ispellchecker SYSTEM PUBLIC ${HUNSPELL_INCLUDE_DIRS})
target_link_libraries(sonnet_ispellchecker PRIVATE KF5::SonnetCore ${HUNSPELL_LIBRARIES})
target_compile_definitions(sonnet_ispellchecker PRIVATE DEFINITIONS SONNET_INSTALL_PREFIX="${CMAKE_INSTALL_PREFIX}")

install(TARGETS sonnet_ispellchecker  DESTINATION ${KDE_INSTALL_PLUGINDIR}/kf5/sonnet/)
