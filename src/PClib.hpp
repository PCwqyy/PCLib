#pragma once
#define PCL_PCLIB

/// @brief Some private util of PCLib placed here
namespace pcpri{};

/** @brief Classes and functions that can easily
 *  cause naming conflicts placed here
 */
namespace pc{};

#include"./TUI/TUI.hpp"

#include"./Container/Large.hpp"
#include"./Container/Command.hpp"
#include"./Container/Rational.hpp"
#include"./Container/Continers.hpp"
#include"./Container/VarSet.hpp"

#ifdef WIN32
#include"Windows/Console.hpp"
#include"Windows/Socket.hpp"
#endif

#include "Utility/File.hpp"