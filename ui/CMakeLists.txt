 PROJECT(sxgoui)
 SET(wxWidgets_USE_LIBS aui html adv core xml base tiff jpeg png zlib regex expat)
 FIND_PACKAGE(wxWidgets)
 IF(wxWidgets_FOUND)
   INCLUDE(${wxWidgets_USE_FILE})
   ADD_EXECUTABLE(sxgoui WIN32 sxgoui.cpp)
   # and for each of your dependant executable/library targets:
   TARGET_LINK_LIBRARIES(sxgoui ${wxWidgets_LIBRARIES})
 ELSE(wxWidgets_FOUND)
   # For convenience. When we cannot continue, inform the user
   MESSAGE("wxWidgets not found!")
 ENDIF(wxWidgets_FOUND)
