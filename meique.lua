useQt5 = option("Qt5", "Use Qt5 instead of Qt4")
qtVersion = "4.8"
qtPrefix = "Qt"
if useQt5 then
    qtVersion = "5.0"
    qtPrefix = "Qt5"
end

qtCore = findPackage(qtPrefix.."Core", qtVersion, REQUIRED)
qtGui = findPackage(qtPrefix.."Gui", qtVersion, REQUIRED)
qtWidgets = useQt5:findPackage("Qt5Widgets", qtVersion, REQUIRED)
qtNetwork = findPackage(qtPrefix.."Network", qtVersion, REQUIRED)
icecc = findPackage("icecc", "0.9.7", REQUIRED)

addSubdirectory("src")
