 PROJECT(sxgoui)
 
# SET(wxWidgets_USE_LIBS  base zlib regex expat)
 FIND_PACKAGE(wxWidgets  REQUIRED base core aui html adv xml)
 
 IF(wxWidgets_FOUND)
   INCLUDE(${wxWidgets_USE_FILE})
 ELSE(wxWidgets_FOUND)
   # For convenience. When we cannot continue, inform the user
   MESSAGE("wxWidgets not found! Using manual configuration")
   include_directories( wxWidgets_INCLUDE_DIR)
   link_directories( wxWidgets_LIB_DIR)
 ENDIF(wxWidgets_FOUND)
 
 
   ADD_EXECUTABLE(sxgoui WIN32 
	sxgo_frame.cpp 
	sxgo_listing_window.cpp
	sxgo_view.cpp
	sxgo_app.cpp
	sxgo_doc.cpp
	sxgo_ram_window.cpp
	sxgo_variables_window.cpp
	sxgo_label_window.cpp
	
	sxgo_doc.hpp
	sxgo_view.hpp 
	sxgo_app.hpp
	sxgo_frame.hpp
	sxgo_listing_window.hpp
	sxgo_ram_window.hpp
	sxgo_variables_window.hpp
	sxgo_label_window.hpp
	)
   TARGET_LINK_LIBRARIES(sxgoui ${wxWidgets_LIBRARIES})

   INSTALL(TARGETS 
		sxgoui
	RUNTIME
	DESTINATION bin)
	
   INSTALL( FILES
            "C:/Program Files/Microsoft Visual Studio 9.0/VC/redist/x86/Microsoft.VC90.CRT/msvcm90.dll"
            "C:/Program Files/Microsoft Visual Studio 9.0/VC/redist/x86/Microsoft.VC90.CRT/msvcp90.dll"
            "C:/Program Files/Microsoft Visual Studio 9.0/VC/redist/x86/Microsoft.VC90.CRT/msvcr90.dll"
            "C:/Program Files/Microsoft Visual Studio 9.0/VC/redist/x86/Microsoft.VC90.CRT/Microsoft.VC90.CRT.manifest"
        DESTINATION bin
	)   

# Make sure the linker can find the sxsim library once it is built.
link_directories (${sxgo_BINARY_DIR}/sxsim)
include_directories( ${sxgo_SOURCE_DIR}/sxsim)

target_link_libraries (sxgoui sxsim)
