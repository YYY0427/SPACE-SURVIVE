#pragma once

// セーブデータの管理を行うシングルトンクラス
class SaveData
{
public:
	// デストラクタ
	~SaveData();

	/// <summary>
	/// 唯一のインスタンスを返す
	/// </summary>
	/// <returns>唯一の実態の参照</returns>
	static SaveData& GetInstance();

	// セーブデータの読み込み
	void Load();

	// セーブデータの書き込み
	// セーブデータのバージョンが一致しない場合は止める
	void Write();

	// セーブデータを新規作成して上書き
	void CreateNewData();

	// 音量の取得
	int GetBgmVolume() const;	// BGM
	int GetSeVolume() const;	// SE

	// 音量設定
	// 音量の設定(最大値を超えると0に戻る)
	void SetBgmVolume();	// BGM
	void SetSeVolume();		// SE
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

	// コピーも代入も禁止
	SaveData(const SaveData&) = delete;			// コピーコンストラクタ
	void operator = (const SaveData&) = delete;	// 代入も禁止

	// データの削除
	void InitData();

private:
	// セーブデータ
	Data data_;
};