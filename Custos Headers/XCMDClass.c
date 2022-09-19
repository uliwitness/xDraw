/* ————————————————————————————————————————————————————————————————————————————————
	Headers
   ————————————————————————————————————————————————————————————————————————————— */

#include	"XCMDClass.h"
#ifndef powerc
#include	<A4Stuff.h>
#endif


/* ————————————————————————————————————————————————————————————————————————————————
	Globals:
   ————————————————————————————————————————————————————————————————————————————— */

XCmdPtr		paramPtr = NULL;
XCMDError	gError	= kXCMDSuccess;		

#if GENERATINGCFM		// Make sure we can do 'xcmd' and 'xfcn'.
long int __procinfo = kPascalStackBased | STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(void*)));
#endif


/* ————————————————————————————————————————————————————————————————————————————————
	Prototypes:
		These are for internal functions.
   ————————————————————————————————————————————————————————————————————————————— */

long		GetCStringLength( const char* thePtr );


/* ————————————————————————————————————————————————————————————————————————————————
	Entry Point
   ————————————————————————————————————————————————————————————————————————————— */

pascal void	main( XCmdPtr pp )
{
  #ifndef powerc
	long	oldA4 = SetCurrentA4();	/* A4 must be set up for globals, strings, and 
										inter-segment calls. */
  #endif
	
	// Set up our global with the param ptr so we needn't pass it around:
	paramPtr = pp;
	gError = noErr;
	
	/* Now set up an empty string as our return value:
		This is disposed if you use one of the utility
		functions to stash it in there. */
	if( !pp->returnValue )
	{
		pp->returnValue = NewHandleClear( 1 );
		if( MemError() != noErr || pp->returnValue == NULL )
		{
			pp->returnValue = NULL;
			return;
		}
	}
	
	// A bit of paranoia:
	if( !paramPtr )
		return;
	
	/* Now we check whether the user passed "?" or "!" as the first and only param.
		If one of those is passed, we call Syntax() resp. Copyright(), else
		Implementation() is called. */
	if( !CheckSpecialParams() )
		Implementation();
	
	// Set result to an error message if anything went wrong.
	HandleError( gError );
	
  #ifndef powerc
	SetA4( oldA4 );					// Reset A4 before leaving XCMD.
  #endif
}


/* ————————————————————————————————————————————————————————————————————————————————
	Check whether one of the special params has been passed:
		'!' returns Copyright, '?' returns Syntax.
   ————————————————————————————————————————————————————————————————————————————— */

Boolean	CheckSpecialParams()
{
	if( ParamCount() == 1 )
	{
		char		firstCh = GetParamChar(1);
		
		if( gError == kXCMDNoCharacterErr )	// Was longer than 1 character.
		{
			gError = kXCMDSuccess;	// Make sure we don't output some error.
			return false;
		}
		
		switch( firstCh )
		{
			case '!':
				Copyright();		// Define this function for your XCMD.
				return true;
				break;
			
			case '?':
				Syntax();			// Define this function for your XCMD.
				return true;
				break;
			
			default:
				return false;
		}
	}
	else
		return false;
}


/* ————————————————————————————————————————————————————————————————————————————————
	Set the return value to an error message for this error:
   ————————————————————————————————————————————————————————————————————————————— */

void	HandleError( const XCMDError err )
{
	if( (err != kXCMDSuccess) && (err != kXCMDSilentError) )
	{
		ReturnString( "Error: ", REPLACE );
		AppendError( err );
		ReturnString( ".", APPEND );
	}
}


/* ————————————————————————————————————————————————————————————————————————————————
	Append a clear-text error message to the error message in the result:
   ————————————————————————————————————————————————————————————————————————————— */

