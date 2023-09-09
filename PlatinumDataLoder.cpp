#include "PlatinumDataLoder.h"
#include <DxLib.h>
#include <string>
#include <cassert>

void PlatinumDataLoder::Load(const char* filePath)
{
	// FMF�w�b�_�[(Platinum�̃h�L�������g�ɏ����Ă���)
	struct Header
	{
		int8_t id[4];			// ���ʎq(FMF_)		1 * 4�o�C�g
		uint32_t size;			// �f�[�^�T�C�Y		4�o�C�g
		uint32_t mapWidth;		// �}�b�v�̕�(200)  4�o�C�g
		uint32_t mapHeight;		// �}�b�v�̍���		4�o�C�g
		uint8_t chipWidth;		// �`�b�v(�Z���P��)�̕�		1�o�C�g
		uint8_t chipHeight;		// �`�b�v(�Z���P��)�̍���	1�o�C�g
		uint8_t layerCount;		// ���C���[��		1�o�C�g
		uint8_t bitCount;		// �P�Z��������̃r�b�g��(��8�Ńo�C�g��)�@1�o�C�g
	};
	// 20�o�C�g

	Header header;
	int handle = FileRead_open(filePath);
	FileRead_read(&header, sizeof(header), handle);

	std::string strId;
	strId.resize(4);
	std::copy_n(header.id, 4, strId.begin());

	if (strId != "FMF_")
	{
		// ���[�h���s
		assert(0);
	}

	mapWidth_ = header.mapWidth;
	mapHeight_ = header.mapHeight;

	int layerCount = header.layerCount;

	// ���C���[1������̃T�C�Y���v�Z����
	// �}�b�v�̕� * �}�b�v�̍��� * (�`�b�v�P������̃o�C�g��)
	int layerDataSize = header.mapWidth * header.mapHeight * (header.bitCount / 8);

	mapData_.resize(layerCount);
	for (auto& layer : mapData_)
	{
		layer.resize(layerDataSize);
		FileRead_read(layer.data(), layerDataSize, handle);
	}

	FileRead_close(handle);

	// ���s���������������̂��߂ɁA�f�[�^�����H
	for (int layerIdx = 0; layerIdx < layerCount; layerIdx++)
	{
		TransposeMapData(layerIdx);
	}
}

const MapData_t& PlatinumDataLoder::GetMapData() const
{
	return mapData_;
}

const int PlatinumDataLoder::GetChipId(int layerId, int chipX, int chipY) const
{
	assert(chipX < mapWidth_);
	assert(chipY < mapHeight_);
	auto index = chipY + chipX * mapHeight_;
	return mapData_[layerId][index];
}

void PlatinumDataLoder::GetMapSize(int& width, int& height)
{
	width = mapWidth_;
	height = mapHeight_;
}

void PlatinumDataLoder::TransposeMapData(int layerId)
{
	// ��������R�s�[���Ă���
	// ���R�Ƃ��Ă̓R�s�[�����ɓ]�u���傤�Ƃ���ƌ��̃f�[�^��������
	auto temp = mapData_[layerId];

	for (int y = 0; y < mapHeight_; y++)
	{
		for (int x = 0; x < mapWidth_; x++)
		{
			// �ʏ��XY�w��̏ꍇ
			// y * mapWidth_ + x�Ƃ��������ɂȂ�
			// ����ɁA��200�c15�͈ێ�����K�v������
			// �c�ɕ��ׂĂ�������
			// 0�ׂ̗�1�i���ɂ���
			int idxSrc = y * mapWidth_ + x;		// Source Index
			int idxDst = y + mapHeight_ * x;	// Destination Index
			mapData_[layerId][idxDst] = temp[idxSrc];
		}
	}
}
