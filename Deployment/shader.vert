#version 330
// Unknown operation 8
// Unknown operation 8
// Unknown operation 8
in vec3 pos;


void main()
{
	gl_Position = vec4(pos[0], pos[1], 0.5, 1.0);
	return;
}

