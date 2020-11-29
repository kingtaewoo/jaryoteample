//메시지큐에 사용할 자료형 정의
//작업코드를 추후 열거형(enum)으로 작성해 가독성을 높일 예정


#ifndef __MYMSG_H__
#define __MYMSG_H__

#define INPUT_LEN 20

//메시지큐 사용을 위한 매크로
#define MSG_TYPE_CLIENT 1
#define MSG_TYPE_ADMIN 2
#define MSG_SIZE_CLIENT (sizeof(MsgClient) - sizeof(long))
#define MSG_SIZE_ADMIN (sizeof(MsgAdmin) - sizeof(long))

//고객 정보 양식
typedef struct __ClientInfo {	//나중에 STL 사용을 위해 클래스로 변경할 예정
	char clientId[20];				//고객 ID
	char clientPw[20];				//고객 PW
	char clientName[20];			//고객 이름
	char clientResRegNum[20];		//고객 주민번호
	char clientAccountNum[20];		//고객 계좌번호
	double clientBalance;			//고객 계좌잔액
} ClientInfo;

//고객과 서버가 통신할 메시지 형태
typedef struct __MsgClient {
	long mtype;						//= MSG_TYPE_CLIENT
	int cmd;						//작업 코드(메시지의 형태는 건들지 않고 이 작업코드로 다른 동작을 실행하도록 구성)
	struct __ClientInfo data;		//고객 정보
	bool is_error;					//에러 토큰(정보 제공 거부나 각종 에러 상황시에 쓰임)
} MsgClient;

//관리자와 서버가 통신할 메시지 형태
//1.관리자 회원가입
//2.관리자 로그인
//3.모든 고객 정보 출력
//4.특정 고객 정보 수정
//5.관리자 로그아웃

//관리자와 서버가 통신할 메시지 형태
typedef struct __MsgAdmin {
	long mtype;				//= MSG_TYPE_ADMIN
	int cmd;				//작업 코드
	char adminId[20];		 	//관리자 ID(1,2,3,4에 필요)
	char adminPw[20];			//관리자 PW(1,2,3,4에 필요)
	bool admin_login;                       //관리자 권한(2,3,4,5에 필요)
	bool admin_Done;                        //요청사항 완수 여부 (4에 필요)
	char ClientInfo[256];		        //고객 정보(추후 변경 가능) (3 ,4에 필요)
	int clientCnt;                          //총 고객의 수 (3에 필요)
	struct __ClientInfo data;               //고객 정보;
} MsgAdmin;
#endif
