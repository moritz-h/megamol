/*
 * CallSlot.h
 *
 * Copyright (C) 2019 by Universitaet Stuttgart (VIS).
 * Alle Rechte vorbehalten.
 */

#ifndef MEGAMOL_GUI_GRAPH_CALLSLOT_H_INCLUDED
#define MEGAMOL_GUI_GRAPH_CALLSLOT_H_INCLUDED


#include "vislib/sys/Log.h"

#include <map>
#include <vector>

#include "GUIUtils.h"


namespace megamol {
namespace gui {
namespace configurator {


// Forward declaration
class Call;
class CallSlot;
class Module;

// Pointer types to classes
typedef std::shared_ptr<Call> CallPtrType;
typedef std::shared_ptr<CallSlot> CallSlotPtrType;
typedef std::shared_ptr<Module> ModulePtrType;

/**
 * Defines call slot data structure for graph.
 */
class CallSlot {
public:
    enum CallSlotType { CALLEE, CALLER };

    CallSlot(int uid);
    ~CallSlot();

    const int uid;

    std::string name;
    std::string description;
    std::vector<size_t> compatible_call_idxs; // (Storing only indices of compatible calls for faster comparison.)
    CallSlotType type;

    bool CallsConnected(void) const;
    bool ConnectCall(CallPtrType call);
    bool DisConnectCall(int call_uid, bool called_by_call);
    bool DisConnectCalls(void);
    const std::vector<CallPtrType>& GetConnectedCalls(void);

    bool ParentModuleConnected(void) const;
    bool ConnectParentModule(ModulePtrType parent_module);
    bool DisConnectParentModule(void);
    const ModulePtrType GetParentModule(void);

    // GUI Presentation -------------------------------------------------------

    ImGuiID GUI_Present(ImVec2 canvas_offset, float canvas_zooming) {
        return this->present.GUI_Present(*this, canvas_offset, canvas_zooming);
    }

    ImVec2 GUI_GetPosition(void) { return this->present.GetPosition(); }

    void GUI_SetLabelVisibility(bool visible) { this->present.label_visible = visible; }

private:
    ModulePtrType parent_module;
    std::vector<CallPtrType> connected_calls;

    /**
     * Defines GUI call slot present.
     */
    class Presentation {
    public:
        Presentation(void);

        ~Presentation(void);

        ImGuiID GUI_Present(CallSlot& call_slot, ImVec2 canvas_offset, float canvas_zooming);

        ImVec2 GetPosition(void) { return this->position; }

        void UpdatePosition(CallSlot& call_slot, ImVec2 canvas_offset, float canvas_zooming);

        bool label_visible;

    private:
        enum Presentations { DEFAULT } presentations;
        ImVec2 position;
        float slot_radius;
        GUIUtils utils;

    } present;
};


} // namespace configurator
} // namespace gui
} // namespace megamol

#endif // MEGAMOL_GUI_GRAPH_CALLSLOT_H_INCLUDED