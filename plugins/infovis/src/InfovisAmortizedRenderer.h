#include "mmcore/CalleeSlot.h"
#include "mmcore/CallerSlot.h"
#include "mmcore/Module.h"
#include "mmcore/param/ParamSlot.h"
#include "mmcore/view/CallRender2D.h"
#include "mmcore/view/Renderer2DModule.h"
#include "vislib/graphics/gl/FramebufferObject.h"

#include "glowl/FramebufferObject.hpp"
#include "glm/matrix.hpp"
#include "vislib/graphics/gl/IncludeAllGL.h"
#include "vislib/graphics/gl/ShaderSource.h"

#include "Renderer2D.h"

namespace megamol {
namespace infovis {
    class InfovisAmortizedRenderer : public Renderer2D {
    public:
        static inline const char* ClassName() {
            return "InfovisAmortizedRenderer";
        }

        /**
         * Answer a human readable description of this module.
         *
         * @return A human readable description of this module.
         */
        static inline const char* Description(void) {
            return "Amortized Renderer.\n"
                   "Amortizes following Renderers to improve response time\n";
        }
        

        static inline bool IsAvailable(void) {
            return true;
        }

        /**
         * Initialises a new instance.
         */
        InfovisAmortizedRenderer(void);

        virtual ~InfovisAmortizedRenderer(void);

    protected:
        virtual bool create(void);

        virtual void release(void);

        virtual bool Render(core::view::CallRender2D& call);

        virtual bool GetExtents(core::view::CallRender2D& call);

        void setupBuffers();

        std::vector<glm::fvec3> calculateHammersley(int until, int ow, int oh);

        void makeShaders();

        void setupAccel(int approach, int ow, int oh, int ssLevel);

        void doReconstruction(int approach, int w, int h, int ssLevel);

        void resizeArrays(int approach, int w, int h, int ssLevel);

    private:
        // required Shaders for different kinds of reconstruction
        std::unique_ptr<vislib::graphics::gl::GLSLShader> pc_reconstruction_shdr_array[6];
        vislib::graphics::gl::ShaderSource vertex_shader_src;
        vislib::graphics::gl::ShaderSource fragment_shader_src;

        GLuint amortizedFboA = 0;
        GLuint amortizedMsaaFboA = 0;
        GLuint msImageArray = 0;
        GLuint imageArrayA = 0;
        GLuint ssboMatrices = 0;
        int frametype = 0;

        int oldApp = -1;
        int oldW = -1;
        int oldH = -1;
        int oldssLevel = -1;
        int oldaLevel = -1;

        GLint origFBO = 0;
        int framesNeeded = 1;
        GLfloat modelViewMatrix_column[16];
        GLfloat projMatrix_column[16];
        std::vector<glm::mat4> invMatrices;
        std::vector<glm::mat4> moveMatrices;
        std::vector<glm::fvec2> hammerPositions;
        std::vector<glm::vec3> camOffsets;

         float backgroundColor[4];

        megamol::core::CallerSlot nextRendererSlot;
        core::param::ParamSlot halveRes;
        core::param::ParamSlot approachSlot;
        core::param::ParamSlot superSamplingLevelSlot;
        core::param::ParamSlot amortLevel;
    };
} // namespace infovis
} // namespace megamol
