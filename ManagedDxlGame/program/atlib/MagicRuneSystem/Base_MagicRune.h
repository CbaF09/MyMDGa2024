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

		// �Q�b�^�[ ( ���[���p2D�A�C�R���摜���擾 )
		const int getRuneGraph() { return graphHandle_; }
		// �Z�b�^�[ ( ���[���p2D�A�C�R���摜���Z�b�g )
		void setRuneGraph(int graphHandle) { graphHandle_ = graphHandle; };

	private:
		// ���[���p2D�e�N�X�`��
		int graphHandle_ = -1;
	};

}