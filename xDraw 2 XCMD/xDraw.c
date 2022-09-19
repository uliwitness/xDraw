/*	∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞

	PROJECT:	xDraw
	
	PURPOSE:	HyperCard XCMD that gives access to QuickDraw.
		
	COPYRIGHT:	(C) Copyright 1999 by M. Uli Kusterer, all rights reserved.
	
				
	REACH ME AT:
				E-MAIL:		witness@weblayout.com
				URL:		http://www.weblayout.com/witness
	
	
	REVISIONS:
		98-05-05	UK		Created.
				
	∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞ */

/* ————————————————————————————————————————————————————————————————————————————————
	Headers:
   ————————————————————————————————————————————————————————————————————————————— */

#include	<HyperXCmd.h>
#include	"SuperXCmd.h"
#include	"xDraw.h"


/* ————————————————————————————————————————————————————————————————————————————————
	Globals:
   ————————————————————————————————————————————————————————————————————————————— */

WindowPtr		gCardWindow;


/* ————————————————————————————————————————————————————————————————————————————————
	Do the actual work:
   ————————————————————————————————————————————————————————————————————————————— */

void	Implementation()
{
	Str255			theCmd;
	
	GetParamString( 1, theCmd );
	
	GetPort( &gCardWindow );
	
	if( EqualString( theCmd, "\pdraw", false, true ) )
		DoDrawMode();
	else if( EqualString( theCmd, "\pdrawToGrc", false, true ) )
		DoDrawToGrc();	// SC only!
	else if( EqualString( theCmd, "\pwithin", false, true ) )
		DoWithin();
	else if( EqualString( theCmd, "\pinvalid", false, true ) )
		DoInvalid( true );
	else if( EqualString( theCmd, "\pvalid", false, true ) )
		DoInvalid( false );
	else if( EqualString( theCmd, "\psavePic", false, true )
			|| EqualString( theCmd, "\psavePicture", false, true )
			|| EqualString( theCmd, "\psavePictRes", false, true ) )
		DoSavePictRes();
	else if( EqualString( theCmd, "\pdrawToCard", false, true ) )
		DoDrawToCard();	// HC only!
	else if( EqualString( theCmd, "\pdrawToScreen", false, true ) )
		DoDrawDesktop();
	else if( EqualString( theCmd, "\psaveFile", false, true )
			|| EqualString( theCmd, "\psavePictFile", false, true ) )
		DoSavePictFile();
	else
		ReturnString( "Error: unsupported mode. Call xDraw \"?\" for Syntax info.", REPLACE );
}


/* ————————————————————————————————————————————————————————————————————————————————
	DoDrawMode:
		Handles the "draw" opcode.
	
	REVISIONS:
		1999-07-18	UK	Extracted from Implementation() and added CGrafPort
						creation to work around B/W ports in HyperCard.
   ————————————————————————————————————————————————————————————————————————————— */

void	DoDrawMode()
{
	CGrafPort		wmPort;
	GrafPtr			oldPort;
	Point			newOrigin = { 0, 0 };
	
	// Get ourselves a nice CGrafPort that points to our window:
	LocalToGlobal( &newOrigin );
	GetPort( &oldPort );
	OpenCPort( &wmPort );
	SetPort( (GrafPtr) &wmPort );
	
	wmPort.portRect = oldPort->portRect;
	SetOrigin( -newOrigin.h, -newOrigin.v );
	SetClip( oldPort->visRgn );
	CopyRgn( wmPort.clipRgn, wmPort.visRgn );
	
	// Draw our stuff:
	DoDraw( kXDRAWAllFeaturesFlag );
	
	// Get rid of port:
	SetPort( oldPort );
	CloseCPort( &wmPort );
}


/* ————————————————————————————————————————————————————————————————————————————————
	DoDrawDesktop:
		Handles the "drawToScreen" opcode.
	
	REVISIONS:
		1999-07-18	UK	Created.
   ————————————————————————————————————————————————————————————————————————————— */

