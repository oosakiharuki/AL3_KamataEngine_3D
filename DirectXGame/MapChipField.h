#include <vector>
#include "Vector3.h"

enum class MapChipType {
	kBlank,//空白
	kBlock//ブロック
};

struct MapChipDate {
	std::vector<std::vector<MapChipType>> date;//空白orブロック
};

class MapChipField {
public:

	void ResetMapChipDate();
	void LoadMapChipCsv(const std::string& filePath);
	MapChipType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex);
	Vector3 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex);




	//ブロックサイズ
	static inline const float kBlockWidth = 1.0f;
	static inline const float kBlockHeight = 1.0f;
	//個数
	static inline const float kNumBlockVertical = 20;
	static inline const float kNumBlockHorizontal = 100;

	MapChipDate mapChipDate_;



};