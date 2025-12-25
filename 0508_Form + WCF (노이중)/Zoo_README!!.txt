이 순서로 만들면 편하더라

1. 멤버필드 - zoo

2. 인터페이스 - IZoo

3. MyDB

4. Service - ZooService
-----------------------------------------------

DB날리기

ALTER TABLE picture NOCHECK CONSTRAINT ALL;
DELETE FROM picture;
DBCC CHECKIDENT ('picture', RESEED, 0);
ALTER TABLE picture CHECK CONSTRAINT ALL;

-------------------------------
서버 : 참조추가 
ㄴ configration
ㄴ serialization
ㄴ service model

클라에선
서버에 App.config에 있는 주소 복사해서 "서비스 참조 추가"
단 서버가 켜져있을때 해야함