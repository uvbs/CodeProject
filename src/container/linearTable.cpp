#include "linearTable.h"
#include "string_util.h"
#include <stdlib.h>

bool Comp(int a,int b)
{
	if(a==b)
		return true;
	else
		return false;
}

////////////////////////////////////////////////////////////////////////////
bool OderedLinearTable::InitList(SqList &list)//��ʼ��˳���
{
	list.elem=new int[LIST_INIT_SIZE];
	if(!list.elem)
		return false;
	list.length=0;
	list.listsize=LIST_INIT_SIZE;
	return true;
}

void OderedLinearTable::PutList(SqList &list)//���˳���
{
	for(int i=0;i<list.length;i++)
	{
		if(i%10==0 && i>0)
			cout<<endl;
		cout<<list.elem[i]<<" ";
	}
	cout<<endl;
}

bool OderedLinearTable::InsertList(SqList &list,int i,int e)//����Ԫ��
{
	int *p,*q;
	if(i<1 || i>list.length+1)
		return false;
	if(list.length>=list.listsize)
	{
		int *newBase=(int*)realloc(list.elem,(list.listsize+LISTINCREMENT)*sizeof(int));
		if(!newBase)
			return false;
		list.elem=newBase;
		list.listsize+=LISTINCREMENT;
	}
	q=&(list.elem[i-1]);
	for(p=&(list.elem[list.length-1]);p>=q;p--)
		*(p+1)=*p;
	*q=e;
	list.length+=1;
	return true;
}

bool OderedLinearTable::DeleteList(SqList &list,int i,int &e)//ɾ��Ԫ��
{
	if(i<1 || i>list.length)
		return false;
	int *p=&(list.elem[i-1]);
	e=*p;
	int *q=&(list.elem[list.length-1]);
	for(;p<q;p++)
		*p=*(p+1);
	list.length-=1;
	return true;
}

int OderedLinearTable::LocateList(SqList &list,int e,pFunction pFun/*=Comp*/)//Ĭ�ϲ���ΪCompare
{
	int i=1;
	int *p=list.elem;
	while(i<=list.length && !pFun(*p++,e)) ++i;
	if(i<=list.length)
		return i;
	else
		return 0;
}

void OderedLinearTable::MergeList(SqList list1,SqList list2,SqList &listResult)//˳���ĺϲ�
{
	int *pList1First=list1.elem;
	int *pList1Last=list1.elem+list1.length-1;
	int *pList2First=list2.elem;
	int *pList2Last=list2.elem+list2.length-1;

	listResult.listsize=listResult.length=list1.length+list2.length;
	int *pResult=listResult.elem=(int*)new int[listResult.listsize];
	if(!listResult.elem)
		return;
	while(pList1First<=pList1Last && pList2First<=pList2Last)
	{
		if(*pList1First<=*pList2First) *pResult++=*pList1First++;
		else *pResult++=*pList2First++;
	}
	while(pList1First<=pList1Last) *pResult++=*pList1First++;
	while(pList2First<=pList2Last) *pResult++=*pList2First++;
}


//////////////////////////////////////////////////////////////
void LinkedLinearTable::InitListFromBegin(LNode * &list,int n)//ǰ�巨
{
	list=new LNode;
	list->next=NULL;
	for(int i=n;i>0;i--)
	{
		LNode *p=new LNode;
		cout<<"������ڵ�����"<<endl;
		cin>>p->data;
		p->next=list->next;
		list->next=p;
	}
}

void LinkedLinearTable::InitListFromEnd(LNode *&list,int n)//��巨
{
	list=new LNode;
	list->next=NULL;
	LNode *pEnd=list;
	for(int i=n;i>0;i--)
	{
		LNode *p=new LNode;
		cout<<"������ڵ�����"<<endl;
		cin>>p->data;
		pEnd->next=p;
		pEnd=p;
	}
	pEnd->next=NULL;
}

void LinkedLinearTable::PutList(LNode *list)
{
	LNode *p=list->next;
	while(p) 
	{
		cout<<p->data<<" ";
		p=p->next;
	}
	cout<<endl;
}

bool LinkedLinearTable::GetListElem(LNode *list,int i,int &e)//�õ���i��λ���ϵ�Ԫ��
{
	LNode *p=list->next;
	int j=1;
	while(j<i && p)
	{
		p=p->next;
		j++;
	}
	if(!p || j>i) 
		return false;
	e=p->data;
	return true;
}

