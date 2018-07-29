#include <Reflect.h>
using namespace Reflect;

#include <iostream>
using std::cout;
using std::endl;

class Film
{
	friend class Reflect::ClassTypeImpl<Film>;
public:
	Film()
	{
	
	}

	Film(std::string title, std::string director, int year)
	{
		_title = title;
		_director = director;
		_year = year;
	}

	Film* test1()
	{
		return this;
	}

	void test2()
	{
		cout << "Film::test2" << endl;
	}

	std::string* test3(Film* film)
	{
		return new std::string(film->_director + " " + _director);
	}

	std::string title() const { return _title; }
	std::string director() const { return _director; }
	int year() const { return _year; }

private:
	std::string _title;
	std::string _director;
	int _year;
};

namespace Reflect
{

	template <>
	class ClassTypeImpl<Film> 
		:	public ClassTypeTmpl<Film>, 
			public Singleton<ClassTypeImpl<Film> >
	{
		friend class Singleton<ClassTypeImpl<Film> >;
	private:
		ClassTypeImpl<Film>()
			:	ClassTypeTmpl<Film>("Film"), 
				Singleton<ClassTypeImpl<Film> >()
		{
			add(Factory::createField("_title", &Film::_title));
			add(Factory::createField("_director", &Film::_director));
			add(Factory::createField("_year", &Film::_year));
			add(Factory::createMethod("test1", &Film::test1));
			add(Factory::createMethod("test2", &Film::test2));
			add(Factory::createMethod("test3", &Film::test3));
		}
	};

} // namespace Reflect


extern "C"
{
	__declspec(dllexport) Reflect::TypeRegistry& exportTypeRegistry()
	{
		return Reflect::TypeRegistry::get();
	}

} // extern "C"

