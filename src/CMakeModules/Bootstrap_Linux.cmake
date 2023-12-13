function (add_external_dependencies PROJECT)
    # No dependencies
    # add_dependencies(PROJECT boost)
endfunction()

set (Boost_NO_WARN_NEW_VERSIONS 1)
FIND_PACKAGE (Boost REQUIRED COMPONENTS thread REQUIRED)

find_package(LibVLC REQUIRED)

function(casparcg_copy_runtime_dependencies)
    # Nothing to copy
endfunction()