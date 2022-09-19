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
#include	<ControlDefinitions.h>
#include	"xDraw.h"
#include	"xDrawOpcodes.h"

#if powerc
#include	<ImageCompression.h>
#endif /*powerc*/

Boolean	DoLineOpcode( char* string, short *x, unsigned long flags )
{
	Rect		box;
			
	// start pos:
	if( !DoFetchNextItemPoint( ',', string, x, &topLeft(box) ) )
	{
		ReturnString( "Error: Item 2 to 3 of 'line' opcode must be start position.", REPLACE );
		return FALSE;
	}
	
	// end pos:
	if( !DoFetchNextItemPoint( ',', string, x, &botRight(box) ) )
	{
		ReturnString( "Error: Item 4 to 5 of 'line' opcode must be end position.", REPLACE );
		return FALSE;
	}
	
	MoveTo( topLeft(box).h, topLeft(box).v );
	LineTo( botRight(box).h, botRight(box).v );
	
	return TRUE;
}


Boolean	DoRectOpcode( char* string, short *x, unsigned long flags )
{
	Rect		box;
			
	// top left:
	if( !DoFetchNextItemPoint( ',', string, x, &topLeft(box) ) )
	{
		ReturnString( "Error: Item 2 to 3 of 'rect' opcode must be topLeft coordinate.", REPLACE );
		return FALSE;
	}
	
	// bottom right:
	if( !DoFetchNextItemPoint( ',', string, x, &botRight(box) ) )
	{
		ReturnString( "Error: Item 4 to 5 of 'rect' opcode must be botRight coordinate.", REPLACE );
		return FALSE;
	}
	
	switch( DoFetchNextItemMode( ',', string, x ) )
	{
		case kXDRAWUnrecognizedMode:
			ReturnString( "Error: Unrecognized mode, must be one of FRAME, FILL (PAINT), INVERT or ERASE.", REPLACE );
			break;
		
		case kXDRAWFillMode:
			PaintRect( &box );
			break;
		
		case kXDRAWFrameMode:
			FrameRect( &box );
			break;
		
		case kXDRAWEraseMode:
			EraseRect( &box );
			break;
		
		case kXDRAWInvertMode:
			InvertRect( &box );
			break;
	}
	
	return TRUE;
}


Boolean	DoOvalOpcode( char* string, short *x, unsigned long flags )
{
	Rect		box;
			
	// top left:
	if( !DoFetchNextItemPoint( ',', string, x, &topLeft(box) ) )
	{
		ReturnString( "Error: Item 2 to 3 of 'oval' opcode must be topLeft coordinate.", REPLACE );
		return FALSE;
	}
	
	// bottom right:
	if( !DoFetchNextItemPoint( ',', string, x, &botRight(box) ) )
	{
		ReturnString( "Error: Item 4 to 5 of 'oval' opcode must be botRight coordinate.", REPLACE );
		return FALSE;
	}
	
	switch( DoFetchNextItemMode( ',', string, x ) )
	{
		case kXDRAWUnrecognizedMode:
			ReturnString( "Error: Unrecognized mode, must be one of FRAME, FILL (PAINT), INVERT or ERASE.", REPLACE );
			break;
		
		case kXDRAWFillMode:
			PaintOval( &box );
			break;
		
		case kXDRAWFrameMode:
			FrameOval( &box );
			break;
		
		case kXDRAWEraseMode:
			EraseOval( &box );
			break;
		
		case kXDRAWInvertMode:
			InvertOval( &box );
			break;
	}
	
	return TRUE;
}


Boolean	DoRoundRectOpcode( char* string, short *x, unsigned long flags )
{
	Rect		box;
	Point		ovalSz;
	long		md;
	
	// top left:
	if( !DoFetchNextItemPoint( ',', string, x, &topLeft(box) ) )
	{
		ReturnString( "Error: Item 2 to 3 of 'round rect' opcode must be topLeft coordinate.", REPLACE );
		return FALSE;
	}
	
	// bottom right:
	if( !DoFetchNextItemPoint( ',', string, x, &botRight(box) ) )
	{
		ReturnString( "Error: Item 4 to 5 of 'round rect' opcode must be botRight coordinate.", REPLACE );
		return FALSE;
	}
	
	md = DoFetchNextItemMode( ',', string, x );
	if( md == kXDRAWUnrecognizedMode )
	{
		ReturnString( "Error: Unrecognized mode, must be one of FRAME, FILL (PAINT), INVERT or ERASE.", REPLACE );
		return FALSE;
	}
	
	if( !DoFetchNextItemPoint( ',', string, x, &ovalSz ) )
		ovalSz.h = 15; ovalSz.v = 15;
	
	switch( md )
	{
		case kXDRAWFillMode:
			PaintRoundRect( &box, ovalSz.h, ovalSz.v );
			break;
		
		case kXDRAWFrameMode:
			FrameRoundRect( &box, ovalSz.h, ovalSz.v );
			break;
		
		case kXDRAWEraseMode:
			EraseRoundRect( &box, ovalSz.h, ovalSz.v );
			break;
		
		case kXDRAWInvertMode:
			InvertRoundRect( &box, ovalSz.h, ovalSz.v );
			break;
	}
	
	return TRUE;
}


Boolean	DoArcOpcode( char* string, short *x, unsigned long flags )
{
	Rect		box;
	Point		ovalSz;
	long		md;
	
	// top left:
	if( !DoFetchNextItemPoint( ',', string, x, &topLeft(box) ) )
	{
		ReturnString( "Error: Item 2 to 3 of 'arc' opcode must be topLeft coordinate.", REPLACE );
		return FALSE;
	}
	
	// bottom right:
	if( !DoFetchNextItemPoint( ',', string, x, &botRight(box) ) )
	{
		ReturnString( "Error: Item 4 to 5 of 'arc' opcode must be botRight coordinate.", REPLACE );
		return FALSE;
	}
	
	md = DoFetchNextItemMode( ',', string, x );
	if( md == kXDRAWUnrecognizedMode )
	{
		ReturnString( "Error: Unrecognized mode, must be one of FRAME, FILL (PAINT), INVERT or ERASE.", REPLACE );
		return FALSE;
	}
	
	if( !DoFetchNextItemPoint( ',', string, x, &ovalSz ) )
	{
		ReturnString( "Error: Item 7 and 8 of 'arc' opcode must be start and arc angles.", REPLACE );
		return FALSE;
	}
	
	switch( md )
	{
		case kXDRAWFillMode:
			PaintArc( &box, ovalSz.h, ovalSz.v );
			break;
		
		case kXDRAWFrameMode:
			FrameArc( &box, ovalSz.h, ovalSz.v );
			break;
		
		case kXDRAWEraseMode:
			EraseArc( &box, ovalSz.h, ovalSz.v );
			break;
		
		case kXDRAWInvertMode:
			InvertArc( &box, ovalSz.h, ovalSz.v );
			break;
	}
	
	return TRUE;
}


