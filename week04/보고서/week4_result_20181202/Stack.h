#ifndef __STACK__
#define __STACK__

#include "LinkedList.h"
#include <string>
using namespace std;


//1. LinkedList 클래스를 템플릿 클래스로 확장
//2. Stack형식으로 Delete 함수 재정의
//3. first, current_size는 class의 멤버 변수이기 때문에 this 포인터를 사용하여 가져오기

//LinkedList class를 상속받는 Stack class
template <typename T>
class Stack : public LinkedList<T>{
	public:
		bool Delete(T &element){
			//first가 0이면 false반환
			if (this->first == 0) return false;

			Node<T> *current = this->first;
			this->first = this->first->link;
			
			//삭제하려는 노드의 데이터값을 element에 저장 
			element = current->data;

			//LinkedList와 달리 Stack은 current가 가리키는 곳을 삭제
			delete current;
			this->current_size--;

			return true;

			}
};

#endif