void	AppendError( const XCMDError err )
{
	switch( err )
	{
		case kXCMDSuccess:		// Don't output any errors if things went right.
		case kXCMDSilentError:	// Don't smash user's error messages.
			break;
		
		case memFullErr:
			ReturnString( "Not enough memory", APPEND );
			break;
		
		case fnfErr:
		case nsvErr:
		case dirNFErr:
		case resFNotFound:
			ReturnString( "File or volume not found", APPEND );
			break;
		
		case ioErr:
			ReturnString( "Read/Write error", APPEND );
			break;
		
		case bdNamErr:
		case dupFNErr:
			ReturnString( "Invalid or duplicate file name", APPEND );
			break;
		
		case fnOpnErr:
			ReturnString( "File not open", APPEND );
			break;
		
		case wPrErr:
		case fLckdErr:
		case vLckdErr:
			ReturnString( "Volume or file are locked", APPEND );
			break;
		
		case posErr:
		case eofErr:
			ReturnString( "Read/write offset out of range", APPEND );
			break;
		
		case dskFulErr:
		case dirFulErr:
			ReturnString( "Volume or folder are full", APPEND );
			break;
			
		case fBsyErr:
		case opWrErr:
			ReturnString( "File is busy or already open", APPEND );
			break;
			
		case permErr:
		case wrPermErr:
			ReturnString( "Invalid permissions to access this file", APPEND );
			break;
		
		case resNotFound:
			ReturnString( "Resource not found", APPEND );
			break;
		
		case addResFailed:
			ReturnString( "Couldn't add resource", APPEND );
			break;
		
		case rmvResFailed:
			ReturnString( "Couldn't remove resource", APPEND );
			break;
		
		case kXCMDTooFewParamsErr:
			ReturnString( "Too few params passed", APPEND );
			break;
		
		case kXCMDCallbackFailed:
			ReturnString( "Callback not successful", APPEND );
			break;
		
		case kXCMDCallbackNotImplemented:
			ReturnString( "Callback not supported by HyperCard", APPEND );
			break;
		
		case kXCMDNoBooleanErr:
			ReturnString( "Param is not a Boolean", APPEND );
			break;
		
		case kXCMDNoCharacterErr:
			ReturnString( "Expected single character as parameter", APPEND );
			break;
		
		case kXCMDPascalStringTooLong:
			ReturnString( "Parameter mustn't be longer than 255 characters", APPEND );
			break;
		
		case kXCMDNullHandlePassed:
			ReturnString( "A NULL Handle was passed to the XCMD shell", APPEND );
			break;
		
		case movieToolboxUninitialized:
			ReturnString( "Couldn't initialize QuickTime", APPEND );
			break;
		
		default:
			ReturnString( "Error #", APPEND );
			ReturnLong( err, APPEND );
	}
}


/* ————————————————————————————————————————————————————————————————————————————————
	Return a String:
   ————————————————————————————————————————————————————————————————————————————— */

void	ReturnString( const char* str, ReturnMode md )
{
	gError = kXCMDSuccess;
	
	if( paramPtr == NULL )
		return;
	
	if( md == REPLACE )
	{
		if( paramPtr->returnValue )
		{
			long		len = GetCStringLength( str );
			
			SetHandleSize( paramPtr->returnValue, len +1 );
			if( MemError() != noErr )
			{
				gError = MemError();
				return;
			}
			
			BlockMoveData( str, (*paramPtr->returnValue), len +1 );
		}
		else
			paramPtr->returnValue = CStrToCharsHnd( str );
	}
	else
	{
		AppendCStrToCharsHnd( paramPtr->returnValue, str );
	}
}


/* ————————————————————————————————————————————————————————————————————————————————
	Return text data:
   ————————————————————————————————————————————————————————————————————————————— */

void	ReturnText( const char* str, long len, ReturnMode md )
{
	gError = kXCMDSuccess;
	
	if( paramPtr == NULL )
		return;
	
	if( md == REPLACE )
	{
		if( paramPtr->returnValue )
			SetHandleSize( paramPtr->returnValue, len +1 );
		else
			paramPtr->returnValue = NewHandle( len+1 );
		
		if( MemError() != noErr )
		{
			gError = MemError();
			return;
		}
		BlockMoveData( str, (*paramPtr->returnValue), len );
		(*(char*) (((long)*paramPtr->returnValue) +len)) = 0;	// Terminate string.
	}
	else
	{
		long vCurrSize;
		
		vCurrSize = GetHandleSize( (Handle)paramPtr->returnValue );
		SetHandleSize( paramPtr->returnValue, vCurrSize +len );
		
		if( MemError() != noErr )
		{
			gError = MemError();
			return;
		}
		BlockMoveData( str, (*paramPtr->returnValue) +vCurrSize -1, len );
		(*(char*) (((long)*paramPtr->returnValue) +vCurrSize +len)) = 0;	// Terminate string.
	}
}

/* ————————————————————————————————————————————————————————————————————————————————
	Return a Boolean:
   ————————————————————————————————————————————————————————————————————————————— */

void	ReturnBoolean( const Boolean flag, ReturnMode md )
{
	if( flag )
		ReturnString( "true", md );
	else
		ReturnString( "false", md );
}


/* ————————————————————————————————————————————————————————————————————————————————
	Return a long:
   ————————————————————————————————————————————————————————————————————————————— */

