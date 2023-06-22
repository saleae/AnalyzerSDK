if(CMAKE_SIZEOF_VOID_P STREQUAL "4")
    message(FATAL_ERROR "Analyzer SDK only supports 64 bit builds")
endif()

if (APPLE)
add_library(Saleae::AnalyzerSDK INTERFACE IMPORTED)
    # add both the x86_64 and arm64 versions of the library.
    add_library(Saleae::AnalyzerSDK_x64 SHARED IMPORTED)
    set_target_properties(Saleae::AnalyzerSDK_x64 PROPERTIES
                                              IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/lib_x64/libAnalyzer.dylib)
    add_library(Saleae::AnalyzerSDK_arm64 SHARED IMPORTED)
    set_target_properties(Saleae::AnalyzerSDK_arm64 PROPERTIES
                                              IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/lib_arm64/libAnalyzer.dylib)
    set_property(TARGET Saleae::AnalyzerSDK PROPERTY INTERFACE_LINK_LIBRARIES Saleae::AnalyzerSDK_x64 Saleae::AnalyzerSDK_arm64)
elseif(WIN32)
add_library(Saleae::AnalyzerSDK SHARED IMPORTED)
    set_target_properties(Saleae::AnalyzerSDK PROPERTIES
                                              IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/lib_x64/Analyzer.dll
                                              IMPORTED_IMPLIB   ${CMAKE_CURRENT_LIST_DIR}/lib_x64/Analyzer.lib)
elseif(UNIX AND NOT APPLE)
add_library(Saleae::AnalyzerSDK SHARED IMPORTED)
    set_target_properties(Saleae::AnalyzerSDK PROPERTIES
                                              IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/lib_x64/libAnalyzer.so
                                              IMPORTED_SONAME   libAnalyzer.so)
endif()

set_target_properties(Saleae::AnalyzerSDK PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_LIST_DIR}/include)