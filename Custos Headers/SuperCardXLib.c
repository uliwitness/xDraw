//*************************************************************************************
//
//  SuperCardXLib.c  
//  
//  Use this in addition to HyperXLib
//
//*************************************************************************************

#include <Strings.h>
#include <HyperXCmd.h>
#include "SuperXCmd.h"


//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••


//********************************************************************************
//  SetPictureData callback.

void InstallPData( XCmdPtr paramPtr, StringPtr theObjectName, PicHandle thePicture )
{
    paramPtr->inArgs[0] = (long) theObjectName;
    paramPtr->inArgs[1] = (long) thePicture;
    paramPtr->request = xreqSetPictureData;
    CallSCBack(paramPtr);
}

//********************************************************************************

void SendError(XCmdPtr paramPtr, short theError)
{
    paramPtr->inArgs[0] = theError;
    paramPtr->request = xreqSendError;
    CallSCBack(paramPtr);
}

//********************************************************************************

PixInfoPtr GetPixByName(XCmdPtr paramPtr, Boolean cardFieldFlag, StringPtr fieldName)
{
    paramPtr->inArgs[0] = (long)cardFieldFlag;
    paramPtr->inArgs[1] = (long)fieldName;
    paramPtr->request = xreqPixByName;
    CallSCBack(paramPtr);
    return((PixInfoPtr)paramPtr->outArgs[0]);
}

//********************************************************************************

PixInfoPtr GetPixByNum(XCmdPtr paramPtr, Boolean cardFieldFlag, short fieldNum)
{
    paramPtr->inArgs[0] = (long)cardFieldFlag;
    paramPtr->inArgs[1] = fieldNum;
    paramPtr->request = xreqPixByNum;
    CallSCBack(paramPtr);
    return((PixInfoPtr)paramPtr->outArgs[0]);
}

//********************************************************************************

PixInfoPtr GetPixByID(XCmdPtr paramPtr, Boolean cardFieldFlag, long fieldID)
{
    paramPtr->inArgs[0] = (long)cardFieldFlag;
    paramPtr->inArgs[1] = fieldID;
    paramPtr->request = xreqPixByID;
    CallSCBack(paramPtr);
    return((PixInfoPtr)paramPtr->outArgs[0]);
}

//********************************************************************************

long AddStar(XCmdPtr paramPtr, Handle theData, StringPtr theName, ResType theType)
{
    paramPtr->inArgs[0] = (long)theType;
    paramPtr->inArgs[1] = (long)theName;
    paramPtr->inArgs[2] = (long)theData;
    paramPtr->request = xreqAddStar;
    CallSCBack(paramPtr);
    return(paramPtr->outArgs[0]);
}

//********************************************************************************

Handle GetStarByName(XCmdPtr paramPtr, StringPtr theName, ResType theType)
{
    paramPtr->inArgs[0] = (long)theType;
    paramPtr->inArgs[1] = (long)theName;
    paramPtr->request = xreqGetStarByName;
    CallSCBack(paramPtr);
    return((Handle)paramPtr->outArgs[0]);
}

//********************************************************************************

Handle GetStarByID(XCmdPtr paramPtr, long theID, ResType theType)
{
    paramPtr->inArgs[0] = (long)theType;
    paramPtr->inArgs[1] = theID;
    paramPtr->request = xreqGetStarByID;
    CallSCBack(paramPtr);
    return (Handle)paramPtr->outArgs[0];
}

//********************************************************************************

void DelStarByID(XCmdPtr paramPtr, long theID, ResType theType)
{
    paramPtr->inArgs[0] = (long)theType;
    paramPtr->inArgs[1] = theID;
    paramPtr->request = xreqDelStarByID;
    CallSCBack(paramPtr);
}

//********************************************************************************

void DelStarByName(XCmdPtr paramPtr, StringPtr theName, ResType theType)
{
    paramPtr->inArgs[0] = (long)theType;
    paramPtr->inArgs[1] = (long)theName;
    paramPtr->request = xreqDelStarByName;
    CallSCBack(paramPtr);
}

//********************************************************************************

void AltStarByName(XCmdPtr paramPtr, StringPtr theName, ResType theType, 
Handle theData)
{
    paramPtr->inArgs[0] = (long)theType;
    paramPtr->inArgs[1] = (long)theName;
    paramPtr->inArgs[2] = (long)theData;
    paramPtr->request = xreqAltStarByName;
    CallSCBack(paramPtr);
}

