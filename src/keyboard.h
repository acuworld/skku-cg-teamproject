#pragma once

struct keypress
{
	bool key_pressed[4]; //WASD
	unsigned int key_pressed_time[4];

	bool isKeyPressed()
	{
		for (uint i = 0; i < 4; i++)
			if (key_pressed[i])
				return true;
		return false;
	}

	void setKeyPress(uint id, bool pressed)
	{
		key_pressed[id] = pressed;
	}

	float getFacingAngle()
	{
		float angle = 0.0f; // camera coordinate -z axis (즉, 정면이 0도)
		uint lr = 0; // 1:L 2:R
		uint fb = 0; // 1:F 2:B

		if (key_pressed[1] && !key_pressed[3])		lr = 1;
		else if (!key_pressed[1] && key_pressed[3])	lr = 2;
		if (key_pressed[0] && !key_pressed[2])		fb = 1;
		else if (!key_pressed[0] && key_pressed[2])	fb = 2;

		if (lr == 0 && fb == 1)			angle = 0.0f;
		else if (lr == 1 && fb == 1)	angle = 45.0f;
		else if (lr == 1 && fb == 0)	angle = 90.0f;
		else if (lr == 1 && fb == 2)	angle = 135.0f;
		else if (lr == 0 && fb == 2)	angle = 180.0f;
		else if (lr == 2 && fb == 2)	angle = 225.0f;
		else if (lr == 2 && fb == 0)	angle = 270.0f;
		else if (lr == 2 && fb == 1)	angle = 315.0f;

		return angle;
	}

	vec3 calculateDifference(vec3 eye, vec3 at)
	{
		float scale = 0.01f;
		float angle = getFacingAngle();
		float rad = angle * (PI / 180);
		vec3 n, f;

		n = -(eye - at).normalize();
		f.x = cos(-rad)*n.x - sin(-rad)*n.z;
		f.z = sin(-rad)*n.x + cos(-rad)*n.z;
		f *= scale;

		return f;
	}
};

