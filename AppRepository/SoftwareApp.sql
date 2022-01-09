CREATE TABLE "Application" (
	"AppID"	INTEGER,
	"AppName"	TEXT,
	"Developer"	TEXT,
	"License"	TEXT,
	"Category"	TEXT,
	"InternetConnection"	TEXT,
	"AppInfo"	TEXT,
	PRIMARY KEY("AppID" AUTOINCREMENT)
);

CREATE TABLE "Minimum_Req" (
	"AppID"	INTEGER,
	"GHzCPU"	REAL,
	"GPU"	TEXT,
	"GB_RAM"	REAL,
	"GB_HDStorage"	REAL
);

CREATE TABLE "OS" (
	"AppID"	INTEGER,
	"OS_Name"	NUMERIC,
	"ID_exec"	INTEGER,
	"Executable_Name"	TEXT
);

CREATE TABLE sqlite_sequence(name,seq);

INSERT INTO "main"."Application" ("AppID", "AppName", "Developer", "License", "Category", "InternetConnection", "AppInfo") VALUES ('1', 'Visual Studio Code', 'Microsoft', 'Free', 'Software Development', 'YES', 'Info about VS Code');
INSERT INTO "main"."Application" ("AppID", "AppName", "Developer", "License", "Category", "InternetConnection", "AppInfo") VALUES ('2', 'Visual Studio 2019', 'Microsoft', 'Free', 'Software Development', 'NO', 'Info about VS 2019');
INSERT INTO "main"."Application" ("AppID", "AppName", "Developer", "License", "Category", "InternetConnection", "AppInfo") VALUES ('3', 'Notepad++', 'Don Ho', 'Open-source', 'Text editor', 'NO', '-');
INSERT INTO "main"."Application" ("AppID", "AppName", "Developer", "License", "Category", "InternetConnection", "AppInfo") VALUES ('4', 'Adobe Photoshop', 'Adobe', 'Shareware', 'Other', 'YES', 'Info about Photoshop');
INSERT INTO "main"."Application" ("AppID", "AppName", "Developer", "License", "Category", "InternetConnection", "AppInfo") VALUES ('5', 'Github', 'Chris Wanstrath', 'Free', '-', 'YES', '-');



INSERT INTO "main"."Minimum_Req" ("AppID", "GHzCPU", "GPU", "GB_RAM", "GB_HDStorage") VALUES ('1', '1.6', 'AMD Radeon', '3.0', '5.0');
INSERT INTO "main"."Minimum_Req" ("AppID", "GHzCPU", "GPU", "GB_RAM", "GB_HDStorage") VALUES ('2', '1.8', 'GeForce RTX', '2.0', '10.0');
INSERT INTO "main"."Minimum_Req" ("AppID", "GHzCPU", "GPU", "GB_RAM", "GB_HDStorage") VALUES ('3', '1.2', '-', '4.0', '1.0');
INSERT INTO "main"."Minimum_Req" ("AppID", "GHzCPU", "GPU", "GB_RAM", "GB_HDStorage") VALUES ('4', '1.8', 'GeForce RTX', '-1.0', '10.0');
INSERT INTO "main"."Minimum_Req" ("AppID", "GHzCPU", "GPU", "GB_RAM", "GB_HDStorage") VALUES ('5', '1.0', '-', '4.0', '1.0');

INSERT INTO "main"."OS" ("AppID", "OS_Name", "ID_exec", "Executable_Name") VALUES ('1', 'Linux', '1', 'apps/1.deb');
INSERT INTO "main"."OS" ("AppID", "OS_Name", "ID_exec", "Executable_Name") VALUES ('1', 'MacOS', '2', 'apps/2.app');
INSERT INTO "main"."OS" ("AppID", "OS_Name", "ID_exec", "Executable_Name") VALUES ('2', 'Windows', '3', 'apps/3.exe');
INSERT INTO "main"."OS" ("AppID", "OS_Name", "ID_exec", "Executable_Name") VALUES ('3', 'MacOS', '', '');
INSERT INTO "main"."OS" ("AppID", "OS_Name", "ID_exec", "Executable_Name") VALUES ('4', 'Linux', '4', 'apps/4.deb');
INSERT INTO "main"."OS" ("AppID", "OS_Name", "ID_exec", "Executable_Name") VALUES ('5', 'Linux', '', '');