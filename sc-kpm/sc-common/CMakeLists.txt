file(GLOB SOURCES CONFIGURE_DEPENDS "*.c" "*.h")

add_library(sc-kpm-common SHARED ${SOURCES})
target_link_libraries(sc-kpm-common LINK_PUBLIC sc-core)

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(sc-kpm-common)
endif()
