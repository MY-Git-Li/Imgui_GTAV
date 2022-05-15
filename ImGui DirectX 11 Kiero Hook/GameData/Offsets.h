#pragma once

#include "iostream"

using namespace std;

class Offsets
{

public:
	static int RID[3];
	static int InVehicle[2];
	static int SpecialAmmo[3];
	class Mask
	{
	public:
		const static string WorldMask;
		const static string BlipMask;
		const static string GlobalMask;
		const static string ReplayInterfaceMask;
		const static string NetworkPlayerMgrMask;
		const static string ViewPortMask;
		const static string CCameraMask;
		const static string AimingPedMask;
		const static string WeatherMask;
		const static string FriendListMask;
		const static string PickupDataMask;
		const static string UnkModelMask;
		const static string PlayerchatterNameMask;


	};
	class Weapon
	{
	public:
		static int AmmoInfo[4];
		static int AmmoModifier[3];
		static int NoSpread[4];
		static int NoRecoil[4];
		static int ImpactType[4];
		static int ImpactExplosion[4];
		static int Range[4];
		static int LockRange[4];
		static int ReloadMult[4];
		static int ReloadVehicleMult[4];

	};
};


