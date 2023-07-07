add_library(Saleae::AnalyzerSDK SHARED IMPORTED)
set_target_properties(Saleae::AnalyzerSDK PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_LIST_DIR}/include)

if(CMAKE_SIZEOF_VOID_P STREQUAL "4")
    message(FATAL_ERROR "Analyzer SDK only supports 64 bit builds")
endif()

if (APPLE)
    if("${CMAKE_OSX_ARCHITECTURES}" STREQUAL "arm64")
        set(LIB_DIR "lib_arm64")
    elseif("${CMAKE_OSX_ARCHITECTURES}" STREQUAL "x86_64")
        set(LIB_DIR "lib_x86_64")
    elseif(CMAKE_OSX_ARCHITECTURES STREQUAL "")
        message("CMAKE_OSX_ARCHITECTURES not set, defaulting to system processor")
        set(LIB_DIR "lib_${CMAKE_SYSTEM_PROCESSOR}")
    else()
        message(FATAL_ERROR "Universal Binaries are not supported. Either set CMAKE_OSX_ARCHITECTURES to a single supported platform, or do not set it.")
    endif()

    message("LIB_DIR: ${LIB_DIR}")
    set_target_properties(Saleae::AnalyzerSDK PROPERTIES
                                              IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/${LIB_DIR}/libAnalyzer.dylib)
elseif(WIN32)
    set_target_properties(Saleae::AnalyzerSDK PROPERTIES
                                              IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/lib_x86_64/Analyzer.dll
                                              IMPORTED_IMPLIB   ${CMAKE_CURRENT_LIST_DIR}/lib_x86_64/Analyzer.lib)
elseif(UNIX AND NOT APPLE)
    set_target_properties(Saleae::AnalyzerSDK PROPERTIES
                                              IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/lib_x86_64/libAnalyzer.so
                                              IMPORTED_SONAME   libAnalyzer.so)
endif()
