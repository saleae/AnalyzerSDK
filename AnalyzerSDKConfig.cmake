add_library(Saleae::AnalyzerSDK SHARED IMPORTED)
set_target_properties(Saleae::AnalyzerSDK PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_LIST_DIR}/include)

if(CMAKE_SIZEOF_VOID_P STREQUAL "4")
    message(FATAL_ERROR "Analyzer SDK only supports 64 bit builds")
endif()

if (APPLE)
    set_target_properties(Saleae::AnalyzerSDK PROPERTIES
                                              IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/lib/libAnalyzer.dylib)
elseif(WIN32)
    set_target_properties(Saleae::AnalyzerSDK PROPERTIES
                                              IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/lib/Analyzer.dll
                                              IMPORTED_IMPLIB   ${CMAKE_CURRENT_LIST_DIR}/lib/Analyzer.lib)
elseif(UNIX AND NOT APPLE)
    set_target_properties(Saleae::AnalyzerSDK PROPERTIES
                                              IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/lib/libAnalyzer.so
                                              IMPORTED_SONAME   libAnalyzer.so)
endif()
