#ifndef __STR__
#define __STR__

class Str {
	private:
		const char *str; //string 내용
		int len; //string 길이

	public:
		Str(int leng); //leng = string 길이
		Str(const char *neyong); //neyong = 초기화 할 내용
		~Str(); //소멸자

		int length(); //string 길이 return
		const char *contents(); //string 내용 return

		int compare(class Str& a); //a의 내용과 strcmp
		int compare(const char *a); //a의 내용과 strcmp

		void operator=(const char *a); //string의 값을 대입
		void operator=(class Str& a); //Str의 내용을 대입
};
#endif
