file(GLOB CORE_HEADERS CONFIGURE_DEPENDS *.h *.hpp)
file(GLOB CORE_SOURCES CONFIGURE_DEPENDS *.cpp)

add_library(core_lib STATIC ${CORE_SOURCES} ${CORE_HEADERS})

target_include_directories(core_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
)