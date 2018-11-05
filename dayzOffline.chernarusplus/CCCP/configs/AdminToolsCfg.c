/*
	DayZ SA CCCP Mod
	
	FileName: AdminToolsCfg.c
	
	Usage:
	FNR:Description

	Authors: DayZ SA CCCP Mod Team and Community Contributors(see credits.md)
	FNR:ModTeamInfo
	
	This work is licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License. To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-sa/4.0/ or send a letter to Creative Commons, PO Box 1866, Mountain View, CA 94042, USA.
*/

	//All admin actions are sent as message to all admins in game
	bool AllAdminsMsg = true;
	
	void AdminTool()
	{
		// Admin List
		// USAGE: m_AdminList.Insert("player.GetIdentity().GetId()", "AdminName");
		
		m_AdminList.Insert("4Mqulobq2FoykRmOOJXBku52yO4K28dqL5b9VyTT+p0=", "DirtySanchez");
		Print("Class AdminTool:: Loading Scripted Mods...");
		
		// TP location List
		// Add every town and hotspot to list
		m_TPLocations.Insert( "Severograd", "8428.0 0.0 12767.1" );
		m_TPLocations.Insert( "Krasnostav", "11172.0 0.0 12314.1" );
		m_TPLocations.Insert( "Mogilevka", "7583.0 0.0 5159.4" );
		m_TPLocations.Insert( "Stary Sobor", "6072.0 0.0 7852.5" );
		m_TPLocations.Insert( "Msta", "11207.0 0.0 5380.54" );
		m_TPLocations.Insert( "Vybor", "3729.51 0.0 8935.56" );
		m_TPLocations.Insert( "Gorka", "9787.1 0.0 8767.19" );
		m_TPLocations.Insert( "Solnicni", "13393.1 0.0 6159.8" );
		m_TPLocations.Insert( "Airport Vybor", "4828.9 0.0 10219.5" );
		m_TPLocations.Insert( "Airport Balota", "4791.53 0.0 2557.47" );
		m_TPLocations.Insert( "Airport Krasnostav", "12004.3 0.0 12655.3" );
		m_TPLocations.Insert( "Chernogorsk Center", "6893.07 0.0 2618.13" );
		m_TPLocations.Insert( "Chernogorsk West", "6546.03 0.0 2317.56" );
		m_TPLocations.Insert( "Chernogorsk East", "7105.76 0.0 2699.53" );
		m_TPLocations.Insert( "Elektrozavodsk Center", "10459.8 0.0 2322.72" );
		m_TPLocations.Insert( "Elektrozavodsk East", "10414.5 0.0 2569.42" );
		m_TPLocations.Insert( "Berezino Center", "12296.9 0.0 9470.51" );
		m_TPLocations.Insert( "Berezino West", "12035.4 0.0 9143.49" );
		m_TPLocations.Insert( "Berezino East", "12932.3 0.0 10172.7" );
		m_TPLocations.Insert( "Svetlojarsk Center", "13835.3 0.0 13202.3" );
		m_TPLocations.Insert( "Zelenogorsk Center", "2660.99 0.0 5299.28" );
		m_TPLocations.Insert( "Zelenogorsk West", "2489.45 0.0 5080.41" );
		
		m_TPLocations.Insert( "PrisonBridge Shore", "2675 0 1978" );
		m_TPLocations.Insert( "PrisonBridge Island", "2651 0.0 1396" );
		m_TPLocations.Insert( "Cherno Center", "6893.07 0.0 2618.13" );
		m_TPLocations.Insert( "Cherno West", "6546.03 0.0 2317.56" );
		m_TPLocations.Insert( "Cherno East", "7105.76 0.0 2699.53" );
		m_TPLocations.Insert( "Zeleno Center", "2660.99 0.0 5299.28" );
		m_TPLocations.Insert( "Zeleno West", "2489.45 0.0 5080.41" );
		m_TPLocations.Insert( "Elektro Center", "10459.8 0.0 2322.72" );
		m_TPLocations.Insert( "Elektro East", "10414.5 0.0 2569.42" );
	}
