#include "lib.h"
#include "natives.h"
#include "base.h"
#include "drawFunctions.h"
#include "Functions.h"

bool init = false;
int frameCount = 0;



//Menu Options
bool PlayerOptions[50];


bool ShowMessageDialog(char* Title, char* Message)
{
	SYSTEM::WAIT(100);
	bool Exit = false;
	bool Cancel = false;
	while (!Exit)
	{
		if (CONTROLS::IS_DISABLED_CONTROL_JUST_PRESSED(0, Button_X))
			Exit = true;
		else if (CONTROLS::IS_DISABLED_CONTROL_JUST_PRESSED(0, Button_Circle))
		{
			Exit = true;
			Cancel = true;
		}

		GRAPHICS::DRAW_RECT(0.5, 0.5, 0.4, 0.3, 0, 0, 0, 120);//Main
		GRAPHICS::DRAW_RECT(0.5, 0.39, 0.38, 0.05, 0, 0, 0, 220);
		GRAPHICS::DRAW_RECT(0.5, 0.53, 0.38, 0.2, 0, 0, 0, 220);
		DRAW_TEXT(Title, 0.5, 0.39 - 0.02, 0.4, 0.4, 0, true, false, true, false, 255, 255, 255, 255);
		DRAW_TEXT(Message, 0.5, 0.53 - 0.04, 0.37, 0.37, 0, true, false, true, false, 255, 255, 255, 255);
		DRAW_TEXT("Are you sure you want to continue?", 0.5, 0.53 - 0.01, 0.37, 0.37, 0, true, false, true, false, 255, 255, 255, 255);

		DRAW_TEXT("Yes", 0.5 - 0.05, 0.53 + 0.03, 0.37, 0.37, 0, true, false, true, false, 255, 255, 255, 255);
		DRAW_TEXT("No", 0.5 + 0.05, 0.53 + 0.03, 0.37, 0.37, 0, true, false, true, false, 255, 255, 255, 255);
		SYSTEM::WAIT(10);
	}
	return !Cancel;
}


