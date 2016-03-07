#pragma once

#include "attributes.hpp"

#ifdef my_std_EXPORTS
# define MY_STD_EXPORT_TAG SHARED_EXPORT
#elif defined my_std_IMPORTS
# define MY_STD_EXPORT_TAG SHARED_IMPORT
#else
# define MY_STD_EXPORT_TAG
#endif