void	DoDrawDesktop()
{
	CGrafPort		wmPort;
	GrafPtr			oldPort;
	Point			newOrigin = { 0, 0 };
	
	// Get ourselves a nice CGrafPort that points to our window:
	GetPort( &oldPort );
	OpenCPort( &wmPort );
	SetPort( (GrafPtr) &wmPort );
	
	wmPort.portRect = (**GetGrayRgn()).rgnBBox;
	SetClip( GetGrayRgn() );
	CopyRgn( wmPort.clipRgn, wmPort.visRgn );
	
	// Draw our stuff:
	DoDraw( kXDRAWAllFeaturesFlag );
	
	// Get rid of port:
	SetPort( oldPort );
	CloseCPort( &wmPort );
}


/* ————————————————————————————————————————————————————————————————————————————————
	DoWithin:
		Handles the "within" opcode by creating a region from the draw opcodes
		and checking whether the specified point lies inside.
	
	REVISIONS:
		1999-07-18	UK	Extracted from Implementation().
   ————————————————————————————————————————————————————————————————————————————— */

void	DoWithin()
{
	Point		pos;
	Str255		pointStr;
	RgnHandle	theRgn;
	
	if( ParamCount() != 3 )
	{
		if( ParamCount() == 4 )	// Be tolerant, support "1,1" and 1,1:
		{
			pos.h = GetParamNum(3);
			pos.v = GetParamNum(4);
		}
		else
		{
			ReturnString( "Error: Invalid parameter count instead of xDraw \"within\",<opcodes>,<point>", REPLACE );
			return;
		}
	}
	else	// Everything proper, fetch whole point:
	{
		GetParamString( 3, pointStr );
		MyStrToPoint( pointStr, &pos );
	}
	
	theRgn = DoMakeRgnFromOpcodes();	// Create region, we have to dispose of it!
	
	if( theRgn )
	{
		ReturnBoolean( PtInRgn( pos, theRgn ), REPLACE );
		DisposeRgn( theRgn );
	}
}


/* ————————————————————————————————————————————————————————————————————————————————
	DoInvalid:
		Handle the "invalid" and "valid" opcodes which create a Region from the
		opcodes and then pass it to InvalRgn() or ValidRgn().
	
	REVISIONS:
		1999-07-18	UK	Extracted from Implementation().
   ————————————————————————————————————————————————————————————————————————————— */

void	DoInvalid( Boolean inval )
{
	RgnHandle		theRgn;
	
	if( ParamCount() != 2 )
	{
		ReturnString( "Error: Invalid number of parameters instead of xDraw \"", REPLACE );
		if( inval )
			ReturnString( "invalid", APPEND );
		else
			ReturnString( "valid", APPEND );
		ReturnString( "\",<opcodes>", APPEND );
		return;
	}
	
	theRgn = DoMakeRgnFromOpcodes();	// Create region, we have to dispose of it!
	
	if( theRgn )
	{
		if( inval )
			InvalRgn( theRgn );
		else
			ValidRgn( theRgn );
		DisposeRgn( theRgn );
	}
}


/* ————————————————————————————————————————————————————————————————————————————————
	DoSavePictRes:
		Handle savePictRes opcode. This generates a PicHandle from the opcodes and
		then saves it as a 'PICT' resource.
	
	REVISIONS:
		1999-07-18	UK	Extracted from Implementation().
   ————————————————————————————————————————————————————————————————————————————— */

