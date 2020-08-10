#include "MainGame.h"
#include "Camera.h"
#include <iostream>
#include <string>


Transform transform;

MainGame::MainGame()
{
	//Game State
	Game_State = GameState::PLAY;

	//Game Display
	Display* Game_Display = new Display(); //Declares new display.

	//Meshes
    Mesh* Mesh_1(); 
	Mesh* Mesh_2();
	Mesh* Mesh_3();

	//Audio
	Audio* audioDevice();

	//Textures
	Texture* Load_Texture_1();
	Texture* Load_Texture_2();
	Overlay* Load_Overlay();

	//Shaders
	Shader* Shader_Pass();
	Shader* Shader_Blur();
	Shader* Toon_Shader();
	Shader* Rim_Shader();
	Shader* ReflectionShader();
	Shader* Geomtry_Shader();
}

MainGame::~MainGame() 
{
}

void MainGame::run()
{
	initSystems(); 
	gameLoop();
}

void MainGame::initSystems()
{
	Game_Display.initDisplay(); 
	whistle = audioDevice.loadSound("..\\res\\bang.wav");
	backGroundMusic = audioDevice.loadSound("..\\res\\background.wav");
	Texture_1.init("..\\res\\bricks.jpg"); //load texture
	Texture_2.init("..\\res\\water.jpg"); //load texture


	shaderSkybox.init("..\\res\\shaderSkybox.vert", "..\\res\\shaderSkybox.frag", false);
	GeoShader.init("..\\res\\GeometryShader.vert", "..\\res\\GeometryShader.frag", true);
	shaderPass.init("..\\res\\RimShader.vert","..\\res\\RimShader.frag", false);
	shaderBlur.init("..\\res\\shaderBlur.vert", "..\\res\\shaderBlur.frag", false);
	//shaderToon.init("..\\res\\shaderToon.vert", "..\\res\\shaderToon.frag", false);
	RimShader.init("..\\res\\RimShader.vert", "..\\res\\RimShader.frag", false);
	ReflectionShader.init("..\\res\\ReflectionShader.vert", "..\\res\\ReflectionShader.frag", false);
	ToonRimShader.init("..\\res\\ToonRimShader.vert", "..\\res\\ToonRimShader.frag", false);
	//Vertex2D vertices[] = { Vertex2D(glm::vec2(-0.5, 1.0), glm::vec2(0.0, 0.0)),
	//						Vertex2D(glm::vec2(0.5, 0.5), glm::vec2(1.0, 0.0)),
	//						Vertex2D(glm::vec2(0.5,-0.5), glm::vec2(1.0, 1.0)),
	//						Vertex2D(glm::vec2(-0.5,-0.5), glm::vec2(0.0, 1.0)) };

	_Overlay.init("..\\res\\bricks.jpg");

	Mesh_1.loadModel("..\\res\\monkey3.obj");
	Mesh_2.loadModel("..\\res\\monkey3.obj");
	Mesh_3.loadModel("..\\res\\monkey3.obj");
	
	myCamera.initCamera(glm::vec3(0, 0, -10.0), 70.0f, (float)Game_Display.getWidth() / Game_Display.getHeight(), 0.01f, 1000.0f);

	counter = 1.0f;

	vector<std::string> faces
	{
		"..\\res\\skybox\\right.jpg",
		"..\\res\\skybox\\left.jpg",
		"..\\res\\skybox\\top.jpg",
		"..\\res\\skybox\\bottom.jpg",
		"..\\res\\skybox\\front.jpg",
		"..\\res\\skybox\\back.jpg"
	};
	cubemapTexture = SkyBox_Texture.loadCubemap(faces); //Load the cubemap using "faces" into cubemapTextures

	float skyboxVertices[] = {
		// positions          
		-6.0f,  6.0f, -6.0f,
		-6.0f, -6.0f, -6.0f,
		6.0f, -6.0f, -6.0f,
		6.0f, -6.0f, -6.0f,
		6.0f,  6.0f, -6.0f,
		-6.0f,  6.0f, -6.0f,

		-6.0f, -6.0f,  6.0f,
		-6.0f, -6.0f, -6.0f,
		-6.0f,  6.0f, -6.0f,
		-6.0f,  6.0f, -6.0f,
		-6.0f,  6.0f,  6.0f,
		-6.0f, -6.0f,  6.0f,

		6.0f, -6.0f, -6.0f,
		6.0f, -6.0f,  6.0f,
		6.0f,  6.0f,  6.0f,
		6.0f,  6.0f,  6.0f,
		6.0f,  6.0f, -6.0f,
		6.0f, -6.0f, -6.0f,

		-6.0f, -6.0f,  6.0f,
		-6.0f,  6.0f,  6.0f,
		6.0f,  6.0f,  6.0f,
		6.0f,  6.0f,  6.0f,
		6.0f, -6.0f,  6.0f,
		-6.0f, -6.0f,  6.0f,

		-6.0f,  6.0f, -6.0f,
		6.0f,  6.0f, -6.0f,
		6.0f,  6.0f,  6.0f,
		6.0f,  6.0f,  6.0f,
		-6.0f,  6.0f,  6.0f,
		-6.0f,  6.0f, -6.0f,

		-6.0f, -6.0f, -6.0f,
		-6.0f, -6.0f,  6.0f,
		6.0f, -6.0f, -6.0f,
		6.0f, -6.0f, -6.0f,
		-6.0f, -6.0f,  6.0f,
		6.0f, -6.0f,  6.0f
	};

	//use openGL functionality to generate & bind data into buffers
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

}

