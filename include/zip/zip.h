#pragma once


// Fundamental external dependencies.


namespace Black
{
inline namespace Format
{
// ZIP file format subsystem.
inline namespace Zip
{
// Internal definitions for ZIP format.
namespace Internal
{

}
}
}
}


// Internal stuff.
#include "internal/constants.h"
#include "internal/aliases.h"
#include "internal/structures.h"

// ZIP file format types.
#include "ZipStructure.h"
#include "ZipFileView.h"
#include "ZipCentralDirectoryLocator.h"
#include "ZipCentralDirectoryView.h"
