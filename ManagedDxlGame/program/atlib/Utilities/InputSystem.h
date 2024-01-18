#pragma once
#include "../dxlib_ext/dxlib_ext.h"

namespace atl {

	// InputSystem
	// 役割		... 複数の入力検知を一つの関数で行う
	// 使い方	... atl::InputSystem::関数名()
	//				内部で tnl::Input を呼び出しています
	// 備考		... 各関数が検知する入力を変更・追加などした際は、各関数のコメント見直しをしてください
	class InputSystem final {
	public:

		// □
		static bool IsButtonTrigger_Square() {
			if (
				tnl::Input::IsPadDownTrigger(ePad::KEY_0)

				) return true;
			else return false;
		};

		// ×
		static bool IsButtonTrigger_Cross() {
			if (
				tnl::Input::IsPadDownTrigger(ePad::KEY_1)

				) return true;
			else return false;
		};

		// 〇
		static bool IsButtonTrigger_Circle() {
			if (
				tnl::Input::IsPadDownTrigger(ePad::KEY_2)

				) return true;
			else return false;
		};

		// △
		static bool IsButtonTrigger_Triangle() {
			if (
				tnl::Input::IsPadDownTrigger(ePad::KEY_3)

				) return true;
			else return false;
		};

		// L1
		static bool IsButtonTrigger_L1() {
			if (
				tnl::Input::IsPadDownTrigger(ePad::KEY_4)

				) return true;
			else return false;
		};

		// R1
		static bool IsButtonTrigger_R1() {
			if (
				tnl::Input::IsPadDownTrigger(ePad::KEY_5)

				) return true;
			else return false;
		};

		// L2
		static bool IsButtonTrigger_L2() {
			if (
				tnl::Input::IsPadDownTrigger(ePad::KEY_6)

				) return true;
			else return false;
		};

		// R2
		static bool IsButtonTrigger_R2() {
			if (
				tnl::Input::IsPadDownTrigger(ePad::KEY_7)

				) return true;
			else return false;
		};

		// Option
		static bool IsButtonTrigger_Option() {
			if (
				tnl::Input::IsPadDownTrigger(ePad::KEY_9)

				) return true;
			else return false;
		};

		// W or UpArrow or num8 or 十字キー上
		static bool IsButtonTrigger_UP() {
			if (
				tnl::Input::IsPadDownTrigger(ePad::KEY_UP) ||
				tnl::Input::IsKeyDownTrigger(eKeys::KB_W) ||
				tnl::Input::IsKeyDownTrigger(eKeys::KB_UP) ||
				tnl::Input::IsKeyDownTrigger(eKeys::KB_NUMPAD8)

				) return true;
			else return false;
		};

		// D or RightArrow or num6 or 十字キー右
		static bool IsButtonTrigger_RIGHT() {
			if (
				tnl::Input::IsPadDownTrigger(ePad::KEY_RIGHT) ||
				tnl::Input::IsKeyDownTrigger(eKeys::KB_D) ||
				tnl::Input::IsKeyDownTrigger(eKeys::KB_RIGHT) ||
				tnl::Input::IsKeyDownTrigger(eKeys::KB_NUMPAD6)

				) return true;
			else return false;
		};

		// S or DownArrow or num2 or 十字キー下
		static bool IsButtonTrigger_DOWN() {
			if (
				tnl::Input::IsPadDownTrigger(ePad::KEY_DOWN) ||
				tnl::Input::IsKeyDownTrigger(eKeys::KB_S) ||
				tnl::Input::IsKeyDownTrigger(eKeys::KB_DOWN) ||
				tnl::Input::IsKeyDownTrigger(eKeys::KB_NUMPAD2)

				) return true;
			else return false;
		};

		// A or LeftArrow or num4 or 十字キー左
		static bool IsButtonTrigger_LEFT() {
			if (
				tnl::Input::IsPadDownTrigger(ePad::KEY_LEFT) ||
				tnl::Input::IsKeyDownTrigger(eKeys::KB_A) ||
				tnl::Input::IsKeyDownTrigger(eKeys::KB_LEFT) ||
				tnl::Input::IsKeyDownTrigger(eKeys::KB_NUMPAD4)

				) return true;
			else return false;
		};

		// num9
		static bool IsButtonTrigger_UP_RIGHT() {
			if (
				tnl::Input::IsKeyDownTrigger(eKeys::KB_NUMPAD9)

				) return true;
			else return false;
		}

		// num7
		static bool IsButtonTrigger_UP_LEFT() {
			if (
				tnl::Input::IsKeyDownTrigger(eKeys::KB_NUMPAD7)

				)return true;
			else return false;
		}

		// num3
		static bool IsButtonTrigger_DOWN_RIGHT() {
			if (
				tnl::Input::IsKeyDownTrigger(eKeys::KB_NUMPAD3)

				)return true;
			else return false;
		}

		// num1
		static bool IsButtonTrigger_DOWN_LEFT() {
			if (
				tnl::Input::IsKeyDownTrigger(eKeys::KB_NUMPAD1)

				)return true;
			else return false;
		}

		// Enter or Z or Space or 〇
		static bool IsButtonTrigger_ENTER() {
			if (
				tnl::Input::IsPadDownTrigger(ePad::KEY_2) ||
				tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN) ||
				tnl::Input::IsKeyDownTrigger(eKeys::KB_Z) ||
				tnl::Input::IsKeyDownTrigger(eKeys::KB_SPACE)

				) return true;
			else return false;
		};

		// ESC or X or ×
		static bool IsButtonTrigger_CANCEL() {
			if (
				tnl::Input::IsPadDownTrigger(ePad::KEY_1) ||
				tnl::Input::IsKeyDownTrigger(eKeys::KB_ESCAPE) ||
				tnl::Input::IsKeyDownTrigger(eKeys::KB_X)

				)return true;
			else return false;
		}

		// LeftControl or num5 or L2
		static bool IsButton_L2_LeftControl() {
			if (
				tnl::Input::IsKeyDown(eKeys::KB_LCONTROL) ||
				tnl::Input::IsKeyDown(eKeys::KB_NUMPAD5) ||
				tnl::Input::IsPadDown(ePad::KEY_6)

				)return true;
			else return false;
		}
	};

}