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
	
	FNR:LicenseInfo
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
	ref array<Man> admins = new array<Man>;
    // Misc Functions For Loading
    string SelectedPos, strCodeline, identityName, identityID;
	#include "$CurrentDir:\mpmissions\dayzOffline.chernarusplus\CCCP\configs\AdminToolsCfg.c"
	#include "$CurrentDir:\mpmissions\dayzOffline.chernarusplus\CCCP\overrides\MS_InvokeOnConnect.c"
	#include "$CurrentDir:\mpmissions\dayzOffline.chernarusplus\CCCP\tools\AdminTools.c"

//Messaging System
	//Join messages
	string admin = "false";
	string joinedServer = "false";
	vector lastLocation = "0 0 0";
	ref array<Man> clients = new array<Man>;
	
	//Param4<string, string, string, string> ClientParams;
	ref array<string, string, string, string, vector> ClientParams = new array<string, string, string, string, vector>; 
	// id, name, joined
	ref array<Param5<string, string, string, string, vector>> ClientsArray = new array<Param5<string, string, string, string, vector>>; 
	ref array<string> clientNames = new array<string>;
	ref array<string> clientIDs = new array<string>;
	//Number Of Players
	string msg = "";
	string playerCountMsg = "";
	//survivors name warning
	ref array<Man> survivors = new array<Man>;
	#include "$CurrentDir:\mpmissions\dayzOffline.chernarusplus\CCCP\configs\ServerMessagesCfg.c"
	#include "$CurrentDir:\mpmissions\dayzOffline.chernarusplus\CCCP\msg\ServerMessages.c"
