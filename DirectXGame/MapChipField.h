#include "Vector3.h"
#include <vector>
#include <string>

enum class MapChipType {
	kBlank, // 空白
	kBlock  // ブロック
};

struct MapChipDate {
	std::vector<std::vector<MapChipType>> date; // 空白orブロック
};

class MapChipField {
public:
	
	void ResetMapChipDate();
	void LoadMapChipCsv(const std::string& filePath);
	MapChipType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex);
	Vector3 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex);


		
	// ブロックサイズ
	static inline const float GetkBlockWidth() { return kBlockWidth; };
	static inline const float GetkBlockHeight() { return kBlockHeight; };
	// 個数
	static inline const uint32_t GetkNumBlockVertical() { return kNumBlockVertical; };
	static inline const uint32_t GetkNumBlockHorizontal() { return kNumBlockHorizontal; };



private:
	// ブロックサイズ
	static inline const float kBlockWidth = 2.0f;
	static inline const float kBlockHeight = 2.0f;
	// 個数
	static inline const uint32_t kNumBlockVertical = 20;
	static inline const uint32_t kNumBlockHorizontal = 100;

	MapChipDate mapChipDate_;
};