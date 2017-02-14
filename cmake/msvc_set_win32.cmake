function(msvc_set_win32 _application)
  if(MSVC)
     set_target_properties(${_application} PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS")
     set_target_properties(${_application} PROPERTIES LINK_FLAGS_RELWITHDEBINFO "/SUBSYSTEM:WINDOWS")
     set_target_properties(${_application} PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
     set_target_properties(${_application} PROPERTIES LINK_FLAGS_MINSIZEREL "/SUBSYSTEM:WINDOWS") 
  endif(MSVC)
endfunction(msvc_set_win32)
