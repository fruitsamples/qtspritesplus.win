//////////
//
//	File:		ImageCompressionUtilities.h
//
//	Contains:	Image Compression Utilities.
//
//	Written by:	Peter Hoddie, Sean Allen, Chris Flick
//	Revised by:	Tim Monroe
//
//	Copyright:	� 1998 by Apple Computer, Inc., all rights reserved.
//
//	Change History (most recent first):
//
//	   <3>	 	11/17/00	rtm		general clean-up to bring this file into conformance with style of other
//									sample code
//	   <2>	 	03/17/00	rtm		moved some things from ImageCompressionUtilities.c to here
//	   <1>	 	03/27/98	rtm		existing file
//
//////////


#ifndef __IMAGECOMPRESSIONUTILITIES__
#define __IMAGECOMPRESSIONUTILITIES__


//////////
//
// header files
//
//////////

#ifndef __MOVIES__
#include <Movies.h>
#endif

#ifndef __COLORPICKER__
#include <ColorPicker.h>
#endif

#ifndef __RESOURCES__
#include <Resources.h>
#endif

#ifndef __ENDIAN__
#include <Endian.h>
#endif

#ifndef __IMAGECOMPRESSION__
#include <ImageCompression.h>
#endif

#ifndef __QUICKDRAW__
#include <Quickdraw.h>
#endif

#ifndef PASCAL_RTN
#define PASCAL_RTN
#endif


//////////
//
// compiler macros
//
//////////

#define BailIf(a, e) 						{if (a) 	{ err = e; goto bail; }}
#define BailOSErr(a) 						{if ((err = a) != noErr)	goto bail;}
#define BailMemErr(a)						{a; if ((err = MemError()) != noErr) goto bail;}

#if TARGET_OS_WIN32
#define GetPortGrafProcs(a)					(CQDProcsPtr)a->grafProcs
#define SetPortGrafProcs(a,b)				a->grafProcs=(CGrafPtr)b
#endif


//////////
//
// constants
//
//////////

#define kCompressDepth					16				// use the Animation compressor at 16 bit depth in these utilities
#define kThreshold						(255-16)

enum {
	kRecoProcInitMsg					= 1,			// theMessage and theRefCon are valid
	kRecoProcDisposeMsg					= 2,			// theMessage and theRefCon are valid
	kRecoProcGetBoundsMsg				= 3,			// theMessage, theRect and theRefCon are valid. Proc fills in bounds with rectangle to use for compressed image
	kRecoProcDrawMsg					= 4				// theMessage, theRect, theDrawingPort, theImageType and theRefCon are valid;
														// theImageType is 'imag' if drawing into image GWorld, 'imap' if drawing into hit-testing GWorld
};

enum {
	kRecoProcOriginalImageType			= 	FOUR_CHAR_CODE('imag'),
	kRecoProcHitTestingImageType		= 	FOUR_CHAR_CODE('imap')
};


//////////
//
// data types
//
//////////

// used for compressing QuickDraw pictures with transparency/hit-testing
typedef struct {
	PicHandle					picture;
} ICUtils_PictureCompressProcData;

// used for recompressing QuickTime compressed data with transparency/hit-testing
typedef struct {
	ImageDescriptionHandle		imageDesc;
	Handle						imageData;
} ICUtils_CompressedImageCompressProcData;

// used for extracting QuickTime compressed image data and description, if any, from a QuickDraw picture
typedef struct {
	CGrafPtr					tempPort;
	Handle						data;
	ImageDescriptionHandle		idh;
} ICUtils_ExtractPictRecord;

// low-level callback procedure based routine to compress with/without transparency and hit-testing
typedef PASCAL_RTN OSErr (*CompressDrawProc)(short theMessage, Rect *theRect, GWorldPtr theDrawingPort, OSType theImageType, void *theRefCon);


//////////
//
// function prototypes
//
//////////

static ImageDescriptionHandle			ICUtils_CreateImageDescription (CodecType theCodecType, PixMapHandle thePixmap);
PASCAL_RTN void							ICUtils_NoDitherBitsProc (const BitMap *theSrcBits, const Rect *theSrcRect, const Rect *theDstRect, short theMode, RgnHandle theMaskRgn);
static void								ICUtils_DrawPictureNoDither (PicHandle thePicture, const Rect *theRect);
static OSErr							ICUtils_PrepareFor16BitCompress (PicHandle *thePicture);

OSErr									ICUtils_RecompressWithTransparencyFromProc (
													CompressDrawProc theDrawProc,
													void *theDrawProcRefCon, 
													Boolean includeHitTesting,
													RGBColor *theKeyColor, 
													RgnHandle theHitTestRegion,
													ImageDescriptionHandle *theImageDesc,
													Handle *theImageData);
													
static PASCAL_RTN OSErr					ICUtils_PictureCompressDrawProc (
													short theMessage,
													Rect *theRect,
													GWorldPtr theDrawingPort,
													OSType theImageType,
													void *theRefCon);
													
static PASCAL_RTN OSErr					ICUtils_ImageCompressDrawProc (
													short theMessage,
													Rect *theRect,
													GWorldPtr theDrawingPort,
													OSType theImageType,
													void *theRefCon);

OSErr									ICUtils_RecompressCompressedImageWithTransparency (
													ImageDescriptionHandle theOrigDesc,
													Handle theOrigImageData,
													RGBColor *theKeyColor, 
													RgnHandle theHitTestRegion,
													ImageDescriptionHandle *theImageDesc,
													Handle *theImageData);
													
OSErr									ICUtils_RecompressPictureWithTransparency (
													PicHandle theOrigPicture,
													RGBColor *theKeyColor, 
													RgnHandle theHitTestRegion,
													ImageDescriptionHandle *theImageDesc,
													Handle *theImageData);
													
OSErr									ICUtils_RecompressPictureFileWithTransparency (
													FSSpec *theFSSpec, 
													RGBColor *theKeyColor, 
													RgnHandle theHitTestRegion,
													ImageDescriptionHandle *theImageDesc,
													Handle *theImageData);

#endif // __IMAGECOMPRESSIONUTILITIES__
