project( parseucd )

set( SOURCE_FILES
    parseucd/parseucd.cpp
)

add_executable( parseucd ${SOURCE_FILES} )

target_link_libraries( parseucd PRIVATE Qt5::Widgets )