void	ReturnLong( long num, ReturnMode md )
{
	Str255		theStr;
	
	NumToStr( paramPtr, num, theStr );
	
	theStr[theStr[0] +1] = 0;	// Zero-terminate string.
	
	ReturnString( (char*) theStr +1, md );
}


/* ————————————————————————————————————————————————————————————————————————————————
	Return an unsigned long:
   ————————————————————————————————————————————————————————————————————————————— */

void	ReturnULong( unsigned long num, ReturnMode md )
{
	Str255		theStr;
	
	LongToStr( paramPtr, num, theStr );
	
	theStr[theStr[0] +1] = 0;	// Zero-terminate string.
	
	ReturnString( (char*) theStr +1, md );
}


/* ————————————————————————————————————————————————————————————————————————————————
	Retrieve the specified parameter as a number:
   ————————————————————————————————————————————————————————————————————————————— */

long	GetParamNum( short index )
{
	Str255		theStr;
	char*		strPtr;
	
	gError = kXCMDSuccess;
	
	if( ParamCount() < index )
	{
		gError = kXCMDTooFewParamsErr;
		return 0;
	}
	
	strPtr = (*(paramPtr->params[index -1]));
	theStr[0] = GetCStringLength( strPtr );
	BlockMoveData( strPtr, ((char*)theStr) +1, theStr[0] );
	
	return( StrToNum( paramPtr, theStr ) );
}


/* ————————————————————————————————————————————————————————————————————————————————
	Retrieve the specified parameter as an unsigned number:
   ————————————————————————————————————————————————————————————————————————————— */

unsigned long	GetParamLong( short index )
{
	Str255		theStr;
	char*		strPtr;
	
	gError = kXCMDSuccess;
	
	if( ParamCount() < index )
	{
		gError = kXCMDTooFewParamsErr;
		return 0;
	}
	
	strPtr = (*(paramPtr->params[index -1]));
	theStr[0] = GetCStringLength( strPtr );
	BlockMoveData( strPtr, ((char*)theStr) +1, theStr[0] );
	
	return( StrToLong( paramPtr, theStr ) );
}


/* ————————————————————————————————————————————————————————————————————————————————
	Retrieve the specified parameter as a char:
   ————————————————————————————————————————————————————————————————————————————— */

char	GetParamChar( short index )
{
	char*		theStr;
	
	gError = kXCMDSuccess;
	
	if( ParamCount() < index )
	{
		gError = kXCMDTooFewParamsErr;
		return 0;
	}
	
	theStr = (*(char**)(paramPtr->params[index -1]));
	
	if( theStr[0] == 0		// Empty string?
		|| theStr[1] != 0 )	// Longer than 1 character?
	{
		gError = kXCMDNoCharacterErr;
		return 0;
	}
	
	return theStr[0];
}


/* ————————————————————————————————————————————————————————————————————————————————
	Retrieve the specified parameter as a Boolean:
   ————————————————————————————————————————————————————————————————————————————— */

Boolean	GetParamBool( short index )
{
	Str255	parmCopy;
	
	gError = kXCMDSuccess;
	
	if( ParamCount() < index )
	{
		gError = kXCMDTooFewParamsErr;
		return 0;
	}
	
	parmCopy[0] = GetCStringLength( (*(paramPtr->params[index -1])) );
	
	BlockMoveData( (*(paramPtr->params[index -1])), parmCopy +1, parmCopy[0] );
	
	if( EqualString( parmCopy, "\pfalse", false, true ) )
	{
		return false;
	}
	else if( EqualString( parmCopy, "\ptrue", false, true ) )
	{
		return true;
	}
	else
	{
		gError = kXCMDNoBooleanErr;
		return 0;
	}
}


/* ————————————————————————————————————————————————————————————————————————————————
	Retrieve the specified parameter as a Pascal String:
		This will set gError to kXCMDPascalStringTooLong if the parameter is
		longer than 255 characters, but will still return the first 255.
   ————————————————————————————————————————————————————————————————————————————— */

void	GetParamString( short index, Str255 str )
{
	long	len;
	
	gError = kXCMDSuccess;
	
	if( ParamCount() < index )
	{
		gError = kXCMDTooFewParamsErr;
		return;
	}
	
	len = GetCStringLength( (*(paramPtr->params[index -1])) );
	if( len > 255 )
	{
		gError = kXCMDPascalStringTooLong;
		len = 255;
	}
	
	str[0] = len;
	BlockMoveData( (*(paramPtr->params[index -1])), str +1, len );
}


/* ————————————————————————————————————————————————————————————————————————————————
	Convert a C String to a zero-terminated CharsHandle to pass back to HyperCard:
   ————————————————————————————————————————————————————————————————————————————— */

