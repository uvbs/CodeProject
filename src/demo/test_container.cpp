#include "type.h"
#include "socketdef.h"
#include "test_container.h"
#include "hash_table.h"
#include "rbtree.h"
#include "list.h"
#include "mcHash.h"
#include "mcBHeap.h"

using std::cout;
using std::cout;
using std::endl;
using container::List;

struct Student
{
public:
	Student(const char* str, int id, int score = 0) : _name(str), _id(id), _score(score) {}
	const string getName()  { return _name; }
	int getId() const {return _id; }
	int getScore() const {return _score; }

private:
	string _name;
	int _id;
	int _score;
};

struct _STU_LT_
{
	bool operator()(const Student *A, const Student *B) const
	{
		return (A->getScore()) > (B->getScore());
	}
};

class Seq : public BinHeap<Student*, _STU_LT_>
{
public:
	Student* GetBest();
	void  Update(Student* item);
};

inline Student* Seq::GetBest()
{
	if (_Size == 0)
		return NULL;

	Student* element = _List[1];
	Remove(1);
	return element;
}


void test_container()
{
	//test List container
	List<int> l;
	for (int i = 0; i < 10; ++i)
	{
		l.push_back(i);
	}
	List<int>::Iterator iter = l.begin();
	while (iter != l.end())
	{
		cout << *iter <<", ";
		++iter;
	}
	cout << endl;

	//test hashtable
	HashTable<char*, int> student_table(100);
	student_table.insert("zhangsan1", 1);
	student_table.insert("lisi2", 2);
	const int *pValue = student_table.find("zhangsan1");
	if (pValue) 
	{
		cout << "value is : " << *pValue << endl;
	}

	//test hashmap
	Student ss1("zhangsan1", 1);
	Student ss2("lisi2", 2);
	mcHashMap<HashInt, Student*> *hash_student = new mcHashMap<HashInt, Student*>;
	hash_student->Insert(1, &ss1);
	hash_student->Insert(2, &ss2);
	Student *obj = NULL;
	if (hash_student->Lookup(1, obj) && obj)
	{
		cout << "name is : " << obj->getName() << endl;
	}

	//test BinHeap
	Student s1("jerry", 1, 100);
	Student s2("tom", 2, 70);
	Student s3("lucy", 3, 90);
	Student s4("robert", 4, 80);
	Seq seq;
	seq.Insert(&s1);
	seq.Insert(&s2);
	seq.Insert(&s3);
	seq.Insert(&s4);
	while (Student* r = seq.GetBest())
	{
		cout << "name: " << r->getName()  << ", score: " << r->getScore() << endl;
	}

	//test rbtree
    rbtree<int8> t;
    t.insert(1);
    t.insert(7);
    t.insert(3);
    t.insert(2);
    t.print();

}