Boolean	DoStringOpcode( char* string, short *x, unsigned long flags )
{
	Point		pos;
	short		vFont;
	long		vStyle = normal,
				vSize;
	Str255		vText,
				str;
	
	vText[0] = 0;
	
	// top left:
	if( !DoFetchNextItemPoint( ',', string, x, &pos ) )
	{
		ReturnString( "Error: Item 2 to 3 of 'string' opcode must be coordinate.", REPLACE );
		return FALSE;
	}
	
	// Font:
	DoFetchNextItem( ',', string, x, str );
	GetFNum( str, &vFont );
	
	// Style:
	DoFetchNextItem( ',', string, x, str );
	LowerText( ((char*)str) +1, str[0] );
	if( DoIsSubString( "\pbold", str ) )
		vStyle |= bold;
	if( DoIsSubString( "\pitalic", str ) )
		vStyle |= italic;
	if( DoIsSubString( "\punderline", str ) )
		vStyle |= underline;
	if( DoIsSubString( "\poutline", str ) )
		vStyle |= outline;
	if( DoIsSubString( "\pshadow", str ) )
		vStyle |= shadow;
	if( DoIsSubString( "\pcondense", str ) )
		vStyle |= condense;
	if( DoIsSubString( "\pextend", str ) )
		vStyle |= extend;
	
	// Size:
	if( !DoFetchNextItemNum( ',', string, x, &vSize ) )
	{
		ReturnString( "Error: Item 6 of 'string' opcode must be text size.", REPLACE );
		return FALSE;
	}
	
	// Text:
		// Here we copy over the rest of the line, allowing commas etc.
	while( string[*x] != '\n'
			&& string[*x] != '\r'
			&& string[*x] != 0 )
	{
		vText[0] ++;
		vText[vText[0]] = string[(*x)++];	// Counter is increased!
		
		if( vText[0] == (sizeof(vText) -1) )
		{
			ReturnString( "Error: 'string' opcode strings can't be longer than ", REPLACE );
			ReturnLong( sizeof(vText) -1, APPEND );
			ReturnString( " characters.", APPEND );
			return FALSE;
		}
	}
	
	MoveTo( pos.h, pos.v );
	TextFont( vFont );
	TextFace( vStyle );
	TextSize( vSize );
	DrawString( vText );
	
	return TRUE;
}


Boolean	DoTextOpcode( char* string, short *x, unsigned long flags )
{
	Rect		box;
	short		vFont,
				vAlignment;
	long		vStyle = normal,
				vSize;
	Str255		vText,
				str;
	
	vText[0] = 0;
	
	// top left:
	if( !DoFetchNextItemPoint( ',', string, x, &topLeft(box) ) )
	{
		ReturnString( "Error: Item 2 to 3 of 'text' opcode must be coordinate.", REPLACE );
		return FALSE;
	}
	
	// bot right:
	if( !DoFetchNextItemPoint( ',', string, x, &botRight(box) ) )
	{
		ReturnString( "Error: Item 4 to 5 of 'text' opcode must be coordinate.", REPLACE );
		return FALSE;
	}
	
	// Font:
	DoFetchNextItem( ',', string, x, str );
	GetFNum( str, &vFont );
	
	// Style:
	DoFetchNextItem( ',', string, x, str );
	LowerText( ((char*)str) +1, str[0] );
	if( DoIsSubString( "\pbold", str ) )
		vStyle |= bold;
	if( DoIsSubString( "\pitalic", str ) )
		vStyle |= italic;
	if( DoIsSubString( "\punderline", str ) )
		vStyle |= underline;
	if( DoIsSubString( "\poutline", str ) )
		vStyle |= outline;
	if( DoIsSubString( "\pshadow", str ) )
		vStyle |= shadow;
	if( DoIsSubString( "\pcondense", str ) )
		vStyle |= condense;
	if( DoIsSubString( "\pextend", str ) )
		vStyle |= extend;
	
	// Size:
	if( !DoFetchNextItemNum( ',', string, x, &vSize ) )
	{
		ReturnString( "Error: Item 7 of 'text' opcode must be text size.", REPLACE );
		return FALSE;
	}
	
	// Align:
	DoFetchNextItem( ',', string, x, str );
	if( str[0] == 0 )
	{
		ReturnString( "Error: Item 8 of 'text' opcode must be text align.", REPLACE );
		return FALSE;
	}
	
	if( EqualString( str, "\pleft", false, true ) )
		vAlignment = teForceLeft;
	else if( EqualString( str, "\pright", false, true ) )
		vAlignment = teJustRight;
	else if( EqualString( str, "\pcenter", false, true ) )
		vAlignment = teJustCenter;
	else
		vAlignment = teFlushDefault;
	
	// Text:
		// Here we copy over the rest of the line, allowing commas etc.
	while( string[*x] != '\n'
			&& string[*x] != '\r'
			&& string[*x] != 0 )
	{
		vText[0] ++;
		vText[vText[0]] = string[(*x)++];	// Counter is increased!
		
		if( vText[0] == (sizeof(vText) -1) )
		{
			ReturnString( "Error: 'text' opcode strings can't be longer than ", REPLACE );
			ReturnLong( sizeof(vText) -1, APPEND );
			ReturnString( " characters.", APPEND );
			return FALSE;
		}
	}
	
	TextFont( vFont );
	TextFace( vStyle );
	TextSize( vSize );
	TETextBox( vText +1, vText[0], &box, vAlignment );
	
	return TRUE;
}


Boolean	DoPictOpcode( char* string, short *x, unsigned long flags )
{
	Rect		box;
	long		resID;
	PicHandle	pic;
	QDProcsPtr	oldProcs;
	GrafPtr		vPort;
	Str255		resName;
	short		i;
	Boolean		isNum = true;
	
	// Icon ID:
	DoFetchNextItem( ',', string, x, resName );
	if( resName[0] == 0 )
	{
		ReturnString( "Error: Item 2 of 'picture' opcode must be PICT resource ID or name.", REPLACE );
		return FALSE;
	}
	
	// Is a number?
	for( i = 1; i <= resName[0] && isNum; i++ )
		isNum = (resName[i] >= '0' && resName[i] <= '9') || (i == 1 && resName[i] == '-');
	
	if( isNum )
	{
		StringToNum( resName, &resID );
		
		// Now get the PICT:
		pic = GetPicture( resID );
		if( pic == NULL || ResError() != noErr )
		{
			ReturnString( "Error: Couldn't find PICT resource with this ID (", REPLACE );
			if( ResError() != noErr )
				ReturnLong( ResError(), APPEND );
			else 
				ReturnLong( MemError(), APPEND );
			ReturnString( ").", APPEND );
			return FALSE;
		}
	}
	else
	{
		pic = (PicHandle) GetNamedResource( 'PICT', resName );
		if( pic == NULL || ResError() != noErr )
		{
			ReturnString( "Error: Couldn't find PICT resource of this name (", REPLACE );
			if( ResError() != noErr )
				ReturnLong( ResError(), APPEND );
			else 
				ReturnLong( MemError(), APPEND );
			ReturnString( ").", APPEND );
			return FALSE;
		}
	}
	
	
	// top left:
	if( !DoFetchNextItemPoint( ',', string, x, &topLeft(box) ) )
	{
		ReturnString( "Error: Item 3 to 4 of 'picture' opcode must be topLeft coordinate.", REPLACE );
		ReleaseResource( (Handle) pic );
		return FALSE;
	}
	
	// bottom right:
	if( !DoFetchNextItemPoint( ',', string, x, &botRight(box) ) )
	{
		// None specified? Default to PICT's best size:
		box.bottom = box.top +(**pic).picFrame.bottom -(**pic).picFrame.top;
		box.right = box.left +(**pic).picFrame.right -(**pic).picFrame.left;
	}
	
	/* Make sure StdProcs points at the OS's defaults:
		We need to do this as AddColor installs its own which
		turn Bitmaps in our PICT B/W. */
	GetPort( &vPort );
	oldProcs = vPort->grafProcs;	// Remember AddColor's.
	vPort->grafProcs = NULL;		// Force default.
	
	DrawPicture( pic, &box );
	
	vPort->grafProcs = oldProcs;	// Restore AddColor's.
	
	ReleaseResource( (Handle) pic );
	
	return TRUE;
}


