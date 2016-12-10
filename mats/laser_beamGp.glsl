#version 400

// Definition of the geometry shader
layout (points) in;
layout (triangle_strip, max_vertices = 8) out;

// Attributes passed from the vertex shader
in vec4 colour_interp[];
in vec4 face_dir[];

// Attributes passed with the material file
uniform mat4 projection_mat;
uniform mat4 view_mat;
uniform vec3 up_vec;
uniform float particle_size;

// Attributes passed to the fragment shader
out vec4 frag_colour;
out vec2 tex_coord;


void main(void){

	// Get the position and direciton of the particle
	vec3 position = vec3(gl_in[0].gl_Position);
	vec3 fwd_vec = normalize(vec3(face_dir[0]));
	
	vec3 globalUpVec = normalize(up_vec);
	
	// Find the vectors that will define the rectangles
	vec3 sideVec = normalize(cross(globalUpVec, fwd_vec));
	vec3 localUpVec = normalize(cross(fwd_vec, sideVec));

	// Define the positions of the four vertices that will form a quad 
	// The positions are based on the position of the particle and its size
	// We simply add offsets to the position (we can think of it as the center of the particle),
	// since we are already in world space
	vec4 v[8];
	v[0] = vec4(position - 0.5*particle_size*localUpVec, 1.0);
    v[1] = vec4(position + 0.5*particle_size*localUpVec, 1.0);
    v[2] = vec4(position - 0.5*particle_size*localUpVec + 5.0*particle_size*fwd_vec, 1.0);
    v[3] = vec4(position + 0.5*particle_size*localUpVec + 5.0*particle_size*fwd_vec, 1.0);
    
    v[4] = vec4(position - 0.5*particle_size*sideVec, 1.0);
    v[5] = vec4(position + 0.5*particle_size*sideVec, 1.0);
    v[6] = vec4(position - 0.5*particle_size*sideVec + 5.0*particle_size*fwd_vec, 1.0);
    v[7] = vec4(position + 0.5*particle_size*sideVec + 5.0*particle_size*fwd_vec, 1.0);

    // Create the new geometry: a quad with four vertices from the vector v
    for (int i = 0; i < 4; i++){
		gl_Position = projection_mat * view_mat * v[i];
		frag_colour = colour_interp[0];
		tex_coord = vec2(floor(i/2)*0.5, (i%2)*0.5);
        EmitVertex();
     }

	 EndPrimitive();
	 
	 for (int i = 4; i < 8; i++){
		gl_Position = projection_mat * view_mat * v[i];
		frag_colour = colour_interp[0];
		tex_coord = vec2(floor((i-4)/2), ((i-4)%2));
        EmitVertex();
     }

	 EndPrimitive();
}