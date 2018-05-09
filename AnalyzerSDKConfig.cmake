add_library(Saleae::AnalyzerSDK SHARED IMPORTED)
set_target_properties(Saleae::AnalyzerSDK PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_LIST_DIR}/include)

if(CMAKE_SIZEOF_VOID_P STREQUAL "4")
    set(SUFFIX)
else()
    set(SUFFIX "64")
endif()

if (APPLE)
    set_target_properties(Saleae::AnalyzerSDK PROPERTIES
                                              IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/lib/libAnalyzer.dylib)
elseif(WIN32)
    set(AnalyzerSDK_IMPORTLIB Analyzer${SUFFIX})
    set_target_properties(Saleae::AnalyzerSDK PROPERTIES
                                              IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/lib/${AnalyzerSDK_IMPORTLIB}.dll
                                              IMPORTED_IMPLIB   ${CMAKE_CURRENT_LIST_DIR}/lib/${AnalyzerSDK_IMPORTLIB}.lib)
elseif(UNIX AND NOT APPLE)
    set(AnalyzerSDK_IMPORTLIB libAnalyzer${SUFFIX})
    set_target_properties(Saleae::AnalyzerSDK PROPERTIES
                                              IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/lib/${AnalyzerSDK_IMPORTLIB}.so
                                              IMPORTED_SONAME   ${AnalyzerSDK_IMPORTLIB}.so)
endif()