//********************************************************************************

void AltStarByID(XCmdPtr paramPtr, long theID, ResType theType, Handle 
theData)
{
    paramPtr->inArgs[0] = (long)theType;
    paramPtr->inArgs[1] = theID;
    paramPtr->inArgs[2] = (long)theData;
    paramPtr->request = xreqAltStarByID;
    CallSCBack(paramPtr);
}

//********************************************************************************

long CountStars(XCmdPtr paramPtr)
{
    paramPtr->request = xreqCountStars;
    CallSCBack(paramPtr);
    return(paramPtr->outArgs[0]);
}

//********************************************************************************

StarInfoPtr GetStarInfo(XCmdPtr paramPtr, long index, ResType theType)
{
    paramPtr->inArgs[0] = index;
    paramPtr->inArgs[1] = (long)theType;
    paramPtr->request = xreqGetStarInfo;
    CallSCBack(paramPtr);
    return((StarInfoPtr)paramPtr->outArgs[0]);
}



// ** MDL 9/26/98 **
void DefineUserProperty(XCmdPtr paramPtr, StringPtr objectDescrip, StringPtr propName)
{
    paramPtr->inArgs[0] = (long)objectDescrip;
    paramPtr->inArgs[1] = (long)propName;
    paramPtr->request = xreqDefUProp;
    CallSCBack(paramPtr);
}

void UnDefineUserProperty(XCmdPtr paramPtr, StringPtr objectDescrip, StringPtr propName)
{
    paramPtr->inArgs[0] = (long)objectDescrip;
    paramPtr->inArgs[1] = (long)propName;
    paramPtr->request = xreqUnDefUProp;
    CallSCBack(paramPtr);
}

void GetUserPropByName(XCmdPtr paramPtr, StringPtr objectDescrip, StringPtr propName, Handle *propData)
{
    paramPtr->inArgs[0] = (long)objectDescrip;
    paramPtr->inArgs[1] = (long)propName;
    paramPtr->inArgs[2] = (long)propData;
    paramPtr->request = xreqGetUProp;
    CallSCBack(paramPtr);
}

void SetUserPropByName(XCmdPtr paramPtr, StringPtr objectDescrip, StringPtr propName, Handle propData)
{
    paramPtr->inArgs[0] = (long)objectDescrip;
    paramPtr->inArgs[1] = (long)propName;
    paramPtr->inArgs[2] = (long)propData;
    paramPtr->request = xreqSetUProp;
    CallSCBack(paramPtr);
}

void CountUserProps(XCmdPtr paramPtr, StringPtr objectDescrip, long *propCount)
{
    paramPtr->inArgs[0] = (long)objectDescrip;
    paramPtr->inArgs[1] = (long)propCount;
    paramPtr->request = xreqCountUProps;
    CallSCBack(paramPtr);
}

void GetUserPropByIndex(XCmdPtr paramPtr, StringPtr objectDescrip, long idx, Handle *propData)
{
    paramPtr->inArgs[0] = (long)objectDescrip;
    paramPtr->inArgs[1] = (long)idx;
    paramPtr->inArgs[2] = (long)propData;
    paramPtr->request = xreqGetUPropIdx;
    CallSCBack(paramPtr);
}

void SetUserPropByIndex(XCmdPtr paramPtr, StringPtr objectDescrip, long idx, Handle propData)
{
    paramPtr->inArgs[0] = (long)objectDescrip;
    paramPtr->inArgs[1] = (long)idx;
    paramPtr->inArgs[2] = (long)propData;
    paramPtr->request = xreqSetUPropIdx;
    CallSCBack(paramPtr);
}

void UserPropNameToIndex(XCmdPtr paramPtr, StringPtr objectDescrip, StringPtr propName, long *idx)
{
    paramPtr->inArgs[0] = (long)objectDescrip;
    paramPtr->inArgs[1] = (long)propName;
    paramPtr->inArgs[2] = (long)idx;
    paramPtr->request = xreqUPropNameToIdx;
    CallSCBack(paramPtr);
}

void UserPropIndexToName(XCmdPtr paramPtr, StringPtr objectDescrip, long idx, StringPtr propName)
{
    paramPtr->inArgs[0] = (long)objectDescrip;
    paramPtr->inArgs[1] = (long)idx;
    paramPtr->inArgs[2] = (long)propName;
    paramPtr->request = xreqUPropIdxToName;
    CallSCBack(paramPtr);
}