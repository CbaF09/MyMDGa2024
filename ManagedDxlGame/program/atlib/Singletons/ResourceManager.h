#pragma once
#include <unordered_map>
#include <string>
#include "../dxlib_ext/dxlib_ext.h"

namespace atl {

	// 内部でフライウェイトパターンを適用しています。
	//  ... 存在しないリソースならロード、ロード済みのリソースならそのハンドルを返す
	class ResourceManager final {
	//------------------------------------------
	// シングルトン設計
	public:
		static ResourceManager* getResourceManager();
		static void deleteResourceManager();
	private:
		static ResourceManager* p_instance_;
	//------------------------------------------
	public:

		// 2D イラスト用 フライウェイトパターン ( filepath, graphHandle)
		int getGraphRes(const std::string& filepath);

		// 音源データ用 フライウェイトパターン ( filepath, playtype,topPositionFlag )
		// filepath ... 再生したいサウンドのfilepath
		// playtype ... DX_PLAYTYPE_NORMAL	... 再生が終わるまで処理を止める
		//			... DX_PLAYTYPE_BACK	... 鳴らし始めるとすぐ次の処理
		//			... DX_PLAYTYPE_LOOP	... 始まるとすぐ次の処理。再生が終わるとすぐ最初から再生。
		// topPositionFlag ... 再生する前に再生開始位置を先頭に戻すか。省略可能 ... 省略した場合は TRUE
		void playSoundRes(const std::string& filepath,int playType, int topPositionFlag = TRUE);

		// filepath ... 音量変更したいサウンドのfilepath
		// volume ... 0 ~ 255 , 255で最大音量 ( 元音声データと同じ音量 )
		void changeVolumeSoundRes(const std::string& filepath, int volume);

		// 再生中のサウンドを停止する
		void stopSoundRes(const std::string& filepath);

		// 指定パスのリソースを解放する。成功 => false , 失敗 => false
		bool deleteResource(const std::string& filepath);

	private:
		// 2D イラスト用 Map ( filepath, graphHandle)
		std::unordered_map<std::string, int> graphResourceMap_;
		// 音源データ用 map ( filepath, soundHandle )
		std::unordered_map<std::string, int> soundResouceMap_;

		// 音源データ用 フライウェイトパターン。ロードと取得を行うだけ。
		// サウンドは直接再生・音量調整・停止が出来た方がいいと思うので、ロードと取得を行うだけの関数は内部に隠蔽
		int getSoundRes(const std::string& filepath);
		
	};

}