/////////////////////////////////////////
//                                     //
//  �t�@�C���̏I�[�Ŏ~�܂�Ȃ�         //
//  �d�l�̂��ߋC��t���邱��           //
//                                     //
/////////////////////////////////////////
#include "Loader.h"
#include "File.h"

#include "../Function.h"

#include <fstream>

using namespace std;

Loader *Loader::instance_ = nullptr;

Loader::Loader()
{
}

Loader::~Loader()
{
	typedef list<File*>::iterator It;
	for (It i = files_.begin(); i != files_.end(); ++i)
	{
		SafeDelete(*i);
	}
}

Loader *Loader::instance()
{
	return instance_;
}

void Loader::create()
{
	assert(!instance_ && "already created");
	instance_ = new Loader();
}

void Loader::destroy()
{
	SafeDelete(instance_);
}

void Loader::createFile(File** f, const char *filename)
{
	// �G���[���o
	assert(!(*f) && "pointer must be NULL.");
	// ���
	*f = new File(filename);
	files_.push_back(*f);
}

void Loader::destroyFile(File** f)
{
	if (!(*f))
	{
		return;
	}

	typedef list<File*>::iterator It;
	for (It i = files_.begin(); i != files_.end(); ++i)
	{
		if (*i == *f)
		{// ���̂�������
			// ���̂ƃ��X�g�̏ꏊ������
			SafeDelete(*f);
			files_.erase(i);
			*f = 0;
			break;
		}
	}
	assert(!(*f) && "can't find!");
}

void Loader::update()
{
	typedef list<File*>::iterator It;
	for (It i = files_.begin(); i != files_.end(); ++i)
	{
		File *f = *i;
		if (!f->isReady())
		{
			ifstream in(f->filename_.c_str(), ifstream::binary);
			in.seekg(0, ifstream::end);
			f->size_ = static_cast<int>(in.tellg());
			in.seekg(0, ifstream::beg);
			f->date_ = new char[f->size_];
			in.read(f->date_, f->size_);
			break;
		}
	}
}