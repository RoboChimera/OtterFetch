#include <sys/utsname.h>

char* fetchUname(void) {
	// For another day :3
	struct utsname unamePointer;
	uname(&unamePointer);

	int versionCSize = snprintf(NULL, 0, "Version: %s %s\n", unamePointer.sysname, unamePointer.release) + 1;
	char *version = (char *)malloc(versionCSize);
	sprintf(version, "Version: %s %s\n", unamePointer.sysname, unamePointer.release);

	return version;
}
