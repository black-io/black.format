#pragma once


// Fundamental external dependencies.


namespace Black
{
inline namespace Format
{
// PNG file format subsystem.
inline namespace Png
{
// Internal definitions for PNG format.
namespace Internal
{

}
}
}
}


// Internal stuff.
#include "internal/constants.h"
#include "internal/structures.h"
#include "internal/constants.operations.h"
#include "internal/constants.functions.h"

// PNG file format types.
#include "PngStructure.h"
#include "PngFileView.h"

// Deferred inline implementations.
#include "internal/constants.operations.inl"
