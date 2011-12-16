qtCore = findPackage("QtCore", "4.7", REQUIRED)
qtGui = findPackage("QtGui", "4.7", REQUIRED)
qtNetwork = findPackage("QtNetwork", "4.7", REQUIRED)
icecc = findPackage("icecc", "0.9.7", REQUIRED)

addSubDirectory("src")
