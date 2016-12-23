#pragma once

#include <list>

class File;

class Loader
{
private:
	Loader();
	~Loader();

	std::list<File*>files_;
	static Loader *instance_;

	template <typename T>
	inline friend void SafeDelete(T*& p);

	template <typename T>
	inline friend void SafeDeleteArray(T*& p);

	template <typename T>
	inline friend void SafeRelease(T*& p);

public:

	static Loader *instance();
	static void create();
	static void destroy();

	void createFile(File**, const char *filename);
	void destroyFile(File** );
	void update();

};

