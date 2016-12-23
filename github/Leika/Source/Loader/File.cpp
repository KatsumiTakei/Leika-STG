#include "File.h"
#include "../Function.h"

using namespace std;

File::File(const char *filename)
: filename_(filename),date_(0),size_(0)
{
}

File::~File()
{
	if (date_)
	{
		SafeDeleteArray(date_);
	}
	size_ = 0;
}

bool File::isReady()const
{// データがあれば実行
	return (date_ != 0);
}

int File::getSize()const
{
	assert(date_ && "Loading is not finished.");
	return size_;
}

const char *File::getDate()const
{
	assert(date_ && "Loading is not finished.");
	return date_;
}