# install(FILES
#         parser.h
#         parserrunnable.h
#         qobjecthelper.h
#         serializer.h
#         serializerrunnable.h
#         qjson_export.h
#         DESTINATION ${INCLUDE_INSTALL_DIR}/qjson COMPONENT Devel
# )

set(qjson_MOC_HDRS
  parserrunnable.h
  serializerrunnable.h
)

qt4_wrap_cpp(qjson_MOC_SRCS ${qjson_MOC_HDRS})

set (qjson_SRCS
    parser.cpp
    qobjecthelper.cpp
    json_scanner.cpp
    json_parser.cc
    parserrunnable.cpp
    serializer.cpp
    serializerrunnable.cpp
)

add_library(qjson ${qjson_SRCS} ${qjson_MOC_SRCS})
target_link_libraries(qjson ${QT_LIBRARIES})

