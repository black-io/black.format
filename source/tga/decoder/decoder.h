#pragma once


// Fundamental external dependencies.
#include <black/format/tga.h>
#include <black/data-oriented/compositions.h>


// Most fundamental stuff.
#include "forwards.h"

// Interfaces.
#include "BasicInputFeeder.h"
#include "BasicColorMapper.h"
#include "BasicColorConverter.h"

// Components.
#include "InputFeeder.h"

#include "StraightInputFeeder.h"
#include "RleInputFeeder.h"

#include "DirectColorMapper.h"
#include "PaletteColorMapper.h"

#include "MonochromeColorConverter.h"
#include "DirectColorConverter.h"
#include "RemapingColorConverter.h"
#include "TransformColorConverter.h"

#include "OutputBuilder.h"

// Support types.
#include "CoordinateCursor.h"

// Routine.
#include "DecodePipeline.h"
