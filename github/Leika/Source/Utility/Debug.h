#pragma once

class Debug
{
private:
	FILE *dummy_;

public:

	static Debug *Instance()
	{
		static Debug instance;
		return &instance;
	}
	template <typename T>
	void Log(T message) {
	
		std::cout << message << std::endl;
	}
	void CreateConsoleWindow();
	void CloseConsoleWindow();
};
extern Debug *debug;