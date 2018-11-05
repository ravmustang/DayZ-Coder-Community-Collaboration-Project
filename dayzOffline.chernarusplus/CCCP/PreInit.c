/*
	DayZ SA CCCP Mod
	
	FileName: PreInit.c
	
	Usage:
	Very easy feature addition
	
	//name
		init variables, arrays and any other needed
		#include config file
		#include function

	Authors: DayZ SA CCCP Mod Team and Community Contributors(see credits.md)
	FNR:ModTeamInfo
	
	This work is licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License. To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-sa/4.0/ or send a letter to Creative Commons, PO Box 1866, Mountain View, CA 94042, USA.
*/


//Debugging
	#include "$CurrentDir:\mpmissions\dayzOffline.chernarusplus\CCCP\configs\DebuggingCfg.c"
	
//Admin Tools
	//UID, name
	ref map<string, string>	m_AdminList = new map<string, string>;
	//name of town, pos
	ref map<string, vector>	m_TPLocations = new map<string, vector>;
	bool m_FreeCamera;
	bool m_IsDebugRunning = false;
	string godMode = "false";
	ref array<Man> m_Admins = new array<Man>;
	TStringArray ChatParseArray = new TStringArray;
    // Misc Functions For Loading
    string SelectedPos, strCodeline, identityName, identityID;
	#include "$CurrentDir:\mpmissions\dayzOffline.chernarusplus\CCCP\configs\AdminToolsCfg.c"
	#include "$CurrentDir:\mpmissions\dayzOffline.chernarusplus\CCCP\tools\AdminTools.c"
	#include "$CurrentDir:\mpmissions\dayzOffline.chernarusplus\CCCP\overrides\MS_InvokeOnConnect.c"
	#include "$CurrentDir:\mpmissions\dayzOffline.chernarusplus\CCCP\overrides\MS_InvokeOnDisconnect.c"

//Messaging System
	//Join messages
	string admin = "false";
	string joinedServer = "false";
	vector lastLocation = "0 0 0";
	ref array<Man> clients = new array<Man>;
	//Number Of Players
	string msg = "";
	string playerCountMsg = "";
	//survivors name warning
	ref array<Man> survivors = new array<Man>;
	#include "$CurrentDir:\mpmissions\dayzOffline.chernarusplus\CCCP\configs\ServerMessagesCfg.c"
	#include "$CurrentDir:\mpmissions\dayzOffline.chernarusplus\CCCP\msg\ServerMessages.c"
