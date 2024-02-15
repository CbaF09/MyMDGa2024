#pragma once

namespace atl {

	class DungeonScene;

	// �C�x���g�񋓎q MagicRune����������^�C�~���O��\��
	enum class e_EventType {
		Debug,
		TurnStart
	};

	// ���O��CSV���ID�𕪂���₷��������ׂ� enum
	enum class e_RuneID {
		NONE = 0,
		HealRune = 1,
		FireRune = 2,
	};

	class Base_MagicRune {
	public:
		virtual ~Base_MagicRune() {};
		// �C�x���g�^�C�v�ɑ΂��Ĕ������鏈�� ( �������z�֐� )
		virtual void onNotify(e_EventType eventType,DungeonScene& dungeonScene) = 0;

		// �Q�b�^�[ ( ���[���p2D�A�C�R���摜���擾 )
		const int getRuneGraph() { return graphHandle_; }
		// �Q�b�^�[ ( ���[���̖��O���擾 )
		const std::string& getRuneName() { return runeName_; }
		// �Q�b�^�[ ( ���[���̐��������擾 )
		const std::string& getRuneDesc() { return runeDesc_; }
		

		// ���[���̏���CSV����ǂݍ��񂾃f�[�^�Ɋ�Â��Ă܂Ƃ߂Đݒ�
		// arg ... �A�C�e��ID ( CSV �� A�� �ɋL�� )
		void setRuneData(e_RuneID itemID);

	private:
		// ���[���̖��O
		std::string runeName_ = "";
		// ���[���̐�����
		std::string runeDesc_ = "";
		// ���[���p�A�C�R���̃O���t�B�b�N�n���h��
		int graphHandle_ = -1;
	};

}