//*************************************************************************************
//
//  SuperXCmd.h  
//  
//  Use this in addition to HyperXCmd.h
//
//*************************************************************************************

#ifndef _SuperXCmdH_
#define _SuperXCmdH_

typedef short SCErr;


    //  These are the equates for error codes returned by 
    //  those Internals Toolbox routines that return an SCErr.

enum errNum {
    isOK = 0,                   //  no error
    noSuchObject = -10000,      //  specified object not found
    noSuchCard = -9999,         //  specified card not found
    noSuchBkgnd = -9998,        //  specified background not found
    noSuchCdBg = -9997,         //  specified card or background not found
    noSuchWind = -9996,         //  specified window not found
    noSuchMenu = -9995,         //  specified menu not found
    noSuchProj = -9994,         //  specified project not found
    badReadType = -9993,        //  invalid card, bkgnd, window, or file type
    badObjType = -9992,         //  invalid object type
    badSpotType = -9991,        //  invalid graphic type
    windNotOpen = -9990,        //  specified window is not currently open
    badRefHdl = -9989,          //  invalid or nil object reference handle
    badIndRange = -9988,        //  supplied index number out of range
    noActiveFld = -9987,        //  no currently active field
    noScript = -9986,           //  specified object has no script
    noSharedFile = -9985,       //  SharedFile project not found
    noSuchStar = -9984,         //  specified project resource not found
    notInColor = -9983,         //  application not running under 8-bit color
    noMemory = -9982,           //  out of memory
    emptyRect = -9981,          //  specified rectangle is empty
    badAddStar = -9980,         //  failed to add new project resource
    badVersion = -9979,         //  invalid SuperCard version
    badMakeSpec = -9978,        //  FSMakeFSSpec failed; spec may be valid
    badNewAlias = -9977,        //  NewAlias failed
    noPictureData = -9976,      //  draw rect graphic has no pictureData
    badObjectInfo = -9975,      //  ObjectInfoHandle is of invalid size
    badObjKind = -9974,         //  ObjectInfo record contains bad object kind
    badObjSelect = -9973,       //  ObjectInfo record contains bad object select mode
    badObjColor = -9972,        //  ObjectInfo record contains bad object color index
    badObjPenSize = -9971,      //  ObjectInfo record contains bad object pen size
    badObjMode = -9970,         //  ObjectInfo record contains bad object transfer mode
    badObjShadow = -9969,       //  ObjectInfo record contains bad object shadow size
    badProcConst = -9968,       //  invalid procedure pointer indentifier
    cantReplace = -9967,        //  a project resource by that name and type exists
    badProcID = -9966,          //  invalid procedure pointer identifier
    badSetStarID = -9965,       //  failed to assign new ID to project resource
    badDataHandle = -9964,      //  attempt to assign invalid data
    badRezID = -9963,           //  attempt to assign invalid resource ID
    cantSetOpenField = -9962,   //  can't set TEHandle of open field
    notByReference = -9961,     //  can't get path of embedded pictureData
    noPictureInfo = -9960,      //  draw graphic has no pictureInfo
    noClutInUse = -9959,        //  no clut currently assigned
    expiredBeta = -9958,        //  Internals Toolbox beta version has expired
    onlyWASTE = -9957,          //  Operation only valid for WASTE flds 
    onlyTextEdit = -9956,       //  Operation only valid for non-WASTE flds 
    badPixel = -9955,           //  Specified pixel is out of range 
    badContextInfo = -9954,     //  ContextInfoHandle handle not allocated
    missingPPCToolbox = -9953   //  Program to Program Communication not supported on this system

};

typedef struct PixInfo {
    Handle theData;
    CTabHandle theTable;
    Rect bounds;
    short colorFlag;
    short rowBytes;
    short penMode;
} PixInfo, *PixInfoPtr;


typedef struct StarInfo {
    long theType;
    long theSize;
    long theID;
    long index;
    Str255 theName;
} StarInfo, *StarInfoPtr;


