FIND_PACKAGE(PythonLibs)
INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})

 ADD_LIBRARY(pysix SHARED
	python_wrapper.cpp
	)
SET_TARGET_PROPERTIES(pysix PROPERTIES SUFFIX  ".pyd")


# Make sure the linker can find the sxsim library once it is built.
link_directories (${sxgo_BINARY_DIR}/sxsim)
include_directories( ${sxgo_SOURCE_DIR}/sxsim)

target_link_libraries (pysix sxsim ${PYTHON_LIBRARY})