Handle	CStrToCharsHnd( const char *str )
{
	Handle		outHand = NULL;
	long		len;
	
	if( (len = GetCStringLength(str)) && (outHand = NewHandle( len +1 )) )
		BlockMoveData( str, (*outHand), len +1 );
	
	return( outHand );
}


/* ————————————————————————————————————————————————————————————————————————————————
	Append a C String to a zero-terminated CharsHandle:
   ————————————————————————————————————————————————————————————————————————————— */

void	AppendCStrToCharsHnd( CharsHandle hnd, const char *str )
{
	long		len = GetCStringLength( str ),	// Length w/o NULL char.
				hndLen;
	
	gError = kXCMDSuccess;
	
	if( len == 0 )
		return;
	
	if( hnd == NULL )
	{
		gError = kXCMDNullHandlePassed;
		return;
	}
	
	hndLen = GetHandleSize( hnd );
	if( hndLen <= 0 )
		hndLen++;		// Balance for missing NULL char.
	SetHandleSize( hnd, hndLen +len );
	
	if( MemError() != noErr )
	{
		gError = MemError();
		return;
	}
	
	HLock( (Handle) hnd );
	BlockMove( str, (*hnd) +hndLen -1, len +1 );	// Overwrite original String's NULL char.
	HUnlock( (Handle) hnd );
	
	if( MemError() != noErr )
	{
		gError = MemError();
		return;
	}
}


short	ParamCount()
{
	return paramPtr->paramCount;
}


/* ————————————————————————————————————————————————————————————————————————————————
	Retrieve the specified parameter as a Rect:
   ————————————————————————————————————————————————————————————————————————————— */

void  GetParamRect( short n, Rect *theRect )
{
	Str255		paramString;
	
	GetParamString( n, paramString );
	
	if( gError == kXCMDSuccess )
		MyStrToRect( paramString, theRect );
}


/* ————————————————————————————————————————————————————————————————————————————————
	Retrieve the specified parameter as an RGB Colour:
   ————————————————————————————————————————————————————————————————————————————— */

void  GetParamColor( short n, RGBColor *col )
{
	Str255		paramString;
	
	GetParamString( n, paramString );
	
	if( gError == kXCMDSuccess )
		MyStrToColor( paramString, col );
}


/* ————————————————————————————————————————————————————————————————————————————————
	Convert a string "#left,#top,#right,#bottom" to a Rect:
   ————————————————————————————————————————————————————————————————————————————— */

void	MyStrToRect( Str255 str, Rect *box )
{
	Str255		numString;
	Byte		x = 0;
	long		n;
	Rect		tempBox = { 0, 0, 0, 0 };
	
	// left:
	numString[0] = 0;
	
	while( ++x <= str[0] && str[x] != ',' )
		numString[ ++(numString[0]) ] = str[x];
	
	StringToNum( numString, &n );
	
	tempBox.left = n;
	
	// top:
	numString[0] = 0;
	
	while( ++x <= str[0] && str[x] != ',' )
		numString[ ++(numString[0]) ] = str[x];
	
	StringToNum( numString, &n );
	
	tempBox.top = n;
	
	// right:
	numString[0] = 0;
	
	while( ++x <= str[0] && str[x] != ',' )
		numString[ ++(numString[0]) ] = str[x];
	
	StringToNum( numString, &n );
	
	tempBox.right = n;
	
	// bottom:
	numString[0] = 0;
	
	while( ++x <= str[0] )
		numString[ ++(numString[0]) ] = str[x];
	
	StringToNum( numString, &n );
	
	tempBox.bottom = n;
	
	(*box) = tempBox;
}


/* ————————————————————————————————————————————————————————————————————————————————
	Convert a string "#red,#green,#blue" to an RGBColor:
   ————————————————————————————————————————————————————————————————————————————— */

void	MyStrToColor( Str255 str, RGBColor *box )
{
	Str255		numString;
	Byte		x = 0;
	long		n;
	RGBColor	tempBox = { 0, 0, 0 };
	
	// red:
	numString[0] = 0;
	
	while( ++x <= str[0] && str[x] != ',' )
		numString[ ++(numString[0]) ] = str[x];
	
	StringToNum( numString, &n );
	
	tempBox.red = n;
	
	// green:
	numString[0] = 0;
	
	while( ++x <= str[0] && str[x] != ',' )
		numString[ ++(numString[0]) ] = str[x];
	
	StringToNum( numString, &n );
	
	tempBox.green = n;
	
	// blue:
	numString[0] = 0;
	
	while( ++x <= str[0] )
		numString[ ++(numString[0]) ] = str[x];
	
	StringToNum( numString, &n );
	
	tempBox.blue = n;
	
	(*box) = tempBox;
}


