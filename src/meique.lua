iceberg = Executable:new("iceberg")

if useQt5 then
    qt = "Qt5"
    qtWidgets = findPackage("Qt5Widgets")
    iceberg:use(qtWidgets)
else
    qt = "Qt"
end

openGL = findPackage("gl")
qtCore = findPackage(qt.."Core")
qtGui = findPackage(qt.."Gui")
qtGL = findPackage(qt.."OpenGL")
qtNetwork = findPackage(qt.."Network")
icecc = findPackage("icecc")
freetype = findPackage("freetype2", REQUIRED)


iceberg:use(qtCore)
iceberg:use(qtGui)
iceberg:use(openGL)
iceberg:use(qtGL)
iceberg:use(qtNetwork)
iceberg:use(icecc)
iceberg:use(freetype)
iceberg:addCustomFlags("-fPIC")

iceberg:addFiles([[
    job.cpp
    main.cpp
    monitor.cpp
    mainwindow.cpp
    hostinfo.cpp
    statusview.cpp
    detailedhostview.cpp
    hostlistview.cpp
    joblistview.cpp
    listview.cpp
    starview.cpp
]])
iceberg:addQtResource("icons.qrc")
iceberg:useQtAutomoc()
-- icecc uses dlopen but it's pkg-config doesn't tell this.
iceberg:addLinkLibraries("dl")

iceberg:install("bin")
iceberg:install("iceberg.desktop", "share/applications")
iceberg:install("../images/hi16-app-iceberg.png", "share/icons/hicolor/16x16/apps", "iceberg.png")
iceberg:install("../images/hi22-app-iceberg.png", "share/icons/hicolor/22x22/apps", "iceberg.png")
iceberg:install("../images/hi32-app-iceberg.png", "share/icons/hicolor/32x32/apps", "iceberg.png")
iceberg:install("../images/hi48-app-iceberg.png", "share/icons/hicolor/48x48/apps", "iceberg.png")
