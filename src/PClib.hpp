#pragma once
#define PCL_PCLIB

/** @brief Some private util of PCLib placed here */
namespace pcpri{};

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

#include"Algorithm/File.hpp"