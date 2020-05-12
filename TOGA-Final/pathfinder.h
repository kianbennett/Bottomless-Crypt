#pragma once

#include <vector>
#include <algorithm> 
#include "vec2.h"
#include "matrix.h"

//struct GridNode {
//	Vec2Int pos;
//	float cost;
//
//	GridNode() : pos(0, 0), cost(0) {}
//	GridNode(Vec2Int pos, float cost) : pos(pos), cost(cost) {}
//};

struct PathNode {
	Vec2Int pos;
	float cost;
	PathNode* parent;
	std::vector<PathNode*> connections;
	int distFromStart;
	float heuristic;
	bool closed;

	PathNode(Vec2Int pos, float cost) : pos(pos), parent(nullptr), connections(), cost(cost), distFromStart(0), 
		heuristic(0), closed(false) {}
};

class Pathfinder {
public:
	static std::vector<Tile> findPathOnTileGrid(Matrix<Tile> tiles, Tile start, Tile destination) {
		// Open list contains points that are being considered for the path
		std::vector<PathNode*> openList;
		// Closed list contains points that have been used up and are no longer being considered
		std::vector<PathNode*> closedList;

		// Create a new grid of nodes based on the tile matrix, since nodes need to be able to keep track of their parent
		Matrix<PathNode*> nodeGrid(tiles.getWidth(), tiles.getHeight());
		for (int i = 0; i < nodeGrid.getWidth(); i++) {
			for (int j = 0; j < nodeGrid.getHeight(); j++) {
				Tile tile = tiles.get(i, j);
				int cost = 0;
				if (tile.type == TileType::Empty) cost = 10;
				PathNode* node = new PathNode(tile.pos, cost);
				nodeGrid.set(i, j, node);
			}
		}

		setUpConnections(nodeGrid);

		// Begin with start point
		PathNode* nodeCurrent = nodeGrid.get(start.pos);
		openList.push_back(nodeCurrent);

		// Keep looping as long as there are still nodes to check and the target hasn't been reached
		while (openList.size() > 0) {
			// Sort open list by score
			std::sort(openList.begin(), openList.end(), compareScore);
			// Get the first node in the list
			nodeCurrent = openList[0];
			//nodeCurrent->heuristic = heuristic(nodeCurrent->pos, destination);

			// Stop if the node is the destination
			if (nodeCurrent->pos == destination.pos) break;

			// Remove the node from the open list and add it to the closed list
			// Erase-remove idiom https://stackoverflow.com/questions/3385229/c-erase-vector-element-by-value-rather-than-by-position
			openList.erase(std::remove(openList.begin(), openList.end(), nodeCurrent), openList.end());
			closedList.push_back(nodeCurrent);

			for (PathNode* node : nodeCurrent->connections) {
				bool closed = std::find(closedList.begin(), closedList.end(), node) != closedList.end();
				if (closed) continue;
				
				bool inOpenList = std::find(openList.begin(), openList.end(), node) != openList.end();
				if (!inOpenList) {
					node->heuristic = heuristic(node->pos, destination.pos);
					node->parent = nodeCurrent;
					openList.push_back(node);
				}
			}
		}

		// Get path by traversing linked list backwards
		std::vector<Tile> path;
		PathNode* n = nodeCurrent;
		while (n != nullptr) {
			path.push_back(tiles.get(n->pos));
			n = n->parent;
		}

		// The path is currently from end to beginning, needs reversing
		std::reverse(path.begin(), path.end());

		return path;
	}

private:
	static void setUpConnections(Matrix<PathNode*> &nodes) {
		for (int i = 0; i < nodes.getWidth(); i++) {
			for (int j = 0; j < nodes.getHeight(); j++) {
				PathNode* node = nodes.get(i, j);
				if (nodes.isInsideMatrix(node->pos.x, node->pos.y + 1)) {
					node->connections.push_back(nodes.get(node->pos.x, node->pos.y + 1));
				}
				if (nodes.isInsideMatrix(node->pos.x + 1, node->pos.y)) {
					node->connections.push_back(nodes.get(node->pos.x + 1, node->pos.y));
				}
				if (nodes.isInsideMatrix(node->pos.x, node->pos.y - 1)) {
					node->connections.push_back(nodes.get(node->pos.x, node->pos.y - 1));
				}
				if (nodes.isInsideMatrix(node->pos.x - 1, node->pos.y)) {
					node->connections.push_back(nodes.get(node->pos.x - 1, node->pos.y));
				}
			}
		}
	}

	// Estimate cost of travelling from one node to another
	// For now use simple line of sight distance
	static float heuristic(Vec2Int a, Vec2Int b) {
		return Vec2Int::distance(a, b);
	}

	static bool compareScore(PathNode* a, PathNode* b) {
		float aCost = a->distFromStart + a->heuristic + a->cost;
		float bCost = b->distFromStart + b->heuristic + b->cost;

		//if (a->parent != nullptr && b->parent != nullptr && a->parent->parent != nullptr && b->parent->parent != nullptr) {
		//	Vec2Int aParentDir = a->parent->pos - a->parent->parent->pos;
		//	Vec2Int bParentDir = b->parent->pos - b->parent->parent->pos;
		//	Vec2Int aDir = a->pos - a->parent->pos;
		//	Vec2Int bDir = b->pos - b->parent->pos;

		//	float aAngle = Vec2Int::angle(aDir, aParentDir);
		//	float bAngle = Vec2Int::angle(bDir, bParentDir);

		//	return aAngle > bAngle;
		//}
		return aCost < bCost;
	}
};