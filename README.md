# xDraw
An old HyperCard native code plugin (XCMD) for doing all sorts of drawing commands in HyperCard, quickly, and even with color.

## Anything special?
XCMDClass.c/.h includes, among other things, implementations of two undocumented HyperCard XCMD callbacks in MyGetMaskAndData and MyChangedMaskAndData, which allow an XCMD to read/change the pixels of the current card's graphics.

## What is included?
This contains the CompileIt source code for xDraw 1, the C source code for xDraw 2, as well as the test stack used during development.

The CheapVersionControl.sit file contains a StuffIt-compressed HyperCard stack that can be used to convert these files from a GitHub-friendly format (Unicode text, Unix line endings, resource fork-only files moved into the data fork, dual-fork files compressed with StuffIt) back into the original Mac format you'd need to run this on an actual Mac or emulator.

## Any warranties?
No. This outdated code is supplied for educational purposes only.
