#include <iostream.h>

//ds ���� 4: 20010078 ��빬
//3�� ������ �������̽��� ������ ����, ���� ��ĸ� ��ũ�� ����Ʈ�Դϴ�.

template<typename T>
class CStack {
protected:
	struct NODE {
		T data; NODE *extra;
	};
	NODE *m_pHead; int m_nElem;
public:
	//Construction/destruction
	CStack(): m_pHead(NULL), m_nElem(0) {}
	int GetCount() const { return m_nElem; } //number of elements
	virtual ~CStack();
	virtual void Add(const T& elem);
	virtual bool Delete(T& elem); //returns the pointer to the element being deleted
};

template<typename T>
class CQueue: public CStack<T> {
protected:
	NODE *m_pTail;
public:
	virtual void Add(const T& elem);
};

template<typename T>
void CStack<T>::Add(const T& elem)
{
	if(m_pHead==NULL) { m_pHead=new NODE; m_pHead->extra=NULL; }
	else { NODE *tmp=m_pHead; m_pHead=new NODE; m_pHead->extra=tmp; }
	m_pHead->data=elem;
}

template<typename T>
void CQueue<T>::Add(const T& elem)
{
	if(m_pHead) { m_pTail->extra=new NODE; m_pTail=m_pTail->extra; }
	else { m_pTail=m_pHead=new NODE; }
	m_pTail->extra=NULL; m_pTail->data=elem;
}

template<typename T>
bool CStack<T>::Delete(T& elem)
{
	if(m_pHead) { //If there's anything to draw out
		NODE *tmp=m_pHead; elem=m_pHead->data; m_pHead=m_pHead->extra; delete tmp;
		return true;
	}
	else return false; //nothing
}

template<typename T>
CStack<T>::~CStack()
{
	NODE *tmp, *tmp2; //��带 ��ȸ�ϸ鼭 ���ҵ��� ��� ����
	for(tmp=m_pHead;tmp; tmp=tmp2) { tmp2=tmp->extra; delete tmp; }
}

int main()
{
	CStack<int> ap; CQueue<int> aq; int x;
	cout<<"���ÿ��� 1 �־��� �����, 2 3 4 5�� �־��ٰ� ���ʴ�� ������\n";
	ap.Add(1); ap.Delete(x); ap.Add(2); ap.Add(3); ap.Add(4); ap.Add(5);
	ap.Delete(x); cout<<x<<" "; ap.Delete(x); cout<<x<<" ";
	ap.Delete(x); cout<<x<<" "; ap.Delete(x); cout<<x<<endl<<endl;

	cout<<"ť���� 1 �־��� �����, 2 3 4 5�� �־��ٰ� ���ʴ�� ������\n";
	aq.Add(1); aq.Delete(x); aq.Add(2); aq.Add(3); aq.Add(4); aq.Add(5);
	aq.Delete(x); cout<<x<<" "; aq.Delete(x); cout<<x<<" ";
	aq.Delete(x); cout<<x<<" "; aq.Delete(x); cout<<x<<endl;
	return 0;
}