void	DoSavePictRes()
{
	PicHandle		thePict;
	Str255			theCmd;
	
	if( ParamCount() < 4 || ParamCount() > 5 )
	{
		ReturnString( "Error: Invalid number of parameters instead of xDraw \"savePictRes\",<opcodes>,<name>,<id>,{replace|unique|ifNotExists}", REPLACE );
		return;
	}
	
	thePict = DoMakePictureFromOpcodes();	// Create picture, we have to dispose of it!
	
	if( thePict )
	{
		long		resID;
		short		mode;
		Str255		resName;
		Handle		theRes;
		
		GetParamString( 3, resName );
		resID = GetParamNum( 4 );
		
		if( ParamCount() < 5 )
			mode = kXDRAWMakePictIfNotThere;
		else
		{
			GetParamString( 5, theCmd );
			if( EqualString( theCmd, "\pUnique", false, true ) )
				mode = kXDRAWMakePictUnique;
			else if( EqualString( theCmd, "\pReplace", false, true ) )
				mode = kXDRAWMakePictReplace;
			else
				mode = kXDRAWMakePictIfNotThere;
		}
		
		theRes = Get1Resource( 'PICT', resID );
		if( theRes )	// Already have resource of this name.
		{
			switch( mode )
			{
				case kXDRAWMakePictUnique:
					resID = UniqueID( 'PICT' );
					break;
				
				case kXDRAWMakePictIfNotThere:
					ReturnString( "Error: PICT resource of this ID already exists.", REPLACE );
					KillPicture( thePict );
					return;
					break;
			}
		}
		
		if( mode == kXDRAWMakePictReplace && theRes != NULL )
		{
			SetHandleSize( theRes, GetHandleSize( (Handle) thePict ) );
			if( MemError() != noErr )
			{
				ReturnString( "Error: Couldn't change resource (", REPLACE );
				ReturnLong( MemError(), APPEND );
				ReturnString( ").", APPEND );
				KillPicture( thePict );
				return;
			}
			
			BlockMoveData( (*(Handle)thePict), (*theRes), GetHandleSize((Handle)thePict) );
			
			KillPicture( thePict );
			thePict = (PicHandle) theRes;
		}
		else
		{
			AddResource( (Handle) thePict, 'PICT', resID, resName );
			
			if( ResError() != noErr )
			{
				ReturnString( "Error: Couldn't create resource (", REPLACE );
				ReturnLong( ResError(), APPEND );
				ReturnString( ").", APPEND );
				KillPicture( thePict );
				return;
			}
		}
		ChangedResource( (Handle) thePict );
		WriteResource( (Handle) thePict );
		KillHandle( (Handle) thePict );
	}
}


/* ————————————————————————————————————————————————————————————————————————————————
	DoDrawToGrc:
		Handle drawToGrc opcode. This opcode is only available in SuperCard and
		allows drawing directly into a graphic object.
	
	REVISIONS:
		2000-12-17	UK	Created.
   ————————————————————————————————————————————————————————————————————————————— */

void	DoDrawToGrc()
{
	PicHandle		thePict;
	
	if( ParamCount() != 3 )
	{
		ReturnString( "Error: Invalid number of parameters instead of xDraw \"drawToGrc\",<opcodes>,<long name of grc>", REPLACE );
		return;
	}
	
	thePict = DoMakePictureFromOpcodes();	// Create picture, we have to dispose of it!
	
	if( thePict )
	{
		Str255		objectName;
		
		GetParamString( 3, objectName );
		InstallPData( paramPtr, objectName, thePict );
		
		//KillHandle( (Handle) thePict ); // Don't kill it, apparently SC takes it over.
	}
}

/* ————————————————————————————————————————————————————————————————————————————————
	DoSavePictFile:
		Handle savePictRes opcode. This generates a PicHandle from the opcodes and
		then saves it as a 'PICT' file.
	
	REVISIONS:
		1999-07-18	UK	Extracted from Implementation().
   ————————————————————————————————————————————————————————————————————————————— */

