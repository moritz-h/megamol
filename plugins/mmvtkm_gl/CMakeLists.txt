# MegaMol
# Copyright (c) 2021, MegaMol Dev Team
# All rights reserved.
#

megamol_plugin(mmvtkm_gl
  BUILD_DEFAULT ON
  DEPENDS_FEATURES
    opengl
    vtkm
  DEPENDS_PLUGINS
    mmstd
    mmstd_gl
    mesh_gl
    mmadios
    mmvtkm)

if (mmvtkm_gl_PLUGIN_ENABLED)
  find_package(VTKm CONFIG REQUIRED)
  target_link_libraries(mmvtkm_gl
    PRIVATE
      vtkm::cont
      vtkm::filter
      vtkm::rendering
      vtkm::worklet)
endif ()
