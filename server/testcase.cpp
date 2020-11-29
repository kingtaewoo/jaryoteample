//서버 프로그램 구동을 위한 테스트케이스

#include "msq.h"
#include <signal.h>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

using namespace std;

int main(int argc, char const *argv[]) {

	cout << "___________________테스트케이스____________________" << endl;

	MsgClient client;
	MsgAdmin admin;

	//메시지큐 생성을 위한 초기화
	key_t msq_key = ftok("key", 35);
	int msq_id = msgget(msq_key, IPC_CREAT);

	//메시지통신 구조체 초기화
	memset(&client, 0x00, sizeof(MsgClient));
	memset(&admin, 0x00, sizeof(MsgAdmin));
	
	while(1) {
		//여기서는 한 프로그램에서 관리자와 클라이언트를 고를 수 있도록 했으나, 나중에는 관리자와 클라이언트가 다른 프로그램에서 실행될 예정임
		cout<< "관리자 : 0 / 클라이언트 : 1 / 종료 : 9" << endl << ">> ";

		int mode;
		cin >> mode;	

		switch(mode) {
		//관리자로 테스트
			case 0: {
					cout<< "----------작업 선택----------\n 1.관리자 등록\n 2.관리자 로그인\n 3.고객정보 출력\n 4.고객정보 수정\n 5.관리자 로그아웃\n>>> ";

					char admin_id[20], admin_pw[20];
					int todo; 
					cin  >> todo;
					admin.cmd = todo;
					cout.clear(); cin.clear();
					//1. 관리자 등록
					//2. 관리자 로그인
					//3. 관리자 고객정보 조회
					//4. 관리자 고객정보 수정
					//5. 관리자 로그아웃
					switch(admin.cmd){
						case 1: {
								cout << "관리자 등록\n 등록할 관리자 ID : " ;
								cin >> admin_id;
								cout << "등록할 관리자 PassWord : ";
								cin >> admin_pw;

								//client에 ID와 PW정보를 담아 메시지 송신
								admin.mtype = MSG_TYPE_ADMIN;
								strcpy(admin.adminId, admin_id);
								strcpy(admin.adminPw, admin_pw);
								admin.admin_login = false;
								int sndSize = msgsnd(msq_id, &admin, MSG_SIZE_ADMIN, 0);
								//msgsnd() 예외처리
								if(sndSize != 0){
									perror("msgsnd() error!(cmd=1)");
									exit(0);
								}

								//메시지 수신(해당 회원의 정보가 구조체에 담긴 메시지를 받음)
								int rcvSize = msgrcv(msq_id, &admin, MSG_SIZE_ADMIN, 0, 0);
								//msgrcv() 에외처리
								if(rcvSize == -1) {
									perror("msgrcv() error!(cmd=1) ");
									exit(-1);
								}
								//메시지 수신 성공 시 서버에서 받은 데이터 출력(임시)
								else if(rcvSize != -1) {
									if(errno != ENOMSG) {
										cout << "-- 관리자 등록 완료 --"; 
										puts(admin.adminId);
										puts(admin.adminPw);

									}
								}
								break;

							}
						case 2: {

								cout << "관리자 로그인\n 관리자 ID : " ;
								cin >> admin_id;
								cout << "관리자 PassWord : ";
								cin >> admin_pw;

								//client에 ID와 PW정보를 담아 메시지 송신
								admin.mtype = MSG_TYPE_ADMIN;
								strcpy(admin.adminId, admin_id);
								strcpy(admin.adminPw, admin_pw);
								admin.admin_login = false;
								int sndSize = msgsnd(msq_id, &admin, MSG_SIZE_ADMIN, 0);
								//msgsnd() 예외처리
								if(sndSize != 0){
									perror("msgsnd() error!(cmd=1)");
									exit(0);
								}

								//메시지 수신(해당 회원의 정보가 구조체에 담긴 메시지를 받음)
								int rcvSize = msgrcv(msq_id, &admin, MSG_SIZE_ADMIN, 0, 0);
								//msgrcv() 에외처리
								if(rcvSize == -1) {
									perror("msgrcv() error!(cmd=1) ");
									exit(-1);
								}
								//메시지 수신 성공 시 서버에서 받은 데이터 출력(임시)
								else if(rcvSize != -1) {
									if(errno != ENOMSG) {
										cout << "-- 관리자 로그인  완료 --";                                                       						            }
								}
								break;

							}
						case 3: {
								puts("모든 고객 정보 출력");
								//모든 고객 정보 출력 
								if(admin.admin_login == true) //권한이 있을 경우
								{
									admin.mtype = MSG_TYPE_ADMIN; 
									int sndSize = msgsnd(msq_id, &admin, MSG_SIZE_ADMIN, 0);
									//msgsnd() 예외처리
									if(sndSize != 0){
										perror("msgsnd() error!(cmd=1)");
										exit(0);
									}
									//고객명수 파악하는 메세지 받기 
									int rcvSize = msgrcv(msq_id, &admin, MSG_SIZE_ADMIN, 0, 0);
									int size = admin.clientCnt;
									int cnt ;
									for (cnt = 0 ; cnt < size ; cnt++)
									{
										int rcvSize = msgrcv(msq_id , &admin , MSG_SIZE_ADMIN , 0 , 0);
										//msgrcv() 에외처리
										if(rcvSize == -1) {
											perror("msgrcv() error!(cmd=1) ");
											exit(-1);
										}
										//메시지 수신 성공 시 서버에서 받은 데이터 출력(임시)
										else if(rcvSize != -1) {
											if(errno != ENOMSG && admin.admin_login == true) {

												cout << admin.ClientInfo << endl;
											} 
											else if(admin.admin_login == false)
											{
												cout << "관리자 권한 없읍!" << endl;
											}
										}
									}
								}
								else {
								 cout << "관리자 권한 없음!" << endl; 
								}
								break;
							}
						case 4: {
								//admin이 권한이 있을 경우		 
								if(admin.admin_login == true)
								{
									cout << "고객 정보 수정" << endl;
									char before[20];
									char after[20];
									cout << "수정할 사람의 ID" << endl;
									cin >> admin_id;
									cout << "수정할 내용 입력" << endl;
									cin >> before;
									cout << "수정 내용 입력" << endl;
									cin >> after;
									//admin ID에 수정할 고객 이름 입력
									admin.mtype = MSG_TYPE_ADMIN;
									strcpy(admin.data.clientId, admin_id);
								        strcpy(admin.data.clientName , before);	//변수 사용을 줄이기위해 임시 사용
									strcpy(admin.data.clientPw , after);
									int sndSize = msgsnd(msq_id, &admin, MSG_SIZE_ADMIN, 0);
									
								}
								else 
								{
									cout << "권한 없음!" << endl;
								}

								break;
							}

						case 5: {
								puts("관리자 로그아웃\n");
								admin.admin_login == false;
								break;
							}

					}
					cout.clear();
					break;
				}
		//클라이언트로 테스트
		case 1: {		
				//사용자 입력 데이터
				char cl_id[20],				//ID
					cl_pw[20],				//PW
					cl_name[20],			//이름
					cl_resreg[20];			//주민번호
					//cl_accountnum[20];
				double cl_balance;			//잔액

				cout<< "----------작업 선택----------\n 1.로그인\n 2.회원가입\n 3. 입금\n 4. 출금\n";

				//작업코드 입력
				int todo;
				cin >> todo;
				client.cmd = todo;
				cout.clear();cin.clear();

				//작업코드에 따라 구조체에 담을 정보의 조합이 달라짐
				//1 : 로그인
				//2 : 회원가입
				//3 : 입금
				//4 : 출금
				switch(client.cmd){
					case 1: {	//로그인
						//ID/PW 입력
						cout<<"ID : "; cin >> cl_id;
						cout<<"PW : "; cin >> cl_pw;
						cout.clear();cin.clear();

						//client에 ID와 PW정보를 담아 메시지 송신
						client.mtype = MSG_TYPE_CLIENT;
						strcpy(client.data.clientId, cl_id);
						strcpy(client.data.clientPw, cl_pw);
						int sndSize = msgsnd(msq_id, &client, MSG_SIZE_CLIENT, 0);
						//msgsnd() 예외처리
						if(sndSize != 0){
							perror("msgsnd() error!(cmd=1)");
							exit(0);
						}
						
						//메시지 수신(해당 회원의 정보가 구조체에 담긴 메시지를 받음)
						int rcvSize = msgrcv(msq_id, &client, MSG_SIZE_CLIENT, 0, 0);
						//msgrcv() 에외처리
						if(rcvSize == -1) {
							perror("msgrcv() error!(cmd=1) ");
							exit(-1);
						}
						//메시지 수신 성공 시 서버에서 받은 데이터 출력(임시)
						else if(rcvSize != -1) {
							if(errno != ENOMSG) {
								puts(client.data.clientId);
								puts(client.data.clientPw);
								puts(client.data.clientName);
								puts(client.data.clientResRegNum);
								puts(client.data.clientAccountNum);
								cout << client.data.clientBalance << endl;
							}
						}
						break;
					}
					case 2: {//회원가입

						//회원가입정보 입력	
						cout<<"ID : "; cin >> cl_id;		//ID는 중복 금지(구현 필요)
						cout<<"PW : "; cin >> cl_pw;		//PW는 중복 가능, 패스워드 확인 절차 추가 필요
						cout<<"NAME : "; cin >> cl_name;	
						cout<<"RESREGNUM : "; cin >> cl_resreg;

						cout.clear();cin.clear();
					
						//메시지 송신
						client.mtype = MSG_TYPE_CLIENT;			//메시지 타입 지정
						strcpy(client.data.clientId, cl_id);	//저장
						strcpy(client.data.clientPw, cl_pw);
						strcpy(client.data.clientName, cl_name);
						strcpy(client.data.clientResRegNum, cl_resreg);
						int sndSize = msgsnd(msq_id, &client, MSG_SIZE_CLIENT, 0);	//전송
						//msgsnd() 예외처리
						if(sndSize != 0){
							perror("msgsnd() error!(cmd=2)");
							exit(0);
						}

						//메시지 수신
						int rcvSize = msgrcv(msq_id, &client, MSG_SIZE_CLIENT, 0, 0);
						//메시지 수신 성공 시
						if(rcvSize != -1) {
							cout << "-----회원가입 성공!-----" << endl;
							cout << " - 생성된 계좌번호 : ";
							//서버에서 생성되는 계좌번호는 중복 금지(추후 이체기능을 위해)
							puts(client.data.clientAccountNum);
							cout<<" - 잔액 : " << client.data.clientBalance << "원" << endl;
						}
						//수신 실패 시
						else {
							cout << "-----회원가입 실패-----" << endl;
						}
						//초기화는 프로그램의 처음과 여기만 진행!(로그인 후에는 client에 서버에서 날아온 정보가 저장된 채로 작업을 진행하기 때문에)
						memset(&client, 0x00, sizeof(MsgClient));
						break;
					}
					case 3: {//입금
						//사용자 입력
						cout << "계좌에 남아있는 금액 : " << (int)client.data.clientBalance << "원" << endl;
						cout<<"입금하실 금액 : "; cin >>cl_balance;
						
						cout.clear();cin.clear();
						//입금할 액수를 client.data.clientBalance에 저장해 메시지 전송 
						client.mtype = MSG_TYPE_CLIENT;
						client.data.clientBalance = cl_balance;
						int sndSize = msgsnd(msq_id, &client, MSG_SIZE_CLIENT, 0);
						//msgsnd() 예외처리
						if(sndSize != 0){
							perror("msgsnd() error!(cmd=3)");
							exit(0);
						}
						//서버에서 입금 처리 후 보내주는 정보 갱신메시지를 수신
						int rcvSize = msgrcv(msq_id, &client, MSG_SIZE_CLIENT, 0, 0);
						//msgrcv() 에외처리
						if(rcvSize == -1) {
							perror("msgrcv() error!(cmd=3)");
							exit(-1);
						}
						cout << "입금 후 잔액 : " << (int)client.data.clientBalance << "원" << endl;
						break;
					}
					case 4: {//출금				
						cout << "계좌에 남아있는 금액 : " << (int)client.data.clientBalance << "원" << endl;
						do {
							//사용자 입력
							cout<<"출금하실 금액 : ";cin >> cl_balance;
							//잔액 부족 시
							if(cl_balance > client.data.clientBalance) 
								cout << "잔액 부족." << endl;
						} while(cl_balance > client.data.clientBalance);
						
						//출금할 금액을 client.data.clientBalance에 담아 구조체 메시지 송신
						client.mtype = MSG_TYPE_CLIENT;
						client.data.clientBalance = cl_balance;
						int sndSize = msgsnd(msq_id, &client, MSG_SIZE_CLIENT, 0);
						//msgsnd() 예외처리
						if(sndSize != 0){
							perror("msgsnd() error!(cmd=4)");
							exit(0);
						}
						//서버에서 출금 처리 후 보내주는 정보 갱신메시지를 수신
						int rcvSize = msgrcv(msq_id, &client, MSG_SIZE_CLIENT, 0, 0);
						//msgrcv() 에외처리
						if(rcvSize == -1) {
							perror("msgrcv() error!(cmd=4)");
							exit(-1);
						}
						cout << "출금 후 잔액 : " << (int)client.data.clientBalance << "원" << endl;
						break;
					}
					default:{	//예상치 못한 오류
						cout << "잘못된 입력. 다시 입력하세요." << endl;
						break;
					}	
				}
				cout.clear();
				break;
			}
			case 9: {//종료
				cout << "테스트케이스 종료." << endl;
				return 0;
			}
			default: {	//예상치 못한 오류
				cout << "잘못된 입력. 다시 입력하십시오." << endl;
				break;
			}
		}
	}



	return 0;
}
