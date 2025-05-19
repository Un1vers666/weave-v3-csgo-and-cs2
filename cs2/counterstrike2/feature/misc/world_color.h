#pragma once

class c_world
{
public:
	void fsn(int stage);
private:
	i_material* reconstruct_material(i_material* mat);
	void world_color_changer();
protected:
};

inline auto g_world = std::make_unique<c_world>();