Boolean	DoPictFileOpcode( char* string, short *x, unsigned long flags )
{
	Rect		box;
	Str255		fileName;
	short		vFileRef;
	long		vPicSize;
	PicHandle	pic = NULL;
	QDProcsPtr	oldProcs;
	GrafPtr		vPort;
	OSErr		vError;
	
	// File path:
	DoFetchNextItem( ',', string, x, fileName );
	if( fileName[0] == 0 )
	{
		ReturnString( "Error: Item 2 of 'pictFile' opcode must be PICT file name.", REPLACE );
		return FALSE;
	}
	
	// Open PICT file:
	vError = FSOpen( fileName, 0, &vFileRef );
	if( vError != noErr )
	{
		switch( vError )
		{
			case fnfErr:
				ReturnString( "Error: File couldn't be found.", REPLACE );
				break;
			
			default:
				ReturnString( "Error: Couldn't open/create file (", REPLACE );
				ReturnLong( vError, APPEND );
				ReturnString( ").", APPEND );
				break;
		}
		return FALSE;
	}
	
	// Move past file header:
	vError = SetFPos( vFileRef, fsFromStart, 512 );
	if( vError != noErr )
	{
		FSClose( vFileRef );
		ReturnString( "Error: Couldn't access read position (", REPLACE );
		ReturnLong( vError, APPEND );
		ReturnString( ").", APPEND );
		return FALSE;
	}
	
	vError = GetEOF( vFileRef, &vPicSize );
	if( vError != noErr )
	{
		FSClose( vFileRef );
		ReturnString( "Error: Couldn't calculate PICT size (", REPLACE );
		ReturnLong( vError, APPEND );
		ReturnString( ").", APPEND );
		return FALSE;
	}
	
	vPicSize -= 512;	// Don't count header.
	
	// Make new Handle for PICT data:
	pic = (PicHandle) NewHandle( vPicSize );
	if( pic == NULL )
	{
		FSClose( vFileRef );
		ReturnString( "Error: Not Enough memory to read picture (", REPLACE );
		ReturnLong( MemError(), APPEND );
		ReturnString( ").", APPEND );
		return FALSE;
	}
	
	// Read data into Handle:
	HLock( (Handle) pic );
	vError = FSRead( vFileRef, &vPicSize, (*pic) );
	HUnlock( (Handle) pic );
	if( vError != noErr )
	{
		DisposeHandle( (Handle) pic );
		FSClose( vFileRef );
		ReturnString( "Error: Couldn't read picture (", REPLACE );
		ReturnLong( vError, APPEND );
		ReturnString( ").", APPEND );
		return FALSE;
	}
	
	// top left:
	if( !DoFetchNextItemPoint( ',', string, x, &topLeft(box) ) )
	{
		FSClose( vFileRef );
		KillPicture( pic );
		ReturnString( "Error: Item 3 to 4 of 'picture' opcode must be topLeft coordinate.", REPLACE );
		ReleaseResource( (Handle) pic );
		return FALSE;
	}
	
	// bottom right:
	if( !DoFetchNextItemPoint( ',', string, x, &botRight(box) ) )
	{
		// None specified? Default to PICT's best size:
		box.bottom = box.top +(**pic).picFrame.bottom -(**pic).picFrame.top;
		box.right = box.left +(**pic).picFrame.right -(**pic).picFrame.left;
	}
	
	/* Make sure StdProcs points at the OS's defaults:
		We need to do this as AddColor installs its own which
		turn Bitmaps in our PICT B/W. */
	GetPort( &vPort );
	oldProcs = vPort->grafProcs;	// Remember AddColor's.
	vPort->grafProcs = NULL;		// Force default.
	
	DrawPicture( pic, &box );
	
	vPort->grafProcs = oldProcs;	// Restore AddColor's.
	
	KillPicture( pic );
	FSClose( vFileRef );
	
	return TRUE;
}


Boolean	DoIconOpcode( char* string, short *x, unsigned long flags )
{
	Rect		box;
	long		resID;
	CIconHandle	pic = NULL;
	Handle		bwIcon;
	
	// Icon ID:
	if( !DoFetchNextItemNum( ',', string, x, &resID ) )
	{
		ReturnString( "Error: Item 2 of 'icon' opcode must be PICT resource ID.", REPLACE );
		return FALSE;
	}
	
	// Now get the icon:
	pic = GetCIcon( resID );
	if( pic == NULL )
	{
		bwIcon = GetIcon( resID );
		
		if( bwIcon == NULL )
		{
			ReturnString( "Error: Couldn't find specified ICON or cicn resource (", REPLACE );
			if( ResError() != noErr )
				ReturnLong( ResError(), APPEND );
			else 
				ReturnLong( MemError(), APPEND );
			ReturnString( ").", APPEND );
			return FALSE;
		}
	}
	
	// top left:
	if( !DoFetchNextItemPoint( ',', string, x, &topLeft(box) ) )
	{
		ReturnString( "Error: Item 3 to 4 of 'icon' opcode must be topLeft coordinate.", REPLACE );
		if( pic )
			DisposeCIcon( pic );
		else
			ReleaseResource( bwIcon );
		return FALSE;
	}
	
	// bottom right:
	if( !DoFetchNextItemPoint( ',', string, x, &botRight(box) ) )
	{
		// None specified? Default to cicn's best size:
		if( pic )
		{
			box.bottom = box.top +(**pic).iconPMap.bounds.bottom -(**pic).iconPMap.bounds.top;
			box.right = box.left +(**pic).iconPMap.bounds.right -(**pic).iconPMap.bounds.left;
		}
		else	// ICONs are fixed-size:
		{
			box.bottom = box.top +32;
			box.right = box.left +32;
		}
	}
	
	if( pic )
	{
		PlotCIcon( &box, pic );
		DisposeCIcon( pic );
	}
	else
	{
		QDProcsPtr	oldProcs;
		GrafPtr		vPort;
		
		// For the reasons of the grafProcs stuff below, see "picture" opcode.
		
		GetPort( &vPort );
		oldProcs = vPort->grafProcs;	// Remember AddColor's.
		vPort->grafProcs = NULL;		// Force default.
	
		PlotIcon( &box, bwIcon );
		ReleaseResource( bwIcon );
	
		vPort->grafProcs = oldProcs;	// Restore AddColor's.
	}
	
	return TRUE;
}


Boolean	DoPolyOpcode( char* string, short *x, unsigned long flags )
{
	long		md;
	Str255		onePt;
	Boolean		first = TRUE;
	PolyHandle	vPoly;
	
	onePt[0] = 0;
	
	md = DoFetchNextItemMode( ',', string, x );
	
	if( md == kXDRAWUnrecognizedMode )
	{
		ReturnString( "Error: Unrecognized mode, must be one of FRAME, FILL (PAINT), INVERT or ERASE.", REPLACE );
		return FALSE;
	}
	
	vPoly = OpenPoly();
	if( vPoly == NULL )
	{
		ReturnString( "Error: Not Enough memory to record Polygon.", REPLACE );
		return FALSE;
	}
	
	while( true )
	{
		DoFetchNextItem( ',', string, x, onePt );
		
		if( onePt[0] != 0 )
		{
			Str255		str;
			long		cx,
						cy;
			short		lx = 1;
			
			// Get first whitespace-delimited word:
			str[0] = 0;
			while( lx <= onePt[0] )
			{
				if( onePt[lx] != ' ' && onePt[lx] != '\t' )	// Don't read leading spaces:
				{
					str[0] ++;
					str[str[0]] = onePt[lx];
				}
				else if( str[0] != 0 )	// Is whitespace and we have already read data --> Delimiter! Exit!
					break;
				
				lx++;
			}
			StringToNum( str, &cx );
			
			// Copy second word:
			str[0] = 0;
			while( lx <= onePt[0] )
			{
				if( onePt[lx] != ' ' && onePt[lx] != '\t' )	// Don't read leading whitespace.
				{
					str[0] ++;
					str[str[0]] = onePt[lx];
				}
				
				lx++;
			}
			StringToNum( str, &cy );
			
			if( first )
			{
				first = FALSE;
				MoveTo( cx, cy );
			}
			else
				LineTo( cx, cy );
		}
		else
			break;
	}
	
	ClosePoly();
	
	switch( md )
	{
		case kXDRAWFrameMode:
			FramePoly( vPoly );
			break;
		
		case kXDRAWFillMode:
			PaintPoly( vPoly );
			break;
		
		case kXDRAWEraseMode:
			ErasePoly( vPoly );
			break;
		
		case kXDRAWInvertMode:
			InvertPoly( vPoly );
			break;
	}
	
	KillPoly( vPoly );
	
	return TRUE;
}


