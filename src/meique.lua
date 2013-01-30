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
]])

iceberg:addQtResource("icons.qrc")

iceberg:usePackage(qtCore)
iceberg:usePackage(qtGui)
iceberg:usePackage(qtNetwork)
iceberg:usePackage(icecc)
iceberg:useQtAutomoc()
-- icecc uses dlopen but it's pkg-config doesn't tell this.
UNIX:iceberg:addLinkLibraries("dl")

iceberg:install("bin")
