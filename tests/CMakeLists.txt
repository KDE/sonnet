remove_definitions(-DQT_NO_CAST_FROM_ASCII)

include(ECMMarkAsTest)

########### test_suggest ###############
# TODO automate and merge with test_filter?
set(test_suggest_SRCS test.cpp)
add_executable(test_suggest ${test_suggest_SRCS})
target_link_libraries(test_suggest KF5::SonnetCore)
ecm_mark_as_test(test_suggest)

########### test_dialog ###############

set(test_dialog_SRCS test_dialog.cpp)
add_executable(test_dialog ${test_dialog_SRCS})
target_link_libraries(test_dialog KF5::SonnetCore KF5::SonnetUi)
ecm_mark_as_test(test_dialog)

########### backgroundtest ###############

set(backgroundtest_SRCS backgroundtest.cpp)
add_executable(backgroundtest ${backgroundtest_SRCS})
ecm_mark_as_test(backgroundtest)
target_link_libraries(backgroundtest KF5::SonnetCore KF5::SonnetUi)

########### test_highlighter ###############

set(test_highlighter_SRCS test_highlighter.cpp)
add_executable(test_highlighter ${test_highlighter_SRCS})
ecm_mark_as_test(test_highlighter)
target_link_libraries(test_highlighter KF5::SonnetCore KF5::SonnetUi)

########### test_configdialog ###############

set(test_configdialog_SRCS test_configdialog.cpp)
add_executable(test_configdialog ${test_configdialog_SRCS})
ecm_mark_as_test(test_configdialog)
target_link_libraries(test_configdialog KF5::SonnetCore KF5::SonnetUi)

########### test_dictionarycombobox ###############

add_executable(test_dictionarycombobox test_dictionarycombobox.cpp)
ecm_mark_as_test(test_dictionarycombobox)
target_link_libraries(test_dictionarycombobox KF5::SonnetCore KF5::SonnetUi)

########### test_textedit ###############

set(test_textedit_SRCS test_textedit.cpp)
add_executable(test_textedit ${test_textedit_SRCS})
target_link_libraries(test_textedit KF5::SonnetCore KF5::SonnetUi)
ecm_mark_as_test(test_textedit)
