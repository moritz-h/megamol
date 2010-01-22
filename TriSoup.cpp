/*
 * TriSoup.cpp
 *
 * Copyright (C) 2009 by VISUS (Universitaet Stuttgart)
 * Alle Rechte vorbehalten.
 */

#include "stdafx.h"
#include "TriSoup.h"
#include "api/MegaMolCore.std.h"
#include "ModuleAutoDescription.h"
#include "vislib/vislibversion.h"
#include "vislib/Log.h"
#include "vislib/ThreadSafeStackTrace.h"
#include "TriSoupRenderer.h"


/*
 * mmplgPluginAPIVersion
 */
TRISOUP_API int mmplgPluginAPIVersion(void) {
    return 100;
}


/*
 * mmplgPluginName
 */
TRISOUP_API const char * mmplgPluginName(void) {
    return "TriSoup";
}


/*
 * mmplgPluginDescription
 */
TRISOUP_API const char * mmplgPluginDescription(void) {
    return "Plugin for rendering TriSoup mesh data";
}


/*
 * mmplgCoreCompatibilityValue
 */
TRISOUP_API const void * mmplgCoreCompatibilityValue(void) {
    static const mmplgCompatibilityValues compRev = {
        sizeof(mmplgCompatibilityValues),
        MEGAMOL_CORE_COMP_REV,
        VISLIB_VERSION_REVISION
    };
    return &compRev;
}


/*
 * mmplgModuleCount
 */
TRISOUP_API int mmplgModuleCount(void) {
    return 1;
}


/*
 * mmplgModuleDescription
 */
TRISOUP_API void* mmplgModuleDescription(int idx) {
    if (idx == 0) {
        return new megamol::core::ModuleAutoDescription<
            megamol::core::misc::TriSoupRenderer>();
    }
    return NULL;
}


/*
 * mmplgCallCount
 */
TRISOUP_API int mmplgCallCount(void) {
    return 0;
}


/*
 * mmplgCallDescription
 */
TRISOUP_API void* mmplgCallDescription(int idx) {
    return NULL;
}


/*
 * mmplgConnectStatics
 */
TRISOUP_API bool mmplgConnectStatics(int which, void* value) {
    static vislib::sys::Log::EchoTargetRedirect etr(NULL);
    switch (which) {

        case 1: // vislib::log
            etr.SetTarget(static_cast<vislib::sys::Log*>(value));
            vislib::sys::Log::DefaultLog.SetLogFileName(static_cast<const char*>(NULL), false);
            vislib::sys::Log::DefaultLog.SetLevel(vislib::sys::Log::LEVEL_NONE);
            vislib::sys::Log::DefaultLog.SetEchoOutTarget(&etr);
            vislib::sys::Log::DefaultLog.SetEchoLevel(vislib::sys::Log::LEVEL_ALL);
            vislib::sys::Log::DefaultLog.EchoOfflineMessages(true);
            return true;

        case 2: // vislib::stacktrace
            return vislib::sys::ThreadSafeStackTrace::Initialise(
                *static_cast<const vislib::SmartPtr<vislib::StackTrace>*>(value), true);

    }
    return false;
}