Boolean	DoPenSizeOpcode( char* string, short *x, unsigned long flags )
{
	Point	pSize;
			
	// top left:
	if( !DoFetchNextItemPoint( ',', string, x, &pSize ) )
	{
		ReturnString( "Error: Item 2 to 3 of 'pen size' opcode must be width & height for pen.", REPLACE );
		return FALSE;
	}
	
	PenSize( pSize.h, pSize.v );
	
	return TRUE;
}


Boolean	DoPenModeOpcode( char* string, short *x, unsigned long flags )
{
	Str255		str;
			
	// top left:
	DoFetchNextItem( ',', string, x, str );
	
	if( str[0] == 0 || EqualString( str, "\pcopy", false, true ) )
		PenMode( patCopy );
	else if( EqualString( str, "\por", false, true ) )
		PenMode( patOr );
	else if( EqualString( str, "\pxOr", false, true ) )
		PenMode( patXor );
	else if( EqualString( str, "\pbic", false, true ) )
		PenMode( patBic );
	else if( EqualString( str, "\pnotCopy", false, true ) )
		PenMode( notPatCopy );
	else if( EqualString( str, "\pnotOr", false, true ) )
		PenMode( notPatOr );
	else if( EqualString( str, "\pnotXor", false, true ) )
		PenMode( notPatXor );
	else if( EqualString( str, "\pnotBic", false, true ) )
		PenMode( notPatBic );
	else if( EqualString( str, "\philite", false, true ) )
		PenMode( hilite );
	else if( EqualString( str, "\pblend", false, true ) )
		PenMode( blend );
	else if( EqualString( str, "\paddPin", false, true ) )
		PenMode( addPin );
	else if( EqualString( str, "\paddOver", false, true ) )
		PenMode( addOver );
	else if( EqualString( str, "\psubPin", false, true ) )
		PenMode( subPin );
	else if( EqualString( str, "\paddMax", false, true )
			|| EqualString( str, "\padMax", false, true ) )
		PenMode( addMax );
	else if( EqualString( str, "\psubOver", false, true ) )
		PenMode( subOver );
	else if( EqualString( str, "\padMin", false, true ) )
		PenMode( adMin );
	else if( EqualString( str, "\ptransparent", false, true ) )
		PenMode( transparent );
	
	return TRUE;
}


Boolean	DoFieldOpcode( char* string, short *x, unsigned long flags )
{
	Rect			box;
	Str255			vState;
	Boolean			state = true;
	
	// top left:
	if( !DoFetchNextItemPoint( ',', string, x, &topLeft(box) ) )
	{
		ReturnString( "Error: Item 2 to 3 of 'button' opcode must be topLeft coordinate.", REPLACE );
		return FALSE;
	}
	
	// bottom right:
	if( !DoFetchNextItemPoint( ',', string, x, &botRight(box) ) )
	{
		ReturnString( "Error: Item 4 to 5 of 'button' opcode must be botRight coordinate.", REPLACE );
		return FALSE;
	}
	
	DoFetchNextItem( ',', string, x, vState );
	state = EqualString( vState, "\ptrue", false, true );
	
  #if GENERATINGCFM
	if( (long) DrawThemeEditTextFrame != kUnresolvedCFragSymbolAddress )
	{
		InsetRect( &box, 1, 1 );	// DrawThemeEditTextFrame() draws outside our rect.
		DrawThemeEditTextFrame( &box, !state );
	}
	else
  #endif
	{
		RGBColor		black = { 0, 0, 0 },
						white = { 65535, 65535, 65535 },
						gray = { 32767,32767,32767 },
						oldFore;
		PenState		oldPen;
		
		GetPenState( &oldPen );
		PenSize( 1, 1 );
		PenMode( patCopy );
		GetForeColor( &oldFore );
		if( state )
			RGBForeColor( &black );
		else
			RGBForeColor( &gray );
		FrameRect( &box );
		InsetRect( &box, -1, -1 );
		box.right--;
		box.bottom--;
		RGBForeColor( &white );
		MoveTo( box.left, box.bottom );
		LineTo( box.right, box.bottom );
		LineTo( box.right, box.top );
		RGBForeColor( &gray );
		LineTo( box.left, box.top );
		LineTo( box.left, box.bottom );
		RGBForeColor( &oldFore );
		SetPenState( &oldPen );
	}
	
	return TRUE;
}


Boolean	DoButtonOpcode( char* string, short *x, unsigned long flags )
{
	Rect			box;
	Str255			vTitle,
					vState;
	Boolean			state = true;
	
	// top left:
	if( !DoFetchNextItemPoint( ',', string, x, &topLeft(box) ) )
	{
		ReturnString( "Error: Item 2 to 3 of 'button' opcode must be topLeft coordinate.", REPLACE );
		return FALSE;
	}
	
	// bottom right:
	if( !DoFetchNextItemPoint( ',', string, x, &botRight(box) ) )
	{
		ReturnString( "Error: Item 4 to 5 of 'button' opcode must be botRight coordinate.", REPLACE );
		return FALSE;
	}
	
	DoFetchNextItem( ',', string, x, vTitle );
	DoFetchNextItem( ',', string, x, vState );
	state = EqualString( vState, "\ptrue", false, true );
	
  #if GENERATINGCFM
	if( (long) DrawControlInCurrentPort != kUnresolvedCFragSymbolAddress )
	{
		ControlHandle	vButton;
		
		vButton = NewControl( gCardWindow, &box, vTitle, false, 0, 0, 1,
							 kControlPushButtonProc, 0 );
		if( !vButton || MemError() != noErr )
		{
			ReturnString( "Error: Couldn't create control.", REPLACE );
			return FALSE;
		}
		
		HiliteControl( vButton, state );
		SetControlVisibility( vButton, true, false );	// Now show ctrl.
		DrawControlInCurrentPort( vButton );
		
		SetControlVisibility( vButton, false, false );	// Hide it or DisposeControl causes an update.
		DisposeControl( vButton );
	}
	else
  #endif
	{
		Rect			tempRect;
		FontInfo		MyFontData;
		short			theNUM,
						rectHeight;
		RGBColor		tinges[8];
		
		// Initialize button color table:
		tinges[0].red = tinges[0].green = tinges[0].blue = state ? 16384 : 39152;
		tinges[7].red = tinges[7].green = tinges[7].blue = state ? 32768 : 65535;
		
		// Calc intermediate colours (store in opposite order if pressed)
		mixColor(&tinges[7], &tinges[0], 0xd, &tinges[ state ? 6 : 1 ]);
		mixColor(&tinges[7], &tinges[0], 0xb, &tinges[ state ? 5 : 2 ]);
		mixColor(&tinges[7], &tinges[0], 0x9, &tinges[ state ? 4 : 3 ]);
		mixColor(&tinges[7], &tinges[0], 0x7, &tinges[ state ? 3 : 4 ]);
		mixColor(&tinges[7], &tinges[0], 0x5, &tinges[ state ? 2 : 5 ]);
		mixColor(&tinges[7], &tinges[0], 0x3, &tinges[ state ? 1 : 6 ]);
		
		// Paint button's content area:
		PenMode( patCopy );
		RGBForeColor( &tinges[3] );
		PaintRoundRect( &box, 8, 8 );
		
		// Draw 3D effect:
		if( true )
		{
			tempRect = box;
			InsetRect( &tempRect, 1, 1 );
			tempRect.left += 1;
			tempRect.top += 1;
			RGBForeColor( &tinges[1] );
			PaintRoundRect( &tempRect, 6, 6 );
			
			tempRect = box;
			InsetRect( &tempRect, 1, 1 );
			tempRect.right -= 1;
			tempRect.bottom -= 1;
			RGBForeColor( &tinges[6] );
			PaintRoundRect( &tempRect, 6, 6 );
			
			tempRect = box;
			InsetRect( &tempRect, 2, 2 );
			tempRect.left += 1;
			tempRect.top += 1;
			RGBForeColor( &tinges[2] );
			PaintRoundRect( &tempRect, 4, 4 );
			
			tempRect = box;
			InsetRect( &tempRect, 2, 2 );
			tempRect.right -= 1;
			tempRect.bottom -= 1;
			RGBForeColor( &tinges[5] );
			PaintRoundRect( &tempRect, 4, 4 );
			
			tempRect = box;
			InsetRect( &tempRect, 3, 3 );
			RGBForeColor( &tinges[4] );
			PaintRoundRect( &tempRect, 3, 3 );
		}
		
		// Draw button border:
		ForeColor( blackColor );
		FrameRoundRect( &box, 8, 8 );
		
		// Calc button title position:
		GetFontInfo ( &MyFontData );
		theNUM = StringWidth( vTitle ) /2;
		rectHeight = (MyFontData.ascent + MyFontData.descent);
		
		// Draw title:
		MoveTo( box.left +((box.right -box.left) /2) -theNUM +1,
				box.top +((box.bottom -box.top) /2) +(rectHeight /2) -MyFontData.descent +1 );
		ForeColor( whiteColor );
		DrawString( vTitle );
		MoveTo( box.left +((box.right -box.left) /2) -theNUM,
				box.top +((box.bottom -box.top) /2) +(rectHeight /2) -MyFontData.descent );
		if( !state )
			ForeColor( blackColor );
		else
			ForeColor( whiteColor );
		DrawString( vTitle );
		
		ForeColor( blackColor );
	}
	
	return TRUE;
}


