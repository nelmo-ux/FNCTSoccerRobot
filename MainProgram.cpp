#include <Wire.h>
#include<LiquidCrystal.h>
#define PKT 5000
LiquidCrystal lcd(27, 28, 29, 34, 35, 36, 37);
bool prim = true, i = true;
int ii = 0, bf_t = 0 ;
bool pf = true, fst_prim;
int compassAddress = 0x42 >> 1;
int p_dig, g_heading;
bool timer_flag, flag;
bool digf, dir_j;

void setup() {
  lcd.begin(16, 2);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(40, INPUT);
  pinMode(41, INPUT);
  pinMode(42, INPUT);
  pinMode(43, INPUT);
  m_spd(0, 0, false);
  Serial.begin(9800);
  prim = true;
  Serial.println("Setup was sucsessful done");
  Wire.begin(); // I2C 通信開始
  Wire.beginTransmission(compassAddress);
  Wire.write('G'); // RAM 書き込み
  Wire.write(0x74); // 書き込み先指定
  Wire.write(0x72); // Continuous Mode に設定
  Wire.endTransmission(); // 通信終了
  delayMicroseconds(70); // 処理時間
  lcd.clear();
  p_dig = 0;
  if (digitalRead(40) == LOW ) err();
}

void err(void) {
  lcd.setCursor(0, 0);
  lcd.print("Error SW1");
  lcd.setCursor(0, 1);
  lcd.print("Position");

  exit(1);
}


void m_mod(int x) {
  int M1_FR  = 4;
  int M2_FR  = 5;
  int M3_FR  = 6;
  int M4_FR  = 7;
  switch (x) {
    case 0: // This case will stop the machine
      lcd.setCursor(6, 0);
      lcd.print("MT: N");
      digitalWrite(M1_FR, HIGH);
      digitalWrite(M2_FR, HIGH);
      digitalWrite(M3_FR, LOW);
      digitalWrite(M4_FR, LOW);
      break;


    case 1: //This case will go FWD the machine
      lcd.setCursor(6, 0);
      lcd.print("MT:FW");
      digitalWrite(M1_FR, LOW);
      digitalWrite(M2_FR, LOW);
      digitalWrite(M3_FR, HIGH);
      digitalWrite(M4_FR, HIGH);

      break;



    case 2: // This case will go Right the machine
      lcd.setCursor(6, 0);
      lcd.print("MT: R");
      digitalWrite(M1_FR, LOW);
      digitalWrite(M2_FR, HIGH);
      digitalWrite(M3_FR, HIGH);
      digitalWrite(M4_FR, LOW);
      break;

    case 3: // This case will go Left the machine
      lcd.setCursor(6, 0);
      lcd.print("MT: L");
      digitalWrite(M1_FR, HIGH);
      digitalWrite(M2_FR, LOW);
      digitalWrite(M3_FR, LOW);
      digitalWrite(M4_FR, HIGH);
      break;

    case 4: // This case will back the machine
      lcd.setCursor(6, 0);
      lcd.print("MT:BK");
      digitalWrite(M1_FR, HIGH);
      digitalWrite(M2_FR, HIGH);
      digitalWrite(M3_FR, LOW);
      digitalWrite(M4_FR, LOW);
      break;

    case 5: //this case will lotate(Right) the machine
      lcd.setCursor(6, 0);
      lcd.print("MT:LR");
      digitalWrite(M1_FR, HIGH);
      digitalWrite(M2_FR, HIGH);
      digitalWrite(M3_FR, HIGH);
      digitalWrite(M4_FR, HIGH);
      break;

    case 6:
      lcd.setCursor(6, 0);
      lcd.print("MT:LL");
      digitalWrite(M1_FR, LOW);
      digitalWrite(M2_FR, LOW);
      digitalWrite(M3_FR, LOW);
      digitalWrite(M4_FR, LOW);
      break;

    default:
      lcd.setCursor(6, 0);
      lcd.print("MT:ERR");
      Serial.println("You sent is not motor code");
      return NULL;
      break;
  }
  Serial.println("Motor setup was complated with CODE");
  return NULL;
}

void primrun(void) {
  lcd.setCursor(0, 0);
  lcd.print("WIP  ");
  if (digitalRead(41) == HIGH) {
    if (digitalRead(43) == HIGH) {
      m_mod(6);
      m_spd(150, 900, true);
      if (digitalRead(42) == HIGH) {
        m_mod(2);
      }
      if (digitalRead(42) == LOW) {
        m_mod(3);
      }
      m_spd(180, 1500, true);
    } else {

      unsigned long a;
      a = millis() + PKT;
      m_mod(1);
      m_spd(255, 500, true);
f_pk:
      if (flag == true) {
        while (millis() > a) {
          m_spd(255, 0, false);
        }
      } else {
        m_mod(rand() % 7);
        m_spd(150, 500, true);
        goto f_pk;
      }
    }
  } else {
    m_mod(1);
    m_spd(255, 1500, true);
  }
  prim = false;
  return NULL;
}

