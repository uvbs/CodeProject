#ifndef STATIC_REFLECT_H
#define STATIC_REFLECT_H

#include <string>

#include <FLib/TypeList.h>
#include <Archive.h>

namespace StaticReflect 
{

	namespace Aux
	{
		template <
			char* _name,
			typename _Type, 
			typename _FieldList = FLib::NullType, 
			typename _ClassList = FLib::NullType
		>
		class TypeInfoData
		{
		public:
			inline static const char* name() { return _name; }

			typedef _Type Type;
			typedef _FieldList FieldList;
			typedef _ClassList ClassList;
		};

		namespace TypeInfoSpecializationHelper
		{
			char intStr[] = "int";
			char charStr[] = "char";
			char floatStr[] = "float";
			char stringStr[] = "string";
		}

	} //namespace Aux

	template <typename _Type>
	class TypeInfo;

	template <
		char* _name,
		typename _ClassType,
		typename _FieldTypeInfo,
		typename typename _FieldTypeInfo::Apply::Type _ClassType::* _data
	>
	class FieldInfo
	{
	public:
		inline static const char* name() { return _name; }

		typedef _ClassType ClassType;
		typedef _FieldTypeInfo FieldTypeInfo;
		
		static inline typename _FieldTypeInfo::Apply::Type _ClassType::* data() { return _data; }
	};

	template<>
	class TypeInfo<char> 
	{
	public:
		class Apply : public Aux::TypeInfoData<Aux::TypeInfoSpecializationHelper::charStr, char> {};
	};

	template<>
	class TypeInfo<int> 
	{
	public:
		class Apply : public Aux::TypeInfoData<Aux::TypeInfoSpecializationHelper::intStr, int> {};
	};

	template<>
	class TypeInfo<float> 
	{
	public:
		class Apply : public Aux::TypeInfoData<Aux::TypeInfoSpecializationHelper::floatStr, float> {};
	};

	template<>
	class TypeInfo<std::string> 
	{
	public:
		class Apply : public Aux::TypeInfoData<Aux::TypeInfoSpecializationHelper::stringStr, std::string> {};
	};

	template <typename TList, typename Fun>
	struct ForEachField;

	template <typename Head, typename Tail, typename Fun>
	struct ForEachField<FLib::TypeList<Head,Tail>, Fun>
	{
		template <typename ObjType>
		static int apply(const ObjType& obj, Serialization::Aux::ConstPointerRegistry* objReg)
		{
			return (Fun::template Apply<typename Head::FieldTypeInfo::Apply::Type>::apply(obj.*Head::data(), objReg) + ForEachField<Tail, Fun>::apply(obj, objReg));
		}

		template <typename ObjType>
		static void apply(const ObjType& obj, Serialization::OutArchive* dst, Serialization::Aux::ConstPointerRegistry* objReg)
		{
			Fun::template Apply<typename Head::FieldTypeInfo::Apply::Type>::apply(obj.*Head::data(), dst, objReg);
			ForEachField<Tail, Fun>::apply(obj, dst, objReg);
		}
	};

	template <typename Fun>
	struct ForEachField<FLib::NullType, Fun>
	{
		template <typename ObjType>
		static int apply(const ObjType& obj, Serialization::Aux::ConstPointerRegistry*)
		{
			return 0;
		}

		template <typename ObjType>
		static void apply(const ObjType& obj, Serialization::OutArchive* dst, Serialization::Aux::ConstPointerRegistry*)
		{
			
		}
	};

} // namespace StaticReflect 

namespace Serialization
{

	namespace Aux
	{

		template <typename T>
		struct FixedTypeSize
		{
			static int apply(const T&, ConstPointerRegistry*) 
			{ 
				return sizeof(T); 
			}
		};

		struct Size
		{
			template <typename T>
			struct Apply
			{
				static int apply(const T& obj, ConstPointerRegistry* objReg) 
				{ 
					return StaticReflect::ForEachField<typename StaticReflect::TypeInfo<T>::Apply::FieldList, Size>::apply(obj, objReg);
				}
			};

			template <>
			struct Apply<int> : FixedTypeSize<int> {};

			template <>
			struct Apply<char> : FixedTypeSize<char> {};

			template <>
			struct Apply<float> : FixedTypeSize<float> {};

			template <>
			struct Apply<std::string>
			{
				static int apply(const std::string& obj, ConstPointerRegistry*)
				{ 
					return (1 + sizeof(int) + obj.size()); 
				}
			};

			template <typename T>
			struct Apply<T*>
			{
				static int apply(const T*& obj, ConstPointerRegistry* objReg) 
				{ 
					
				}
			};
		};

		template <typename T>
		struct FixedTypeSerialize
		{
			static void apply(const T& obj, OutArchive* dst, ConstPointerRegistry*) 
			{ 
				return dst->write(&obj, sizeof(T)); 
			}
		};

		struct Serialize
		{
			template <typename T>
			struct Apply
			{
				static void apply(const T& obj, OutArchive* dst, ConstPointerRegistry* objReg) 
				{
					StaticReflect::ForEachField<typename StaticReflect::TypeInfo<T>::Apply::FieldList, Serialize>::apply(obj, dst, objReg);
				}
			};

			template <>
			struct Apply<int> : FixedTypeSerialize<int> {};

			template <>
			struct Apply<char> : FixedTypeSerialize<char> {};

			template <>
			struct Apply<float> : FixedTypeSerialize<float> {};

			template <>
			struct Apply<std::string>
			{
				static void apply(const std::string& obj, OutArchive* dst, ConstPointerRegistry*) 
				{
					storeString(obj, dst);
				}
			};

			template <typename T>
			struct Apply<T*>
			{
				static void apply(const T*& obj, OutArchive* dst, ConstPointerRegistry* objReg) 
				{
					
				}
			};
		};
	}

	template <typename T>
	OutArchive* staticSerialize(const T& obj)
	{
		Serialization::Aux::ConstPointerRegistry objReg;
		objReg.add(&obj);
		int size = (1 + (int) sizeof(int) + (int) strlen(StaticReflect::TypeInfo<T>::Apply::name()));
		size += Aux::Size::Apply<T>::apply(obj, &objReg);
		OutArchive* dst = new OutArchive(size);
		objReg.clear();
		objReg.add(&obj);
		Aux::storeString(StaticReflect::TypeInfo<T>::Apply::name(), dst);
		Aux::Serialize::Apply<T>::apply(obj, dst, &objReg);
		return dst;
	}
}

#endif
