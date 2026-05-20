// OLED上部SW
#define F_SW4 23    // 右端
#define F_SW3 19    // 右から２番目 
#define F_SW2 18    // 左から２番目
#define F_SW1 13    // 左端



// ***************  キースキャンタスク  *****************

int key_num = 16;   // キーの数 -2は NUM と 電卓 のキャラクターコードに変換しないキー
int num_on = 0;         // numロック


// キーコードの変換 **********************

char key_cov( int key ){

                          //    整数　Num  実数  Num
                          //          on        on                                                       Key code
  char key_ch[key_num][4] = { { '1', 'H', '1', 's' },   //                                                  1
                              { '2', 'O', '2', 'c' },   //                                                  2
                              { '3', 'B', '3', 't' },   //                                                  3
                              { '4', ' ', '4', 'S' },   //                                                  4
                              { '5', ' ', '5', 'A' },   //                                                  5
                              { '6', ' ', '6', 'T' },   //                                                  6
                              { '7', ' ', '7', ' ' },   //                                                  7
                              { '8', ' ', '8', ' ' },   //                                                  8
                              { '9', ' ', '9', ' ' },   //                                                  9
                              { '0', ' ', '0', ' ' },   //                                                 10
                              { '.', 'R', '.', 'R' },   // *                                               11
                              { 'E', 'E', 'E', 'E' },   // #                                               12
                              { '+', 'm', '+', 'm' },   // A                                               13
                              { '-', 'i', '-', 'i' },   // B                                               14
                              { '*', 'C', '*', 'C' },   // C                                               15
                              { '/', 'r', '/', 'r' }};  // D                                               16
                              // m i

  //int  cal_mode  = 0;   // 0:整数計算モード  1:実数計算モード
  // 整数 num off ->  0 0 = 0
  // 整数 num on  ->  0 1 = 1
  // 実数 num off ->  1 0 = 2
  // 実数 num on  ->  1 1 = 3
  
  char ret_char = key_ch[ key - 1 ][ (cal_mode << 1) | num_on ];
  if ( ret_char == '.' && cal_mode == 0 ) cal_mode = 1;    // 実数計算モードに変更

  return( ret_char  );
}




/// キーボード管理タスク
void Task_kbCtr(void *pvParameters)
{
  (void) pvParameters;

  int kb_y[] = { 12, 14, 27, 26 };
  int kb_x[] = { 25, 33, 32, 35 };

  for ( int i = 0 ; i < 4 ; i++ )
    pinMode( kb_y[i], OUTPUT);

// OLED上部SW
  pinMode( F_SW4 , INPUT_PULLUP);    // 右端
  pinMode( F_SW3 , INPUT_PULLUP);    // 右から２番目 
  pinMode( F_SW2 , INPUT_PULLUP);    // 左から２番目
  pinMode( F_SW1 , INPUT_PULLUP);    // 左端

  int key_code[4][4] = {{  1,  4,  7, 11 }, 
                        {  2,  5,  8, 10 },
                        {  3,  6,  9, 12 }, 
                        { 13, 14, 15, 16 }}; 

  delay(100);

  for (;;) // A Task shall never return or exit.
  {

    for ( int j = 0 ; j < 4 ; j++ ){      // Y軸スキャン
      digitalWrite( kb_y[j], HIGH);
      delay(1);

      for ( int i = 0 ; i < 4 ; i++ ){    // X軸スキャン

        // キー押下確認
//        if ( key_down_chk( kb_x[i] ) ){   

        int shikiichi = 2500; // キー押下判定のAD値の閾値
  
        if ( analogRead( kb_x[i] ) > shikiichi ){    // キー押下？

          int chata = 2;    // チャタ対策のキーチェック回数
          int key_down_cnt = 0;

          do{     // チャタ対策
            delay(5);   // 入力チェック間隔
            if ( analogRead( kb_x[i] ) > shikiichi )
              key_down_cnt++;
            else
              break;    // キー入力無し
          }while( key_down_cnt < chata );
            // キー入力有り
            int key = key_code[j][i];

            in_key = key_cov( key );
            Serial.print(" key_code = ");
            Serial.print(key);
            Serial.print(" ");
            Serial.println(in_key);

          while( analogRead( kb_x[i] ) > 500 ){   // キー放した？
            delay(10);            // キー放した確認間隔
          }
          
        }
        // キー入力無し
      }

        digitalWrite( kb_y[j], LOW);
    }

    if ( digitalRead( F_SW4 ) == 0 ){       // num キー  OLED上部キー右端
      do{
        delay( 10 );
      }while( !digitalRead( F_SW4 ) );

      num_on ^= 1;
      shift = num_on;
      dsp_start = 1;    // num 表示

    }

    // キー表示
    if (digitalRead( F_SW1 ) == 0 ){
      dsp_start = 2;
      do{                 // キー解放待ち
        delay( 10 );
      }while( !digitalRead( F_SW1 ) );
    }


  }


  
}


// キー押下確認
/*
int key_down_chk( int kb_x ){

  int chata = 2;    // チャタ対策のキーチェック回数
  int shikiichi = 2500; // キー押下判定のAD値の閾値
  
  if ( analogRead( kb_x ) > shikiichi ){    // キー押下？

    int key_down_cnt = 0;

    do{     // チャタ対策
      delay(2);   // 入力チェック間隔
      if ( analogRead( kb_x ) > shikiichi )
        key_down_cnt++;
      else
        break;    // キー入力無し
    }while( key_down_cnt < chata );

    while( analogRead( kb_x ) > 200 ){   // キー放した？
    //Serial.println(analogRead( kb_x ));
      delay(10);      // キー放した確認間隔
    }
    return(1);        // キー入力有り
  }
  return(0);          // キー入力無し
}
*/