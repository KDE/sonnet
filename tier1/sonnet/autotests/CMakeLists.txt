remove_definitions(-DQT_NO_CAST_FROM_ASCII)

include(ECMMarkAsTest)

find_package(Qt5Test 5.2.0 REQUIRED NO_MODULE)

########### unittests ###############

macro(SONNET_UNIT_TESTS)
  foreach(_testname ${ARGN})
    add_executable(${_testname} ${_testname}.cpp)
    add_test(sonnet-${_testname} ${_testname})
    target_link_libraries(${_testname} KF5::SonnetCore Qt5::Test)
    ecm_mark_as_test(${_testname})
  endforeach()
endmacro(SONNET_UNIT_TESTS)

sonnet_unit_tests(
 test_filter
 test_core
)