/* ————————————————————————————————————————————————————————————————————————————
	DoFieldTextOpcode:
		Handles the "field" and "field text" opcodes. This also installs a
		custom RectProc into the current port's QDProcs field to avoid the
		background behind the field text from being erased.
	
	REVISIONS:
		2001-03-24	UK	Created (OS X Released :-).
   ————————————————————————————————————————————————————————————————————————— */

pascal void		MyRgnProc( GrafVerb verb, const RgnHandle r )
{
	if( verb != kQDGrafVerbErase )	// Do nothing when requested to erase.
		StdRgn( verb, r );
}


Boolean	DoFieldTextOpcode( char* string, short *x, unsigned long flags )
{
	Str255		vLayerStr,
				vNameStr;
	TEHandle	vTECopy;
	long		vNum,
				vID;
	Rect		vTEBox;
	QDProcsPtr	oldProcs;
	QDProcs		myProcs;
	GrafPtr		vPort;
	
	DoFetchNextItem( ',', string, x, vLayerStr );
	if( !DoFetchNextItemNum( ',', string, x, &vID ) )
		vID = 0;
	if( DoFetchNextItemNum( ',', string, x, &vNum ) )
		vNum = 0;
	DoFetchNextItem( ',', string, x, vNameStr );
	vTECopy = GetFieldTE( paramPtr, EqualString( vLayerStr, "\pcard", false, true ),
							vID, vNum, vNameStr );
	vTEBox = (**vTECopy).destRect;
	GetPort( &vPort );
	oldProcs = vPort->grafProcs;	// Remember AddColor's.
	SetStdProcs( &myProcs );
	myProcs.rgnProc = NewQDRgnUPP( MyRgnProc );
	vPort->grafProcs = &myProcs;		// Use our custom procs.
	
	TEUpdate( &vTEBox, vTECopy );
	
	vPort->grafProcs = oldProcs;	// Restore AddColor's.
	DisposeRoutineDescriptor( myProcs.rgnProc );
	
	DisposeHandle( (Handle) vTECopy );	// Just a copy of the Handle, not its pointed-to fields.
	
	return true;
}


/* ————————————————————————————————————————————————————————————————————————————
	FitRectInRect:
		Proportionally scale one rect until it fits into another. Then it
		centers the resulting rect inside the enclosing one using
		CenterRectInRect().
	
	TAKES:
		destRect -		The rect to scale.
		boundsRect -	The rect destRect is supposed to fit into.
	
	GIVES:
		destRect -		The scaled rect.
	
	REVISIONS:
		1999-07-04	UK	Fixed bug that caused too little scaling w/ some rects.
		1999-05-27	UK	Created.
   ————————————————————————————————————————————————————————————————————————— */

void	FitRectInRect( Rect* destRect, Rect* boundsRect )
{
	short		overlayH,
				overlayV;
	float		scaleFactor;
	Rect		newRect = (*destRect);
	
	OffsetRect( &newRect, -newRect.left, -newRect.top );	// Offset to 0,0.
	overlayH = boundsRect->right -boundsRect->left;
	overlayV = boundsRect->bottom -boundsRect->top;
	
	if( newRect.right > newRect.bottom )	// Horz is bigger:
	{
		scaleFactor = ((float)newRect.bottom) / ((float)newRect.right);
		newRect.right = overlayH;
		newRect.bottom = (long) ( ((float)overlayH) * scaleFactor );
		
		if( newRect.bottom > boundsRect->bottom -boundsRect->top )
		{
			scaleFactor = ((float)newRect.right) / ((float)newRect.bottom);
			newRect.bottom = overlayV;
			newRect.right = (long) ( ((float)overlayV) * scaleFactor );
		}
	}
	else	// vert is bigger or square:
	{
		scaleFactor = ((float)newRect.right) / ((float)newRect.bottom);
		newRect.bottom = overlayV;
		newRect.right = (long) ( ((float)overlayV) * scaleFactor );
		
		if( newRect.right > boundsRect->right -boundsRect->left )
		{
			scaleFactor = ((float)newRect.bottom) / ((float)newRect.right);
			newRect.right = overlayH;
			newRect.bottom = (long) ( ((float)overlayH) * scaleFactor );
		}
	}
	
	CenterRectInRect( &newRect, boundsRect );
	
	(*destRect) = newRect;
}


/* ————————————————————————————————————————————————————————————————————————————
	CenterRectInRect:
		Center one rect inside another.
	
	TAKES:
		destRect -		The rect to center.
		boundsRect -	The rect destRect is to be centered in.
	
	GIVES:
		destRect -		The centered rect.
	
	REVISIONS:
		2002-02-06	UK		Added support for alpha channels.
		1999-05-27	UK		Extracted from DrawPICT().
   ————————————————————————————————————————————————————————————————————————— */

void	CenterRectInRect( Rect* destRect, Rect* boundsRect )
{
	short		hPos,
				vPos;
	
	OffsetRect( destRect, -destRect->left, -destRect->top );		// Offset to 0,0.
	
	hPos = (boundsRect->right -boundsRect->left) -destRect->right;
	vPos = (boundsRect->bottom -boundsRect->top) -destRect->bottom;
	OffsetRect( destRect, boundsRect->left +(hPos / 2), boundsRect->top +(vPos / 2) );
}


