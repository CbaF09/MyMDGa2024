#pragma once
#include <string>
#include "../../dxlib_ext/dxlib_ext.h"

namespace atl {

	class TextLogManager {
	//------------------------------------------
	// シングルトン設計
	public:
		static TextLogManager* getTextLogManager();
		inline static void deleteTextLogManager() { delete getTextLogManager(); }
	private:
		TextLogManager() {};
		~TextLogManager() {};
	//------------------------------------------

	public:
		// ゲッター
		inline const std::list<std::string>& getTextLog() const { return textLogList_; }

		// テキストログの先頭への追加, MAX_LOG_COUNTを超過している場合、末尾を削除する
		// arg ... 追加するテキスト。半角は入力しないでください
		void addTextLog(const std::string& addText);


		// 全テキストログの削除
		inline void clearTextLog() { textLogList_.clear(); }

		// テキストログを描画する。
		// arg ... 描画位置の左上頂点座標
		void displayTextLog(int x, int y,float deltaTime);

	private:
		std::list<std::string> textLogList_;

		// ---------------------------- -
		// 文字列描画アニメーション用 
		int32_t drawCharCount_ = 0;
		int32_t drawLogLine_ = 0;
		float totalDeltaTime_ = 0.0f;	// 累積時間
		const float CHAR_INTERVAL = 0.05f; // 表示文字を増やす間隔 ( 秒 )

		const int32_t MAX_LOG_COUNT = 7;
	};

}