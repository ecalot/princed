// conf.cpp : implementation of the cMaps class
//

#include "stdafx.h"
#include "Princed V3.h"
#include "string.h"
#include "RegisterWIN32.h"

#define CONF_PROGRAM_NAME    "Princed V3";
#define CONF_PROGRAM_COMPANY "Princed Development Team";


class cConfig {
private:
	CRegisterWIN32 registro;

public:
	cConfig() {
		registro.Open(HKEY_LOCAL_MACHINE, _T("Software\\Princed Development Team\\Princed V3\\settings"), FALSE);
		checkConf();
	}

	~cConfig() {
		registro.Close();
	}

	void setSetting(char *setting,char *value) {		
		registro.Write(setting, value);
	}

	void getSetting(char *setting,char *value) {		
		registro.Read(setting, value);
	}

	void setSetting(char *setting,int value) {		
		registro.Write(setting, value);
	}

	void getSetting(char *setting,int &value) {		
		registro.Read(setting, value);
	}

	void setSetting(char *setting,char value) {		
		registro.Write(setting, value?"Yes":"No");
	}

	void getSetting(char *setting,char &value) {		
		char *sValue;
		registro.Read(setting, sValue);
		value=(strcmp(sValue,"Yes"));
	}

private:
	void checkConf() {
		//!ToDo!: buscar toda la configuracion y si no está seteada, poner el default


	}
}
;