bool LinkedLinearTable::InsertList(LNode * list,int i,int e)//�ڵ�i��λ��ǰ�߲���Ԫ��e
{
	LNode *p=list;
	int j=0;
	while(p && j<i-1)
	{
		p=p->next;
		j++;
	}
	if(!p || j>i-1)
		return false;
	LNode *s=new LNode;
	s->data=e;
	s->next=p->next;
	p->next=s;
	return true;
}

bool LinkedLinearTable::DeleteList(LNode * list,int i,int &e)//ɾ����i��Ԫ��
{
	LNode *p=list;
	int j=0;
	while(p->next && j<i-1)
	{
		p=p->next;
		j++;
	}
	if(!(p->next) || (j>i-1))
		return false;
	LNode *q=p->next;
	p->next=q->next;
	e=q->data;
	delete q;
	return true;
}

void LinkedLinearTable::MergeList(LNode *&listA,LNode *&listB,LNode *&listC)
{
	LNode *pa=listA->next;
	LNode *pb=listB->next;
	LNode *pc;
	listC=pc=listA;
	while(pa && pb)
	{
		if(pa->data<=pb->data)
		{
			pc->next=pa;
			pc=pa;
			pa=pa->next;
		}
		else
		{
			pc->next=pb;
			pc=pb;
			pb=pb->next;
		}
	}
	pc->next=(pa?pa:pb);
	delete(listB);//ɾ��û�õı�ͷ�ڵ�
}

LinkedLinearTable::LNode* LinkedLinearTable::Reverse(LNode *h)
{
	LNode *p,*q;
	p=h->next;
	h->next=NULL;
	while(p)
	{
		q=p;
		p=p->next;
		q->next=h->next;
		h->next=q;
	}
	return h;
}

LinkedLinearTable::LNode * LinkedLinearTable::DeleteSame(LNode *h)
{
	LNode *p=h->next;
	LNode *q,*r;
	while(p->next!=NULL)
	{
		r=p;
		q=p->next;
		while(q->next!=NULL)
		{
			if(q->data==p->data)
			{
				r->next=q->next;
				free(q);
				q=r->next ;
			}
			else
			{
				r=q;
				q=q->next ;
			}	
		}
		p=p->next ;
	}
	return h;
}

