#pragma once

/// <summary>
/// セーブデータの管理を行うシングルトンクラス
/// </summary>
class SaveData
{
public:
	// デストラクタ
	~SaveData();

	// SaveDataの唯一のインスタンスを返す
	static SaveData& GetInstance();

	// セーブデータの読み込み
	void Load();

	// セーブデータの書き込み
	void Write();

	// セーブデータを新規作成して上書き
	void CreateNewData();

	// ゲッター
	int GetBgmVolume() const;	// BGM音量	0~5
	int GetSeVolume() const;	// SE音量	0~5

	// 音量設定
	void SetBgmVolume(); // BGM
	void SetSeVolume();	 // SE
private:
	// セーブデータ
	struct Data
	{
		int version = 0;		// データのバージョン
		int volumeBgm = 3;		// BGM音量	0~5
		int volumeSe = 3;		// SE音量	0~5
	};
private:
	// コンストラクタ(シングルトンパターンなのでprivateに置く)
	SaveData();

	// コピーも代入も禁止する
	SaveData(const SaveData&) = delete;			// コピーコンストラクタ
	void operator = (const SaveData&) = delete;	// 代入も禁止

	// データの削除
	void InitData();

private:
	Data data_;
};