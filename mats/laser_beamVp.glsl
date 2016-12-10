#version 400

// Attributes passed automatically by OGRE
in vec3 vertex; // actually the end point
in vec3 normal; 
in vec4 colour;

// Attributes passed with the material file
uniform mat4 world_mat;

// Specific values
uniform float timer;
uniform float maxTimer;

// Attributes forwarded to the fragment shader
out vec4 colour_interp;
out vec4 face_dir;

void main()
{
	float startTime = normal.x;
	float endTime = startTime + maxTimer;
	
	float posVal = (timer - startTime) / (endTime - startTime);
	
	vec3 origin = vec3(world_mat * vec4(0.0, 0.0, 0.0, 1.0));
	
    gl_Position = vec4(mix(origin, vertex, posVal), 1.0);

    colour_interp = colour;
    face_dir = vec4(vertex, 1.0);
}