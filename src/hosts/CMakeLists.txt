add_executable(gluthost
    GlutHost.cpp
    GlutHostMain.cpp
)

target_link_libraries(gluthost
    aku
    moaicore
    utf8
    jansson
    ${OPENGL_LIBRARIES}
    ${GLUT_LIBRARIES}
    GLEW
)

