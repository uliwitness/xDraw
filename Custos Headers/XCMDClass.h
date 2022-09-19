#pragma once

#if PRAGMA_STRUCT_ALIGN
	#pragma options align=mac68k
#elif PRAGMA_STRUCT_PACKPUSH
	#pragma pack(push, 2)
#elif PRAGMA_STRUCT_PACK
	#pragma pack(2)
#endif

/* ————————————————————————————————————————————————————————————————————————————————
	Headers:
   ————————————————————————————————————————————————————————————————————————————— */

#include	<HyperXCmd.h>


/* ————————————————————————————————————————————————————————————————————————————————
	Constants:
   ————————————————————————————————————————————————————————————————————————————— */

// Possible append modes for the ReturnXX calls:
enum ReturnMode
{
	APPEND = 1,
	REPLACE
};

typedef enum ReturnMode	ReturnMode;


// Possible values for gError:
enum XCMDError
{
	kXCMDSuccess				= xresSucc,		// No error.
	kXCMDCallbackFailed			= xresFail,		// Callback failed.
	kXCMDCallbackNotImplemented	= xresNotImp,	// Callback not supported by host.
	kXCMDTooFewParamsErr 		= 1000,			// You tried to call GetParam() on a param that doesn't exist.
	kXCMDNoBooleanErr,							// You called GetParamBool() on a value that isn't TRUE or FALSE.
	kXCMDNoCharacterErr,						// You called GetParamChar() on a value that is no character (too long?).
	kXCMDPascalStringTooLong,					// You called GetParamString() on a value longer than 255 characters.
	kXCMDNullHandlePassed,						// You passed a NULL handle as a parameter to some function.
	kXCMDSilentError							// Set gError to this to indicate a custom error for which you already set the result.
};

typedef long	XCMDError;


/* ————————————————————————————————————————————————————————————————————————————————
	Globals:
   ————————————————————————————————————————————————————————————————————————————— */

extern XCmdPtr		paramPtr;	// XCMD Block Ptr passed by HyperCard.
extern XCMDError	gError;		// This is where the calls below return errors.


/* ————————————————————————————————————————————————————————————————————————————————
	Prototypes:
   ————————————————————————————————————————————————————————————————————————————— */

pascal void		main( XCmdPtr pp );

void	AppendError( const XCMDError err );
void	HandleError( const XCMDError err );	// Don't use anymore.
Boolean	CheckSpecialParams();

void	Implementation();
void	Copyright();
void	Syntax();

void	ReturnString( const char* str, ReturnMode md );
void	ReturnText( const char* str, long length, ReturnMode md );
void	ReturnBoolean( const Boolean flag, ReturnMode md );
void	ReturnLong( const long num, ReturnMode md );
void	ReturnULong( unsigned long num, ReturnMode md );
	
short			ParamCount();
void			GetParamString( short index, Str255 str );
long			GetParamNum( short index );
unsigned long	GetParamLong( short index );
Boolean			GetParamBool( short index );
char			GetParamChar( short index );
void			GetParamRect( short n, Rect *theRect );
void			GetParamColor( short n, RGBColor *col );

void	MyStrToRect( Str255 str, Rect *box );
void	MyStrToColor( Str255 str, RGBColor *box );
void	MyStrToPoint( Str255 str, Point *box );
void	MyStrToBool( Str255 str, Boolean *box );
void	MyEvalExpr( Str255 expr, Str255 result );
void	MyGetMaskAndData( BitMap* maskBM, BitMap* dataBM );
void	MyChangedMaskAndData();

void	MyCatStrs( Str255 strA, const Str255 strB );
void	MyCopyStr( const Str255 strA, Str255 strB );

void	KillHandle( Handle resData );
Handle	CStrToCharsHnd( const char * str );
void	AppendCStrToCharsHnd( CharsHandle hnd, const char *str );
long	GetCStringLength( const char* aPtr );


#if PRAGMA_STRUCT_ALIGN
	#pragma options align=reset
#elif PRAGMA_STRUCT_PACKPUSH
	#pragma pack(pop)
#elif PRAGMA_STRUCT_PACK
	#pragma pack()
#endif