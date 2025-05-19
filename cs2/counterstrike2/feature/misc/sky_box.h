#pragma once

class c_skybox
{
public:
	void fsn(int stage);
private:
	i_material* reconstruct_sky_material(i_material* mat);
	void sky_box_changer();
protected:
};

inline auto g_skybox = std::make_unique<c_skybox>();