#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include "Base_Scene.h"

namespace atl {

	// �Q�[���N���A���̃V�[��
	// �G�s���[�O�ƃ^�C�g���ɖ߂鏈���̂�
	
	class GameClearScene final : public Base_Scene{
	public:
		GameClearScene() {
			// �e�N�X�`���ɂ�郁�������[�N�΍�
			dxe::Texture::DestroyUnReferenceTextures();
		}
		~GameClearScene();

	private:
		// -------------------------------------
		// �ϐ�

		// �v�����[�O�֘A
		const int EPILOGUE_FONT = CreateFontToHandle(NULL, 25, -1, DX_FONTTYPE_ANTIALIASING);

		const int32_t EPILOGUE_BACKGROUND_BRIGHT = 126;	// �w�i�̖��邳 ( 0 �Ō��Ɠ����A255 �Ő^���� )
		const tnl::Vector2i TEXT_POSITION{ 25,150 };	// ��ԏ�̍s�̕\���ʒu
		const tnl::Vector2i TEXT_OFFSET{ 0,45 };		// ��s���ƂɃI�t�Z�b�g������
		const float LOG_LINE_INTERVAL = 5.0f;			// �\���s�𑝂₷�Ԋu ( �b )
		int32_t drawLogLine_ = 0;		// �`�悳���s��
		float totalDeltaTime_ = 0.0f;	// �ݐώ���
		int32_t textAlpha_ = 255;		// �e�L�X�g�̓����x�p�ϐ�
		const int32_t ALPHA_MINUS_SPEED = 2; // �e�L�X�g�������ɂȂ��Ă������x
		const float FULL_ALPHA_TIME = 4.5;	// �S�e�L�X�g���`�悳�ꂽ��A�ő哧���x�̂܂܂̑ҋ@����
		enum class e_EpiloguePage {
			ZERO = 0,
			ONE,
			TWO,
			THREE,
			FOUR,
			PAGE_MAX
		}currentPage_ = e_EpiloguePage::ZERO;
		const std::vector<std::vector<std::string>> epiloguePage = {
			{// 0�y�[�W��
				"�ŏ�K�ɒH�蒅�����B",
				"�����ɂ́A����P������ȏ񂪒������Ă���B",
				"�u�͂��߂܂��āA���C�Y�}���v",
				"������Ɍ������ē���������ƁA�N�炩�Ȑ����ǂ����炩�������Ă����B",
				"�w�₠�A�N���B�҂��Ă�����A���ҏ���o���Ă��炸���Ƃˁx",
				"���C�Y�}���̑喂�����\�\�����̏�\�\���̏�ɂ̓��C�Y�}���̖��͂ƈӎv���h���Ă���B",
			},
			{ // 1�y�[�W��
				"�w�킴�킴�����J�����Ă��܂Ȃ��ˁB�������̐g�̂��x",
				"�S�Ȃ����A���F�ɍ��킹�ĕ����̏񂪖��ł��Ă���悤�Ɍ�����B",
				"�u���Ȃ��́c�c���̂������ɂ��āA�ǂ����l���Ȃ̂ł����H�v",
				"�w�܂��A�����͂Ȃ��ˁx",
				"�������ݍ��񂾎�����������肪�A��������ƕԂ���Ă��܂����B",
				"������������A�������x��������Ă������Ȃ̂�������Ȃ��B",
				"���C�Y�}���͉��S�N���̖̂��@�g�����B�Ƃ����ɑ�����������Ă���B",
				"���̏�̓��C�Y�}���̍�i�ł���Ɠ����ɁA�̃��C�Y�}�����̐l�ƂȂ����̂��B",
			},
			{// 2�y�[�W��
				"�u�Ȃ��A���ɏ��ҏ���v",
				"�w���N�����Ă���ƁA���܂ɂ͘b�����肪�~�����Ȃ���̂��x",
				"\"���C�Y�}���̏��ҏ�́A�Ⴂ���K�����@�g���̌��Ɍ����\"",
				"������j����[���Ƒ�����҂�����B�����Ƒ�����ҁA���邢�͕��̂Ƒ�����ҁc�c",
				"���͖����A���̏��ҏ���ǂ�������΂����̂������炸�ɂ����B�����Ȃ��B",
				"�w�m���Ă��邾�낤���A���ҏ�̊����͂��܂蒷���͂Ȃ��B�N�̂��̂́A���낻��c�c�x",
				"�����ƁA����o�肫��܂ł̎��ԁB�����ď��ҏ�̊����B���ɕ����҂͎��̏��A�����Ȃ��B",
			},
			{// 3�y�[�W��
				"����ł����͂����֗����B",
				"�u�Ȃ��A����Ȏ��𑱂���̂ł����v",
				"���C�Y�}���́A�����Ԃ��󂯂Ă��瓚�����B",
				"�w�c�c�������ȁA�Ȍ��Ɍ����Ȃ�\�\�x",
				"���ҏ󂪌���͂��߂��B�����؂ꂾ�B",
				"�w�\�\�K���ɂȂ��Ăق�������A�Ƃ����������ȁx",
			},
			{// 4�y�[�W��
				"���ҏ�̌��ɕ�܂�A�C�t���Ǝ��͓��̊O�ɋ����B",
				"�������ҏ�͖��������B���͂��ߑ������ƁA��������H���ċA�鎖�ɂ����B",
				"�Ƃɒ�������A�����x��ŁA�܂��`���ɏo�|���悤�B",
			}
		};
		
		// -------------------------------------
		// ���\�b�h

		void sceneUpdate(float deltaTime) override;
		void draw(float deltaTime);

		SEQUENCE(GameClearScene,&GameClearScene::seqInit);

		bool seqInit(float deltaTime);
		bool seqEpilogue1(float deltaTime);

		void invokeDrawLogLineIncrement();

		bool seqToTitleScene(float deltaTime);

	};

}