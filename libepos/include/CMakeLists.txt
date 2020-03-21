set(hdrs 
  ${CMAKE_CURRENT_SOURCE_DIR}/EPOS.h
  ${CMAKE_CURRENT_SOURCE_DIR}/EPOS_defines.h
  ${CMAKE_CURRENT_SOURCE_DIR}/EPOSException.h
  ${CMAKE_CURRENT_SOURCE_DIR}/EPOSObjectDictionary_defines.h
)

set(install_hdrs ${hdrs})


set(headers ${hdrs} PARENT_SCOPE)