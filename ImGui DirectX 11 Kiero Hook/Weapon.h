#pragma once
#include "includes.h"
class Weapon
{
public:
	static void FillCurrentAmmo();/// ������ǰ������ҩ
	static void InfiniteAmmo(bool isEnable);/// ���޵�ҩ
	static void NoReload(bool isEnable);/// ���軻��
	static void AmmoModifier(byte flag); //��ҩ�༭��Ĭ��0�����޵�ҩ1�����޵���2�����޵�ҩ�͵���3
	static void NoRecoil();/// �޺�����
	static void NoSpread(); /// ���ӵ���ɢ
	static void ImpactType(byte type);/// �����ӵ����ͣ�2:Fists��3:Bullet��5:Explosion
	static void ImpactExplosion(int id);/// �����ӵ�����
	static void Range(); /// �������
	static void ReloadMult(bool isEnable);/// �������ٻ���
};