void menuActions(){
	UI::HIDE_HELP_TEXT_THIS_FRAME();
	CAM::SET_CINEMATIC_BUTTON_ACTIVE(0);
    CONTROLS::DISABLE_CONTROL_ACTION(0, Button_Dpad_Up, true);
    CONTROLS::DISABLE_CONTROL_ACTION(0, Button_Dpad_Down, true);
    CONTROLS::DISABLE_CONTROL_ACTION(0, Button_Dpad_Right, true);
    CONTROLS::DISABLE_CONTROL_ACTION(0, Button_Dpad_Left, true);
    CONTROLS::DISABLE_CONTROL_ACTION(0, Button_Dpad_Right_2, true);
    CONTROLS::DISABLE_CONTROL_ACTION(0, Button_Dpad_Left_2, true);
    CONTROLS::DISABLE_CONTROL_ACTION(0, Button_Circle, true);
    CONTROLS::DISABLE_CONTROL_ACTION(0, Button_X, true);
    CONTROLS::SET_INPUT_EXCLUSIVE(0, Button_Cirlce_2);
    CONTROLS::SET_INPUT_EXCLUSIVE(0, Button_Dpad_Right);
    CONTROLS::SET_INPUT_EXCLUSIVE(0, Button_Dpad_Left);
}
void OtherLoop(Player player,Ped ped){
	if(PlayerOptions[0]){//GodMode
		PLAYER::SET_PLAYER_INVINCIBLE(player, true);
        PED::SET_PED_CAN_BE_KNOCKED_OFF_VEHICLE(ped, 1);
        ENTITY::SET_ENTITY_INVINCIBLE(ped, true);
        PED::SET_PED_CAN_RAGDOLL(ped, false);
        PED::SET_PED_CONFIG_FLAG(ped, 32, false);
    }else{
		PLAYER::SET_PLAYER_INVINCIBLE(player, false);
        PED::SET_PED_CAN_BE_KNOCKED_OFF_VEHICLE(ped, 0);
        ENTITY::SET_ENTITY_INVINCIBLE(ped, false);
        PED::SET_PED_CAN_RAGDOLL(ped, true);
        PED::SET_PED_CONFIG_FLAG(ped, 32, true);
	}
	if(PlayerOptions[1]){//Invisible
			ENTITY::SET_ENTITY_VISIBLE(PLAYER::PLAYER_PED_ID(), 0,0);
    }else{
	       ENTITY::SET_ENTITY_VISIBLE(PLAYER::PLAYER_PED_ID(), 1,1);
	}
	if(PlayerOptions[2]){//Unlimited Ammo
		WEAPON::SET_PED_INFINITE_AMMO_CLIP(ped, true);
	}else{
		WEAPON::SET_PED_INFINITE_AMMO_CLIP(ped, false);
	}
	if(PlayerOptions[3]){//No Cops
		PLAYER::SET_MAX_WANTED_LEVEL(0);               
	}else{
		PLAYER::SET_MAX_WANTED_LEVEL(5);
	}
	if(PlayerOptions[4]){//Heat Vision
		GRAPHICS::SET_SEETHROUGH(true);
	}else{
		GRAPHICS::SET_SEETHROUGH(false);
	}
	if(PlayerOptions[5]){//Special Vision
		GRAPHICS::SET_TIMECYCLE_MODIFIER("MP_heli_cam");
	}else{
		GRAPHICS::SET_TIMECYCLE_MODIFIER("DEFAULT");
	}
	
}
void SelectedOption(){
	SelectedIndexSize = 0.6;
	IncreaseSelectedIndexSize = true;
	DecreaseSelectedIndexSize = true;
	
	Vector3 MyCoordsRightNow = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), false);
	Vehicle vehID = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false);
	Ped pedID = PLAYER::PLAYER_PED_ID();
	Ped TempID = 0;
	Vector3 TargetCoords;
	Blip WaypointID = 0;
	float ZAxis = 0;
	int NumberOfOptions = 0;
	int Value = 0;
	int NetworkHandle = 0;

	int NumberOfNearbyVehiclesFound = 0;

	switch (CurrentMenuID){
		case 0://Main Menu
		EnterSubMenu(CurrentSelectedIndex+1);
		break;
		case 1:
#pragma region Player
		switch (CurrentSelectedIndex)
		{
		case 0://God
			InvertOption("Player", "Godmode");
			if (!IsOptionChecked("Player", "Godmode")){ PlayerOptions[0] = false; }else{ PlayerOptions[0] = true; }			
		break;
		case 1://Insiible
			InvertOption("Player", "Invisible");
			if (!IsOptionChecked("Player", "Invisible")){ PlayerOptions[1] = false; }else{ PlayerOptions[1] = true; }
		break;
		case 2://Unlimited Ammo
		InvertOption("Player", "Unlimited Ammo");
		if (!IsOptionChecked("Player", "Unlimited Ammo")){ PlayerOptions[2] = false; }else{ PlayerOptions[2] = true; }
		break;
		case 3://No Cops
		InvertOption("Player", "No Cops");
		if (!IsOptionChecked("Player", "No Cops")){ PlayerOptions[3] = false; }else{ PlayerOptions[3] = true; }
		break;
		case 4://Heat Vision
		InvertOption("Player", "Heat Vision");
		if (!IsOptionChecked("Player", "Heat Vision")){ PlayerOptions[4] = false; }else{ PlayerOptions[4] = true; }
		break;
		case 5://Secial Vision
		InvertOption("Player", "Special Vision");
		if (!IsOptionChecked("Player", "Special Vision")){ PlayerOptions[5] = false; }else{ PlayerOptions[5] = true; }
		break;
		}
		break;
		#pragma region Menu2
		case 2:
		break;
#pragma region Menu3
		case 3:
		break;

	}
}