//ɾ������dataΪx�Ľڵ�
void LinkedLinearTable::Delete(LNode *h,int x)
{
	LNode *q=h,*p=h->next;
	while((p->next)!=NULL)
	{
		if(p->data==x)
		{
			q->next=p->next;
			free(p);
			p=q->next;
		}
		else
		{
			q=p;
			p=p->next ;
		}
	}
	if(p->data==x)
	{
		q->next=NULL;
		free(p);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////
//��ǰ�巨��������ͷ�ڵ��˫��ѭ������
DoubleLinkedCircularTable::DLnode* DoubleLinkedCircularTable::CreateFromBegin(int tag)
{
	int x;
	DLnode *p,*h=(DLnode *)malloc(sizeof(DLnode));
	h->next=h;
	h->prior=h;
	h->data=tag;
	printf("Input x:");
	scanf("%d",&x);
	while(x!=tag)
	{
		p=(DLnode*)malloc(sizeof(DLnode));
		p->data=x;
		p->next=h->next;
		h->next->prior=p;
		h->next=p;
		p->prior=h;
		printf("Input x:");
		scanf("%d",&x);
	}
	return h;
}

//�ú�巨��������ͷ�ڵ��˫��ѭ������
DoubleLinkedCircularTable::DLnode *DoubleLinkedCircularTable::CreateFromEnd(int tag)
{
	int x;
	DLnode *h,*r,*p;
	h=(DLnode*)malloc(sizeof(DLnode));
	h->data=tag;
	h->prior=h;
	h->next=h;
	r=h;
	printf("Input x:");
	scanf("%d",&x);
	while(x!=tag)
	{
		p=(DLnode*)malloc(sizeof(DLnode));
		p->data=x;
		r->next=p;
		p->next=h;
		h->prior=p;
		p->prior=r;
		r=p;
		printf("Input x:");
		scanf("%d",&x);
	}
	return h;
}

//����ֵΪx�Ľڵ�
DoubleLinkedCircularTable::DLnode* DoubleLinkedCircularTable::Insert(DLnode *h,int x)
{
	DLnode *p=h->next,*s;
	s=(DLnode*)malloc(sizeof(DLnode));
	s->data=x;
	while(p!=h)
	{
		if(p->data<x)
			p=p->next;
		else
			break;
	}
	s->next=p;
	s->prior=p->prior;
	p->prior->next=s;
	p->prior=s;
	return h;
}

//ɾ������ֵΪx�Ľڵ�
DoubleLinkedCircularTable::DLnode * DoubleLinkedCircularTable::Delete(DLnode *h,int x)
{
	DLnode *p=h->next,*q;
	while(p!=h)
	{
		q=p;
		p=p->next;
		if(q->data==x)
		{
			q->prior->next=q->next;
			q->next->prior=q->prior;
			free(q);
		}
	}
	return h;
}

//�Ѹ����ڵ�ŵ������ڵ�ǰ��
DoubleLinkedCircularTable::DLnode * DoubleLinkedCircularTable::Move(DLnode *h)
{
	//��������: -2,3,-4,5,-6,7,-8
	DLnode *p=h->next;
	DLnode *q=h->prior;
	while(p!=q)
	{
		while(p!=q&&p->data<0)
			p=p->next;
		while(q!=p&&q->data>=0)
			q=q->prior;
		if(p!=q)
		{
			int x=p->data;
			p->data=q->data;
			q->data=x;
		}
	}
	return h;
}

DoubleLinkedCircularTable::DLnode * DoubleLinkedCircularTable::Move2(DLnode *h)
{
	//��������: -2,3,-4,5,-6,7,-8
	DLnode *p=h->next;
	DLnode *q=h->prior;
	while(p!=q&&q->next!=p)
	{
		while(p!=q&&q->next!=p&&p->data<0)
			p=p->next;
		while(q!=p&&q->next!=p&&q->data>=0)
			q=q->prior;
		if(p!=q)
		{
			int x=p->data;
			p->data=q->data;
			q->data=x;
			//
			p=p->next;
			q=q->prior;
		}
	}
	return h;
}

void DoubleLinkedCircularTable::PutList(DLnode *h)
{
	DLnode *p;
	p=h->next;
	while(p!=h)
	{
		printf("%d  ",p->data);
		p=p->next;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
FileTest::FileTest()
{
	m_start = m_last = NULL;
}
FileTest::~FileTest()
{
	while(m_start != m_last)
	{
		struct Line* p = m_start;
		m_start = m_start->next;
		free(p);
	}
	if(m_start)
	{
		free(m_start);
	}
	m_start=m_last=NULL;
}

/* ��ʾ�˵������û�ѡ�� */
int FileTest::menu_select()
{
	char s[80];
	int c;
	printf("\t\t1.����\n");
	printf("\t\t2.ɾ��һ��\n");
	printf("\t\t3.��ʾһ��\n");
	printf("\t\t4.�ļ�����\n");
	printf("\t\t5.װ���ļ�\n");
	printf("\t\t6.�˳�\n");
	do{
		printf("\n\n\t\t�밴����ѡ��");
		gets(s);
		c=strToInt(s);
	}while(c<0||c>6);
	return(c);
}

/*���ı�����ָ���ж˲�  */
int FileTest::enter(int linenum)
{
	struct Line *info;
	char t[81];
	for(;;) {
		/*  */
		info=(struct Line *)malloc(sizeof(struct Line));
		if(!info){
			printf("\t! �ڴ治����\n");
			return(NULL);
		}
		printf("%d:",linenum);
		gets(info->text);
		info->num=linenum;
		if(*info->text){
			if(find(linenum)) patchup(linenum,1);
			if(*info->text) m_start=dls_store(info);
		}
		else break;
		linenum++;
	}
	return(linenum);
}

/*���ı����ݲ����ļ��м�ʱ����������ݵ��кű�������1����  */
/*ɾ��ʱ����ɾ�����ı�������кű����1 */
void FileTest::patchup(int n,int incr)
{
	struct Line *i;
	i=find(n);
	while(i){
		i->num=i->num+incr;
		i=i->next;
	}
}
/*���к��������� */
struct FileTest::Line* FileTest::dls_store(struct Line *i)
{
	struct Line *old,*p;
	if(m_last==NULL){
		i->next=NULL;
		i->prior=NULL;
		m_last=i;
		return(i);
	}
	p=m_start;
	old=NULL;
	while(p){
		if(p->num<i->num){
			old=p;
			p=p->next;
		}
		else{
			if(p->prior){
				p->prior->next=i;
				i->next=p;
				p->prior=i;
				return m_start;
			}
			i->next=p;
			i->prior=NULL;
			p->prior=i;
			return(i);
		}
	}
	old->next=i;
	i->next=NULL;
	i->prior=old;
	m_last=i;
	return m_start;
}

/*ɾ��һ�� */
void FileTest::delete_text()
{
	struct Line *info;
	char s[80];
	int linenum;
	printf("\t�к�:");
	gets(s);
	linenum=strToInt(s);
	info=find(linenum);
	if(info){
		if(m_start==info){
			m_start=info->next;
			if(m_start) m_start->prior=NULL;
			else   m_last=NULL;
		}
		else{
			info->prior->next=info->next;
			if(info!=m_last)
				info->next->prior=info->prior;
			else m_last=info->prior;
		}
		free(info);
		patchup(linenum+1,-1);
	}
}

/*����һ���ı�  */
struct FileTest::Line* FileTest::find(int linenum)
{
	struct Line *info;
	info=m_start;
	while(info){
		if(linenum==info->num) return(info);
		info=info->next;
	}
	return(NULL);
}

/*��ʾ�ı�  */
void FileTest::list()
{
	struct Line *info;
	info=m_start;
	while(info){
		printf("%d:%s\n",info->num,info->text);
		info=info->next;
	}
	printf("\n\n");
}

/*���ļ� */
void FileTest::save(char *fname)
{
	struct Line *info;
	char *p;
	FILE *fp;
	if((fp=fopen("text.txt","w"))==NULL){
		printf("\t�ļ��򲻿���\n");
		exit(0);
	}
	printf("\t���ڴ����ļ���\n");
	info=m_start;
	while(info){
		p=info->text;
		while(*p) putc(*p++,fp);
		//putc('\r',fp);
		putc('\n',fp);
		info=info->next;
	}
	fclose(fp);
}

/*װ���ļ� */
void FileTest::load(char *fname)
{
	struct Line *info,*temp;
	char *p;
	FILE *fp;
	int t,size,inct;
	if((fp=fopen("text.txt","r"))==NULL){
		printf("\t�ļ��򲻿���\n");
		exit(0);
	}
	while(m_start){
		temp=m_start;
		m_start=m_start->next;
		free(temp);
	}
	printf("\n\t��װ���ļ� ��\n");
	size=sizeof(struct Line);
	m_start=(struct Line*)malloc(size);
	if(!m_start){
		printf("\n\t�ڴ��Ѿ����꣡");
		return;
	}
	info=m_start;
	p=info->text;
	inct=1;
	while((*p=getc(fp))!=EOF){
		p++;
		while((*p=getc(fp))!='\n') p++;
		//getc(fp);      /*����'\n'  */
		*p='\0';
		info->num=inct++;
		info->next=(struct Line*)malloc(size);
		if(!info->next){
			printf("\n\t�ڴ��Ѿ����꣡");
			return;
		}
		info->prior=temp;
		temp=info;
		info=info->next;
		p=info->text;
	}
	temp->next=NULL;
	m_last=temp;
	free(info);
	m_start->prior=NULL;
	fclose(fp);
}

int FileTest::test()
{
	char s[80],fname[80];
	do{
		int choice=menu_select();
		switch(choice) 
		{
		case 1:
			   {
				   printf("\t�кţ�");
				   gets(s);
					int linenum=strToInt(s);
					enter(linenum);
			   }
			break;
		case 2:
			{
				delete_text();
			}
			break;
		case 3:
			{
				list();
			}
			break;
		case 4:
			{
				printf("\t�ļ�����");
				gets(fname);
				save(fname);
			}
			break;
		case 5:
			{
				printf("\t�ļ�����");
				gets(fname);
				load(fname);
			}
			break;
		case 6:
			{
				exit(0);
			}
			break;
		}
	}while(1);
}