void	DoSavePictFile()
{
	PicHandle		thePict;
	Str255			theCmd;
	
	if( ParamCount() < 3 || ParamCount() > 4 )
	{
		ReturnString( "Error: Invalid number of parameters instead of xDraw \"savePictFile\",<opcodes>,<filePath>[,<creator>]", REPLACE );
		return;
	}
	
	thePict = DoMakePictureFromOpcodes();	// Create picture, we have to dispose of it!
	
	if( thePict )
	{
		Str255		fileName;
		short		vFileRef;
		long		vPicSize;
		OSType		vCreator = '    ';
		OSErr		vError;
		
		GetParamString( 3, fileName );
		
		if( ParamCount() < 4 )
			vCreator = 'ttxt';
		else
		{
			GetParamString( 4, theCmd );
			if( theCmd[0] <= 4 )
				BlockMoveData( ((char*)theCmd) +1, &vCreator, theCmd[0] );
			else
			{
				KillPicture( thePict );
				ReturnString( "Error: File creator must be 4 characters long.", REPLACE );
				return;
			}
		}
		
		// Open file:
		vError = FSOpen( fileName, 0, &vFileRef );
		if( vError == fnfErr )
		{
			vError = Create( fileName, 0, vCreator, 'PICT' );
			if( vError == noErr )
				vError = FSOpen( fileName, 0, &vFileRef );
		}
		if( vError != noErr )
		{
			KillPicture( thePict );
			switch( vError )
			{
				case dskFulErr:
				case dirFulErr:
					ReturnString( "Error: Disk/Folder is full.", REPLACE );
					break;
				
				case wPrErr:
				case fLckdErr:
				case vLckdErr:
					ReturnString( "Error: Volume is locked.", REPLACE );
					break;
				
				default:
					ReturnString( "Error: Couldn't open/create file (", REPLACE );
					ReturnLong( vError, APPEND );
					ReturnString( ").", APPEND );
					break;
			}
			return;
		}
		
		vPicSize = GetHandleSize((Handle)thePict);
		
		vError = SetEOF( vFileRef, 512 +vPicSize );	// 512 is header that's at start of every PICT file.
		if( vError != noErr )
		{
			FSClose( vFileRef );
			KillPicture( thePict );
			switch( vError )
			{
				case dskFulErr:
				case dirFulErr:
					ReturnString( "Error: Disk/Folder is full.", REPLACE );
					break;
				
				case wPrErr:
				case fLckdErr:
				case vLckdErr:
					ReturnString( "Error: Volume/file is locked.", REPLACE );
					break;
				
				default:
					ReturnString( "Error: Couldn't resize file (", REPLACE );
					ReturnLong( vError, APPEND );
					ReturnString( ").", APPEND );
					break;
			}
			return;
		}
		
		vError = SetFPos( vFileRef, fsFromStart, 512 );	// Move past header.
		if( vError != noErr )
		{
			FSClose( vFileRef );
			KillPicture( thePict );
			ReturnString( "Error: Couldn't access write position (", REPLACE );
			ReturnLong( vError, APPEND );
			ReturnString( ").", APPEND );
			return;
		}
		
		HLock( (Handle) thePict );
		vError = FSWrite( vFileRef, &vPicSize, (*thePict) );
		HUnlock( (Handle) thePict );
		if( vError != noErr )
		{
			FSClose( vFileRef );
			KillPicture( thePict );
			
			switch( vError )
			{
				case dskFulErr:
				case dirFulErr:
					ReturnString( "Error: Disk/Folder is full.", REPLACE );
					break;
				
				case wPrErr:
				case fLckdErr:
				case vLckdErr:
					ReturnString( "Error: Volume/file is locked.", REPLACE );
					break;
				
				default:
					ReturnString( "Error: Couldn't write to file (", REPLACE );
					ReturnLong( vError, APPEND );
					ReturnString( ").", APPEND );
					break;
			}
			return;
		}
		
		FSClose( vFileRef );
		KillPicture( thePict );
	}
}


/* ————————————————————————————————————————————————————————————————————————————————
	DoDrawToCard:
		Draw the opcodes into the card bitmap. this uses an undocumented callback
		which is only supported by HyperCard.
	
	REVISIONS:
		1999-07-18	UK	Extracted from Implementation().
   ————————————————————————————————————————————————————————————————————————————— */

