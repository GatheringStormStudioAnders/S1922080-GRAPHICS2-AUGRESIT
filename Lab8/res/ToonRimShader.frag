#version 400

//Toon
uniform vec3 LightDir;

in vec3 Normal;

//Rim Vars
uniform vec3 IVD;

//out vec4 FragColor;

vec4 Toon();
vec4 Rim();

void main()
{
	gl_FragColor = Toon() * Rim();
}

vec4 Toon()
{
	float Intensity;
	vec3 Colour;

	Intensity = dot(LightDir, Normal);
	if(Intensity > 0.9)
	{
		Colour = vec3(1, 0, 0);
	}
	else if (Intensity > 0.8 && Intensity < 1)
	{
		Colour = vec3(0.9, 0, 0);
	}
	else if (Intensity > 0.8)
	{
		Colour = vec3(0.8, 0, 0);
	}
	else if (Intensity > 0.7)
	{
		Colour = vec3(0, 0.7, 0);
	}
		else if (Intensity > 0.6)
	{
		Colour = vec3(0, 0.6, 0);
	}
	else if (Intensity > 0.5)
	{
		Colour = vec3(0, 0, 0.5);
	}
	else if (Intensity > 0.4)
	{
		Colour = vec3(0, 0.4, 0);
	}
	else if (Intensity > 0.3)
	{
		Colour = vec3(0.3, 0, 0);
	}
	else if (Intensity > 0.2)
	{
		Colour = vec3(0, 0.2, 0);
	}
	else
	{
		Colour = vec3(0, 0, 0.1);
	}

	return vec4(Colour, 1.0); //Returning vec4 cause (R,G,B,Alpha)
}

vec4 Rim()
{
	float RimShader = 1 - max(dot(Normal, IVD), 0.0);
	vec3 Colour = vec3(smoothstep(0.0, 1.0, RimShader));
	return vec4(Colour, 1.0);
}