#version 400

// Attributes passed from the vertex shader
in vec4 colour_interp;
in vec2 tex_coord;

// Attributes passed with the material file
uniform sampler2D tex_samp;

out vec4 FragColor;

void main() 
{
	// Get pixel from texture
	vec4 outval = texture(tex_samp, tex_coord);
	
	// Adjust specified object colour according to the grayscale texture value
    //outval = vec4(outval.r*frag_colour.r, outval.g*frag_colour.g, outval.b*frag_colour.b, sqrt(sqrt(outval.r))*frag_colour.a);
	
	FragColor = outval;
	//FragColor = colour_interp;
}