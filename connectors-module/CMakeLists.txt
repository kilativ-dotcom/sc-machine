file(GLOB SOURCES CONFIGURE_DEPENDS
        "*.cpp" "*.hpp"
        "agent/*.cpp" "agent/*.hpp"
)

# Generate and link your library with using libraries.
add_library(connectors-agents SHARED ${SOURCES})
target_link_libraries(connectors-agents LINK_PUBLIC sc-memory)
target_include_directories(connectors-agents PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