#if powerc
Boolean	DoImageFileOpcode( char* string, short *x, unsigned long flags )
{
	Rect					box,
							boundsRect;
	Str255					fileName,
							drawMode;
	OSErr					vError;
	FSSpec					vFileSpec;
	GWorldPtr				vOldGW;
	GDHandle				vOldDevice;
	GraphicsImportComponent	vGI;
	ImageDescriptionHandle		imgDesc;
	
	if( ((long)GetGraphicsImporterForFile) == NULL )
	{
		ReturnString( "Error: Graphics importers (needed for the 'image file' opcode) are not supported on this Mac.", REPLACE );
		return FALSE;
	}
	
	// File path:
	DoFetchNextItem( ',', string, x, fileName );
	if( fileName[0] == 0 )
	{
		ReturnString( "Error: Item 2 of 'image file' opcode must be PICT file name.", REPLACE );
		return FALSE;
	}
	
	// Make file spec:
	vError = FSMakeFSSpec( 0, 0, fileName, &vFileSpec );
	if( vError != noErr )
	{
		switch( vError )
		{
			case fnfErr:
				ReturnString( "Error: File couldn't be found.", REPLACE );
				break;
			
			default:
				ReturnString( "Error: Couldn't open/create file (", REPLACE );
				ReturnLong( vError, APPEND );
				ReturnString( ").", APPEND );
				break;
		}
		return FALSE;
	}
	
	// Do GI stuff:	
	GetGWorld( &vOldGW, &vOldDevice );
	GetGraphicsImporterForFile( &vFileSpec, &vGI );	// Make new GI component to import
	GraphicsImportSetGWorld( vGI, vOldGW, vOldDevice );	// Tell GI to draw into our window.
	
	// Get additional params:
	// top left:
	if( !DoFetchNextItemPoint( ',', string, x, &topLeft(box) ) )
	{
		ReturnString( "Error: Item 3 to 4 of 'image file' opcode must be topLeft coordinate.", REPLACE );
		return FALSE;
	}
	
	
	// None specified? Default to image's best size:
	GraphicsImportGetNaturalBounds( vGI, &boundsRect );	// BoundsRect is always at 0,0.
	
	// bottom right:
	if( !DoFetchNextItemPoint( ',', string, x, &botRight(box) ) )
	{
		box.bottom = box.top +boundsRect.bottom;
		box.right = box.left +boundsRect.right;
	}
	else
	{
		// File path:
		DoFetchNextItem( ',', string, x, drawMode );
		if( EqualString( drawMode, "\pnaturalSize", false, true ) )	// Use natural size at coords passed in:
		{
			box.bottom = box.top +boundsRect.bottom;
			box.right = box.left +boundsRect.right;
		}
		else if( EqualString( drawMode, "\pscaleToFit", false, true ) )	// Use natural size relationship but scale to fit rect:
		{
			FitRectInRect( &boundsRect, &box );
			box = boundsRect;
		}
		// Else just draw to fit in rect passed.
	}
	
	// Turn on alpha channels:
	vError = GraphicsImportGetImageDescription( vGI, &imgDesc );
	if( vError == noErr )
	{
		switch( (**imgDesc).cType )
		{
			case kPNGCodecType:
			case kTIFFCodecType:
			case kTargaCodecType:
				vError = GraphicsImportSetGraphicsMode( vGI, graphicsModeStraightAlpha, NULL );	// Turn on alpha!
				break;
		}
		
		DisposeHandle( (Handle) imgDesc );
	}
	
	// Actually draw:
	GraphicsImportSetBoundsRect( vGI, &box );
	GraphicsImportDraw( vGI );	// Actually have the component draw.
	CloseComponent( vGI );	// Get rid of GI.
	
	SetGWorld( vOldGW, vOldDevice );
	
	return TRUE;
}
#endif /*powerc*/


/* ————————————————————————————————————————————————————————————————————————————————
	DoProcessOpcode:
		Draw the proper shape for the opcode passed or perform the action
		associated with this shape.
		
	TAKES:
		string -	The opcode to draw. This is a comma-delimited list where the
					first item identifies the opcode and subsequent items are
					parameters. THIS IS A ZERO-TERMINATED C STRING.
		flags -		A bit field with flags to turn off certain opcodes. used for
					Region and Picture recording.
	
	GIVES:
		Boolean -	TRUE if successful, FALSE on error.
	
	REVISIONS:
		1999-07-04	UK	Created.
   ————————————————————————————————————————————————————————————————————————————— */

