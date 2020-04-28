#pragma once

#include "ecs.h"
#include "c_weapon.h"

//enum class MonsterType {
//	Slime, Rat, Goblin, Kobold, Orc, OrcCaptian, Centaur, Werewolf
//};

//std::string getMonsterName(MonsterType type) {
//	switch (type) {
//		case MonsterType::Slime:
//			return "Slime";
//		case MonsterType::Rat:
//			return "Rat";
//		case MonsterType::Goblin:
//			return "Goblin";
//		case MonsterType::Orc:
//			return "Orc";
//		case MonsterType::OrcCaptian:
//			return "Orc Captain";
//		case MonsterType::Centaur:
//			return "Centaur";
//		case MonsterType::Werewolf:
//			return "Wereworlf";
//		case MonsterType::Dragon:
//			return "Dragon";
//	}
//}

struct MonsterComponent {
	DamageType vulnerability, immunity;

	MonsterComponent(DamageType vulnerability = DamageType::None, DamageType immunity = DamageType::None) :
		vulnerability(vulnerability), immunity(immunity) {}
};