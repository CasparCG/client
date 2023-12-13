set(CASPARCG_RUNTIME_DEPENDENCIES "" CACHE INTERNAL "")
set(CASPARCG_RUNTIME_DEPENDENCIES_DIRS "" CACHE INTERNAL "")

function(casparcg_add_runtime_dependency FILE_TO_COPY)
	set(CASPARCG_RUNTIME_DEPENDENCIES "${CASPARCG_RUNTIME_DEPENDENCIES}" "${FILE_TO_COPY}" CACHE INTERNAL "")
endfunction()
function(casparcg_add_runtime_dependency_dir FILE_TO_COPY)
	set(CASPARCG_RUNTIME_DEPENDENCIES_DIRS "${CASPARCG_RUNTIME_DEPENDENCIES_DIRS}" "${FILE_TO_COPY}" CACHE INTERNAL "")
endfunction()

