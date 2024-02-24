#pragma once

namespace atl {


	/// <summary>
	/// 
	/// MagicRune �̊��N���X�B
	/// �I�u�U�[�o�[�p�^�[���̃T�u�W�F�N�g�ɂ�����B
	/// �I�u�U�[�o�[�ɂ����鑶�݂́AMagicRuneSystemManager
	/// 
	/// </summary>
	
	class DungeonScene;

	// �C�x���g�񋓎q MagicRune����������^�C�~���O��\��
	enum class e_EventType {
		Debug,			// �f�o�b�O�p
		TurnStart,		// �^�[���J�n��
	};

	// ���O��CSV���ID�𕪂���₷��������ׂ� enum
	enum class e_RuneID {
		NONE,
		FireRune,
		StoneRune,
	};

	class Base_MagicRune {
	public:
		virtual ~Base_MagicRune() {};
		
		// �C�x���g�^�C�v�ɑ΂��Ĕ������鏈�� ( �������z�֐� )
		virtual void onNotify(e_EventType eventType) = 0;
		// �����������ɔ������鏈��
		virtual void onEquipMomentNotify() {};
		// �������O�������ɔ������鏈��
		virtual void onRemoveMomentNotify() {};

		// �Q�b�^�[ ( ���[���p2D�A�C�R���摜���擾 )
		const int getRuneGraph() { return graphHandle_; }
		// �Q�b�^�[ ( ���[���̖��O���擾 )
		const std::string& getRuneName() { return runeName_; }
		// �Q�b�^�[ ( ���[���̐��������擾 )
		const std::string& getRuneDesc() { return runeDesc_; }

		// ���[���̏���CSV����ǂݍ��񂾃f�[�^�Ɋ�Â��Ă܂Ƃ߂Đݒ�
		// arg ... �A�C�e��ID ( CSV �� A�� �ɋL�� ) �� enum �œ���
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