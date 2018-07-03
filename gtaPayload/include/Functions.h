void givePedWeapon(Ped ped, Hash weapon) {
    WEAPON::GIVE_DELAYED_WEAPON_TO_PED(ped, weapon, 9999, true);
}
void playSound(char* sound)
{
		AUDIO::PLAY_SOUND_FRONTEND(-1, sound, "HUD_FRONTEND_DEFAULT_SOUNDSET", false);
}