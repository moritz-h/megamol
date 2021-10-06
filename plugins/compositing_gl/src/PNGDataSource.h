/*
 * PNGDataSource.h
 *
 * Copyright (C) 2015-2015 by CGV (TU Dresden)
 * Alle Rechte vorbehalten.
 */

#ifndef MEGAMOL_DATATOOLS_PNGDATASOURCE_H_INCLUDED
#define MEGAMOL_DATATOOLS_PNGDATASOURCE_H_INCLUDED
#pragma once

#include "mmcore/Module.h"
#include "mmcore/param/ParamSlot.h"
#include "mmcore/Call.h"
#include "mmcore/CalleeSlot.h"

#include "glowl/Texture2D.hpp"

namespace megamol {
namespace compositing {

    class PNGDataSource : public core::Module {
    public:

        static const char *ClassName(void) { return "PNGDataSource"; }
        static const char *Description(void) { return "Data source for loading .png files"; }
        static bool IsAvailable(void) { return true; }

        PNGDataSource(void);
        virtual ~PNGDataSource(void);

    protected:

        virtual bool create(void);
        virtual void release(void);

    private:

        bool getDataCallback(core::Call& caller);

        /** Slot for loading the .png file */
        core::param::ParamSlot m_filename_mlot;

        /** Slot for requesting the output textures from this module, i.e. lhs connection */
        megamol::core::CalleeSlot m_output_tex_slot;

        /** Texture that holds the data from the loaded .png file */
        std::shared_ptr<glowl::Texture2D> m_output_texture;

        uint32_t m_version;
    };

} /* end namespace compositing */
} /* end namespace megamol */

#endif /* MEGAMOL_DATATOOLS_PNGDATASOURCE_H_INCLUDED */
