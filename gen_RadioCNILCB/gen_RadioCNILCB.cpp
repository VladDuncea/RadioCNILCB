/*

Winamp generic plugin template code.
This code should be just the basics needed to get a plugin up and running.
You can then expand the code to build your own plugin.

Updated details compiled June 2009 by culix, based on the excellent code examples
and advice of forum members Kaboon, kichik, baafie, burek021, and bananskib.
Thanks for the help everyone!

*/
#include "stdafx.h"
#include <windows.h>
#include "gen_RadioCNILCB.h"
#include <thread>
#include "wa_ipc.h"
#include <stdio.h>
#include <ctime>


#define WINAMP_PREV          40044
#define WINAMP_PLAY          40045
#define WINAMP_PAUSE         40046
#define WINAMP_STOP          40047
#define WINAMP_NEXT          40048

// these are callback functions/events which will be called by Winamp
int  init(void);
void config(void);
void quit(void);


// this structure contains plugin information, version, name...
// GPPHDR_VER is the version of the winampGeneralPurposePlugin (GPP) structure
winampGeneralPurposePlugin plugin = {
	GPPHDR_VER,  // version of the plugin, defined in "gen_myplugin.h"
	PLUGIN_NAME, // name/title of the plugin, defined in "gen_myplugin.h"
	init,        // function name which will be executed on init event
	config,      // function name which will be executed on config event
	quit,        // function name which will be executed on quit event
	0,           // handle to Winamp main window, loaded by winamp when this dll is loaded
	0            // hinstance to this dll, loaded by winamp when this dll is loaded
};


// event functions follow

int init() {

	//SendMessage(plugin.hwndParent, WM_WA_IPC, 0, IPC_SET_REPEAT);
	//SendMessage(plugin.hwndParent, WM_WA_IPC, 0, IPC_STARTPLAY);
	/*
	if (GetAsyncKeyState(VK_MEDIA_PREV_TRACK))
		SendMessage(winamp, WM_COMMAND, WINAMP_BUTTON1, 0);//Previous track


	if (GetAsyncKeyState(VK_MEDIA_PLAY_PAUSE))
	{
		if (SendMessage(winamp, WM_WA_IPC, 0, IPC_ISPLAYING) == 0) //not playing
			SendMessage(winamp, WM_COMMAND, WINAMP_BUTTON2, 0);//Play
		else
			SendMessage(winamp, WM_COMMAND, WINAMP_BUTTON3, 0);//Pause
	}
	else if (GetAsyncKeyState(VK_MEDIA_STOP))
		SendMessage(winamp, WM_COMMAND, WINAMP_BUTTON4, 0);//Stop
	else if (GetAsyncKeyState(VK_MEDIA_NEXT_TRACK))
		SendMessage(winamp, WM_COMMAND, WINAMP_BUTTON5, 0);//next track
	*/

	std::thread([&]
	{
		while (true)
		{
			time_t t = time(0);
			struct tm * now = localtime(  &t );
			//yer
			int eu = 0;
			//MessageBox(plugin.hwndParent, L""+eu, L"", MB_OK);
			if (now->tm_min > 50)
			{
				if (SendMessage(plugin.hwndParent, WM_WA_IPC, 0, IPC_ISPLAYING) == 3 || SendMessage(plugin.hwndParent, WM_WA_IPC, 0, IPC_ISPLAYING) == 0)
					SendMessage(plugin.hwndParent, WM_COMMAND, WINAMP_PLAY, 0);//Play
				//now->tm_min = 0;
				//now->tm_hour += 1;
			}
			else if (now->tm_min < 50)
			{
				//MessageBox(plugin.hwndParent, L"Config event triggered for gen_myplugin.", L"", MB_OK);
				if (SendMessage(plugin.hwndParent, WM_WA_IPC, 0, IPC_ISPLAYING) == 1)
					SendMessage(plugin.hwndParent, WM_COMMAND, WINAMP_PAUSE, 0);//Pause
			}
			
			time_t timeout_time_t = mktime(now);
			std::chrono::system_clock::time_point timeout_tp =
				std::chrono::system_clock::from_time_t(timeout_time_t);
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
	}).detach();
	//A basic messagebox that tells you the 'init' event has been triggered.
	//If everything works you should see this message when you start Winamp once your plugin has been installed.
	//You can change this later to do whatever you want (including nothing)
	//MessageBox(plugin.hwndParent, L"Init event triggered for gen_myplugin. Plugin installed successfully!", L"", MB_OK);
	return 0;
}

void config() 
{
	//A basic messagebox that tells you the 'config' event has been triggered.
	//You can change this later to do whatever you want (including nothing)
	MessageBox(plugin.hwndParent, L"Config event triggered for gen_myplugin.", L"", MB_OK);
	wchar_t msg[1024];

	int version = SendMessage(plugin.hwndParent, WM_WA_IPC, 0, IPC_GETVERSION);
	int majVersion = WINAMP_VERSION_MAJOR(version);
	int minVersion = WINAMP_VERSION_MINOR(version);

	wsprintf(msg, L"The version of Winamp is: %x\n"
		L"Major version: %x\nMinor version: %x\n",
		version,
		majVersion,
		minVersion
	);

	MessageBox(plugin.hwndParent, msg, L"Winamp Version", MB_OK);

	char *winampVersion;

	winampVersion = (char *)SendMessage(plugin.hwndParent, WM_WA_IPC, 0, IPC_GETVERSIONSTRING);
	MessageBoxA(NULL, winampVersion, "Winamp Version 2", MB_OK);

	enqueueFileWithMetaStruct eFWMS = { 0 };
	eFWMS.filename = "F:\\Vlad\\Muzica\\11.mp3";
	eFWMS.title = "Test It Good";
	eFWMS.length = 300;
	SendMessage(plugin.hwndParent, WM_WA_IPC, (WPARAM)&eFWMS, IPC_ENQUEUEFILE);
}

void quit() {
	//A basic messagebox that tells you the 'quit' event has been triggered.
	//If everything works you should see this message when you quit Winamp once your plugin has been installed.
	//You can change this later to do whatever you want (including nothing)
	//MessageBox(0, L"Quit event triggered for gen_myplugin.", L"", MB_OK);
}


// This is an export function called by winamp which returns this plugin info.
// We wrap the code in 'extern "C"' to ensure the export isn't mangled if used in a CPP file.
extern "C" __declspec(dllexport) winampGeneralPurposePlugin * winampGetGeneralPurposePlugin() {
	return &plugin;
}