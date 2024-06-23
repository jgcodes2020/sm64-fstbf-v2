add_executable(sm64-fstbf-v2
    "formatting.hpp"
    "main.cpp"
    "platform.cpp"
    "platform.hpp"
    "surface.hpp"
    "trig.cpp"
    "trig.hpp"
    "vmath.hpp"
)
set_target_properties(sm64-fstbf-v2 PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)
add_sycl_to_target(TARGET sm64-fstbf-v2)