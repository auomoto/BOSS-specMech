/*------------------------------------------------------------------------------
main.h
------------------------------------------------------------------------------*/

#ifdef MAINH
#define MAINH

typedef struct {
	char cverb,				// Single character command
		cobject,			// Single character object
		cvalue[CVALUESIZE],	// Input value string for object
		cid[CIDSIZE];		// Command ID string
} ParsedCMD;

ParsedCMD pcmd[CSTACKSIZE];	// Split the command line into its parts

#endif