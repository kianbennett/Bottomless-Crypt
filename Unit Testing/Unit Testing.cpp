#include "CppUnitTest.h"
#include "../TOGA-Final/pathfinder.h"
#include "../TOGA-Final/bsp.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(UnitTesting) {
	public:
		
		// Set up a simple grid to confirm the shortest path is found
		TEST_METHOD(PathfindingTest) {
			
			Matrix<Tile> tileGrid = Matrix<Tile>(4, 4);
			for (int i = 0; i < tileGrid.getWidth(); i++) {
				for (int j = 0; j < tileGrid.getHeight(); j++) {
					tileGrid.set(i, j, Tile(TileType::Floor, Vec2Int(i, j)));
				}
			}

			tileGrid.get(2, 0).type = TileType::Empty;
			tileGrid.get(2, 1).type = TileType::Empty;
			tileGrid.get(2, 2).type = TileType::Empty;

			Vec2Int start = Vec2Int(0, 1);
			Vec2Int destination = Vec2Int(3, 1);

			std::vector<Tile> path = Pathfinder::findPathOnTileGrid(tileGrid, tileGrid.get(start), tileGrid.get(destination));

			Assert::AreEqual((int) path.size(), 8);
		}

		// Ensure the BSP always produces at least one room
		TEST_METHOD(DungeonGenerationTest) {
			int minRooms = INT_MAX;

			BSP bsp;

			for (int i = 0; i < 100; i++) {
				bsp.build(Vec2Int(), Vec2Int(16, 16), 5);
				if (bsp.rooms.size() < minRooms) minRooms = bsp.rooms.size();
			}

			Assert::IsTrue(minRooms >= 1);
		}
	};
}