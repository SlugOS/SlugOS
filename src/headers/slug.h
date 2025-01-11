#pragma once

/*
The equivalent to dos.h in our case it contains the basics that are needed
*/

// Return status stuff

typedef int slug_status; // similar to EFI_STATUS

/*
SLUG_SUCCESS: When everything is fine and no failures occure this should be returned
SLUG_ERROR: This is a generic error message for when the error is unknown or something else happens
SLUG_UNSUPPORTED: When something like hardware or some other thing is not supported or is just a placeholder
*/
#define SLUG_SUCCESS 0
#define SLUG_ERROR 1
#define SLUG_UNSUPPORTED 2
