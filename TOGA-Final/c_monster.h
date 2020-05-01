#pragma once

#include "ecs.h"
#include "c_weapon.h"

struct MonsterComponent {
	DamageType vulnerability, immunity;

	MonsterComponent(DamageType vulnerability = DamageType::None, DamageType immunity = DamageType::None) :
		vulnerability(vulnerability), immunity(immunity) {}
};