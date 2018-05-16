add_library(Saleae::AnalyzerSDK SHARED IMPORTED)
set_target_properties(Saleae::AnalyzerSDK PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_LIST_DIR}/include)

if(CMAKE_SIZEOF_VOID_P STREQUAL "4")
    set(LIBDIR "x86")
else()
    set(LIBDIR "x86_64")
endif()

if (APPLE)
    set_target_properties(Saleae::AnalyzerSDK PROPERTIES
                                              IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/lib/${LIBDIR}/libAnalyzer.dylib)
elseif(WIN32)
    set_target_properties(Saleae::AnalyzerSDK PROPERTIES
                                              IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/lib/${LIBDIR}/Analyzer.dll
                                              IMPORTED_IMPLIB   ${CMAKE_CURRENT_LIST_DIR}/lib/${LIBDIR}/Analyzer.lib)
elseif(UNIX AND NOT APPLE)
    set_target_properties(Saleae::AnalyzerSDK PROPERTIES
                                              IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/lib/${LIBDIR}/libAnalyzer$.so
                                              IMPORTED_SONAME   libAnalyzer.so)
endif()
