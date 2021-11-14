#pragma once
#include "CommHeader.hpp"
#include <limits>


//===========================================================================
// define commonly used constants.
//===========================================================================
const int _INT_MAX           = (std::numeric_limits<int>::max)();
const unsigned int _UINT_MAX = (std::numeric_limits<unsigned int>::max)();
const double _DBL_MAX        = (std::numeric_limits<double>::max)();
const double _DBL_EPSILON    = (std::numeric_limits<double>::epsilon)();
const float _FLOAT_MAX       = (std::numeric_limits<float>::max)();
const float _FLOAT_EPSILON   = (std::numeric_limits<float>::epsilon)();

