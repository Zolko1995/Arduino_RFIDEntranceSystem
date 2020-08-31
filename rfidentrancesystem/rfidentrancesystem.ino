#include<SPI.h> //Az SPI kommunikációs protokoll használatához szükséges könyvtár
#include<Wire.h> //Wire könyvtár beillesztése az I2C busz használatához
#include<LiquidCrystal_I2C.h> //Az I2C Folyékony kristályos LCD kijelző kezelő könyvtára
#include <MFRC522.h> //Az RFID olvasó használatához szükséges könyvtár

#define SS_PIN 10 //Az SS Soros bemeneti port definiálása
#define RST_PIN 9 //AZ RST vagyis nullázó port definiálása
#define LED_G 8 //Az engedélyezést jelző led portjának a definiálása
#define LED_R 7 //Az elutasítást jelző led portjának a definiálása

MFRC522 mfrc522 (SS_PIN, RST_PIN); //Típus definiálás az MFRC522-es RFID olvasónak
LiquidCrystal_I2C lcd(0x27, 20, 4); //Az általunk használt kijelző karakterkészlete 20 karakter és 3 sor

int piezo = 0; //A piezo elem által használt kimenet tárolása egy globális integer változóban

void setup()
{
  Serial.begin(9600); //A soros porton történő kommunikáció bitrátája
  SPI.begin(); //Az SPI buszon történő kommunikáció indítása
  mfrc522.PCD_Init(); //Az MFRC522-es RFID olvasó modul inicializálása
  pinMode(LED_G, OUTPUT); //Az engedélyezést jelző LED diódát tartalmazó Pin kimenetté alakítása
  pinMode(LED_R, OUTPUT); //Az elutasítást jelző LED diódát tartalmazó Pin kimenetté alakítása
  pinMode(piezo, OUTPUT); //A piezo elemet tartalmazó pin kimenetté alakítása
  lcd.init(); //Az LCD kijelző inicializálása
  lcd.backlight(); //Az LCD kijelző háttérvilágításának bekapcsolása
  lcd.setCursor(6, 0); //Kurzor pozicionálás ez esetben 6. karakter a 0. sorban
  lcd.print("UDOVOZOLJUK"); //Megadott karakterlánc kiíratása
  lcd.setCursor(0, 1); //Kurzor pozicionálás ez esetben 0. karakter a 1. sorban
  lcd.print("A RENDSZER AKTIV"); //Megadott karakterlánc kiíratása
  Serial.println("A RENDSZER AKTIV"); //Megadott karakterlánc kiíratása a soros portra
  lcd.setCursor(9, 2); //Kurzor pozicionálás ez esetben 9. karakter a 2. sorban
  lcd.print(":)"); //Megadott karakterlánc kiíratása
  delay(5000); //Várakozás 5 másodpercig
  Serial.println("Tap your card!"); //Megadott karakterlánc kiíratása
  lcd.clear(); //Az LCD kijelző tartalmának a törlése
  lcd.setCursor(6, 0); //Kurzor pozicionálás ez esetben 6. karakter a 0. sorban
  lcd.print("ERINTSE"); //Megadott karakterlánc kiíratása
  lcd.setCursor(8, 1); //Kurzor pozicionálás ez esetben 8. karakter a 1. sorban
  lcd.print("A"); //Megadott karakterlánc kiíratása
  lcd.setCursor(8, 2); //Kurzor pozicionálás ez esetben 8. karakter a 2. sorban
  lcd.print("KARTYAJAT"); //Megadott karakterlánc kiíratása
  delay(700); //Várakozás 700 milliszekundum ideig
  lcd.setCursor(12, 2); //Kurzor pozicionálás ez esetben 12. karakter a 2. sorban
  lcd.print("."); //Megadott karakterlánc kiíratása
  delay(700); //Várakozás 700 milliszekundum ideig
  lcd.setCursor(13, 2); //Kurzor pozicionálás ez esetben 13. karakter a 2. sorban
  lcd.print("."); //Megadott karakterlánc kiíratása
  delay(700); //Várakozás 700 milliszekundum ideig
  lcd.setCursor(14, 2); //Kurzor pozicionálás ez esetben 14. karakter a 2. sorban
  lcd.print("."); //Megadott karakterlánc kiíratása
}

