add_library(zipfs STATIC
    pch.h
    zipfs.c
    zipfs.h
    zipfs-pch.c
    zipfs_replace.h
    zipfs_replace_locale.h
    zipfs_replace_new_delete.h
    zipfs_replace_stdio.h
    zipfs_replace_stdlib.h
    ZIPFSString.c
    ZIPFSString.h
    zipfs_util.c
    zipfs_util.h
    zipfs_vfscanf.c
    ZIPFSVirtualPath.c
    ZIPFSVirtualPath.h
    ZIPFSZipFile.c
    ZIPFSZipFile.h
)

target_link_libraries(zipfs
    tlsf
)
