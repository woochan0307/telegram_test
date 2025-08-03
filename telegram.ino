#include <ESP8266WiFi.h>          // WiFi 연결용
#include <WiFiClientSecure.h>     // HTTPS 통신용
#include <UniversalTelegramBot.h> // 텔레그램 봇 라이브러리
#include <ArduinoJson.h>          // JSON 파싱용

// 와이파이 설정
const char* ssid = "와이파이_이름";
const char* password = "와이파이_비밀번호";

// 텔레그램 봇 토큰 (BotFather로 받은 토큰을 넣으세요)
const char* botToken = "123456789:ABCdefGHIjkLMNOPqrSTUvwxYZ";

// WiFiClientSecure 객체 생성
WiFiClientSecure client;
UniversalTelegramBot bot(botToken, client);  // 봇 객체

// 마지막으로 확인한 메시지 ID 저장용
int lastMessageId = 0;

void setup() {
  Serial.begin(115200);

  // 와이파이 연결
  WiFi.begin(ssid, password);
  Serial.print("Wi-Fi 연결 중...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ Wi-Fi 연결 완료!");
  Serial.print("IP 주소: ");
  Serial.println(WiFi.localIP());

  // 텔레그램 API SSL 인증서 검증 생략 (더 안정적으로 연결됨)
  client.setInsecure();
}

void loop() {
  // 새 메시지가 있는지 확인 (5초 간격으로 확인)
  int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

  while (numNewMessages) {
    Serial.println("📨 새 메시지를 확인 중...");

    for (int i = 0; i < numNewMessages;  i++ ) {
      String chat_id = String(bot.messages[i].chat_id);  // 보낸 사람 ID
      String text = bot.messages[i].text;                // 받은 메시지 내용
      Serial.print("받은 메시지: ");
      Serial.println(text);

      // 메시지에 따라 답변 (지금은 무조건 hello로 답변)
      bot.sendMessage(chat_id, "hello", "Markdown");
    }

    // 다음 메시지를 위해 업데이트
    numNewMessages = bot.getUpdates(bot.last_message_received + 1);
  }

  delay(5000); // 5초마다 확인
}
