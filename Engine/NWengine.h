#pragma once

/*


					 /$$   /$$ /$$      /$$                               /$$
					| $$$ | $$| $$  /$ | $$                              |__/
					| $$$$| $$| $$ /$$$| $$  /$$$$$$  /$$$$$$$   /$$$$$$  /$$ /$$$$$$$   /$$$$$$
					| $$ $$ $$| $$/$$ $$ $$ /$$__  $$| $$__  $$ /$$__  $$| $$| $$__  $$ /$$__  $$
					| $$  $$$$| $$$$_  $$$$| $$$$$$$$| $$  \ $$| $$  \ $$| $$| $$  \ $$| $$$$$$$$
					| $$\  $$$| $$$/ \  $$$| $$_____/| $$  | $$| $$  | $$| $$| $$  | $$| $$_____/
					| $$ \  $$| $$/   \  $$|  $$$$$$$| $$  | $$|  $$$$$$$| $$| $$  | $$|  $$$$$$$
					|__/  \__/|__/     \__/ \_______/|__/  |__/ \____  $$|__/|__/  |__/ \_______/
										    /$$  \ $$
									           |  $$$$$$/
										    \______/

																														*/

#include "Utilities.h"
#include "Globals.h"
#include "NWTime.h"

#ifndef NW_DLL_ENGINE

#include <iostream>
#include "Primitives.h"
#include "Inputs.h"
#include "Context.h"
#include "RessourcesLoader.h"
#include "Camera.h"
#include "Renderer.h"
#include "Scene.h"
#include "Components.h"
#include "ScriptManager.h"

#include "Batch.h"
#include "Text.h"
#include "NWproj.h"

namespace NWengine {
	int Run();
	void MainLoop();
	void Shutdown();
}

#endif

#ifdef _WINDLL
	extern "C" {
		__declspec(dllexport) int DllRun();
	}
#endif