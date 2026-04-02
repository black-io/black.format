#pragma once


// Fundamental external dependencies.
#include <black/format/tga.h>
#include <black/data-oriented/compositions.h>


// Most fundamental stuff.
#include "forwards.h"

// Interfaces.
#include "BasicInputFeeder.h"
#include "BasicColorMapper.h"

// Components.
#include "CoordinateCursor.h"
#include "StraightInputFeeder.h"
#include "RleInputFeeder.h"

#include "MonochromeColorMapper.h"
#include "PaletteColorMapper.h"
#include "TrueColorMapper.h"

// Routine.
#include "DecodePipeline.h"
