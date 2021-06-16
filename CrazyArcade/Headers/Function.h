#pragma once

template <class T>
void Safe_Delete(T& obj)
{
	if (obj)
	{
		delete obj;
		obj = nullptr;
	}
}

template<class T>
void Safe_Delete_Array(T& obj)
{
	if (obj)
	{
		delete[] obj;
		obj = nullptr;
	}
}