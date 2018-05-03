if (APPLE)
    set(AnalyzerSDK_IMPORTLIB libAnalyzer.dylib)
elseif(MSVC)
    if( CMAKE_CL_64 )
        set(AnalyzerSDK_IMPORTLIB Analyzer64.lib)
    else()
         set(AnalyzerSDK_IMPORTLIB Analyzer.lib)
    endif()
elseif(UNIX AND NOT APPLE)
    set(AnalyzerSDK_IMPORTLIB libAnalyzer.so)
endif()


add_library(Saleae::AnalyzerSDK SHARED IMPORTED)

set_property(TARGET Saleae::AnalyzerSDK PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_LIST_DIR}/include)
set_property(TARGET Saleae::AnalyzerSDK PROPERTY IMPORTED_IMPLIB ${CMAKE_CURRENT_LIST_DIR}/lib/${AnalyzerSDK_IMPORTLIB})
