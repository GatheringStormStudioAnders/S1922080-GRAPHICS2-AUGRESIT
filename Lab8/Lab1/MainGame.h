#pragma once
#include <SDL\SDL.h>
#include <GL/glew.h>
#include "Display.h" 
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "transform.h"
#include "Audio.h"
#include "Overlay.h"

enum class GameState{PLAY, EXIT};

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();

private:

	void initSystems();
	void processInput();
	void gameLoop();
	void drawGame();
	void playAudio(unsigned int Source, glm::vec3 pos);
	void blobEffect();
	void setADSLighting();
	void setToonLighting();
	void setRimShader();
	void setReflection();
	void setGeoshader();
	void SetToonRimShader();

	void Skybox();

	GLuint skyboxVAO, skyboxVBO, cubemapTexture;
	vector<std::string> faces;

	Display Game_Display;
	GameState Game_State;
	Mesh Mesh_1;
	Mesh Mesh_2;
	Mesh Mesh_3;

	Texture Texture_1;
	Texture Texture_2;
	Texture SkyBox_Texture;

	Overlay _Overlay;
	Shader shaderPass;
	Shader shaderBlur;
	Shader shaderToon;
	Shader RimShader;
	Shader shaderSkybox;
	Shader ReflectionShader;
	Shader GeoShader;
	Shader ToonRimShader;

	glm::mat4 ModelView;
	glm::mat4 view;

	Camera myCamera;
	Audio audioDevice;

	float counter;
	unsigned int whistle;
	unsigned int backGroundMusic;

	float time;
	glm::vec2 mouse;
	glm::vec2 resolution;

	glm::vec4 FragColor;
};

