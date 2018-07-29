#include <iostream>
using std::cout;
using std::endl;

#include <Reflect.h>
#include <StaticReflect.h>

namespace FilmString 
{
	char className[] = "Film";
	char field0[] = "_title";
	char field1[] = "_director";
	char field2[] = "_year";
}

class Film
{
	friend class Reflect::ClassTypeImpl<Film>;
	friend class StaticReflect::TypeInfo<Film>;
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

	virtual ~Film() {}

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
		}
	};

} // namespace Reflect

namespace StaticReflect
{

	template <>
	class TypeInfo<Film>
	{
		typedef FieldInfo<FilmString::field0, Film, TypeInfo<std::string>, &Film::_title> FieldInfo0;
		typedef FieldInfo<FilmString::field1, Film, TypeInfo<std::string>, &Film::_director> FieldInfo1;
		typedef FieldInfo<FilmString::field2, Film, TypeInfo<int>, &Film::_year> FieldInfo2;
		typedef TYPELIST_3(FieldInfo0, FieldInfo1, FieldInfo2) FieldList;

	public:
		class Apply : public Aux::TypeInfoData<FilmString::className, Film, FieldList> {};
	};

} // namespace StaticReflect

int main()
{
	Film film("Titolo", "Regista", 1234);
	Serialization::OutArchive* out = Serialization::staticSerialize(film);
	cout << out->size() << endl;

	Reflect::ObjHolder* obj = Serialization::deserialize(new Serialization::InArchive(out->get()));
	cout << obj->get<Film>().title() << endl;
	cout << obj->get<Film>().director() << endl;
	cout << obj->get<Film>().year() << endl;

	system ("pause");
	return 0;
}