void	DoDrawToCard()
{
	BitMap		maskBits,
				dataBits;
	GrafPtr		cardWinPort;
	GrafPort	myPort;
	
	GetPort( &cardWinPort );	// Keep around current port.
	
	// Make a GrafPort that points to our card image:
	MyGetMaskAndData( &maskBits, &dataBits );	// Get HC's card graphics.
	OpenPort( &myPort );
	SetOrigin( 0, 0 );
	MovePortTo( 0, 0 );
	PortSize( cardWinPort->portRect.right, cardWinPort->portRect.bottom );
	RectRgn( myPort.visRgn, &myPort.portRect );
	RectRgn( myPort.clipRgn, &myPort.portRect );
	BlockMoveData( &dataBits, &myPort.portBits, sizeof(BitMap) );
	
	// Draw opcodes:
	DoDraw( kXDRAWNoPortsFlag );
	
	// Kill port and clean up:
	SetPort( cardWinPort );
	ClosePort( &myPort );
	MyChangedMaskAndData();		// Tell HC we changed it so it updates the card.
}


/* ————————————————————————————————————————————————————————————————————————————————
	DoMakeRgnFromOpcodes:
		Draw all opcodes in parameter 2 that are eligible for region recording
		and return the region handle. If an error occurs this sets the XCMD result
		to an error message and returns NULL.
	
	REVISIONS:
		1999-07-04	UK	Created.
   ————————————————————————————————————————————————————————————————————————————— */

RgnHandle	DoMakeRgnFromOpcodes()
{
	RgnHandle		theRgn;
	Boolean			success;
	
	theRgn = NewRgn();
	if( theRgn == NULL )
	{
		ReturnString( "Error: Couldn't create region (", REPLACE );
		ReturnLong( MemError(), APPEND );
		ReturnString( ").", APPEND );
	}
	
	OpenRgn();
	success = DoDraw( kXDRAWRegionRecordFlag );
	CloseRgn( theRgn );
	
	if( !success )
	{
		DisposeRgn( theRgn );
		return NULL;
	}
	else
		return theRgn;
}


/* ————————————————————————————————————————————————————————————————————————————————
	DoMakePictureFromOpcodes:
		Draw all opcodes in parameter 2 that are eligible for picture recording
		and return the Pict handle. If an error occurs this sets the XCMD result
		to an error message and returns NULL.
	
	REVISIONS:
		1999-07-04	UK	Created.
   ————————————————————————————————————————————————————————————————————————————— */

PicHandle	DoMakePictureFromOpcodes()
{
	PicHandle		theRgn;
	Boolean			success;
	GrafPtr			vPort;
	OpenCPicParams	vPicHeader;
	
	GetPort( &vPort );
	
	vPicHeader.srcRect = vPort->portRect;
	vPicHeader.hRes = 0x00480000;		// 72 dpi
	vPicHeader.vRes = 0x00480000;		// 72 dpi
	vPicHeader.version = -2;			// Version 2 PICT.
	vPicHeader.reserved1 = 0;
	vPicHeader.reserved2 = 0;
	
	theRgn = OpenCPicture( &vPicHeader );
	success = DoDraw( kXDRAWPictureRecordFlag );
	ClosePicture();
	
	if( !success )
	{
		KillPicture( theRgn );
		return NULL;
	}
	else
		return theRgn;
}


/* ————————————————————————————————————————————————————————————————————————————————
	DoDraw:
		Draw all opcodes which are fetched from parameter 2. This scans the string
		with all the opcodes in parameter 2 and passes each line to
		DoProcessOpcode() which does the actual work.
	
	REVISIONS:
		1999-07-04	UK	Created.
   ————————————————————————————————————————————————————————————————————————————— */

