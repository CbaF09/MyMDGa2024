#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include "Base_MagicRune.h"

namespace atl {
	
	/// <summary>
	/// 
	/// ルーンに関するマネージャークラス。
	/// オブザーバーパターンのオブザーバーにあたる。
	/// Base_MagicRuneの配列を保持。
	/// notifyOnEvent で、Base_MagicRuneの具象クラスで定義されるonNotifyを発火させる。
	/// 
	/// </summary>
	
	class Base_MagicRune;

	class MagicRuneSystemManager final {

	//------------------------------------------
	// シングルトン設計
	public:
		static MagicRuneSystemManager* getMagicRuneSystemManager();
		static void deleteMagicRuneSystemManager() { delete getMagicRuneSystemManager(); };
	private:
		MagicRuneSystemManager() {};
		~MagicRuneSystemManager() { equipmentMagicRunes_.clear(); };
	//------------------------------------------

	public:
		// ゲッター ( 最大装備数 )
		inline const int32_t getMaxEquipmentRune() const { return MAX_EQUIPMENT_RUNE; }
		// ゲッター ( 装備中のルーン配列 )
		inline const std::vector<Shared<Base_MagicRune>>& getEquipmentMagicRunes() const { return equipmentMagicRunes_;}

		// ルーンを追加する
		bool equipRune(Shared<Base_MagicRune> newRune);
		
		// ルーンを削除する
		void removeRune(int32_t index);

		// ルーンを発動させる
		// arg ... 発火させるイベント。Base_MagicRune で定義
		void notifyOnEvent(e_EventType eventType);

	private:
		// 最大装備数は5個まで
		int32_t MAX_EQUIPMENT_RUNE = 5;

		// 装備中のルーンの配列
		std::vector<Shared<Base_MagicRune>> equipmentMagicRunes_{};

	};

}