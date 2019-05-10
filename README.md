# IOT-with-metamodeling-and-raspberry-and-aruino
Develops IOT using a metamodeling

The above program requires two laptops (servers, clients) and Internet modem and a raspberry pi with speaker and aduinos (robot arm, automobile).

install metamodeling program in cliend laptops with extended http protocols.
https://www.adoxx.org/live/download-15


Adoxx Server
1.node js 설치
https://nodejs.org/ko/
2.js파일 실행
cmd	
node hserver.js 실행하기위한 필수 module 설치
Error: Cannot find module 'express'
npm install express
Error: Cannot find module 'serialport'
npm install serialport
Error: Cannot find module 'request'
npm install request
3.hserver.js 코드 수정하기
서버 코드 읽어보며 네부네트워크 설정등 필요!
4.공유기 설정
내부 네트워크
ip 포트포워드 (express listen하기 위해)	
192.168.0.xxx	프로토콜 tcp/udp	port 1xxxx
5.ip static 192.168.0.xxx로 변경
이후 웹에서 192.168.0.xxx:1xxxx/?id=adoxx
idle뜨는지 확인>>뜨면 express listen 성공
6.아두이노 블루투스 HC-06  연결하여 COM번호확인
7.hserver.js COM번호 편집
req.query.id 가 adoxx
8.방화벽 풀기, 1xxxx포트에 대해서 오픈

Adoxx Client
1.Adoxx Development Toolkit 실행	 (Development Library import 후 User 추가하기위해)
아이디 비번 Admin, password or Password//초기 아이디 비번
2.Libraries - Settings - Libraries Management - Management - import - Library import - Browse… - 해당 ABL파일 선택후 import
Library, Dynamic Library, Static Library 이름 변경후 저장
3.User - User list -  Add - User name 등 적고 - User group… - 빨간색금지버튼 클릭해서 초록색으로 바꾸기 - Add
4.Adoxx Modelig Toolkit 실행 (Import Models Using ADL Importl, model 실행 )
5.Import/Export(퍼즐모양) - ADL import (꺽인방향표) - Browse… - ADL파일 선택 -  Model in file:안의 모든 모델 선택 - Tar get Model 안의 model 선택 - import (체크 안되있는거 있으면 체크)
6.Modeling - Extras - Shell window - asc파일 내용을 복붙 - Go!

에러사항
 웹에서 주소에 192.168.0.20:10000/?id=adoxx 쳤을때 idle가 안뜸
공유기 또는 node js 또는 hserver.js코드 문제
client 노트북에서 idle가 안뜸
서버 노트북으로 접속이 안되는 문제
공유기 포트 포워딩 또는 서버 노트북에서 포트 10000에대해서 방화벽 오픈 또는 개인 네트워크 빼고 방화벽 승인

Ai speaker
KT API Link 가입 및 서비스 신청
라즈베리파이 세팅
https://apilink.kt.co.kr/console/gigagenie/console/myservice



