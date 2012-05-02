find_package(Lua51 REQUIRED)
find_package(CURL REQUIRED)
find_package(Freetype REQUIRED)
find_package(ZLIB REQUIRED)

include_directories(
    ${MOAI_SOURCE_DIR}/3rdparty
    ${MOAI_SOURCE_DIR}/3rdparty/box2d-2.2.1
    ${MOAI_SOURCE_DIR}/3rdparty/contrib
    # Private Lua API is used, requiring inclusion of private header lstate.h
    ${MOAI_SOURCE_DIR}/3rdparty/lua-5.1.3/src
    ${MOAI_SOURCE_DIR}/3rdparty/ooid-0.99
    ${MOAI_SOURCE_DIR}/3rdparty/tlsf-2.0
    ${MOAI_SOURCE_DIR}/src
    ${MOAI_SOURCE_DIR}/src/config-default
    ${MOAI_SOURCE_DIR}/src/zipfs

    ${FREETYPE_INCLUDE_DIRS}
)

link_directories(
    ${MOAI_SOURCE_DIR}/3rdparty/box2d-2.1.2/Box2D/Box2D
    ${MOAI_SOURCE_DIR}/3rdparty/contrib
)

add_definitions(
    ${PNG_DEFINITIONS}
)

add_subdirectory(zipfs)
add_subdirectory(aku)
add_subdirectory(moaicore)
add_subdirectory(uslscore)
add_subdirectory(hosts)