#define		xreqSetPictureData	4503
#define     xreqPixByName       5000
#define     xreqPixByID         5001
#define     xreqPixByNum        5002
#define     xreqAddStar         5003
#define     xreqGetStarByName   5004
#define     xreqGetStarByID     5005
#define     xreqDelStarByName   5006
#define     xreqDelStarByID     5007
#define     xreqAltStarByName   5008
#define     xreqAltStarByID     5009
#define     xreqCountStars      5010
#define     xreqGetStarInfo     5011
#define     xreqSendError       5012

// ** MDL 9/26/98 **
#define     xreqSetStarID       5013
#define     xreqDefUProp        5014
#define     xreqUnDefUProp      5015
#define     xreqGetUProp        5016
#define     xreqSetUProp        5017
#define     xreqCountUProps     5018
#define     xreqGetUPropIdx     5019
#define     xreqSetUPropIdx     5020
#define     xreqUPropNameToIdx  5021
#define     xreqUPropIdxToName  5022
// **
#define     xreqInstallPData    4503


//********************************************************************************

typedef pascal void (*CallbackProcPtr)(void);

#if defined(powerc) || defined (__powerc)
    #pragma options align=mac68k
    enum { uppSCCallbackInfo = kPascalStackBased };
    #define CallSCBack( xpb )   CallUniversalProc( (UniversalProcPtr) (xpb)->entryPoint, uppSCCallbackInfo )
#else
    #define CallSCBack( xpb )   (*((CallbackProcPtr) (xpb)->entryPoint))()
#endif


//****************************************************************************************
// SuperCard's additional 14 callbacks

void        InstallPData        (XCmdPtr paramPtr, StringPtr theObjectName, PicHandle thePicture );
void        SendError           (XCmdPtr paramPtr, short theError);

PixInfoPtr  GetPixByName        (XCmdPtr paramPtr, Boolean cardFieldFlag, StringPtr fieldName);
PixInfoPtr  GetPixByNum         (XCmdPtr paramPtr, Boolean cardFieldFlag, short fieldNum);
PixInfoPtr  GetPixByID          (XCmdPtr paramPtr, Boolean cardFieldFlag, long fieldID);
Handle      GetStarByName       (XCmdPtr paramPtr, StringPtr theName, ResType theType);
Handle      GetStarByID         (XCmdPtr paramPtr, long theID, ResType theType);
void        DelStarByID         (XCmdPtr paramPtr, long theID, ResType theType);
void        DelStarByName       (XCmdPtr paramPtr, StringPtr theName, ResType theType);
void        AltStarByName       (XCmdPtr paramPtr, StringPtr theName, ResType theType, Handle theData);
void        AltStarByID         (XCmdPtr paramPtr, long theID, ResType theType, Handle theData);
long        AddStar             (XCmdPtr paramPtr, Handle theData, StringPtr theName, ResType theType);

long        CountStars          (XCmdPtr paramPtr);
StarInfoPtr GetStarInfo         (XCmdPtr paramPtr, long index, ResType theType);
void        SetStarID           (XCmdPtr paramPtr, long oldID, ResType theType, long newID);

// ** MDL 9/26/98 **
// SuperCard 3.5's additional 9 callbacks for accessing user properties
void        DefineUserProperty          (XCmdPtr paramPtr, StringPtr objectDescrip, StringPtr propName);
void        UnDefineUserProperty        (XCmdPtr paramPtr, StringPtr objectDescrip, StringPtr propName);
void        GetUserPropByName           (XCmdPtr paramPtr, StringPtr objectDescrip, StringPtr propName, Handle *propData);
void        SetUserPropByName           (XCmdPtr paramPtr, StringPtr objectDescrip, StringPtr propName, Handle propData);
void        UserPropNameToIndex         (XCmdPtr paramPtr, StringPtr objectDescrip, StringPtr propName, long *index);
void        CountUserProps              (XCmdPtr paramPtr, StringPtr objectDescrip, long *count);
void        GetUserPropByIndex          (XCmdPtr paramPtr, StringPtr objectDescrip, long index, Handle *propData);
void        SetUserPropByIndex          (XCmdPtr paramPtr, StringPtr objectDescrip, long index, Handle propData);
void        UserPropIndexToName         (XCmdPtr paramPtr, StringPtr objectDescrip, long index, StringPtr propName);
// **

#endif  // #ifndef _SuperXCmdH_