// for M5Stack Atom MATRIX
// post : http://192.168.4.1/call/ ---> 呼び出し
// post : http://192.168.4.1/ok/   ---> サーバー生死チェック
//        サーバー応答 : [200 CALLING] ---> LED=青（呼び出し中）
//        サーバー応答 : [200 AGREE]   ---> LED=緑（応答）
//        サーバー応答 : [200 OK]      ---> LED=白（待機状態）
//        

#include <M5Atom.h>
#include <WiFi.h>
#include <HTTPClient.h>

HTTPClient http;  // HTTPクライアントを使用するための準備


const char ssid[] = "HAS3-01";              // 接続先アクセスポイントのSSID
const char pass[] = "12348765";             // 接続先アクセスポイントのパスワード
String url = "192.168.4.1";                 // POST先IPアドレス
int cnt = 0;


///////////////////////////////////////////////////////////////////////////////
// POST処理
String httpPost(String address)
{
	String payload;                         // 受信ペイロード
	http.begin("http://" + url + address);  // 送信先アドレス
	int httpCode = http.POST("");           // POST
	if(httpCode == HTTP_CODE_OK){           // コード＝200？
		payload = http.getString();         // ペイロード取得
	}
	else{                                   // 受信失敗
		
	}
	http.end();                             // 通信終了
	
	return payload;
}


///////////////////////////////////////////////////////////////////////////////
// 初期設定 
void setup()
{
	M5.begin(true, false, true);
	
	// LEDマトリクスを消灯
	for(int i=0; i<25; i++){
		M5.dis.drawpix(i, CRGB::Black);   // LED色指定
	}
	
	// Wi-Fi接続
	WiFi.begin(ssid, pass);
	while(WiFi.status() != WL_CONNECTED){ // 接続完了まで待機
		// LEDで「×」赤表示
		M5.dis.drawpix(0, CRGB::Red);     // LED色指定（赤）
		M5.dis.drawpix(4, CRGB::Red);
		M5.dis.drawpix(6, CRGB::Red);
		M5.dis.drawpix(8, CRGB::Red);
		M5.dis.drawpix(12, CRGB::Red);
		M5.dis.drawpix(16, CRGB::Red);
		M5.dis.drawpix(18, CRGB::Red);
		M5.dis.drawpix(20, CRGB::Red);
		M5.dis.drawpix(24, CRGB::Red);
		delay(1000);
	}
}


///////////////////////////////////////////////////////////////////////////////
// メインループ
void loop()
{
	M5.update();                               // ボタン状態初期化
	// ボタン処理
	String payload;
	if(M5.Btn.wasPressed()){                   // ボタンが押されていれば
		payload = httpPost("/call/");          // 呼び出しをPOST
		if(payload == "200 CALLING"){          // 応答が正常
			for(int i=0; i<25; i++){
				M5.dis.drawpix(i, CRGB::Blue); // LED色指定（全青）
			}
		}
		else{                                  // 応答エラー [×]表示
			M5.dis.drawpix(0, CRGB::Magenta);  // LED色指定（紫）
			M5.dis.drawpix(4, CRGB::Magenta);
			M5.dis.drawpix(6, CRGB::Magenta);
			M5.dis.drawpix(8, CRGB::Magenta);
			M5.dis.drawpix(12, CRGB::Magenta);
			M5.dis.drawpix(16, CRGB::Magenta);
			M5.dis.drawpix(18, CRGB::Magenta);
			M5.dis.drawpix(20, CRGB::Magenta);
			M5.dis.drawpix(24, CRGB::Magenta);
		}
		cnt = 0;
	}
	else{
		if(cnt > 30){  // 3sec
			// Serverが生きているかチェック
			payload = httpPost("/ok/");               // 定期通信
			if(payload == "200 CALLING"){             // 応答＝呼び出し中
				for(int i=0; i<25; i++){
					M5.dis.drawpix(i, CRGB::Blue);    // LED色指定（全青）
				}
			}
			else if(payload == "200 AGREE"){          // 応答＝了解済み
				for(int i=0; i<25; i++){
					M5.dis.drawpix(i, CRGB::Green);   // LED色指定（全緑）
				}
			}
			else if(payload == "200 OK"){             // 応答＝定期通信OK
				M5.dis.drawpix(0, CRGB::White);       // LED色指定（左上白）
				for(int i=1; i<25; i++){
					M5.dis.drawpix(i, CRGB::Black);   // LED色指定（黒）
				}
				delay(200);
				M5.dis.drawpix(0, CRGB::Black);       // LED色指定（左上黒）
			}
			else{                                     // 応答エラー [×]表示
				M5.dis.drawpix(0, CRGB::Magenta);     // LED色指定（紫）
				M5.dis.drawpix(4, CRGB::Magenta);
				M5.dis.drawpix(6, CRGB::Magenta);
				M5.dis.drawpix(8, CRGB::Magenta);
				M5.dis.drawpix(12, CRGB::Magenta);
				M5.dis.drawpix(16, CRGB::Magenta);
				M5.dis.drawpix(18, CRGB::Magenta);
				M5.dis.drawpix(20, CRGB::Magenta);
				M5.dis.drawpix(24, CRGB::Magenta);
			}
			cnt = 0;
		}
	}
	cnt += 1;
	delay(100);
}
