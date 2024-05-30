#pragma once
#include <string>
#include <vector>
#include"Vector3.h"
enum class MapChipType {
	kBlank, // 空白
	kBlock, // ブロック
};
struct MapChipData {
	std::vector<std::vector<MapChipType>> data;
};

class MapChipField {

private:
	// 1ブロックのサイズ
	static inline const float kBlockWidth = 2.0f;
	static inline const float kBlockHeight = 2.0f;
	// ブロックの個数
	static inline const uint32_t kNumBlockVirtical = 10;
	static inline const uint32_t kNumBlockHorizontal = 10;

	MapChipData mapChipData_;

public:
	void ResetMapChipData();
	void LoadMapChipCsv(const std::string& filePath);
	MapChipField();
	~MapChipField();

	MapChipType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex);
	Vector3 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex);
	static inline const uint32_t GetNumBlockVirtial() { return kNumBlockVirtical; }
	static inline const uint32_t GetNumBlockHorizontal() { return kNumBlockHorizontal; }
};