void MainGame::gameLoop()
{
	while (Game_State != GameState::EXIT)
	{
		processInput();
		drawGame();
		playAudio(backGroundMusic, glm::vec3(0.0f,0.0f,0.0f));
	}
}

void MainGame::processInput()
{
	SDL_Event evnt;

	while(SDL_PollEvent(&evnt)) //get and process events
	{
		switch (evnt.type)
		{
			case SDL_QUIT:
				Game_State = GameState::EXIT;
				break;
			case SDLK_ESCAPE:
				Game_State = GameState::EXIT;
				break;
		}
	}
	
}

void MainGame::Skybox()
{
	ModelView = transform.GetModel() * myCamera.GetView();
	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
	shaderSkybox.Bind();
	shaderSkybox.setInt("skybox", 0);
	view = glm::mat4(glm::mat3(ModelView)); // remove translation from the view matrix
	shaderSkybox.setMat4("view", view);
	shaderSkybox.setMat4("projection", myCamera.GetProjection());
	// skybox cube
	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS); // set depth function back to default
}

void MainGame::playAudio(unsigned int Source, glm::vec3 pos)
{
	
	ALint state; 
	alGetSourcei(Source, AL_SOURCE_STATE, &state);
	/*
	Possible values of state
	AL_INITIAL
	AL_STOPPED
	AL_PLAYING
	AL_PAUSED
	*/
	if (AL_PLAYING != state)
	{
		//audioDevice.playSound(Source, pos);
	}
}

void MainGame::setADSLighting()
{
	ModelView = transform.GetModel() * myCamera.GetView();
	
	shaderPass.setMat4("ModelViewMatrix", ModelView);
	shaderPass.setMat4("ProjectionMatrix", myCamera.GetProjection()); 
	
	glm::mat4 normalMatrix = transpose(inverse(ModelView));
	
	shaderPass.setMat4("NormalMatrix", normalMatrix);

	shaderPass.setVec4("Position", glm::vec4(10.0,10.0,10.0,1.0));
	shaderPass.setVec3("Intensity", glm::vec3(0.0, 0.0, 0.0));

	shaderPass.setVec3("ka", glm::vec3(0.5, 0.5, 0.5));
	shaderPass.setVec3("kd", glm::vec3(0.5, 0.5, 0.5));
	shaderPass.setVec3("ks", glm::vec3(0.5, 0.5, 0.5));

	shaderPass.setFloat("Shininess", 0.5);
}

void MainGame::setReflection()
{
	//modelView = transform.GetModel() * myCamera.GetView();
	ReflectionShader.setVec3("cameraPos", myCamera.getPos());
	ReflectionShader.setMat4("view", myCamera.GetView());
	ReflectionShader.setMat4("projection", myCamera.GetProjection());
	ReflectionShader.setMat4("model", ModelView);
}

void MainGame::setToonLighting()
{
	shaderToon.setVec3("lightDir", glm::vec3(0.5, 0.5, 0.5));
}

