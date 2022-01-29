#include <stdio.h>
#include <string>
#include "Stack.h"

void prnMenu(){
	cout<<"*******************************************"<<endl;
	cout<<"* 1. 삽입    2. 삭제    3. 출력   4. 종료 *"<<endl;
	cout<<"*******************************************"<<endl;
	cout<<endl;
	cout<<"원하시는 메뉴를 골라주세요: ";
}

int main(){
	    int mode, selectNumber, tmpItem;
		LinkedList<int> *p;
		
		//프로그램을 종료하는 경우 flag = true
		bool flag = false;
		
		//사용자는 Stack과 Linked List중에 선택가능
		cout<<"자료구조 선택(1: Stack, Other: Linked List): ";
		cin>>mode;
		
		//Stack 자료구조를 선택한 경우
		if(mode == 1)
			p = new Stack<int>();// 정수를 저장하는 스택
			
		else //Linked List 자료구조를 선택한 경우
			p = new LinkedList<int>();

		do{
			prnMenu();
			cin>>selectNumber;
			
			switch(selectNumber){
			    	//새로운 값을 스택 또는 연결리스트에 삽입
				case 1:
					cout<<"원하시는 값을 입력해주세요: ";
					cin>>tmpItem;    p->Insert(tmpItem);
					cout<<tmpItem<<"가 삽입되었습니다."<<endl;
					break;
				//현재 가리키는 값을 삭제
				//스택 : 가장 나중에 삽입된 것, 연결리스트 : 가장 먼저 삽입된 것을 삭제
				case 2:
					if(p->Delete(tmpItem)==true)
						cout<<tmpItem<<"가 삭제되었습니다."<<endl;
						
					else cout<<"비어있습니다. 삭제 실패"<<endl;
					break;
				//현재 스택/리스트의 크기 출력(저장된 데이터의 개수)
				case 3:
					cout<<"크기: "<<p->GetSize()<<endl;
					p->Print();
					break;
				//프로그램 종료
				case 4:
					flag = true;     break;
					
				default:
					cout<<"잘못 입력하셨습니다."<<endl;
					break;
			
			}

			//flag가 true면 break		
			if(flag) break;
			
		} while(1);
		
		return 0;
}
