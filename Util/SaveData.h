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

	// パッドのスティックの感度の取得
	int GetPadStickSensitivityX() const;	
	int GetPadStickSensitivityY() const;	

	// パッドのスティックのリバースかどうかの取得
	bool GetPadStickReverseX() const;		
	bool GetPadStickReverseY() const;		

	// 音量設定
	// 音量の設定(最大値を超えると0に戻る)
	void SetBgmVolume();	// BGM
	void SetSeVolume();		// SE

	// パッドのスティックの感度の設定
	void SetPadStickSensitivityX();		 
	void SetPadStickSensitivityY();		 

	// パッドのスティックのリバースの設定
	void SetPadStickReverseX();			
	void SetPadStickReverseY();			
private:
	// セーブデータ
	struct Data
	{
		int version = 0;		// データのバージョン
		int volumeBgm = 3;		// BGM音量	
		int volumeSe = 3;		// SE音量	
		int padStickSensitivityX = 6;	// パッドのスティックの感度X
		int padStickSensitivityY = 3;	// パッドのスティックの感度Y
		bool padStickReverseX = false;	// パッドのスティックの横のリバース
		bool padStickReverseY = false;	// パッドのスティックの縦のリバース
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