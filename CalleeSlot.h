/*
 * CalleeSlot.h
 *
 * Copyright (C) 2008-2009 by VISUS (Universitaet Stuttgart).
 * Alle Rechte vorbehalten.
 */

#ifndef MEGAMOLCORE_CALLEESLOT_H_INCLUDED
#define MEGAMOLCORE_CALLEESLOT_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */

#include "api/MegaMolCore.std.h"
#include "AbstractSlot.h"
#include "Call.h"
#include "CallDescription.h"
#include "CallDescriptionManager.h"
#include "vislib/Array.h"
#include "vislib/IllegalParamException.h"
#include "vislib/IllegalStateException.h"


namespace megamol {
namespace core {

    /** Forward declaration */
    class Module;


    /**
     * A slot connection a Call to member pointers of a module.
     */
    class MEGAMOLCORE_API CalleeSlot : public AbstractSlot {
    public:

        /**
         * Ctor.
         *
         * @param name The name of this slot.
         * @param desc A human readable description of this slot.
         */
        CalleeSlot(const vislib::StringA& name,
            const vislib::StringA& desc);

        /** Dtor. */
        virtual ~CalleeSlot(void);

        /**
         * Connects a call to this slot.
         *
         * @param call The call to connect.
         *
         * @return 'true' on success, 'false' on failure
         */
        bool ConnectCall(megamol::core::Call *call) {
            vislib::sys::AbstractReaderWriterLock& lock = this->Parent()->ModuleGraphLock();
            lock.LockExclusive();
            if (call == NULL) {
                this->SetStatusDisconnected(); // TODO: This is wrong! Reference counting!
                lock.UnlockExclusive();
                return true;
            }

            CallDescription* desc = NULL;
            for (unsigned int i = 0; i < this->callbacks.Count(); i++) {
                if ((desc = CallDescriptionManager::Instance()->Find(
                        this->callbacks[i]->CallName()))->IsDescribing(call))
                    break;
                desc = NULL;
            }
            if (desc == NULL) {
                lock.UnlockExclusive();
                return false;
            }

            vislib::StringA cn(desc->ClassName());
            for (unsigned int i = 0; i < desc->FunctionCount(); i++) {
                vislib::StringA fn(desc->FunctionName(i));
                for (unsigned int j = 0; j < this->callbacks.Count(); j++) {
                    if (cn.Equals(this->callbacks[j]->CallName(), false)
                            && fn.Equals(this->callbacks[j]->FuncName(),
                            false)) {
                        call->funcMap[i] = j;
                        break;
                    }
                }
            }
            call->callee = this;
            this->SetStatusConnected();
            lock.UnlockExclusive();
            return true;
        }

        /**
         * Do not call this method directly!
         *
         * @param func The id of the function to be called.
         * @param call The call object calling this function.
         *
         * @return The return value of the function.
         */
        bool InCall(unsigned int func, Call& call);

        /**
         * Clears the cleanup mark for this and all dependent objects.
         */
        virtual void ClearCleanupMark(void);

        /**
         * Answers whether a given call is compatible with this slot.
         *
         * @param desc The description of the call to test.
         *
         * @return 'true' if the call is compatible, 'false' otherwise.
         */
        inline bool IsCallCompatible(CallDescription* desc) const {
            if (desc == NULL) return false;
            vislib::StringA cn(desc->ClassName());

            for (unsigned int i = 0; i < desc->FunctionCount(); i++) {
                bool found = false;
                vislib::StringA fn(desc->FunctionName(i));
                for (unsigned int j = 0; j < this->callbacks.Count(); j++) {
                    if (cn.Equals(this->callbacks[j]->CallName(), false)
                            && fn.Equals(this->callbacks[j]->FuncName(),
                            false)) {
                        found = true;
                        break;
                    }
                }

                if (!found) return false;
            }

            return true;
        }

