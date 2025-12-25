package org.example;

public class WbLib
{
    public void Logo()
    {
        System.out.println("PROGRAM START");
    }

    public void Menu()
    {
        //"\n1. 생성 2. 삭제 3.   선택 4.       입금 5. 출금 6. 이체 7. 출력 0. 종료"
        System.out.println("[1] Create Account");   //계좌 생성
        System.out.println("[2] Delete Account");   //계좌 삭제
        System.out.println("[3] Select Account");   //계좌번호로 검색해서 번호랑 잔액 출력
        System.out.println("-------------------");
        System.out.println("[4] Input Account");    //입금
        System.out.println("[5] Output Account");   //출금
        System.out.println("[6] Send Money");        //이체
        System.out.println("-------------------");
        System.out.println("[7] Print All Accounts");  // 전체 계좌 출력
        System.out.println("-------------------");
        System.out.println("[0] Program End");
    }

    public void Ending()
    {
        System.out.println("PROGRAM END");
    }



}
