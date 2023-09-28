#pragma once

class SoundManager;

// セーブデータの管理を行うシングルトンクラス
class SaveData
{
private:
	// セーブデータ
	struct Data
	{
		int version = 0;		// データのバージョン
		int masterVolume = 5;	// 全体の音量
		int bgmVolume = 5;		// BGM音量	
		int seVolume = 5;		// SE音量	
		int padStickSensitivityX = 6;	// パッドのスティックの感度X
		int padStickSensitivityY = 3;	// パッドのスティックの感度Y
		bool padStickReverseX = false;	// パッドのスティックの横のリバース
		bool padStickReverseY = false;	// パッドのスティックの縦のリバース
	};
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

	Data GetSaveData();
	void SetConfigValue(int& configValue, int splitNum);

	// 音量設定
	// 音量の設定(最大値を超えると0に戻る)
	void SetMasterVolume();	// 全体音量
	void SetBgmVolume();	// BGM
	void SetSeVolume();		// SE

	// パッドのスティックの感度の設定
	// 感度の設定(最大値を超えると0に戻る)
	void SetPadStickSensitivityX();		 
	void SetPadStickSensitivityY();		 

	// パッドのスティックのリバースの設定
	void SetPadStickReverseX();			
	void SetPadStickReverseY();			

private:
	// コンストラクタ
	// シングルトンパターンなのでprivate
	SaveData();

	// コピーと代入の禁止
	SaveData(const SaveData&) = delete;			// コピーコンストラクタ禁止
	void operator = (const SaveData&) = delete;	// 代入禁止

	// データの削除
	void InitData();

private:
	// セーブデータ
	Data saveData_;

	SoundManager& soundManger_;
};