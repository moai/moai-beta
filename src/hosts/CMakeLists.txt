add_executable(gluthost
    GlutHost.cpp
    GlutHostMain.cpp
)

target_link_libraries(gluthost
    aku
    moaicore
    utf8
    jansson
    glut
    GLEW
)

