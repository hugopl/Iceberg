find_package(Icecream REQUIRED)

if (QT5)
  find_package(Qt5Widgets REQUIRED)
  set(CMAKE_AUTOMOC TRUE)
  set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif(QT5)

add_definitions (${QT_DEFINITIONS} )

include_directories(${CMAKE_CURRENT_BINARY_DIR}
                    ${LIBICECREAM_INCLUDE_DIR}
                    ${QT_INCLUDE_DIR}
                    ${QT_QTCORE_INCLUDE_DIR}
                    ${QT_QTGUI_INCLUDE_DIR}
                    ${QT_QTNETWORK_INCLUDE_DIR})

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

if (QT5)
  qt5_add_resources(iceberg_RCC icons.qrc)
else(QT5)
  qt4_automoc(${iceberg_SRCS})
  qt4_add_resources(iceberg_RCC icons.qrc)
endif(QT5)

add_executable(iceberg ${iceberg_SRCS} ${iceberg_RCC})

set(ICEBERG_LIBS ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${LIBICECREAM_LIBRARIES} ${QT_QTNETWORK_LIBRARY} ${CMAKE_DL_LIBS})
if (QT5)
  qt5_use_modules(iceberg Widgets)
  set (ICEBERG_LIBS ${ICEBERG_LIBS} ${Qt5Widgets_LIBRARIES})
endif(QT5)

target_link_libraries(iceberg ${ICEBERG_LIBS})

########### install files ###############

install(TARGETS iceberg DESTINATION bin)
install(FILES iceberg.desktop DESTINATION share/applications )
install(FILES ../images/hi16-app-iceberg.png DESTINATION share/icons/hicolor/16x16/apps RENAME iceberg.png)
install(FILES ../images/hi22-app-iceberg.png DESTINATION share/icons/hicolor/22x22/apps RENAME iceberg.png)
install(FILES ../images/hi32-app-iceberg.png DESTINATION share/icons/hicolor/32x32/apps RENAME iceberg.png)
install(FILES ../images/hi48-app-iceberg.png DESTINATION share/icons/hicolor/48x48/apps RENAME iceberg.png)

