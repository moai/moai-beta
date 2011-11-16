#include <jni.h>
#include <time.h>
#include <android/log.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include <moaicore/moaicore.h>
#include <moaiext-android/moaiext-android.h>
#include <aku/AKU.h>
#include <aku/AKU-untz.h>
#include <aku/AKU-luaext.h>

//================================================================//
// Utility macros
//================================================================//

	#define GET_ENV() 	\
		JNIEnv* env; 	\
		jvm->GetEnv ( ( void** )&env, JNI_VERSION_1_4 );

	#define GET_STRING(jstr, cstr) \
		const char* cstr = env->GetStringUTFChars( jstr, NULL );

	#define RELEASE_STRING(jstr, cstr) \
		env->ReleaseStringUTFChars ( jstr, cstr );
		
	#define PRINT(str) \
		__android_log_write ( ANDROID_LOG_INFO, "MoaiLog", str );
		
//================================================================//
// JNI set up
//================================================================//

	JavaVM* 		jvm;

	jmethodID 		mGenerateGuidFunc;
	jobject			mMoaiView;

	//----------------------------------------------------------------//
	int JNI_OnLoad ( JavaVM* vm, void* reserved ) {
    
		jvm = vm;
		return JNI_VERSION_1_4;
	}
		
//================================================================//
// Generate GUID callback
//================================================================//

	//----------------------------------------------------------------//
	const char* GenerateGUID () {

		// get environment
		GET_ENV ();

	    // call generate guid method in java
		jstring jguid = ( jstring )env->CallObjectMethod ( mMoaiView, mGenerateGuidFunc );

		// convert jstring to cstring
		GET_STRING ( jguid, guid );
		const char* retVal;
		copystr ( retVal, guid );
		RELEASE_STRING ( jguid, guid );
	
		// return guid string
		return retVal;
	}

