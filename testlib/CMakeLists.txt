
set (ANALYZER_SDK_ROOT "${PROJECT_SOURCE_DIR}/AnalyzerSDK")


set(TEST_HARNESS_SOURCES
    AnalyzerStubs.cpp
    HelperStubs.cpp
    MockChannelData.cpp
    MockChannelData.h
    MockSimulatedChannelDescriptor.cpp
    MockSimulatedChannelDescriptor.h
    MockResults.h
    MockResults.cpp
    MockSettings.h
    SettingsStubs.cpp 
    StreamHelpers.h
    StreamHelpers.cpp
    TestAnalyzerData.h
    TestInstance.cpp
    TestInstance.h
    TestMacros.h
)

add_library(AnalyzerTestHarness STATIC ${TEST_HARNESS_SOURCES})

target_include_directories(AnalyzerTestHarness PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(AnalyzerTestHarness PUBLIC ${ANALYZER_SDK_ROOT}/include)

#------------------------------------------------------------------------
# internal verification of the test-harness

add_executable(TestHarnessVerification TestHarnessVerification.cpp TrivialAnalyzer.cpp)
target_link_libraries(TestHarnessVerification AnalyzerTestHarness)

add_test(TestHarnessVerification ${EXECUTABLE_OUTPUT_PATH}/TestHarnessVerification)
