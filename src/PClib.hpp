#pragma once
#define PCL_PCLIB

/// @brief Some private util of PCLib placed here
namespace pcpri{};

/** @brief Classes and functions that can easily
 *  cause naming conflicts placed here
 */
namespace pc{};

#define PCL_VERSION "25v20d"

#include"Exception.hpp"

#include"./Algorithm/Sorting.hpp"

#include"./Container/Large.hpp"
#include"./Container/String.hpp"
#include"./Container/Command.hpp"
#include"./Container/Containers.hpp"
#include"./Container/VarSet.hpp"
#include"./Container/SyncedData.hpp"

#include"./Math/Fraction.hpp"

#ifdef WIN32
#include"Windows/Console.hpp"
#include"Windows/Socket.hpp"
#endif

#include"./TUI/Color.hpp"
#include"./TUI/Ansi.hpp"
#include"./TUI/TUI.hpp"

#include "Utility/File.hpp"
#include "Utility/IO.hpp"