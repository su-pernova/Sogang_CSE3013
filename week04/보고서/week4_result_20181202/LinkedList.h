#ifndef __LINKEDLIST__
#define __LINKEDLIST__

#include <iostream>
#include <string>
using namespace std;


//LinkedList Node
//파라미터적 다형성이 지원되는 템플릿 기반의 클래스로 변환
template <typename T>
class Node{
	public:
		//데이터를 저장할 변수
		T data;
		
		//노드구조체 이용; 다음노드의 주소를 저장할 포인터
		Node<T> *link;
		
		//element값을 받아 data 필드에 저장한다
		Node(T element){
		  data = element;
		  link = 0;
		}
};

//LinkedList Class
template <typename T>
class LinkedList{
	protected:
		//첫번째 노드의 주소를 저장할 포인터
		Node<T> *first;
		int current_size;
	public:
		//생성자, 초기화 
		LinkedList(){
			first = 0;
			current_size = 0;
		};

		//노드 개수를 리턴
		int GetSize(){
			return current_size;
		};

		//맨 앞에 원소를 삽입, LinkedList와 Stack 둘다 같다
		void Insert(T element);
		
		//맨 뒤의 원소를 삭제, 제일 나중에 들어온 원소 삭제 (LinkedList/Queue의 형태)
		virtual bool Delete(T &element);

		//리스트 출력
		void Print();
};


//새 노드를 맨 앞에 붙이고 값을 넣음
template <typename T>
void LinkedList<T>::Insert(T element){
	
    	//노드 생성
	Node<T> *newnode = new Node<T>(element);

	//새 노드가 첫번째 노드를 가리킴
	//newnode는 포인터이기 때문에 -> 를 사용해 함수, 변수를 불러옴 newnode.link와 뜻은 같다
	newnode -> link = first;
	first = newnode;
	current_size++;
}

//마지막 노드의 값을 리턴하면서 메모리에서 할당 해제
template <typename T>
bool LinkedList<T>::Delete(T &element){

	if (first == 0)
		return false;
	
	Node<T> *current = first;
	Node<T> *previous = 0;
	
	//마지막 노드까지 찾아가는 반복문
	while(1) {
		if (current->link == 0){  //마지막 노드를 찾았을 때
			if (previous)
				previous -> link = current -> link;
			else
				first = first -> link;
			break;
		}
		//아직 마지막 노드를 찾지 못했을 때. 다음 노드로 이동
		previous = current;
		current = current -> link;
	}
	//삭제하는 노드의 data값을 element에 저장
	element = current -> data;
	//마지막 노드 삭제
	delete current;
	//전체 리스트 사이즈 감소
	current_size--;

	return true;
}

//리스트를 출력하는 Print 함수
template <typename T>
void LinkedList<T>::Print(){
	Node<T> *i;
	int index = 1;
	//인덱스 번호와 데이터값을 함께 출력해준다
	if (current_size != 0){
		for( i = first; i != NULL; i=i->link){
		    	//마지막 노드인 경우(-> 미포함)
			if (index == current_size){
				cout << "[" << index << "|";
				cout << i -> data <<"]";
			}
			// 마지막 노드가 아닌 경우(-> 포함)
			else {
				cout << "[" << index << "|";
				cout << i -> data << "]->";
				index++;
			}
		}
		cout << endl;
	}
}

#endif
