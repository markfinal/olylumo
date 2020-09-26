find_package(TBB QUIET)

add_library(RayTrace SHARED)
target_sources(RayTrace PRIVATE
    source/hitablelist.cpp
    source/image.cpp
    source/lambertian.cpp
    source/metal.cpp
    source/quadratic.cpp
    source/ray.cpp
    source/raycast.cpp
    source/rgba.cpp
    source/scene.cpp
    source/sphere.cpp
    source/utils.cpp
    source/vec4.cpp
)
target_include_directories(RayTrace PUBLIC include)
target_compile_features(RayTrace PRIVATE cxx_final cxx_override cxx_nonstatic_member_init cxx_nullptr cxx_auto_type)
set_target_properties(RayTrace PROPERTIES CXX_VISIBILITY_PRESET hidden)
if(TBB_FOUND)
    target_compile_definitions(RayTrace PRIVATE USE_TBB)
endif()
