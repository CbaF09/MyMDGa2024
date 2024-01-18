#pragma once
#include "IE_SceneUpdater.h"


namespace atl {

	// 各シーンクラス用 親クラス
	// 役割 ... 全シーンに同一に提供される事が予想される機能を、まとめて継承する
	//			そのクラスがシーンであるかの明確化（このクラスを継承していれば、それはシーンである）
	// NOTE : sceneUpdate も onEvent も、継承先でオーバーライドさせたいので純粋仮想関数にしている
	//		  結果、現状このクラスはインターフェイスとなっているが、今後インターフェイスでなくなる可能性があるのと
	//		  IE_Base_Scene という名前は個人的にちょっと嫌 ( インターフェイスならBaseとなるのは当たり前 ) なので
	//		  接頭辞の命名規則から外れているが、一旦インターフェイスのまま放置する。
	//		  ... インターフェイスでなくなったら、このNOTEは消してください
	class Base_Scene :
		public IE_SceneUpdater {

	public:
		void sceneUpdate(float deltaTime) override = 0;

	private:
		// NONE
	};

};