//================================================================//
// JNI Functions
//================================================================//

	//----------------------------------------------------------------//
	extern "C" int Java_@PACKAGE_UNDERSCORED@_MoaiView_AKUCreateContext ( JNIEnv* env, jclass obj ) {
		return AKUCreateContext ();
	}

	//----------------------------------------------------------------//
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiView_AKUDeleteContext ( JNIEnv* env, jclass obj, jint akuContextId ) {
		AKUDeleteContext ( akuContextId );
	}

	//----------------------------------------------------------------//
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiView_AKUDetectGfxContext ( JNIEnv* env, jclass obj ) {
		AKUDetectGfxContext ();
	}

	//----------------------------------------------------------------//
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiActivity_AKUAppDidStartSession ( JNIEnv* env, jclass obj ) {
		MOAIApp::Get ().DidStartSession ();
	}

	//----------------------------------------------------------------//
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiActivity_AKUEnqueueCompassEvent ( JNIEnv* env, jclass obj, jint deviceId, jint sensorId, jint heading ) {
		AKUEnqueueCompassEvent ( deviceId, sensorId, heading );
	}

	//----------------------------------------------------------------//
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiActivity_AKUEnqueueLevelEvent ( JNIEnv* env, jclass obj, jint deviceId, jint sensorId, jfloat x, jfloat y, jfloat z ) {
		AKUEnqueueLevelEvent ( deviceId, sensorId, x, y, z );
	}

	//----------------------------------------------------------------//
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiActivity_AKUEnqueueLocationEvent ( JNIEnv* env, jclass obj, jint deviceId, jint sensorId, jint longitude, jint latitude, jint altitude, jfloat hAccuracy, jfloat vAccuracy, jfloat speed ) {
		AKUEnqueueLocationEvent ( deviceId, sensorId, longitude, latitude, altitude, hAccuracy, vAccuracy, speed );
	}

	//----------------------------------------------------------------//
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiView_AKUEnqueueTouchEvent ( JNIEnv* env, jclass obj, jint deviceId, jint sensorId, jint touchId, jboolean down, jint x, jint y, jint tapCount ) {
		AKUEnqueueTouchEvent ( deviceId, sensorId, touchId, down, x, y, tapCount );
	}

	//----------------------------------------------------------------//
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiView_AKUExtLoadLuacrypto ( JNIEnv* env, jclass obj ) {
		AKUExtLoadLuacrypto ();
	}

	//----------------------------------------------------------------//
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiView_AKUExtLoadLuacurl ( JNIEnv* env, jclass obj ) {
		AKUExtLoadLuacurl ();
	}

	//----------------------------------------------------------------//
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiView_AKUExtLoadLuasocket ( JNIEnv* env, jclass obj ) {
		AKUExtLoadLuasocket ();
	}

	//----------------------------------------------------------------//
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiView_AKUExtLoadLuasql ( JNIEnv* env, jclass obj ) {
		AKUExtLoadLuasql ();
	}


	//----------------------------------------------------------------//
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiView_AKUFinalize	( JNIEnv* env, jclass obj ) {
		AKUFinalize ();
	}

	//----------------------------------------------------------------//
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiView_AKUInit ( JNIEnv* env, jclass obj, jobject moaiView ) {

		// create MOAIApp class
		MOAIApp::Affirm ();
		REGISTER_LUA_CLASS ( MOAIApp );

		// register callbacks into Java
		mMoaiView = ( jobject ) env->NewGlobalRef ( moaiView );
		jclass moaiViewClass = env->GetObjectClass ( mMoaiView );
		
		mGenerateGuidFunc = env->GetMethodID ( moaiViewClass, "getGUID", "()Ljava/lang/String;" );
	}

	//----------------------------------------------------------------//
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiActivity_AKUMountVirtualDirectory ( JNIEnv* env, jclass obj, jstring jvirtualPath, jstring jarchive ) {
		GET_STRING ( jvirtualPath, virtualPath );
		GET_STRING ( jarchive, archive );
		AKUMountVirtualDirectory ( virtualPath, archive );
		RELEASE_STRING ( jvirtualPath, virtualPath );
		RELEASE_STRING ( jarchive, archive );
	}

	//----------------------------------------------------------------//
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiView_AKUPause ( JNIEnv* env, jclass obj, jboolean paused ) {
		AKUPause ( paused );
	}

	//----------------------------------------------------------------//
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiView_AKURender ( JNIEnv* env, jclass obj ) {
		AKURender ();
	}

	//----------------------------------------------------------------//
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiView_AKUReserveInputDevices ( JNIEnv* env, jclass obj, jint total ) {
		AKUReserveInputDevices ( total );
	}

	//----------------------------------------------------------------//
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiView_AKUReserveInputDeviceSensors ( JNIEnv* env, jclass obj, jint deviceId, jint total ) {
		AKUReserveInputDeviceSensors ( deviceId, total );
	}

	//----------------------------------------------------------------//
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiView_AKUResize ( JNIEnv* env, jclass obj, jint width, jint height ) {
		AKUResize ( width, height );
	}
	
	//----------------------------------------------------------------//
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiView_AKURunScript ( JNIEnv* env, jclass obj, jstring jfilename ) {
		GET_STRING ( jfilename, filename );
		AKURunScript ( filename );
		RELEASE_STRING ( jfilename, filename );
	}

	//----------------------------------------------------------------//
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiActivity_AKUSetConnectionType ( JNIEnv* env, jclass obj, jlong connectionType ) {
		MOAIEnvironment::Get ().SetConnectionType ( connectionType );
	}

	//----------------------------------------------------------------//
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiView_AKUSetContext ( JNIEnv* env, jclass obj, jint akuContextId ) {
		AKUSetContext ( akuContextId );
	}
	
	//----------------------------------------------------------------//
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiView_AKUSetDeviceProperties ( JNIEnv* env, jclass obj, jstring jappName, jstring jabi, jstring jdevBrand, jstring jdevName, jstring jdevManufacturer, jstring jdevModel, jstring jdevProduct, jstring josBrand, jstring josVersion, jstring judid ) {

		// get the environment
		MOAIEnvironment& moaiEnv = MOAIEnvironment::Get ();
	
		// set up environment callbacks
		moaiEnv.SetGUIDFunc ( &GenerateGUID );

		// convert jstrings to cstrings
		GET_STRING ( jappName, appName );
		GET_STRING ( jabi, abi );
		GET_STRING ( jdevBrand, devBrand );
		GET_STRING ( jdevName, devName );
		GET_STRING ( jdevManufacturer, devManufacturer );
		GET_STRING ( jdevModel, devModel );
		GET_STRING ( jdevProduct, devProduct );
		GET_STRING ( josBrand, osBrand );
		GET_STRING ( josVersion, osVersion );
		GET_STRING ( judid, udid );
	
		// set environment properties
		moaiEnv.SetAppDisplayName 	( appName );
		moaiEnv.SetCPUABI 			( abi );
		moaiEnv.SetDevBrand 		( devBrand );
		moaiEnv.SetDevName 			( devName );
		moaiEnv.SetDevManufacturer	( devManufacturer );
		moaiEnv.SetDevModel			( devModel );
		moaiEnv.SetDevProduct		( devProduct );
		moaiEnv.SetOSBrand			( osBrand );
		moaiEnv.SetOSVersion		( osVersion );
		moaiEnv.SetUDID				( udid );

		// release jstrings
		RELEASE_STRING ( jappName, appName );
		RELEASE_STRING ( jabi, abi );
		RELEASE_STRING ( jdevBrand, devBrand );
		RELEASE_STRING ( jdevName, devName );
		RELEASE_STRING ( jdevManufacturer, devManufacturer );
		RELEASE_STRING ( jdevModel, devModel );
		RELEASE_STRING ( jdevProduct, devProduct );
		RELEASE_STRING ( josBrand, osBrand );
		RELEASE_STRING ( josVersion, osVersion );
		RELEASE_STRING ( judid, udid );
	}

	//----------------------------------------------------------------//
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiActivity_AKUSetDocumentDirectory ( JNIEnv* env, jclass obj, jstring jpath ) {
		GET_STRING ( jpath, path );
		MOAIEnvironment::Get ().SetDocumentDirectory ( path );
		RELEASE_STRING ( jpath, path );
	}

	//----------------------------------------------------------------//
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiView_AKUSetInputConfigurationName ( JNIEnv* env, jclass obj, jstring jname ) {
		GET_STRING ( jname, name );
		AKUSetInputConfigurationName ( name );
		RELEASE_STRING ( jname, name );
	}

	//----------------------------------------------------------------//
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiView_AKUSetInputDevice ( JNIEnv* env, jclass obj, jint deviceId, jstring jname ) {
		GET_STRING ( jname, name );
		AKUSetInputDevice ( deviceId, name );
		RELEASE_STRING ( jname, name );
	}

	//----------------------------------------------------------------//
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiView_AKUSetInputDeviceCompass ( JNIEnv* env, jclass obj, jint deviceId, jint sensorId, jstring jname ) {
		GET_STRING ( jname, name );
		AKUSetInputDeviceCompass ( deviceId, sensorId, name );
		RELEASE_STRING ( jname, name );
	}

	//----------------------------------------------------------------//
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiView_AKUSetInputDeviceLevel ( JNIEnv* env, jclass obj, jint deviceId, jint sensorId, jstring jname ) {
		GET_STRING ( jname, name );
		AKUSetInputDeviceLevel ( deviceId, sensorId, name );
		RELEASE_STRING ( jname, name );
	}

	//----------------------------------------------------------------//
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiView_AKUSetInputDeviceLocation ( JNIEnv* env, jclass obj, jint deviceId, jint sensorId, jstring jname ) {
		GET_STRING ( jname, name );
		AKUSetInputDeviceLocation ( deviceId, sensorId, name );
		RELEASE_STRING ( jname, name );
	}
	
	//----------------------------------------------------------------//
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiView_AKUSetInputDeviceTouch ( JNIEnv* env, jclass obj, jint deviceId, jint sensorId, jstring jname ) {
		GET_STRING ( jname, name );
		AKUSetInputDeviceTouch ( deviceId, sensorId, name );
		RELEASE_STRING ( jname, name );
	}

	//----------------------------------------------------------------//
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiView_AKUSetWorkingDirectory ( JNIEnv* env, jclass obj, jstring jpath ) {

		GET_STRING ( jpath, path );

		USFileSys::SetCurrentPath ( path );
		USLuaRuntime::Get ().SetPath ( path );
	
		RELEASE_STRING ( jpath, path );
	}

	//----------------------------------------------------------------//
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiView_AKUUntzInit ( JNIEnv* env, jclass obj ) {
		AKUUntzInit ();
	}
	
	//----------------------------------------------------------------//
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiView_AKUUpdate ( JNIEnv* env, jclass obj ) {
		AKUUpdate ();
	}

	//----------------------------------------------------------------//
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiActivity_AKUAppWillEndSession ( JNIEnv* env, jclass obj ) {
		MOAIApp::Get ().WillEndSession ();
	}