        /**
         * Registers the member 'func' as callback function for call 'callName'
         * function 'funcName'.
         *
         * @param callName The class name of the call.
         * @param funcName The name of the function of the call to register
         *                 this callback for.
         * @param func The member function pointer of the method to be used as
         *             callback. Use the class of the method as template
         *             parameter 'C'.
         */
        template<class C>
        void SetCallback(const char *callName, const char *funcName,
                bool (C::*func)(Call&)) {
            if (this->GetStatus() != AbstractSlot::STATUS_UNAVAILABLE) {
                throw vislib::IllegalStateException("You may not register "
                    "callbacks after the slot has been enabled.",
                    __FILE__, __LINE__);
            }

            vislib::StringA cn(callName);
            vislib::StringA fn(funcName);
            for (unsigned int i = 0; i < this->callbacks.Count(); i++) {
                if (cn.Equals(this->callbacks[i]->CallName(), false)
                        && fn.Equals(this->callbacks[i]->FuncName(), false)) {
                    throw vislib::IllegalParamException("callName funcName",
                        __FILE__, __LINE__);
                }
            }
            Callback *cb = new CallbackImpl<C>(callName, funcName, func);
            this->callbacks.Add(cb);
        }

        /**
         * Answers whether the given parameter is relevant for this view.
         *
         * @param searched The already searched objects for cycle detection.
         * @param param The parameter to test.
         *
         * @return 'true' if 'param' is relevant, 'false' otherwise.
         */
        virtual bool IsParamRelevant(
            vislib::SingleLinkedList<const AbstractNamedObject*>& searched,
            const vislib::SmartPtr<param::AbstractParam>& param) const;

    private:

        /**
         * Nested base class for callback storage
         */
        class Callback {
        public:

            /**
             * Ctor
             *
             * @param callName The class name of the call.
             * @param funcName The name of the function.
             */
            Callback(const char *callName, const char *funcName)
                    : callName(callName), funcName(funcName) {
                // intentionally empty
            }

            /** Dtor */
            virtual ~Callback(void) {
                // intentionally empty
            }

            /**
             * Call this callback.
             *
             * @param owner The owning object.
             * @param call The calling call.
             *
             * @return The return value of the function.
             */
            virtual bool CallMe(Module *owner, Call& call) = 0;

            /**
             * Gets the call class name.
             *
             * @return The call class name.
             */
            inline const char * CallName(void) const {
                return this->callName;
            }

            /**
             * Gets the function name.
             *
             * @return The function name.
             */
            inline const char * FuncName(void) const {
                return this->funcName;
            }

        private:

            /** the class name of the call */
            vislib::StringA callName;

            /** the name of the function */
            vislib::StringA funcName;

        };

        /**
         * Nested class for callback storage
         */
        template<class C> class CallbackImpl : public Callback {
        public:

            /**
             * Ctor
             *
             * @param func The callback member of 'C'
             */
            CallbackImpl(const char *callName, const char *funcName,
                    bool (C::*func)(Call&)) : Callback(callName, funcName),
                    func(func) {
                // intentionally empty
            }

            /** Dtor. */
            virtual ~CallbackImpl(void) {
                // intentionally empty
            }

            /**
             * Call this callback.
             *
             * @param owner The owning object.
             * @param call The calling call.
             *
             * @return The return value of the function.
             */
            virtual bool CallMe(Module *owner, Call& call) {
                C *c = dynamic_cast<C*>(owner);
                if (c == NULL) return false;
                return (c->*func)(call);
            }

        private:

            /** The callback method */
            bool (C::*func)(Call&);

        };

#ifdef _WIN32
#pragma warning (disable: 4251)
#endif /* _WIN32 */
        /** The registered callbacks */
        vislib::Array<Callback*> callbacks;
#ifdef _WIN32
#pragma warning (default: 4251)
#endif /* _WIN32 */
    };


} /* end namespace core */
} /* end namespace megamol */

#endif /* MEGAMOLCORE_CALLEESLOT_H_INCLUDED */
