#version 330

// TODO: get vertex attributes from each location
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 text_coord;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 object_color;

// TODO: output values to fragment shader
out vec3 frag_color;
out vec3 frag_pos;
out vec2 frag_coord;
out vec3 frag_norm;

void main()
{
	// TODO: send output to fragment shader
	frag_color = object_color;
	frag_pos = v_position;
	frag_coord = text_coord;
	frag_norm = v_normal;

	// TODO: compute gl_Position
	gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}
