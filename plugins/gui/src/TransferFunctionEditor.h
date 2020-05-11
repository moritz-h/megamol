/*
 * TransferFunctionEditor.h
 *
 * Copyright (C) 2019 by Universitaet Stuttgart (VIS).
 * Alle Rechte vorbehalten.
 */

#ifndef MEGAMOL_GUI_TRANSFERFUNCTIONEDITOR_INCLUDED
#define MEGAMOL_GUI_TRANSFERFUNCTIONEDITOR_INCLUDED


#include "mmcore/param/TransferFunctionParam.h"
#include "mmcore/view/TransferFunction.h"

#include "vislib/sys/Log.h"

#include <cmath>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

#include "GUIUtils.h"


namespace megamol {
namespace gui {

/**
 * 1D Transfer Function Editor.
 */
class TransferFunctionEditor {
public:
    TransferFunctionEditor(void);

    ~TransferFunctionEditor(void) = default;

    /**
     * Set transfer function data to use in editor.
     *
     * @param tfs The transfer function encoded as string in JSON format.
     *
     * @return True if string was successfully converted into transfer function data, false otherwise.
     */
    void SetTransferFunction(const std::string& tfs, bool active_parameter_mode);

    /**
     * Get current transfer function data.
     *
     * @return The transfer function encoded as string in JSON format
     */
    bool GetTransferFunction(std::string& tfs);

    /**
     * Set the currently active parameter.
     */
    void SetActiveParameter(core::param::TransferFunctionParam* param) { this->active_parameter = param; }

    /**
     * Get the currently active parameter.
     */
    core::param::TransferFunctionParam* GetActiveParameter(void) { return this->active_parameter; }

    /**
     * Draws the transfer function editor.
     */
    bool Draw(bool active_parameter_mode);

    /**
     * Returns true if editor is in minimized view.
     */
    inline bool IsMinimized(void) const { return !this->showOptions; }

    /**
     * Create texture.
     */
    void CreateTexture(GLuint& inout_id, GLsizei width, GLsizei height, float* data) const;

private:
    void drawTextureBox(const ImVec2& size, bool flip_xy);

    void drawScale(const ImVec2& pos, const ImVec2& size, bool flip_xy);

    void drawFunctionPlot(const ImVec2& size);

    /** The global input widget state buffer. */
    struct WidgetBuffer {
        float min_range;
        float max_range;
        float gauss_sigma;
        float range_value;
        int tex_size;
    };

    // VARIABLES -----------------------------------------------------------

    /** Utils being used all over the place */
    GUIUtils utils;

    /** The currently active parameter whose transfer function is currently loaded into this editor. */
    core::param::TransferFunctionParam* active_parameter;

    /** Array holding current colors and function values. */
    megamol::core::param::TransferFunctionParam::TFNodeType nodes;

    /** Min/Max intervall the data should be mapped. */
    std::array<float, 2> range;

    /** Flag indicating if propagated range should be overwriten by editor */
    bool range_overwrite;

    /** Current interpolation option. */
    megamol::core::param::TransferFunctionParam::InterpolationMode mode;

    /** Current texture size. */
    UINT textureSize;

    /** Indicating modified transfer function. Recalculate texture data. */
    bool textureInvalid;

    /** Indicates whether changes are already applied or not. */
    bool pendingChanges;

    /** Current texture data. */
    std::vector<float> texturePixels;

    /** OpenGL Texture IDs. */
    GLuint texture_id_vert;
    GLuint texture_id_horiz;

    /** Currently active color channels in plot. */
    std::array<bool, 4> activeChannels;

    /** Currently selected node. */
    unsigned int currentNode;

    /** Currently selected color channel of selected node. */
    unsigned int currentChannel;

    /** Offset from center of point to initial drag position. */
    ImVec2 currentDragChange;

    /** Flag for applying all changes immediately. */
    bool immediateMode;

    /** Flag indicating if all options should be shown*/
    bool showOptions;

    /** The global input widget state buffer. */
    WidgetBuffer widget_buffer;

    /** Legend alignment flag. */
    bool flip_xy;
};

} // namespace gui
} // namespace megamol

#endif // MEGAMOL_GUI_TRANSFERFUNCTIONEDITOR_INCLUDED
