iceberg = Executable:new("iceberg")
iceberg:addFiles([[
job.cc
main.cpp
monitor.cc
mainwindow.cc
hostinfo.cc
statusview.cc
detailedhostview.cc
hostlistview.cc
joblistview.cc
listview.cc
ganttstatusview.cc
summaryview.cc
starview.cc
poolview.cc
]])

iceberg:addQtResource("icons.qrc")

iceberg:usePackage(qtCore)
iceberg:usePackage(qtGui)
iceberg:usePackage(qtNetwork)
iceberg:usePackage(qt3Support)
iceberg:usePackage(icecc)
iceberg:useQtAutomoc()

iceberg:install("bin")
