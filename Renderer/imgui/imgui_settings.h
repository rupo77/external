#include "imgui.h"

namespace c {

	inline ImVec4 shadow = ImColor(3, 3, 3, 150);
	inline ImVec4 shadow_circle = ImColor(3, 3, 3, 200);


	namespace bg
	{
		inline ImVec4 background_0 = ImColor(0, 0, 0, 200);
		inline ImVec4 background_1 = ImColor(28, 29, 32, 220);
		inline ImVec4 border = ImColor(61, 71, 82, 200);

		inline ImVec2 size = ImVec2(660, 740);
		inline float rounding = 6.f;
	}

	namespace child
	{
		inline ImVec4 background_0 = ImColor(0, 0, 0, 255 / 2);
		inline ImVec4 background_1 = ImColor(32, 32, 32, 0);
		inline ImVec4 border = ImColor(71, 81, 92, 100);

		inline ImVec4 text_gradient_0 = ImColor(255, 255, 255, 255);
		inline ImVec4 text_gradient_1 = ImColor(108, 112, 150, 255);

		inline float rounding = 8;
	}

	namespace logo_text
	{
		inline ImVec4 text_gradient_0 = ImColor(255, 255, 255, 255);
		inline ImVec4 text_gradient_1 = ImColor(108, 112, 150, 255);
	}

	namespace checkbox
	{
		inline ImVec4 mark_active = ImColor(0, 0, 0, 255);
		inline ImVec4 mark_inactive = ImColor(0, 0, 0, 0);

		inline ImVec4 circle_active = ImColor(255, 255, 255, 255);
		inline ImVec4 circle_inactive = ImColor(80, 78, 91, 255);

		inline ImVec4 background_active_0 = ImColor(196, 186, 255, 255);
		inline ImVec4 background_active_1 = ImColor(116, 99, 220, 255);

		inline ImVec4 background_inactive_0 = ImColor(60, 58, 71, 255);
		inline ImVec4 background_inactive_1 = ImColor(32, 32, 32, 255);

		inline ImVec4 border_active = ImColor(116, 99, 220, 255);
		inline ImVec4 border_inactive = ImColor(83, 80, 104, 255);

		inline ImVec4 icon = ImColor(80, 82, 104, 255);

		inline float rounding = 4;
	}
	
	namespace slider
	{
		inline ImVec4 circle_active = ImColor(255, 255, 255, 255);

		inline ImVec4 background_active_0 = ImColor(196, 186, 255, 255);
		inline ImVec4 background_active_1 = ImColor(116, 99, 220, 255);

		inline ImVec4 background_inactive_0 = ImColor(60, 58, 71, 255);
		inline ImVec4 background_inactive_1 = ImColor(32, 32, 32, 255);
	}

	namespace button
	{
		inline ImVec4 background_active_0 = ImColor(56, 54, 69, 255);
		inline ImVec4 background_active_1 = ImColor(29, 29, 33, 255);

		inline ImVec4 background_inactive_0 = ImColor(46, 44, 53, 255);
		inline ImVec4 background_inactive_1 = ImColor(29, 29, 32, 255);

		inline ImVec4 gradient_line_0 = ImColor(179, 177, 255, 255);
		inline ImVec4 gradient_line_1 = ImColor(179, 177, 255, 0);


		inline float rounding = 4;
	}

	namespace combo
	{
		inline ImVec4 background_active_0 = ImColor(46, 44, 53, 255);
		inline ImVec4 background_active_1 = ImColor(29, 29, 33, 255);

		inline ImVec4 circle_active = ImColor(116, 99, 220, 255);
		inline ImVec4 circle_inactive = ImColor(116, 99, 220, 0);

		inline ImVec4 icon = ImColor(80, 82, 104, 255);

		inline float rounding = 4;
	}

	namespace picker
	{
		inline ImVec4 background = ImColor(28, 29, 32, 220);
		inline float rounding = 4;
	}


	namespace scroll
	{
		inline ImVec4 background = ImColor(116, 99, 220, 255);
		inline float scroll_size = 8.f;
	}

	namespace input
	{
		inline ImVec4 background_active_0 = ImColor(56, 54, 69, 255);
		inline ImVec4 background_active_1 = ImColor(29, 29, 33, 255);

		inline ImVec4 background_inactive_0 = ImColor(46, 44, 53, 255);
		inline ImVec4 background_inactive_1 = ImColor(29, 29, 32, 255);

		inline ImVec4 text_selected = ImColor(5, 5, 5, 50);


		inline ImVec4 icon = ImColor(80, 82, 104, 255);
		inline float rounding = 4;
	}

	namespace tab
	{
		inline ImVec4 image_color0 = ImColor(192, 182, 255, 100);
		inline ImVec4 image_color1 = ImColor(196, 186, 255, 255);

		inline ImVec4 shadow_color0 = ImColor(192, 182, 255, 0);
		inline ImVec4 shadow_color1 = ImColor(196, 186, 255, 255);
	}

	namespace keybind
	{
		inline ImVec4 background_active_0 = ImColor(56, 54, 69, 255);
		inline ImVec4 background_active_1 = ImColor(29, 29, 33, 255);

		inline ImVec4 background_inactive_0 = ImColor(46, 44, 53, 255);
		inline ImVec4 background_inactive_1 = ImColor(29, 29, 32, 255);

		inline ImVec4 icon = ImColor(80, 82, 104, 255);
		inline float rounding = 2;
	}

	namespace text
	{
		inline ImVec4 text_active = ImColor(255, 255, 255, 255);
		inline ImVec4 text_hov = ImColor(134, 143, 157, 255);
		inline ImVec4 text = ImColor(83, 80, 104, 255);
	}

	namespace grid
	{
		inline ImVec4 grid_one = ImColor(35, 40, 45, 255);
		inline ImVec4 grid_two = ImColor(15, 20, 25, 255);
	}

}