void loop() //ciklus
{
  if ( !mfrc522.PICC_IsNewCardPresent())//Feltétel vizsgálat, ha új kártya van jelen:
  {
    return; //kilépés a feltétel vizsgálatból
  }
  if (! mfrc522.PICC_ReadCardSerial())//Feltétel vizgsálat kártya ID-jének kiolvasása
  {
    return; //kilépés a feltétel vizsgálatból
  }
  Serial.print("UID tag:"); //Megadott karakterlánc kiíratása a soros portra
  String content = ""; //String típusú üres karakterláncot tartalmazó változó
  byte letter; //Byte típusú változó
  for (byte i = 0; i < mfrc522.uid.size; i++) //for ciklus mely végighalad a kártya ID-ján:
  {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? "0" : ""); //Az ID ben lévő számok Hexadecimális stringre alakítása
    Serial.print(mfrc522.uid.uidByte[i], HEX); //Az átalakított ID kiíratása a soros portra
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ")); //A content valtozó konkatenálása az ID vel és 0x hozzarendelése
    content.concat(String(mfrc522.uid.uidByte[i], HEX)); //A konkatenáció kiíratása a soros portra
  }
  Serial.println(); //Üres karakterlánc kiíratása a soros portra
  Serial.print("UZENET:"); //Megadott karakterlánc kiíratása a soros portra
  content.toUpperCase(); //A változó tartalmának nagy karakterekre alakítása
  if (content.substring(1) == "F0 1E 78 7A") //Feltétel vizsgálat:
  {
    Serial.println("BELEPES ENGEDELYEZVE"); //Megadott karakterlánc kiíratása a soros portra
    Serial.println(); //Üres karakterlánc kiíratása a soros portra
    lcd.clear(); //Az LCD kijelző tartalmának a törlése
    lcd.setCursor(3, 1); //Kurzor pozicionálás ez esetben 3. karakter a 1. sorban
    lcd.print("BELEPES ENGEDELYEZVE"); //Megadott karakterlánc kiíratása
    delay(1000); //Várakozás 1 másodpercig
    digitalWrite(LED_G, HIGH); //Engedélyezést jelző Led bekapcsolva
    tone(piezo, 500); //Piezo elem megszólaltatása 500 hz frekvencian
    delay(300); //Várakozás 300 milliszekundum ideig
    noTone(piezo); //Piezo eleme elhallgattatása
    delay(10000); //Várakozás 10 másodpercig
    digitalWrite(LED_G, LOW); //Engedélyezést jelző Led kikapcsolva
  }
  else //Egyébként
  {
    Serial.println("BELEPES MEGTAGADVA"); //Megadott karakterlánc kiíratása a soros portra
    Serial.println(); //Üres karakterlánc kiíratása a soros portra
    lcd.clear(); //Az LCD kijelző tartalmának a törlése
    lcd.setCursor(3, 1); //Kurzor pozicionálás ez esetben 3. karakter a 1. sorban
    lcd.print("BELEPES MEGTAGADVA"); //Megadott karakterlánc kiíratása
    digitalWrite(LED_R, HIGH); //Elutasítást jelző Led bekapcsolva
    delay(1000); //Várakozás 1 másodpercig
    tone(piezo, 1200); //Piezo elem megszólaltatása 1200 hz frekvencian
    delay(300); //Várakozás 300 milliszekundum ideig
    noTone(piezo); //Piezo eleme elhallgattatása
    delay(3000); //Várakozás 3 másodpercig
    digitalWrite(LED_R, LOW); //Elutasítást jelző Led kikapcsolva
  }
}
