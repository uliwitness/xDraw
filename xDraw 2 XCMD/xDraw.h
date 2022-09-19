/*	∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞

	PROJECT:	TrackRect
	
	PURPOSE:	XFCN that allows using GWorlds. You can draw into these using
				xDraw or other XCMDs.
		
	COPYRIGHT:	(C) Copyright 1999 by M. Uli Kusterer, all rights reserved.
	
				
	REACH ME AT:
				E-MAIL:		witness@weblayout.com
				URL:		http://www.weblayout.com/witness
	
	
	REVISIONS:
		99-04-09	UK		Created.
				
	∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞ */

#pragma once

/* ————————————————————————————————————————————————————————————————————————————————
	Headers:
   ————————————————————————————————————————————————————————————————————————————— */

#include	"XCMDClass.h"


/* ————————————————————————————————————————————————————————————————————————————————
	Constants:
   ————————————————————————————————————————————————————————————————————————————— */

// Modes returned by DoFetchNextItemMode():
enum
{
	kXDRAWUnrecognizedMode = 0,
	kXDRAWFillMode,
	kXDRAWFrameMode,
	kXDRAWInvertMode,
	kXDRAWEraseMode
};

// Possible parameters to DoDraw:
enum
{
	kXDRAWAllFeaturesFlag =	0,			// Allow use of everything.
	
	kXDRAWForceCicnsFlag =	(1 << 0),	// Do a CopyBits on cicns so they record. (NYI)
	kXDRAWNoPortsFlag =		(1 << 1),	// Error on port changes and valid/invalid.
	kXDRAWOnlyFrameFlag =	(1 << 2),	// Error on non-framed shapes (things that don't record to Regions) (NYI).
	
	kXDRAWRegionRecordFlag = (kXDRAWNoPortsFlag | kXDRAWForceCicnsFlag | kXDRAWOnlyFrameFlag),
	kXDRAWPictureRecordFlag = (kXDRAWNoPortsFlag | kXDRAWForceCicnsFlag)
};

// Possible ways to create PICT res:
enum
{
	kXDRAWMakePictReplace = 0,
	kXDRAWMakePictUnique,
	kXDRAWMakePictIfNotThere
};


/* ————————————————————————————————————————————————————————————————————————————————
	Class definition:
   ————————————————————————————————————————————————————————————————————————————— */

extern WindowPtr		gCardWindow;


/* ————————————————————————————————————————————————————————————————————————————————
	Class definition:
   ————————————————————————————————————————————————————————————————————————————— */

void		Implementation();
void		DoDrawMode();
void		DoDrawDesktop();
void		DoWithin();
void		DoInvalid( Boolean inval );
void		DoSavePictRes();
void		DoSavePictFile();
void		DoDrawToCard();
void		DoDrawToGrc();

Boolean		DoDraw( unsigned long flags );
Boolean		DoProcessOpcode( char* string, unsigned long flags );
RgnHandle	DoMakeRgnFromOpcodes();
PicHandle	DoMakePictureFromOpcodes();

Boolean		DoIsSubString( Str255 str1, Str255 str2 );
Boolean		DoFetchNextItemNum( char delim, char* string, short *x, long *num );
long		DoFetchNextItemMode( char delim, char* string, short *x );
Boolean		DoFetchNextItemPoint( char delim, char* string, short *x, Point *num );
void		DoFetchNextItem( char delim, char* string, short *x, Str255 outStr );


void		Syntax();
void		Copyright();