Boolean	DoProcessOpcode( char* string, unsigned long flags )
{
	Str255		str;
	short		x = 0;
	
	// Scan our opcode:
	while( string[x] != 0 )
	{
		// Get opcode name:
		DoFetchNextItem( ',', string, &x, str );
		
		// Now compare opcode and process it:
		if( EqualString( str, "\pline", false, true ) )
		{
			if( !DoLineOpcode( string, &x, flags ) )
				return FALSE;
		}
		else if( EqualString( str, "\prect", false, true ) )
		{
			if( !DoRectOpcode( string, &x, flags ) )
				return FALSE;
		}
		else if( EqualString( str, "\poval", false, true ) )
		{
			if( !DoOvalOpcode( string, &x, flags ) )
				return FALSE;
		}
		else if( EqualString( str, "\pround rect", false, true )
				|| EqualString( str, "\prRec", false, true ) )
		{
			if( !DoRoundRectOpcode( string, &x, flags ) )
				return FALSE;
		}
		else if( EqualString( str, "\parc", false, true ) )
		{
			if( !DoArcOpcode( string, &x, flags ) )
				return FALSE;
		}
		else if( EqualString( str, "\pstring", false, true )
				|| EqualString( str, "\ptStr", false, true ) )
		{
			if( !DoStringOpcode( string, &x, flags ) )
				return FALSE;
		}
		else if( EqualString( str, "\ptext", false, true ) )
		{
			if( !DoTextOpcode( string, &x, flags ) )
				return FALSE;
		}
		else if( EqualString( str, "\ppict", false, true )
			|| EqualString( str, "\ppicture", false, true ) )
		{
			if( !DoPictOpcode( string, &x, flags ) )
				return FALSE;
		}
		else if( EqualString( str, "\ppictFile", false, true ) )
		{
			if( !DoPictFileOpcode( string, &x, flags ) )
				return FALSE;
		}
		else if( EqualString( str, "\picon", false, true ) )
		{
			if( !DoIconOpcode( string, &x, flags ) )
				return FALSE;
		}
		else if( EqualString( str, "\ppoly", false, true )
				|| EqualString( str, "\ppolygon", false, true ) )
		{
			if( !DoPolyOpcode( string, &x, flags ) )
				return FALSE;
		}
		else if( EqualString( str, "\ppen size", false, true )
				|| EqualString( str, "\ppSiz", false, true ) )
		{
			if( !DoPenSizeOpcode( string, &x, flags ) )
				return FALSE;
		}
		else if( EqualString( str, "\ppen mode", false, true )
				|| EqualString( str, "\ppMod", false, true ) )
		{
			if( !DoPenModeOpcode( string, &x, flags ) )
				return FALSE;
		}
		else if( EqualString( str, "\ptext mode", false, true ) )
		{
			Str255		str;
			
			// top left:
			DoFetchNextItem( ',', string, &x, str );
			
			if( str[0] == 0 || EqualString( str, "\pcopy", false, true ) )
				TextMode( patCopy );
			else if( EqualString( str, "\por", false, true ) )
				TextMode( patOr );
			else if( EqualString( str, "\pxOr", false, true ) )
				TextMode( patXor );
			else if( EqualString( str, "\pbic", false, true ) )
				TextMode( patBic );
			else if( EqualString( str, "\pnotCopy", false, true ) )
				TextMode( notPatCopy );
			else if( EqualString( str, "\pnotOr", false, true ) )
				TextMode( notPatOr );
			else if( EqualString( str, "\pnotXor", false, true ) )
				TextMode( notPatXor );
			else if( EqualString( str, "\pnotBic", false, true ) )
				TextMode( notPatBic );
			else if( EqualString( str, "\philite", false, true ) )
				TextMode( hilite );
			else if( EqualString( str, "\pblend", false, true ) )
				TextMode( blend );
			else if( EqualString( str, "\paddPin", false, true ) )
				TextMode( addPin );
			else if( EqualString( str, "\paddOver", false, true ) )
				TextMode( addOver );
			else if( EqualString( str, "\psubPin", false, true ) )
				TextMode( subPin );
			else if( EqualString( str, "\paddMax", false, true )
					|| EqualString( str, "\padMax", false, true ) )
				TextMode( addMax );
			else if( EqualString( str, "\psubOver", false, true ) )
				TextMode( subOver );
			else if( EqualString( str, "\padMin", false, true ) )
				TextMode( adMin );
			else if( EqualString( str, "\ptransparent", false, true ) )
				TextMode( transparent );
			else if( EqualString( str, "\pdisabled", false, true )
					|| EqualString( str, "\pgrayishTextOr", false, true ) )
				TextMode( grayishTextOr );
		}
		else if( EqualString( str, "\pfore pattern", false, true )
				|| EqualString( str, "\ppen pattern", false, true )
				|| EqualString( str, "\pfPat", false, true ) )
		{
			long		patID,
						patIndex;
			PatHandle	thePat;
			Pattern		pat;
			
			// top left:
			if( !DoFetchNextItemNum( ',', string, &x, &patID ) )
			{
				ReturnString( "Error: Item 2 of 'pen pattern' opcode must be 'PAT ' or 'PAT#' resource ID.", REPLACE );
				return FALSE;
			}
			
			thePat = GetPattern( patID );
			if( !thePat )
			{
				if( !DoFetchNextItemNum( ',', string, &x, &patIndex ) )
				{
					ReturnString( "Error: Item 3 of 'pen pattern' opcode missing (index in 'PAT#') or 'PAT ' resource doesn't exist.", REPLACE );
					return FALSE;
				}
				else
				{
					GetIndPattern( &pat, patID, patIndex );
					PenPat( &pat );
				}
			}
			else
			{
				PenPat( (*thePat) );
				ReleaseResource( (Handle) thePat );
			}
		}
		else if( EqualString( str, "\pback pattern", false, true )
				|| EqualString( str, "\pbPat", false, true ) )
		{
			long		patID,
						patIndex;
			PatHandle	thePat;
			Pattern		pat;
			
			// top left:
			if( !DoFetchNextItemNum( ',', string, &x, &patID ) )
			{
				ReturnString( "Error: Item 2 of 'pen pattern' opcode must be 'PAT ' or 'PAT#' resource ID.", REPLACE );
				return FALSE;
			}
			
			thePat = GetPattern( patID );
			if( !thePat )
			{
				if( !DoFetchNextItemNum( ',', string, &x, &patIndex ) )
				{
					ReturnString( "Error: Item 3 of 'pen pattern' opcode missing (index in 'PAT#') or 'PAT ' resource doesn't exist.", REPLACE );
					return FALSE;
				}
				else
				{
					GetIndPattern( &pat, patID, patIndex );
					BackPat( &pat );
				}
			}
			else
			{
				BackPat( (*thePat) );
				ReleaseResource( (Handle) thePat );
			}
		}
		else if( EqualString( str, "\pfore color", false, true )
				|| EqualString( str, "\ppen color", false, true )
				|| EqualString( str, "\pfCol", false, true ) )
		{
			long			n;
			RGBColor		theCol;
			
			// Try constant:
			DoFetchNextItem( ',', string, &x, str );
			
			if( str[0] == 0 )
			{
				ReturnString( "Error: Item 2 of 'pen color' opcode must be color constant or red value.", REPLACE );
				return FALSE;
			}
			
			if( EqualString( str, "\pblack", false, true ) )
				ForeColor( blackColor );
			else if( EqualString( str, "\pwhite", false, true ) )
				ForeColor( whiteColor );
			else if( EqualString( str, "\pred", false, true ) )
				ForeColor( redColor );
			else if( EqualString( str, "\pgreen", false, true ) )
				ForeColor( greenColor );
			else if( EqualString( str, "\pblue", false, true ) )
				ForeColor( blueColor );
			else if( EqualString( str, "\pcyan", false, true ) )
				ForeColor( cyanColor );
			else if( EqualString( str, "\pmagenta", false, true ) )
				ForeColor( magentaColor );
			else if( EqualString( str, "\pyellow", false, true ) )
				ForeColor( yellowColor );
			else	// Must be RGB value:
			{
				StringToNum( str, &n );
				theCol.red = n;
				
				if( !DoFetchNextItemNum( ',', string, &x, &n ) )
				{
					ReturnString( "Error: Item 3 of 'pen color' opcode must be green value.", REPLACE );
					return FALSE;
				}
				theCol.green = n;
				
				if( !DoFetchNextItemNum( ',', string, &x, &n ) )
				{
					ReturnString( "Error: Item 4 of 'pen color' opcode must be blue value.", REPLACE );
					return FALSE;
				}
				theCol.blue = n;
				
				RGBForeColor( &theCol );
			}
		}
		else if( EqualString( str, "\pback color", false, true )
				|| EqualString( str, "\pbCol", false, true ) )
		{
			long			n;
			RGBColor		theCol;
			
			// Try constant:
			DoFetchNextItem( ',', string, &x, str );
			
			if( str[0] == 0 )
			{
				ReturnString( "Error: Item 2 of 'back color' opcode must be color constant or red value.", REPLACE );
				return FALSE;
			}
			
			if( EqualString( str, "\pblack", false, true ) )
				BackColor( blackColor );
			else if( EqualString( str, "\pwhite", false, true ) )
				BackColor( whiteColor );
			else if( EqualString( str, "\pred", false, true ) )
				BackColor( redColor );
			else if( EqualString( str, "\pgreen", false, true ) )
				BackColor( greenColor );
			else if( EqualString( str, "\pblue", false, true ) )
				BackColor( blueColor );
			else if( EqualString( str, "\pcyan", false, true ) )
				BackColor( cyanColor );
			else if( EqualString( str, "\pmagenta", false, true ) )
				BackColor( magentaColor );
			else if( EqualString( str, "\pyellow", false, true ) )
				BackColor( yellowColor );
			else	// Must be RGB value:
			{
				StringToNum( str, &n );
				theCol.red = n;
				
				if( !DoFetchNextItemNum( ',', string, &x, &n ) )
				{
					ReturnString( "Error: Item 3 of 'back color' opcode must be green value.", REPLACE );
					return FALSE;
				}
				theCol.green = n;
				
				if( !DoFetchNextItemNum( ',', string, &x, &n ) )
				{
					ReturnString( "Error: Item 4 of 'back color' opcode must be blue value.", REPLACE );
					return FALSE;
				}
				theCol.blue = n;
				
				RGBBackColor( &theCol );
			}
		}
		else if( EqualString( str, "\pport", false, true ) )
		{
			if( (flags & kXDRAWNoPortsFlag) == kXDRAWNoPortsFlag )
			{
				ReturnString( "Error: 'port' opcode not permitted in this mode.", REPLACE );
				return FALSE;
			}
			else
			{
				WindowPtr		vWindow = FrontWindow();
				Str255			vTitle;
				GrafPtr			vPort;
				
				DoFetchNextItem( ',', string, &x, str );
				
				if( str[0] == 0 )
				{
					ReturnString( "Error: Item 2 of 'port' opcode must be window name or ID.", REPLACE );
					return FALSE;
				}
				
				while( vWindow != NULL )
				{
					GetWTitle( vWindow, vTitle );
					
					if( EqualString( str, vTitle, false, true ) )
					{
						SetPort( vWindow );
						break;
					}
					
					vWindow = GetNextWindow( vWindow );
				}
				
				if( vWindow == NULL )	// None found. Is not window by name, may even be GWorld. Play safe.
				{
					StringToNum( str, (long*) &vPort );
					SetGWorld( (CGrafPtr) vPort, NULL );			// Danger! How find out whether port exists?
				}
			}
		}
		else if( EqualString( str, "\popenGWorld", false, true ) )
		{
			if( (flags & kXDRAWNoPortsFlag) == kXDRAWNoPortsFlag )
			{
				ReturnString( "Error: 'openGWorld' opcode not permitted in this mode.", REPLACE );
				return FALSE;
			}
			else
			{
				GWorldPtr		vPort;
				PixMapHandle	pm;
				
				DoFetchNextItem( ',', string, &x, str );
				
				if( str[0] == 0 )
				{
					ReturnString( "Error: Item 2 of 'openGWorld' opcode must be GWorld ID.", REPLACE );
					return FALSE;
				}
				
				StringToNum( str, (long*) &vPort );
				pm = GetGWorldPixMap( vPort );
				if( LockPixels(pm) )
					SetGWorld( vPort, NULL );
				else
				{
					ReturnString( "Error: Couldn't lock down GWorld in 'openGWorld'.", REPLACE );
					return FALSE;
				}
			}
		}
		else if( EqualString( str, "\pcloseGWorld", false, true ) )
		{
			if( (flags & kXDRAWNoPortsFlag) == kXDRAWNoPortsFlag )
			{
				ReturnString( "Error: 'closeGWorld' opcode not permitted in this mode.", REPLACE );
				return FALSE;
			}
			else
			{
				GWorldPtr		vPort;
				PixMapHandle	pm;
				
				DoFetchNextItem( ',', string, &x, str );
				
				if( str[0] == 0 )
				{
					ReturnString( "Error: Item 2 of 'closeGWorld' opcode must be GWorld ID.", REPLACE );
					return FALSE;
				}
				
				StringToNum( str, (long*) &vPort );
				pm = GetGWorldPixMap( vPort );
				UnlockPixels(pm);
				SetGWorld( (CGrafPtr) FrontWindow(), NULL );
			}
		}
		else if( EqualString( str, "\pclip", false, true ) )
		{
			Rect		box;
			
			// top left:
			if( !DoFetchNextItemPoint( ',', string, &x, &topLeft(box) ) )
			{
				ReturnString( "Error: Item 2 to 3 of 'clip' opcode must be topLeft coordinate.", REPLACE );
				return FALSE;
			}
			
			// bottom right:
			if( !DoFetchNextItemPoint( ',', string, &x, &botRight(box) ) )
			{
				ReturnString( "Error: Item 4 to 5 of 'clip' opcode must be botRight coordinate.", REPLACE );
				return FALSE;
			}
			
			ClipRect( &box );
		}
		else if( EqualString( str, "\pbutton", false, true ) )
		{
			if( !DoButtonOpcode( string, &x, flags ) )
				return FALSE;
		}
		else if( EqualString( str, "\peditfield", false, true ) )
		{
			if( !DoFieldOpcode( string, &x, flags ) )
				return FALSE;
		}
		else if( EqualString( str, "\pfield", false, true )
				|| EqualString( str, "\pfield text", false, true ) )
		{
			if( !DoFieldTextOpcode( string, &x, flags ) )
				return FALSE;
		}
		else if( EqualString( str, "\pinvalidate rect", false, true )
				|| EqualString( str, "\pinvR", false, true ) )
		{
			if( (flags & kXDRAWNoPortsFlag) == kXDRAWNoPortsFlag )
			{
				ReturnString( "Error: 'invalidate rect' opcode not permitted in this mode.", REPLACE );
				return FALSE;
			}
			else
			{
				Rect		box;
			
				// top left:
				if( !DoFetchNextItemPoint( ',', string, &x, &topLeft(box) ) )
				{
					ReturnString( "Error: Item 2 to 3 of 'invalidate rect' opcode must be topLeft coordinate.", REPLACE );
					return FALSE;
				}
				
				// bottom right:
				if( !DoFetchNextItemPoint( ',', string, &x, &botRight(box) ) )
				{
					ReturnString( "Error: Item 4 to 5 of 'invalidate rect' opcode must be botRight coordinate.", REPLACE );
					return FALSE;
				}
				
				InvalRect( &box );
			}
		}
		else if( EqualString( str, "\pvalidate rect", false, true )
				|| EqualString( str, "\pvalR", false, true ) )
		{
			if( (flags & kXDRAWNoPortsFlag) == kXDRAWNoPortsFlag )
			{
				ReturnString( "Error: 'validate rect' opcode not permitted in this mode.", REPLACE );
				return FALSE;
			}
			else
			{
				Rect		box;
			
				// top left:
				if( !DoFetchNextItemPoint( ',', string, &x, &topLeft(box) ) )
				{
					ReturnString( "Error: Item 2 to 3 of 'invalidate rect' opcode must be topLeft coordinate.", REPLACE );
					return FALSE;
				}
				
				// bottom right:
				if( !DoFetchNextItemPoint( ',', string, &x, &botRight(box) ) )
				{
					ReturnString( "Error: Item 4 to 5 of 'invalidate rect' opcode must be botRight coordinate.", REPLACE );
					return FALSE;
				}
				
				ValidRect( &box );
			}
		}
		else if( EqualString( str, "\pimage file", false, true )
				|| EqualString( str, "\pimgF", false, true ) )
		{
		  #if powerc
			if( !DoImageFileOpcode( string, &x, flags ) )
				return FALSE;
		  #else
			ReturnString( "Error: The 'image file' opcode is only supported on PowerPC Macintoshes.", REPLACE );
			return FALSE;
		  #endif
		}
		else
		{
			char*		aStr;
			
			aStr = p2cstr(str);
			
			ReturnString( "Error: Found unknown opcode \"", REPLACE );
			ReturnString( aStr, APPEND );
			ReturnString( "\".", APPEND );
			return FALSE;
		}
		
		if( string[x] != '\n'
			&& string[x] != 0
			&& string[x] != '\r' )
		{
			ReturnString( "Error: Found additional characters after an opcode.", REPLACE );
			return FALSE;
		}
	}
	
	return TRUE;
}


/* ————————————————————————————————————————————————————————————————————————————————
	Utility function for calculating intermediate colours:
		(original routine by Dair Grant)
   ————————————————————————————————————————————————————————————————————————————— */

void mixColor( RGBColor *light, RGBColor *dark, short shade, 
				RGBColor *result )
{
	shade = 0x0F - shade;
		// This is necessary because we give shades between light and
		// dark (0% is light), but for colors, $0000 is black and $FFFF 
		// is dark.

	result->red	  = (long) (light->red   - dark->red)   * shade / 15 + dark->red;
	result->green = (long) (light->green - dark->green) * shade / 15 + dark->green;
	result->blue  = (long) (light->blue  - dark->blue)  * shade / 15 + dark->blue;
	
	// Account for overflow:
	if( result->red > light->red )
		result->red = light->red;
	if( result->green > light->green )
		result->green = light->green;
	if( result->blue > light->blue )
		result->blue = light->blue;
	
	// Make sure we don't get negative:
	if( result->red < dark->red )
		result->red = dark->red;
	if( result->green < dark->green )
		result->green = dark->green;
	if( result->blue < dark->blue )
		result->blue = dark->blue;
}