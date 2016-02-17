#pragma once

#include "attributes.hpp"

#ifdef my_std_EXPORTS
# define MY_STD_EXPORT_TAG SHARED_EXPORT
#else
# define MY_STD_EXPORT_TAG SHARED_IMPORT
#endif