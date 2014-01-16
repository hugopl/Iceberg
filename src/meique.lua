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

iceberg:use(qtCore)
iceberg:use(qtGui)
iceberg:use(qtNetwork)
useQt5:iceberg:use(qtWidgets)
iceberg:use(icecc)
iceberg:useQtAutomoc()
-- icecc uses dlopen but it's pkg-config doesn't tell this.
UNIX:iceberg:addLinkLibraries("dl")

iceberg:install("bin")
