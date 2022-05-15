#pragma once
#include "includes.h"
class Weapon
{
public:
	static void FillCurrentAmmo();/// 补满当前武器弹药
	static void InfiniteAmmo(bool isEnable);/// 无限弹药
	static void NoReload(bool isEnable);/// 无需换弹
	static void AmmoModifier(byte flag); //弹药编辑，默认0，无限弹药1，无限弹夹2，无限弹药和弹夹3
	static void NoRecoil();/// 无后坐力
	static void NoSpread(); /// 无子弹扩散
	static void ImpactType(byte type);/// 启用子弹类型，2:Fists，3:Bullet，5:Explosion
	static void ImpactExplosion(int id);/// 设置子弹类型
	static void Range(); /// 武器射程
	static void ReloadMult(bool isEnable);/// 武器快速换弹
};

