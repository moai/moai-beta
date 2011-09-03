// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslscore/uslscore.h>

extern "C" {
	#include <zlib.h>
	#include <moaio/MOAIOZipFile.h>
}

//----------------------------------------------------------------//
static void _cleanup () {

	USGlobals::Get ()->Finalize ();
	moaio_cleanup ();
}

//----------------------------------------------------------------//
// TODO: this should be part of the unit tests
static void _typeCheck () {

	// make sure our fixed size typedefs are what we think
	// they are on the current platform/compiler
	assert ( sizeof ( cc8 )	== 1 );

	assert ( sizeof ( u8 )	== 1 );
	assert ( sizeof ( u16 )	== 2 );
	assert ( sizeof ( u32 )	== 4 );
	assert ( sizeof ( u64 )	== 8 );
	
	assert ( sizeof ( s8 )	== 1 );
	assert ( sizeof ( s16 )	== 2 );
	assert ( sizeof ( s32 )	== 4 );
	assert ( sizeof ( s64 )	== 8 );
}

//================================================================//
// uslscore
//================================================================//

//----------------------------------------------------------------//
void testMoaio () {

	//MOAIOZipFile* zipFile = MOAIOZipFile_New ( "test.zip" );
	//MOAIOZipFile_Delete ( zipFile );

	//cc8* relpath;
	//
	//relpath = moaio_get_rel_path ( "C:/foo/bar/baz.txt" );
	//relpath = moaio_get_rel_path ( "/foo/bar/baz.txt" );
	//relpath = moaio_get_rel_path ( "./foo/bar/baz.txt" );
	//relpath = moaio_get_rel_path ( "foo/bar/baz.txt" );

	moaio_set_virtual_path ( "test", "test.zip" );
	moaio_chdir ( "test//foo/baz////" );
	
	moaio_rmdir ( "test" );
	moaio_rmdir ( "test/foo" );
	
	MOAIODIR dir = moaio_dir_open ();
	
	while ( moaio_dir_read_entry ( dir )) {
		cc8* name = moaio_dir_entry_name ( dir );
		int isSubDir = moaio_dir_entry_is_subdir ( dir );
		
		printf ( "%s: %s\n", isSubDir ? "dir" : "file", name );
	}
	moaio_dir_close ( dir );
	
	moaio_chdir ( "b" );
	
	moaio_stat filestat;
	moaio_get_stat ( "././/./..//c////Metamorphosis.txt", &filestat );
	
	MOAIOFILE file = moaio_fopen ( "././/./..//c////Metamorphosis.txt", "r" );
	
	char buffer [ 1024 ];
	
	moaio_fread ( buffer, 1024, 1, file );
	moaio_fseek ( file, -512, SEEK_CUR );
	
	moaio_fread ( buffer, 1024, 1, file );
	buffer [ 1023 ] = 0;
	printf ( "%s\n", buffer );
	
	moaio_fclose ( file );
}

//----------------------------------------------------------------//
void uslscore::InitGlobals ( USGlobals* globals ) {

	static bool sysInit = true;
	if ( sysInit ) {

		_typeCheck ();

		//#ifdef _WIN32
		//	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF );
		//#endif
		
		srand (( u32 )time ( 0 ));
		atexit ( _cleanup );
		
		moaio_init ();
		
		testMoaio ();
		
		sysInit = false;
	}

	USGlobals::Get ()->Set ( globals );

	USLuaRuntime::Get ();
}