Boolean	DoDraw( unsigned long flags )
{
	char		theString[512];		// 512 byte limit to length of one opcode.
	short		x = 0,
				tsx = 0;
	CGrafPtr	vOldPort;
	GDHandle	vOldDevice;
	long		vBlackPat[2] = { 0xFFFFFFFF, 0xFFFFFFFF };
	
	GetGWorld( &vOldPort, &vOldDevice );
	
	while( (*paramPtr->params[1])[x] != 0 )	// Scan complete string:
	{
		// Fetch one line and copy to theString:
		while( (*paramPtr->params[1])[x] != '\n'
				&& (*paramPtr->params[1])[x] != '\r'
				&& (*paramPtr->params[1])[x] != 0 )
		{
			theString[tsx++] = (*paramPtr->params[1])[x++];
			theString[tsx] = 0;
			
			if( tsx >= (sizeof(theString) -1) )
			{
				ReturnString( "Error: Can' handle lines longer than ", REPLACE );
				ReturnLong( (sizeof(theString) -1), APPEND );
				ReturnString( " characters.", APPEND );
				
				goto error;
			}
		}
		
		if( !DoProcessOpcode( theString, flags ) )
			goto error;
		
		if( (*paramPtr->params[1])[x] == 0 )	// End of string.
			goto restore;
		
		theString[0] = 0;
		tsx = 0;
		
		x++;
	}
	
restore:
	SetGWorld( vOldPort, vOldDevice );
	ForeColor( blackColor );
	BackColor( whiteColor );
	PenMode( patCopy );
	TextMode( patOr );
	PenPat( (Pattern*) vBlackPat );
	BackPat( (Pattern*) vBlackPat );
	
	return TRUE;

error:
	SetGWorld( vOldPort, vOldDevice );
	ForeColor( blackColor );
	BackColor( whiteColor );
	PenMode( patCopy );
	TextMode( patOr );
	PenPat( (Pattern*) vBlackPat );
	BackPat( (Pattern*) vBlackPat );
	
	return FALSE;
}


/* ————————————————————————————————————————————————————————————————————————————————
	DoIsSubString:
		Returns TRUE if a given string is a substring of another string. I.e.
		DoIsSubString( "\pbar", "\pfoobar" ) == TRUE, DoIsSubString( "\pjeff",
		"\pfoobar" ) == FALSE.
	
	NOTE:
		This is case-sensitive.
		
	TAKES:
		str1 -		The string which might be a substring.
		str2 -		The string that might contain str1.
	
	GIVES:
		Boolean -	TRUE if str1 is in str2, FALSE if str2 doesn't contain str1.
	
	REVISIONS:
		1999-07-04	UK	Created.
   ————————————————————————————————————————————————————————————————————————————— */

Boolean	DoIsSubString( Str255 str1, Str255 str2 )
{
	short		x = 1,
				y, z;
	
	while( x <= (str2[0] -str1[0] +1) )
	{
		if( str2[x] == str1[1] )
		{
			y = 2;
			z = x +1;
			
			while( y <= str1[0] )
			{
				if( str1[y] != str2[z] )
					y = str1[0] +1;	// Exit this loop, not a match.
				else if( y == str1[0] )	// Was last char.
					return TRUE;
				
				y++;
				z++;
			}
		}
		
		x++;
	}
	
	return FALSE;
}


/* ————————————————————————————————————————————————————————————————————————————————
	DoFetchNextItemNum:
		Fetches the next item in string starting at the specified offset and
		converts it to a number.
		
	TAKES:
		delim -		The item delimiter to use.
		string -	The C-string (zero-terminated) to get the item from.
		x -			The offset to start scanning the string at.
	
	GIVES:
		Boolean -	TRUE if successful, FALSE if item doesn't exist.
		x -			Set to the first character after the delimiter, or to the
					NULL character of the string.
		num -		The variable pointed to by this is set to the value read.
	
	REVISIONS:
		1999-07-04	UK	Created.
   ————————————————————————————————————————————————————————————————————————————— */

Boolean	DoFetchNextItemNum( char delim, char* string, short *x, long *num )
{
	Str255		str;
	
	DoFetchNextItem( delim, string, x, str );
	if( str[0] == 0 )
		return FALSE;
	else
	{
		StringToNum( str, num );
		return TRUE;
	}
}


/* ————————————————————————————————————————————————————————————————————————————————
	DoFetchNextItemPoint:
		Fetches the next two items in string starting at the specified offset and
		converts them to a point.
		
	NOTE:
		This may change num even when returning FALSE.
		
	TAKES:
		delim -		The item delimiter to use.
		string -	The C-string (zero-terminated) to get the item from.
		x -			The offset to start scanning the string at.
	
	GIVES:
		Boolean -	TRUE if successful, FALSE if item doesn't exist.
		x -			Set to the first character after the delimiter, or to the
					NULL character of the string.
		num -		The variable pointed to by this is set to the value read.
	
	REVISIONS:
		1999-07-04	UK	Created.
   ————————————————————————————————————————————————————————————————————————————— */

