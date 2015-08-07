#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "27";
	static const char MONTH[] = "01";
	static const char YEAR[] = "2015";
	static const char UBUNTU_VERSION_STYLE[] = "15.01";
	
	//Software Status
	static const char STATUS[] = "Alpha";
	static const char STATUS_SHORT[] = "a";
	
	//Standard Version Type
	static const long MAJOR = 0;
	static const long MINOR = 4;
	static const long BUILD = 457;
	static const long REVISION = 2607;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 834;
	#define RC_FILEVERSION 0,4,457,2607
	#define RC_FILEVERSION_STRING "0, 4, 457, 2607\0"
	static const char FULLVERSION_STRING[] = "0.4.457.2607";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 57;
	

}
#endif //VERSION_H
