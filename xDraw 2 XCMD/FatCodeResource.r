#include "MixedMode.r"

/*
Use the project ProcInfo.π to determine the correct value for the ProcInfo field below.

A single '$' means a hex value.  You can also use the C format of 0x... if you want.
Run ProcInfo.π to prove for yourself that a C routine (kCStackBased) that has no
arguments and returns void has a ProcInfo of $1 or 0x1.
Two dollar signs indicate a rez variable, as in $$Resource.  $$Resource tells rez 
to read the resource fork of a file and return, as a string, the data of a resource 
of a certain type and ID.
*/


type 'XCMD' as 'sdes';

resource 'XCMD' (128, "xDraw") {
	$C0,											// 68K ProcInfo
	$C0,											// PowerPC ProcInfo
	$$Resource("Fat XCMD Temp.rsrc", 'KODE', 128),	// Specify filename, type, and ID of resource
													//   containing 68k code
	$$Resource("Fat XCMD Temp.rsrc", 'KODE', 129)	// Specify filename, type, and ID of resource
													//   containing a pef container
};

