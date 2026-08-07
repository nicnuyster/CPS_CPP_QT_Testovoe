
build/
obsidian_docks/

cmake.txt
src/
main.h
main.cpp

app/ все сюда
 application.cpp
 application.h
 
db/
	models/ классы данных Доктор, кабинеты и т.п. 
	repos/ чистые sql транзакции
	servises/ оркестратор и "бизнес логика"
	controllers/ UI к сервисам
ui/
	mainwindow.cpp
	mainwindow.h
	anotherwidget.cpp

utils/
	logger.cpp логгер всего приложения
	debuginfo.cpp версии всего (только для дебага)

config/

migrations/ миграции
	001_creat_table.sql
