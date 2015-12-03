#pragma once

struct camera
{
	vec3	eye = vec3(0, 0, 10);
	vec3	at = vec3(0, 0, 0);
	vec3	up = vec3(0, 1, 0);
	mat4	view_matrix = mat4::lookAt(eye, at, up);

	float	fovy = PI / 4.0f; // must be in radian
	float	aspect_ratio;
	float	dNear = 1.0f;
	float	dFar = 1000.0f;
	mat4	projection_matrix;

	vec2 sphere_angle = vec2(90.0, 180.0); //theta, pi

	void resetCamera()
	{
		sphere_angle = vec2(90.0, 180.0);
	}

	void applyFirstPerson(ivec2 window_size, double x, double y)
	{
		ivec2 window_center = window_size / 2;
		ivec2 diff = ivec2((int)x, (int)y) - window_center;
		float scale = 0.1f;

		// 마우스가 움직이지 않으면 연산 생략
		if (diff == ivec2(0, 0))
			return;

		// 상하 각도는 1~179도
		sphere_angle.x -= diff.y * scale; //theta (0 < t < 180)
		if (sphere_angle.x > 179.0f)
			sphere_angle.x = 179.0f;
		else if (sphere_angle.x < 1.0f)
			sphere_angle.x = 1.0f;

		// 좌우는 360도 다 돌아가도록
		sphere_angle.y -= diff.x * scale; //pi (0 < t < 360)
		if (sphere_angle.y > 360.0f)
			sphere_angle.y -= 360.0f;
		else if (sphere_angle.y < 0.0f)
			sphere_angle.y += 360.0f;

		// 구면좌표계를 직교좌표계로 변환
		vec3 orthogonal_pos = vec3(
			sin(sphere_angle.x * (PI / 180))*sin(sphere_angle.y * (PI / 180)),
			-cos(sphere_angle.x * (PI / 180)),
			sin(sphere_angle.x * (PI / 180))*cos(sphere_angle.y * (PI / 180))
			);
		
		// at 좌표 적용
		at = eye + orthogonal_pos;
	}
};