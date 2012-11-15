// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

// SFML
#define SFML_DYNAMIC // Needed when linking with dlls
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

// Zinnia
#include <zinnia.h>

// Data Structures
#include <hash_map>
#include <hash_set>
#include <vector>

// IO
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>