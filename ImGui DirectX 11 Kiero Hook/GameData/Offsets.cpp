#include "Offsets.h"

const string Offsets::Mask::WorldMask = "48 8B 05 ?? ?? ?? ?? 45 ?? ?? ?? ?? 48 8B 48 08 48 85 C9 74 07";
const string Offsets::Mask::BlipMask = "4C 8D 05 ?? ?? ?? ?? 0F B7 C1";
const string Offsets::Mask::GlobalMask = "4C 8D 05 ?? ?? ?? ?? 4D 8B 08 4D 85 C9 74 11";
const string Offsets::Mask::ReplayInterfaceMask = "48 8D 0D ?? ?? ?? ?? 48 8B D7 E8 ?? ?? ?? ?? 48 8D 0D ?? ?? ?? ?? 8A D8 E8";
const string Offsets::Mask::NetworkPlayerMgrMask = "48 8B 0D ?? ?? ?? ?? E8 ?? ?? ?? ?? 48 8B C8 E8 ?? ?? ?? ?? 48 8B CF";
const string Offsets::Mask::ViewPortMask = "48 8B 15 ?? ?? ?? ?? 48 8D 2D ?? ?? ?? ?? 48 8B CD";
const string Offsets::Mask::CCameraMask = "48 8B 05 ?? ?? ?? ?? 4A 8B 1C F0";
const string Offsets::Mask::AimingPedMask = "48 8B 0D ?? ?? ?? ?? 48 85 C9 74 0C 48 8D 15 ?? ?? ?? ?? E8 ?? ?? ?? ?? 48 89 1D";
const string Offsets::Mask::WeatherMask = "48 83 EC ?? 8B 05 ?? ?? ?? ?? 8B 3D ?? ?? ?? ?? 49";
const string Offsets::Mask::FriendListMask = "48 8B 0D ?? ?? ?? ?? 8B C6 48 8D 5C 24 70";
const string Offsets::Mask::PickupDataMask = "48 8B 05 ?? ?? ?? ?? 48 8B 1C F8 8B";
const string Offsets::Mask::UnkModelMask = "4C 8B 15 ?? ?? ?? ?? 49 8B 04 D2 44 39 40 08";
const string Offsets::Mask::PlayerchatterNameMask = "48 8D 0D ?? ?? ?? ?? E8 ?? ?? ?? ?? 48 8D 0D ?? ?? ?? ?? 83";

int	Offsets::RID[3] = { 0x08, 0x10C8, 0x90 };
int	Offsets::InVehicle[2] = { 0x08, 0x14C7 };

int Offsets::SpecialAmmo[3] = { 0x08, 0x10C8, 0x218 };

int  Offsets::Weapon::AmmoInfo[4] = { 0x08, 0x10D8, 0x20, 0x60 };
int  Offsets::Weapon::AmmoModifier[3] = { 0x08, 0x10D0, 0x78 };
int  Offsets::Weapon::NoSpread[4] = { 0x8, 0x10D8, 0x20, 0x7C };
int  Offsets::Weapon::NoRecoil[4] = { 0x8, 0x10D8, 0x20, 0x2F4 };
int  Offsets::Weapon::ImpactType[4] = { 0x08, 0x10D8, 0x20, 0x20 };
int  Offsets::Weapon::ImpactExplosion[4] = { 0x08, 0x10D8, 0x20, 0x24 };
int  Offsets::Weapon::Range[4] = { 0x08, 0x10D8, 0x20, 0x28C };
int  Offsets::Weapon::LockRange[4] = { 0x08, 0x10D8, 0x20, 0x288 };
int  Offsets::Weapon::ReloadMult[4] = { 0x08, 0x10D8, 0x20, 0x134 };
int  Offsets::Weapon::ReloadVehicleMult[4] = { 0x08, 0x10D8, 0x20, 0x130 };