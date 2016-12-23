#pragma once

#include <string>

class Loader;

class File
{
private:
	File(const char *filename);
	~File();

	friend Loader;

	std::string filename_;
	char *date_;
	int size_;

	template <typename T>
	inline friend void SafeDelete(T*& p);

	template <typename T>
	inline friend void SafeDeleteArray(T*& p);

	template <typename T>
	inline friend void SafeRelease(T*& p);

public:

	bool isReady() const;
	int getSize() const;
	const char *getDate() const;

};

