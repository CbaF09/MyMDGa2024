#pragma once

namespace atl {

	class DungeonScene;

	// �C�x���g�񋓎q MagicRune����������^�C�~���O��\��
	enum class e_EventType {
		Debug,
		TurnStart
	};

	class Base_MagicRune {
	public:
		virtual ~Base_MagicRune() {};
		// �C�x���g�^�C�v�ɑ΂��Ĕ������鏈��
		virtual void onNotify(e_EventType eventType,DungeonScene& dungeonScene) = 0;
	};

}