Boolean	DoFetchNextItemPoint( char delim, char* string, short *x, Point *num )
{
	long		nb;
	
	if( !DoFetchNextItemNum( delim, string, x, &nb ) )
		return FALSE;
	else
	{
		num->h = nb;
		
		if( !DoFetchNextItemNum( delim, string, x, &nb ) )
			return FALSE;
		else
			num->v = nb;
		
		return TRUE;
	}
}


/* ————————————————————————————————————————————————————————————————————————————————
	DoFetchNextItemMode:
		Fetches the next item in string starting at the specified offset and
		returns a constant identifying what mode it is. Supported modes are:
		
		<unrecognized>	- 0
		"fill"/"paint"	- 1
		"frame"			- 2
		"invert"		- 3
		"erase"			- 4
		
	TAKES:
		delim -		The item delimiter to use.
		string -	The C-string (zero-terminated) to get the item from.
		x -			The offset to start scanning the string at.
	
	GIVES:
		x -			Set to the first character after the delimiter, or to the
					NULL character of the string.
		long -		The variable pointed to by this is set to the value read.
	
	REVISIONS:
		1999-07-04	UK	Created.
   ————————————————————————————————————————————————————————————————————————————— */

long	DoFetchNextItemMode( char delim, char* string, short *x )
{
	Str255		str;
	
	DoFetchNextItem( delim, string, x, str );
	if( str[0] == 0 )
		return kXDRAWUnrecognizedMode;
	else
	{
		if( EqualString( "\pfill", str, false, true ) )
			return kXDRAWFillMode;
		else if( EqualString( "\pframe", str, false, true ) )
			return kXDRAWFrameMode;
		else if( EqualString( "\perase", str, false, true ) )
			return kXDRAWEraseMode;
		else if( EqualString( "\pinvert", str, false, true ) )
			return kXDRAWInvertMode;
		else if( EqualString( "\ppaint", str, false, true ) )
			return kXDRAWFillMode;
		else
			return kXDRAWUnrecognizedMode;
	}
}


/* ————————————————————————————————————————————————————————————————————————————————
	DoFetchNextItem:
		Fetches the next item in string starting at the specified offset.
		
	TAKES:
		delim -		The item delimiter to use.
		string -	The C-string (zero-terminated) to get the item from.
		x -			The offset to start scanning the string at.
	
	GIVES:
		outStr -	This Pascal string is set to the item's text.
		x -			Set to the first character after the delimiter, or to the
					NULL character of the string.
	
	REVISIONS:
		1999-07-04	UK	Created.
   ————————————————————————————————————————————————————————————————————————————— */

void	DoFetchNextItem( char delim, char* string, short *x, Str255 outStr )
{
	outStr[0] = 0;
	
	while( string[*x] != delim )
	{
		if( string[*x] == 0 )
			return;
		
		outStr[0] ++;
		outStr[outStr[0]] = string[*x];
		
		(*x)++;
	}
	
	// Move past delimiter:
	if( string[*x] == delim )
		(*x)++;
}


/* ————————————————————————————————————————————————————————————————————————————————
	Return Syntax information:
   ————————————————————————————————————————————————————————————————————————————— */

void	Syntax()
{
	ReturnString( "XDraw <Draw|within|drawToCard|drawToScreen|valid|invalid|savePictRes|savePictFile|drawToGrc>[,<opcodes>[,<name|objDescriptor>[,<id>,<replace>]]]", REPLACE );
}


/* ————————————————————————————————————————————————————————————————————————————————
	Return Copyright information:
   ————————————————————————————————————————————————————————————————————————————— */

void	Copyright()
{
	ReturnString( "xDraw 2.3.6, © Copyright 1995-2002 by M. Uli Kusterer, all rights rsvd.", REPLACE );
}




