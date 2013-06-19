freetype = findPackage("freetype2", REQUIRED)

iceberg = Executable:new("iceberg")
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
blingblingview.cpp

core/frustum.cpp
core/fxfont.cpp
core/plane.cpp
core/quadtree.cpp
core/regex.cpp
core/resource.cpp
core/shader.cpp
core/stringhash.cpp
core/texture.cpp
core/vbo.cpp
core/vectors.cpp
]])

iceberg:addQtResource("icons.qrc")

iceberg:usePackage(qtCore)
iceberg:usePackage(qtGui)
iceberg:usePackage(qtGL)
iceberg:usePackage(qtNetwork)
iceberg:usePackage(icecc)
iceberg:usePackage(freetype)
iceberg:useQtAutomoc()
-- icecc uses dlopen but it's pkg-config doesn't tell this.
UNIX:iceberg:addLinkLibraries("dl")

iceberg:install("bin")