/* ————————————————————————————————————————————————————————————————————————————————
	Convert a string "#h,#v" to a Point:
   ————————————————————————————————————————————————————————————————————————————— */

void	MyStrToPoint( Str255 str, Point *box )
{
	Str255		numString;
	Byte		x = 0;
	long		n;
	
	// h:
	numString[0] = 0;
	
	while( ++x <= str[0] && str[x] != ',' )
		numString[ ++(numString[0]) ] = str[x];
	
	StringToNum( numString, &n );
	
	box->h = n;
	
	// v:
	numString[0] = 0;
	
	while( ++x <= str[0] )
		numString[ ++(numString[0]) ] = str[x];
	
	StringToNum( numString, &n );
	
	box->v = n;
}


/* ————————————————————————————————————————————————————————————————————————————————
	Convert a string "true" or "false" to a Boolean:
   ————————————————————————————————————————————————————————————————————————————— */

void	MyStrToBool( Str255 str, Boolean *box )
{
	(*box) = EqualString( str, "\pTrue", false, true );
}


/* ————————————————————————————————————————————————————————————————————————————————
	Evaluate an expression storing the result in a PString:
   ————————————————————————————————————————————————————————————————————————————— */

void	MyEvalExpr( Str255 expr, Str255 result )
{
	Handle 		theText;
	
	theText = EvalExpr( paramPtr, expr );
	gError = paramPtr->result;
	
	if( gError == kXCMDSuccess )
	{
		ZeroToPas( paramPtr, (*theText), result );
		DisposeHandle( theText );
	}
}


/* ————————————————————————————————————————————————————————————————————————————————
	Get the card window's bitmap and mask for drawing:
		This is an undocumented callback in HyperCard.
   ————————————————————————————————————————————————————————————————————————————— */

void	MyGetMaskAndData( BitMap* maskBM, BitMap* dataBM )
{
	paramPtr->request = 29;
	paramPtr->inArgs[0] = (long) maskBM;
	paramPtr->inArgs[1] = (long) dataBM;
	
	CALL_ZERO_PARAMETER_UPP( paramPtr->entryPoint, kPascalStackBased );
	
	gError = paramPtr->result;
}


/* ————————————————————————————————————————————————————————————————————————————————
	Tell HyperCard that we just changed the BitMaps obtained using
		MyGetMaskAndData():
		This is an undocumented callback in HyperCard.
   ————————————————————————————————————————————————————————————————————————————— */

void	MyChangedMaskAndData()
{
	paramPtr->request = 30;
	
	CALL_ZERO_PARAMETER_UPP( paramPtr->entryPoint, kPascalStackBased );
	
	gError = paramPtr->result;
}


/* ————————————————————————————————————————————————————————————————————————————————
	Append strB to strA:
   ————————————————————————————————————————————————————————————————————————————— */

void	MyCatStrs( Str255 strA, const Str255 strB )
{
	short		vLen = strB[0],
				vOverflow;
	
	// Make sure we don't exceed string's end:
	vOverflow = 255 -strA[0] -strB[0];
	if( vOverflow < 0 )
		vLen += vOverflow;
	
	// Actually copy:
	BlockMoveData( strB +1, strA +strA[0] +1, vLen );
	strA[0] += vLen;
}


/* ————————————————————————————————————————————————————————————————————————————————
	Copy strA to strB:
   ————————————————————————————————————————————————————————————————————————————— */

void	MyCopyStr( const Str255 strA, Str255 strB )
{
	BlockMoveData( strA, strB, strA[0] +1 );
}


/* ————————————————————————————————————————————————————————————————————————————————
	Dispose of a Handle if it's not a resource:
   ————————————————————————————————————————————————————————————————————————————— */

void	KillHandle( Handle resData )
{
	if( resData != NULL )	// Handle was already allocated.
	{
		if( HomeResFile( resData ) == -1 )	// Not owned by res Mgr, we're responsible.
			DisposeHandle( resData );
	}
}


/* ————————————————————————————————————————————————————————————————————————————————
	GetCStringLength:
		My implementation of strlen so we don't have to include 40 k of standard
		library.
   ————————————————————————————————————————————————————————————————————————————— */

long	GetCStringLength( const char* aPtr )
{
	long	x = 0;
	
	while( aPtr[x] != 0 )
		x++;
	
	return( x );
}





