find_package(Icecream REQUIRED)

add_definitions (${QT_DEFINITIONS} -DQT3_SUPPORT_WARNINGS -DQT3_SUPPORT)

include_directories(${CMAKE_CURRENT_BINARY_DIR}
                    ${LIBICECREAM_INCLUDE_DIR}
                    ${QT_INCLUDE_DIR}
                    ${QT_QTCORE_INCLUDE_DIR}
                    ${QT_QTGUI_INCLUDE_DIR}
                    ${QT_QTNETWORK_INCLUDE_DIR}
                    ${QT_QT3SUPPORT_INCLUDE_DIR})

set(iceberg_SRCS
  main.cpp
  job.cpp
  monitor.cpp
  mainwindow.cpp
  hostinfo.cpp
  statusview.cpp
  detailedhostview.cpp
  hostlistview.cpp
  joblistview.cpp
  listview.cpp
  starview.cpp
)

qt4_automoc(${iceberg_SRCS})
qt4_add_resources(iceberg_RCC icons.qrc)
add_executable(iceberg ${iceberg_SRCS} ${iceberg_RCC})

target_link_libraries(iceberg ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${LIBICECREAM_LIBRARIES} ${QT_QTNETWORK_LIBRARY} ${QT_QT3SUPPORT_LIBRARY} ${CMAKE_DL_LIBS})

########### install files ###############

install(TARGETS iceberg DESTINATION bin)
install(FILES iceberg.desktop DESTINATION share/applications )
install(FILES ../images/hi16-app-iceberg.png DESTINATION share/icons/hicolor/16x16/apps RENAME iceberg.png)
install(FILES ../images/hi22-app-iceberg.png DESTINATION share/icons/hicolor/22x22/apps RENAME iceberg.png)
install(FILES ../images/hi32-app-iceberg.png DESTINATION share/icons/hicolor/32x32/apps RENAME iceberg.png)
install(FILES ../images/hi48-app-iceberg.png DESTINATION share/icons/hicolor/48x48/apps RENAME iceberg.png)

