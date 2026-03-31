#pragma once


// Fundamental external dependencies.


namespace Black
{
inline namespace Format
{
// TGA file format subsystem.
inline namespace Tga
{
// Internal definitions for TGA format.
namespace Internal
{

}
}
}
}


// Internal stuff.
#include "internal/constants.h"
#include "internal/constants.operations.h"
#include "internal/constants.functions.h"
#include "internal/structures.h"

// TGA file format types.
#include "TgaStructure.h"
#include "TgaFileView.h"
#include "TgaImageDecoder.h"
#include "TgaImageEncoder.h"
