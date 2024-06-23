include(CheckIPOSupported)
include(CheckCXXCompilerFlag)

check_cxx_compiler_flag("-march=native" CCFLAGS_NATIVE_ARCH)
check_cxx_compiler_flag("-ffp-contract=off" CCFLAGS_NO_FP_CONTRACT)
check_cxx_compiler_flag("-fno-fast-math" CCFLAGS_NO_FAST_MATH)
check_ipo_supported(RESULT CCFLAGS_IPO)

function(setup_compiler_flags target)
    if (CCFLAGS_NATIVE_ARCH)
        target_compile_options(${target} PUBLIC "-march=native")
    endif()
    if (CCFLAGS_NO_FP_CONTRACT)
        target_compile_options(${target} PUBLIC "-ffp-contract=off")
    endif()
    if (CCFLAGS_NO_FAST_MATH)
        target_compile_options(${target} PUBLIC "-fno-fast-math")
    endif()
    if (CCFLAGS_IPO)
        set_target_properties(${target} PROPERTIES
            INTERPROCEDURAL_OPTIMIZATION ON
        )
    endif()
endfunction()