#pragma once
/**
* @file   TaskManager.h
* @brief  Task の更新と描画
* @author Katsumi Takei
* @date   2016 / 06 / 29
* @date   最終更新 2016 / 11 / 02
*/

#include "Task.h"
#include "../Function.h"

class TaskManager
{
private:
	std::list<Task*> tasklist;
	std::list<Task*> newlist;
	DrawMode mode_;

	TaskManager() {}
	~TaskManager() {}

	void CleanUp() 
	{
		std::list <Task*>(tasklist).swap(tasklist);
		tasklist.insert(tasklist.end(), newlist.begin(), newlist.end());
		newlist.clear();
		std::list <Task*>(newlist).swap(newlist);
		tasklist.sort(compareByA);
	}

public:
	static TaskManager *Instance() 
	{
		static TaskManager instance;
		return &instance;
	}

	void Add(Task *task) 
	{
		newlist.push_back(task);
	}

	void Clear()
	{
		tasklist.clear();
		newlist.clear();
	}

	void SetMode(DrawMode mode)
	{
		mode_ = mode;
	}

	void Update() 
	{
		for (std::list<Task*>::iterator it = tasklist.begin(); it != tasklist.end(); ++it)
		{
			Task *task = *it;
			task->update();

			if (!task->getIsLiving()) 
			{
				it = tasklist.erase(it);
				it--;
			}
		}
		CleanUp();
	}

	void Draw()
	{
		for (std::list<Task*>::iterator it = tasklist.begin(); it != tasklist.end(); ++it) {
			Task *task = *it;
			if (mode_ == task->drawMode())
			{
				task->draw();
			}
		}
	}
};