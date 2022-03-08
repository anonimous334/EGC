#pragma once
#include <Component/SimpleScene.h>
#include "LabCamera.h"

class Laborator5 : public SimpleScene
{
public:
	float left, right, bottom, top, zNear, zFar;
	float fov, aspect;
	Laborator5();
	~Laborator5();

	void Init() override;

private:
	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;

	//void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix) override;
	void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color);

	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;
	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
	void OnWindowResize(int width, int height) override;
	int Coliziune(glm::mat4 Car, glm::mat4 Platforma);

protected:
	Laborator::Camera* camera;
	glm::mat4 projectionMatrix;
	glm::mat4 CarMatrix;
	glm::mat4 PlatMatrix;
	bool renderCameraTarget = false;
	float maxStep = 3.0f;
	float gravity = -10;
	float jump_power = 4;
	float UpWardsSpeed = 0;
	float terrain = 0.4;
	bool inAir = false;
	int generare = 1;

	float scaleZ = 5.0f;
	float dim = scaleZ * 1;
	float huita = -2.0f;
	float padding = 1;
	int cade = 1;

	int flag_afis = 1;
	glm::mat3 Color_Plat;
	int initial = 1;

	float norm_speed = 3.0;
	float plus_speed = 1.0;
	int flag_speed = 0;
	int Not_1 = 0;
	int cade_precis = 0;
};
