remove_definitions(-DQT_NO_CAST_FROM_ASCII)

include(ECMMarkAsTest)
include(ECMAddTests)

find_package(Qt5Test ${REQUIRED_QT_VERSION} REQUIRED NO_MODULE)

########### unittests ###############

macro(SONNET_UNIT_TESTS)
  foreach(_testname ${ARGN})
    ecm_add_test(${_testname}.cpp TEST_NAME sonnet-${_testname} LINK_LIBRARIES KF5::SonnetCore Qt5::Test)
  endforeach()
endmacro(SONNET_UNIT_TESTS)

sonnet_unit_tests(
 test_filter
 test_core
 test_suggest
 test_settings
)