void atk(void) {
  switch (dig()) {
    case 0:
      m_mod(1);
      m_spd(255, 800, false);
      break;

    case 1:
      m_mod(2);
      m_spd(255, 800, false);
      m_mod(6);
      m_spd(150, 200, false);
      m_mod(2);
      m_spd(255, 200, false);
      break;

    case 2:
      m_mod(2);
      m_spd(255, 200, false);
      m_mod(1);
      m_spd(150, 1000, false);
      m_mod(6);
      m_spd(255, 200, false);
      break;

    case 3:
      m_mod(3);
      m_spd(255, 800, false);
      m_mod(5);
      m_spd(150, 200, false);
      m_mod(3);
      m_spd(255, 200, false);
      break;
  }
  return NULL;
}

int dig() {
  int a = 0;
  Wire.requestFrom(compassAddress, 2);
f2:
  if (Wire.available() > 1) {
    a = Wire.read();
    a = a << 8;
    a += Wire.read();
    a /= 10;
  }
  if (a == 0)goto f2;
  int f = a;
  int ff = p_dig - a;
  if ( ff > 180  ) ff = ff - 360;
  if ( ff < -180 ) ff = ff + 360;
  bf_t = millis();
  if ( -60 <= ff && ff <= 60) {
    lcd.setCursor(7, 1);
    lcd.print("Dir_M:FW");
    return 0;
  } else if (-120 <= ff &&  ff <= -60) {
    lcd.setCursor(7, 1);
    lcd.print("Dir_M: R");
    return 1;
  } else if (120 <= abs(ff) && abs(ff) < 180) {
    lcd.setCursor(7, 1);
    lcd.print("Dir_M:RV");
    return 2;
  } else if (60 < ff && ff < 120) {
    lcd.setCursor(7, 1);
    lcd.print("Dir_M: L");
    return 3;
  }

  return 10;

}

void m_spd(int a, int b, bool c) {
  bool time_jadge;
  unsigned long time1 = millis();
  time_jadge = true;
  flag = true;
  analogWrite(3, a);
  analogWrite(9, a);
  analogWrite(10, a);
  analogWrite(11, a);
  if (b == 0) return NULL;
  if (c == true) {
    while (time_jadge == true) {
      if (analogRead(A0) > 200) {
        time_jadge = false;
        i = false;
        flag = false;
        atk();
      }
      //int alpha =  time1 + b;
      if (millis() > (time1 + b)) {
        time_jadge = false;
        analogWrite(3, 0);
        analogWrite(9, 0);
        analogWrite(10, 0);
        analogWrite(11, 0);
      }
    }
  } else {
    delay(b);
    analogWrite(3, 0);
    analogWrite(9, 0);
    analogWrite(10, 0);
    analogWrite(11, 0);
  }
  return NULL;
}

void md_det(void) {
  lcd.setCursor(7, 0);
  if (digitalRead(43) == LOW) {
    if (digitalRead(41) == HIGH) {
      lcd.print("Attack");
    } else {
      lcd.print("Norm");
      if (digitalRead(42) == HIGH) {
        lcd.print("LF");
      } else {
        lcd.print("RT");
      }
    }
  } else {
    lcd.print("WARN: PK");
  }
}
unsigned long t, bf;
bool llcd = true;

void loop() {
  if (digitalRead(40) == LOW) {
    lcd.setCursor(11, 0);
    lcd.print(" |");
    lcd.print((millis() - t) / 1000);

    if (prim == true)primrun();
    m_mod(5);
    m_spd(150, 800, true);
    if (flag == true) {
      m_mod(6);
      m_spd(150, 800, true);
      if (flag == true) {
        m_mod(rand() % 6);
        m_spd(150, 500, true);
      }
    }
    llcd = true;
  } else {
    if (llcd == true ) {
      lcd.clear();
      llcd = false;
    }
    prim = true;
    lcd.setCursor(0, 0);
    lcd.print("Ready");
    lcd.setCursor(0, 1);
    lcd.print("Dir:");
    Wire.requestFrom(compassAddress, 2);
f1:
    if (Wire.available() > 1) {
      p_dig = Wire.read();
      p_dig = p_dig << 8;
      p_dig += Wire.read();
      p_dig /= 10;
    }
    if (p_dig == 0)goto f1;
    //if (digf == true) return a;
    lcd.print(p_dig);
    md_det();
    t = millis();
  }
}
