#pragma once

#include "vec2.h"
#include <SDL.h>

struct BSPNode {
	BSPNode* parent;
	BSPNode* leftChild;
	BSPNode* rightChild;

	Vec2Int pos;
	Vec2Int size;

	int level;

	BSPNode(int level, Vec2Int pos, Vec2Int size, BSPNode* parent) :
		level(level), pos(pos), size(size), parent(parent), leftChild(nullptr), rightChild(nullptr) {}

	Vec2Int centrePoint() {
		return Vec2Int(pos.x + size.x / 2.0f, pos.y + size.y / 2.0f);
	}
};

class BSP {
public:
	BSPNode* root;
	std::vector<SDL_Rect> leaves;
	std::unordered_map<BSPNode*, SDL_Rect> rooms;

	BSP() : maxLevel(0), root(nullptr) {}

	void build(Vec2Int origin, Vec2Int size, int cuts) {
		maxLevel = cuts;
		leaves.clear();
		rooms.clear();
		
		delete root;
		root = new BSPNode(0, origin, size, nullptr);
		split(root);
		//checkCorridor(root->leftChild, root->rightChild);
	}

	void split(BSPNode* node) {
		bool vertical = rand() % (node->size.y > node->size.x ? 2 : 4);
		int size = vertical ? node->size.x : node->size.y;
		if (size < 10) {
			vertical = !vertical;
		}
		size = vertical ? node->size.x : node->size.y;
		if (size < 8 || node->level >= maxLevel) {
			SDL_Rect leaf = SDL_Rect{
				node->pos.x, node->pos.y, node->size.x, node->size.y
			};
			SDL_Rect room = SDL_Rect{
				node->pos.x + 1, node->pos.y + 1,
				node->size.x - 2, node->size.y - 2,
			};
			leaves.push_back(leaf);

			bool useRoom = room.w > 2 && room.h > 2;
			if(useRoom) rooms.emplace(node, room);
			//rooms.emplace(node, room);
			return;
		}
		int cutPoint = 2 + rand() % (size - 4);
		Vec2Int leftSize = Vec2Int(vertical ? cutPoint : node->size.x, vertical ? node->size.y : cutPoint);
		Vec2Int rightSize = Vec2Int(vertical ? node->size.x - cutPoint : node->size.x, vertical ? node->size.y : node->size.y - cutPoint);
		Vec2Int leftPos = node->pos;
		Vec2Int rightPos = Vec2Int(vertical ? node->pos.x + cutPoint : node->pos.x, vertical ? node->pos.y : node->pos.y + cutPoint);

		node->leftChild = new BSPNode(node->level + 1, leftPos, leftSize, node);
		node->rightChild = new BSPNode(node->level + 1, rightPos, rightSize, node);
		
		split(node->leftChild);
		split(node->rightChild);
	}

private:
	int maxLevel;
};