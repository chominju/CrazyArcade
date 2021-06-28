#pragma once

class CScene;

class CScene_Manager
{
	DECLARE_SINGLETON(CScene_Manager)

public:
	CScene_Manager();
	~CScene_Manager();

public:
	HRESULT Change_Scene_Manager(SCENE_ID nextScene);
	void	Update_Scene_Manager();
	void	Render_Scene_Manager();
	void	Release_Scene_Manager();

private:
	SCENE_ID m_currentScene;
	SCENE_ID m_nextScene;
	CScene *m_scene;
};