void monitorButtons(){

	if (CONTROLS::IS_DISABLED_CONTROL_JUST_PRESSED(0, Button_Dpad_Down)){
				SelectedIndexSize = 0.4;
				IncreaseSelectedIndexSize = true;
				playSound("NAV_UP_DOWN");
				do
				{
					CurrentSelectedIndex++;
					if (CurrentSelectedIndex >= Menus[CurrentMenuID].Size)
						CurrentSelectedIndex = 0;
				} while (Menus[CurrentMenuID].Options[CurrentSelectedIndex].Type == Disabled);
			}
			if (CONTROLS::IS_DISABLED_CONTROL_JUST_PRESSED(0, Button_Dpad_Up)){
				playSound("NAV_UP_DOWN");
				SelectedIndexSize = 0.4;
				IncreaseSelectedIndexSize = true;
				do
				{
					CurrentSelectedIndex--;
					if (CurrentSelectedIndex < 0)
						CurrentSelectedIndex = Menus[CurrentMenuID].Size - 1;
				} while (Menus[CurrentMenuID].Options[CurrentSelectedIndex].Type == Disabled);
			}
			if (CONTROLS::IS_DISABLED_CONTROL_JUST_PRESSED(0, Button_X))
			{
				if (Menus[CurrentMenuID].Options[CurrentSelectedIndex].Type != Disabled)
				{
					SelectedOption();
					playSound("SELECT");


				}
			}
			if (CONTROLS::IS_DISABLED_CONTROL_JUST_PRESSED(0, Button_Circle))
			{
					playSound("Back");
				if (CurrentMenuLayer != 0)
				{
					if (CurrentMenuLayer == 1)
					{
						CurrentSelectedIndex = CurrentMenuID - 1;
						CurrentMenuID = 0;
						CurrentMenuLayer = 0;
					}
					else
					{
						CurrentSelectedIndex = PreviousSelectedIndex[CurrentMenuLayer - 1];
						CurrentMenuID = PreviousMenuID[CurrentMenuLayer - 1];
						CurrentMenuLayer--;
					}
				}
				else
				{
					
					open = false;
				}
		}
}

void drawMenu(){
		GRAPHICS::DRAW_RECT(PixelX(Menu_X), PixelY(360), PixelX(275), PixelY(720), 0, 0, 0, 150);//Background
		int Select = CurrentSelectedIndex;
		if (CurrentSelectedIndex >= 20)
			Select = 19;
		
		DrawTitle(Menus[CurrentMenuID].MenuName);

		GRAPHICS::DRAW_RECT(PixelX(Menu_X), PixelY(90) + PixelY(30 * Select), PixelX(275), PixelY(30), 0, 0, 0, 200);//Select

							int Start = 0;
		if (CurrentSelectedIndex >= 20)
			Start = CurrentSelectedIndex - 19;

		for (int i = 0; i < 20; i++)
		{
			if (i + Start == CurrentSelectedIndex)
			{
				if (DecreaseSelectedIndexSize)
				{
					SelectedIndexSize -= 0.04;
					if (SelectedIndexSize < 0.4)
					{
						SelectedIndexSize = 0.4;
						DecreaseSelectedIndexSize = false;
					}
				}
				else if (IncreaseSelectedIndexSize)
				{
					SelectedIndexSize += 0.04;
					if (SelectedIndexSize > 0.6)
					{
						SelectedIndexSize = 0.6;
						IncreaseSelectedIndexSize = false;
					}
				}
				DrawOption(Menus[CurrentMenuID].Options[i + Start].Name, i, Menus[CurrentMenuID].Options[i + Start].Type, true);
			}
			else
				DrawOption(Menus[CurrentMenuID].Options[i + Start].Name, i, Menus[CurrentMenuID].Options[i + Start].Type, false);
		}
		char buffer[15];
		sprintf(buffer, "%i/%i", CurrentSelectedIndex + 1, Menus[CurrentMenuID].Size);
		DRAW_TEXT(buffer, PixelX(Menu_X), PixelY(75) + PixelY(30 * 20), 0.4, 0.4, 0, true, false, true, false, 240, 240, 240, 240);
}
extern "C" void _main(void) {
	if (!init) {
		initLibs();
		GenerateMenus();

		init = true;
	}
	

	int newFrameCount = GAMEPLAY::GET_FRAME_COUNT();
	if (newFrameCount > frameCount) {
		frameCount = newFrameCount;
		
			if(!open)	{
					if (CONTROLS::IS_DISABLED_CONTROL_PRESSED(0, Button_Square) && CONTROLS::IS_DISABLED_CONTROL_PRESSED(0, Button_Dpad_Right_2)){
						playSound("YES");
						SelectedIndexSize = 0.6;
						CurrentMenuID = 0;
						CurrentSelectedIndex = 0;
						CurrentMenuLayer = 0;
						IncreaseSelectedIndexSize = true;
						open = true;
	
					}
			}else{
					monitorButtons();
					drawMenu();
					menuActions();
				}
				Ped playerPed = PLAYER::PLAYER_PED_ID();
				Player player = PLAYER::PLAYER_ID();
				OtherLoop(player,playerPed);
	}
}