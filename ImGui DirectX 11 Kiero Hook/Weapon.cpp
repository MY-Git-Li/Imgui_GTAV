#include "Weapon.h"

void Weapon::FillCurrentAmmo()
{ // Ped µÃÂ
	long pWeapon_AmmoInfo = Memory::ReadMem<long>(DataManger::hProcess,Globals::WorldPTR,READ_WRITE_OFFSET_PARAMETER(Offsets::Weapon::AmmoInfo));

	int getMaxAmmo = Memory::ReadMem<int>(DataManger::hProcess, pWeapon_AmmoInfo + 0x28);

	long my_offset_0 = pWeapon_AmmoInfo;
	long my_offset_1;
	byte ammo_type;

	do
	{
		my_offset_0 = Memory::ReadMem<long>(DataManger::hProcess, my_offset_0 + 0x08);
		my_offset_1 = Memory::ReadMem<long>(DataManger::hProcess, my_offset_0 + 0x00);

		if (my_offset_0 == 0 || my_offset_1 == 0)
		{
			return;
		}

		ammo_type = Memory::ReadMem<byte>(DataManger::hProcess, my_offset_1 + 0x0C);

	} while (ammo_type == 0x00);

	Memory::WriteMem<int>(DataManger::hProcess,my_offset_1 + 0x18, getMaxAmmo);
}

void Weapon::InfiniteAmmo(bool isEnable)
{
	if (isEnable)
	{
		long addrAmmo =DataManger::FindPattern("41 2B D1 E8");
		if (addrAmmo == 0)
		{
			addrAmmo = DataManger::FindPattern("90 90 90 E8");
		}
		byte offset[] = { 0x90,0x90,0x90 };
		Memory::WriteMem<byte[]>(DataManger::hProcess, addrAmmo, READ_WRITE_OFFSET_PARAMETER(offset));
	}
	else
	{
		long addrAmmo = DataManger::FindPattern("41 2B D1 E8");
		if (addrAmmo == 0)
		{
			addrAmmo = DataManger::FindPattern("90 90 90 E8");
		}
		byte offset[] = { 0x41, 0x2B, 0xD1 };
		Memory::WriteMem<byte[]>(DataManger::hProcess,addrAmmo, READ_WRITE_OFFSET_PARAMETER(offset));
	}
}

void Weapon::NoReload(bool isEnable)
{
	if (isEnable)
	{
		long addrAmmo = DataManger::FindPattern("41 2B C9 3B C8 0F");
		if (addrAmmo == 0)
		{
			addrAmmo = DataManger::FindPattern("90 90 90 3B C8 0F");
		}
		byte offset[] = { 0x90, 0x90, 0x90 };
		DataManger::Write<byte[]>(addrAmmo, READ_WRITE_OFFSET_PARAMETER(offset));
	}
	else
	{
		long addrAmmo = DataManger::FindPattern("41 2B C9 3B C8 0F");
		if (addrAmmo == 0)
		{
			addrAmmo = DataManger::FindPattern("90 90 90 3B C8 0F");
		}
		byte offset[] = { 0x41, 0x2B, 0xC9 };
		DataManger::Write(addrAmmo, READ_WRITE_OFFSET_PARAMETER(offset));
	}
}

void Weapon::AmmoModifier(byte flag)
{
	DataManger::Write<byte>(Globals::WorldPTR, READ_WRITE_OFFSET_PARAMETER(Offsets::Weapon::AmmoModifier), flag);
}

void Weapon::NoRecoil()
{
	DataManger::Write<float>(Globals::WorldPTR, READ_WRITE_OFFSET_PARAMETER(Offsets::Weapon::NoRecoil), 0.0f);
}

void Weapon::NoSpread()
{
	DataManger::Write<float>(Globals::WorldPTR, READ_WRITE_OFFSET_PARAMETER(Offsets::Weapon::NoSpread), 0.0f);
}

void Weapon::ImpactType(byte type)
{
	DataManger::Write<byte>(Globals::WorldPTR, READ_WRITE_OFFSET_PARAMETER(Offsets::Weapon::ImpactType), type);
}

void Weapon::ImpactExplosion(int id)
{
	DataManger::Write<int>(Globals::WorldPTR, READ_WRITE_OFFSET_PARAMETER(Offsets::Weapon::ImpactExplosion), id);
}

void Weapon::Range()
{
	DataManger::Write<float>(Globals::WorldPTR, READ_WRITE_OFFSET_PARAMETER(Offsets::Weapon::Range), 2000.0f);
	DataManger::Write<float>(Globals::WorldPTR, READ_WRITE_OFFSET_PARAMETER(Offsets::Weapon::LockRange), 1000.0f);
}

void Weapon::ReloadMult(bool isEnable)
{
	if (isEnable)
	{
		DataManger::Write<float>(Globals::WorldPTR, READ_WRITE_OFFSET_PARAMETER(Offsets::Weapon::ReloadMult), 4.0f);
	}
	else
	{
		DataManger::Write<float>(Globals::WorldPTR, READ_WRITE_OFFSET_PARAMETER(Offsets::Weapon::ReloadMult), 1.0f);
	}
}
