set(sonnet_nsspellchecker_PART_SRCS
    nsspellcheckerclient.mm
    nsspellcheckerdict.mm
)

add_library(sonnet_nsspellchecker MODULE ${sonnet_nsspellchecker_PART_SRCS})

target_link_libraries(sonnet_nsspellchecker PRIVATE KF5::SonnetCore "-framework Cocoa")

set_target_properties(sonnet_nsspellchecker PROPERTIES OUTPUT_NAME "nsspellchecker")
install(TARGETS sonnet_nsspellchecker  DESTINATION ${KDE_INSTALL_PLUGINDIR}/kf5/sonnet/)