void MainGame::setRimShader()
{
	RimShader.setMat4("u_vm", myCamera.GetView());
	RimShader.setMat4("u_pm", myCamera.GetProjection());
}

void MainGame::setGeoshader()
{
	GeoShader.setFloat("time", counter);
}

void MainGame::blobEffect()
{
	GLuint blockIndex = glGetUniformBlockIndex(shaderBlur.getProgram(), "BlobSettings");

	GLint blockSize;
	glGetActiveUniformBlockiv(shaderBlur.getProgram(), blockIndex,
		GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize); //get information about blobsettings and save it in blockSize

	GLubyte * blockBuffer = (GLubyte *)malloc(blockSize); //allocates the requested memory and returns a pointer to it.

														  // Query for the offsets of each block variable
	const GLchar *names[] = { "InnerColor", "OuterColor",
		"RadiusInner", "RadiusOuter" };

	GLuint indices[4];
	glGetUniformIndices(shaderBlur.getProgram(), 4, names, indices); // glGetUniformIndices retrieves the indices of a number of uniforms within program

	GLint offset[4];
	glGetActiveUniformsiv(shaderBlur.getProgram(), 4, indices, GL_UNIFORM_OFFSET, offset); //Returns information about several active uniform variables for the specified program object

	GLfloat outerColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	GLfloat innerColor[] = { 1.0f, 1.0f, 0.75f, 1.0f };

	GLfloat innerRadius = 0.0f, outerRadius = 3.0f;

	memcpy(blockBuffer + offset[0], innerColor,
		4 * sizeof(GLfloat)); //destination, source, no of bytes. 
	memcpy(blockBuffer + offset[1], outerColor,
		4 * sizeof(GLfloat));
	memcpy(blockBuffer + offset[2], &innerRadius,
		sizeof(GLfloat));
	memcpy(blockBuffer + offset[3], &outerRadius,
		sizeof(GLfloat));

	GLuint uboHandle;

	glGenBuffers(1, &uboHandle);

	glBindBuffer(GL_UNIFORM_BUFFER, uboHandle);
	glBufferData(GL_UNIFORM_BUFFER, blockSize, blockBuffer,
		GL_DYNAMIC_DRAW); //creates and initializes a buffer object's data store - targer, size, data, usage

	glBindBufferBase(GL_UNIFORM_BUFFER, blockIndex, uboHandle); // bind a buffer object to an indexed buffer target - trager, index, buffer
}

void MainGame::SetToonRimShader()
{
	ToonRimShader.setVec3("LightDir", glm::vec3(1, 1, 1));
	ToonRimShader.setVec3("IVD", glm::vec3(myCamera.getPos() - myCamera.getForward()));
}

void MainGame::drawGame()
{
	Game_Display.clearDisplay(0.0f, 0.0f, 0.0f, 1.0f);

	Skybox();

	transform.SetPos(glm::vec3(0, 0, -8.0));
	transform.SetRot(glm::vec3(sinf(counter), counter * 5, 0.0));
	transform.SetScale(glm::vec3(0.6, 0.6, 0.6));
	
	ReflectionShader.Bind();
	setReflection();
	ReflectionShader.Update(transform, myCamera);

	Mesh_1.draw();
	
	transform.SetPos(glm::vec3(-sinf(counter), -1.0, -sinf(counter)*5));
	transform.SetRot(glm::vec3(0.0, counter * 5, 0.0));
	transform.SetScale(glm::vec3(0.6, 0.6, 0.6));
	
	ToonRimShader.Bind();
	SetToonRimShader();
	ToonRimShader.Update(transform, myCamera);
	Mesh_2.draw();


	transform.SetPos(glm::vec3(-sinf(counter), -0.75, -sinf(counter)));
	transform.SetRot(glm::vec3(0.0, counter * 5, 0.0));
	transform.SetScale(glm::vec3(0.6, 0.6, 0.6));

	Texture_1.Bind(0);
	GeoShader.Bind();
	setGeoshader();
	GeoShader.Update(transform, myCamera);
	Mesh_3.draw();


	counter = counter + 0.001f;

	glEnableClientState(GL_COLOR_ARRAY); 
	glEnd();

	Game_Display.swapBuffer();
} 

