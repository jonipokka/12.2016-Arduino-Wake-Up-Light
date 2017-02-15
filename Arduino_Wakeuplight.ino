// INCLUDET
#define DisplaySerial Serial // Using Hardware Serial
#include "Picaso_Serial_4DLib\Picaso_Const4D.h"
#include "Picaso_Serial_4DLib\Picaso_Serial_4DLib.h"
#include <math.h>
Picaso_Serial_4DLib Display(&DisplaySerial);

#define RGB2COLOR(r, g, b) ((((r>>3)<<11) | ((g>>2)<<5) | (b>>3)))

// ALKUMÄÄRITYKSET
/// Valikkojen määritykset
int menu = 1;
int select = 1;
int menu_id = 1, currentMenu = 1;
/// Ajan muuttujat
int timer1_counter;
unsigned int day = 0;
unsigned int hour = 0;
unsigned int minute = 0;
unsigned int second = 0;
char * paiva;
int preSeconds, preMinutes, preHours;
float preSecondsX = 0, preSecondsY = 100, preMinutesX = 0, preMinutesY = 100, preHoursX = 0, preHoursY = 100;
/// Hälytyksen muuttujat
int alarm_hour = 0;
int alarm_minute = 30;
bool alarm;
/// Muita muuttujia
int canpress = 2;
int update_inverval = 0;
long cnt = 0;
long lastCnt = 0;
bool draw = true;
/// Piirtämiseen liittyviä
unsigned short colourvalue = 0xFFE0; // HEXANA
char tempstring[256] = { 0 }; // Temporary String Variable for putstr
bool redraw_menu = true;
int cX = 162;
int cY = 160;
const float DegreesToRadians = 0.0174533;
int i;
/// Touch muuttujia
bool alarm_hour_pressed = false, alarm_minute_pressed = false;
bool change_hour_pressed = false, change_minute_pressed = false, change_second_pressed = false, change_day_pressed = false;
bool change_r_pressed = false, change_g_pressed = false, change_b_pressed = false;
bool is_pressed = false;
bool time_changed = false, colour_changed = false;
int new_hour = 0, new_minute = 0, new_second = 0, new_day = 0;
int button_y1 = 1, button_y2 = 24;
int buttonCoord[6][2] = {
  { 1, 62 } ,
  { 65, 126 } ,
  { 129, 190 } ,
  { 193, 254 } ,
  { 257, 318 }
};
int touch_X, touch_Y, old_touch_Y;
int r_pos = 255, g_pos = 255, b_pos = 0, r_value = 255, g_value = 255, b_value = 0;
unsigned short rgb_value;
bool preset_pressed = false;
int slider_touch = 62;
bool volume_pressed = false;
bool preview_pressed = false;
char * audio_library[8]{ "alarm1.wav" , "alarm2.wav", "alarm3.wav", "alarm4.wav",
"alarm5.wav", "alarm6.wav", "alarm7.wav", "alarm8.wav" };
int selected_alarm = 0;
int saved_X = 0;
int saved_Y = 0;
bool play_alarm = false;
int snooze_counter = 0;
float pwm_r = 255, pwm_g = 255, pwm_b = 0;
int seconds_total;
int seconds_alarm_total;


/*
int mainButton_x1 = 1, mainButton_x2 = 62;
int alarmButton_x1 = 65, alarmButton_x2 = 126;
int timeButton_x1 = 129, timeButton_x2 = 190;
int ledButton_x1 = 193, ledButton_x2 = 254;
int audioButton_x1 = 257, audioButton_x2 = 318;
*/

// TULOSTAMINEN &
// PIIRTOFUNKTIOT

/// Päivän tulostus
void draw_Day(int days, int size, int x, int y) {
  Display.txt_Height(size);
  Display.txt_Width(size);
  Display.txt_FGcolour(colourvalue);
  Display.gfx_MoveTo(x, y);

  if (days == 0)
    paiva = "    Maanantai   ";
  if (days == 1)
    paiva = "     Tiistai    ";
  if (days == 2)
    paiva = "   Keskiviikko  ";
  if (days == 3)
    paiva = "     Torstai    ";
  if (days == 4)
    paiva = "    Perjantai   ";
  if (days == 5)
    paiva = "    Lauantai    ";
  if (days == 6)
    paiva = "    Sunnuntai   ";

  sprintf(tempstring, paiva, colourvalue);
  Display.putstr(tempstring);
}

/// Valikon piirto
void draw_Menu() {
  /// Painikkeiden reunat
  for (int i = 0; i < 5; i++)
  {
    Display.gfx_Rectangle(buttonCoord[i][0], button_y1, buttonCoord[i][1], button_y2, colourvalue);
  }
  /// Fontin väri
  Display.txt_FGcolour(colourvalue);
  /// Fontin koko
  Display.txt_Width(1);
  Display.txt_Height(1);

  Display.gfx_MoveTo(16, 7);
  sprintf(tempstring, "MAIN", colourvalue);
  Display.putstr(tempstring);
  Display.gfx_MoveTo(76, 7);
  sprintf(tempstring, "ALARM", colourvalue);
  Display.putstr(tempstring);
  Display.gfx_MoveTo(144, 7);
  sprintf(tempstring, "TIME", colourvalue);
  Display.putstr(tempstring);
  Display.gfx_MoveTo(212, 7);
  sprintf(tempstring, "LED", colourvalue);
  Display.putstr(tempstring);
  Display.gfx_MoveTo(269, 7);
  sprintf(tempstring, "AUDIO", colourvalue);
  Display.putstr(tempstring);
}

/// Kellonajan piirto
void draw_Time(int hr, int mi, int se, int size, int x, int y) {
  Display.txt_Height(size);
  Display.txt_Width(size);
  Display.txt_FGcolour(colourvalue);
  Display.gfx_MoveTo(x, y);
  sprintf(tempstring, "%002d:%002d:%002d", hr, mi, se, colourvalue);
  Display.putstr(tempstring);
}

void draw_Time2(int time, int size, int x, int y) {
  Display.txt_Height(size);
  Display.txt_Width(size);
  Display.txt_FGcolour(colourvalue);
  Display.gfx_MoveTo(x, y);
  sprintf(tempstring, "%002d", time, colourvalue);
  Display.putstr(tempstring);
}

/// Kellon "tickkien" piirtäminen
void draw_Ticks() {

  for (int i = 0; i <= 60; i++) {

    /// "Tickkien välit"
    int lengthTick = 10;
    int medTick = 30;
    int longTick = 50;
    int r = 70;

    int len = 2;

    if (i % 5 == 0) { len = 9; }
    if (i % 15 == 0) { len = 15; }

    double di = (double)i;

    float tick_x1 = (70)* cos((di * 6)*DegreesToRadians); //calculate the position of the X coordinate
    float tick_y1 = (70)* sin((di * 6)*DegreesToRadians); //calculate the position of the Y coordinate
    float tick_x2 = (70 - len) * cos((di * 6)*DegreesToRadians); //calculate the position of the X coordinate
    float tick_y2 = (70 - len) * sin((di * 6)*DegreesToRadians); //calculate the position of the Y coordinate

    Display.gfx_Line(cX + (int)tick_x1, cY + (int)tick_y1, (int)tick_x2 + cX, (int)tick_y2 + cY, colourvalue);
  }
}

///Analogisen kellon pohjan piirtäminen
void draw_AnalogBack() {
  Display.gfx_Circle(162, 160, 70, colourvalue);
  Display.gfx_Circle(162, 160, 71, colourvalue);
  Display.gfx_Circle(162, 160, 72, colourvalue);
  draw_Ticks();
}

/// Sekunttiviisarin piirtäminen
void drawSecondsHand(int seconds)
{

  int len = 2;
  seconds = seconds - 15;
  if (seconds % 5 == 0) { len = 9; }
  if (seconds % 15 == 0) { len = 15; }

  float SecondsX, SecondsY;

  // Vanhan viivan päälle mustaa
  Display.gfx_Line(cX, cY, preSecondsX + cX, preSecondsY + cY, BLACK); //erase the previous second hand

  SecondsX = (70 - len) * cos((seconds * 6)*DegreesToRadians); //calculate the position of the X coordinate
  SecondsY = (70 - len) * sin((seconds * 6)*DegreesToRadians); //calculate the position of the Y coordinate
  preSecondsX = (int)SecondsX; //save the previous coordinates
  preSecondsY = (int)SecondsY; //save the previous coordinates

                 // Uusi viiva
  Display.gfx_Line(cX, cY, (int)SecondsX + cX, (int)SecondsY + cY, colourvalue); //draw the new seconds hand

}

/// Minuuttiviisarien piirtäminen
void drawMinutesHand(int minutes)
{
  minutes = minutes - 15;

  float MinutesX, MinutesY;


  MinutesX = (50) * cos((minutes * 6)*DegreesToRadians); //calculate the position of the X coordinate
  MinutesY = (50) * sin((minutes * 6)*DegreesToRadians); //calculate the position of the Y coordinate

                               // Vanhan viivan päälle mustaa
  if ((int)MinutesX != preMinutesX || (int)MinutesY != preMinutesY) {
    Display.gfx_Line(cX, cY, preMinutesX + cX, preMinutesY + cY, BLACK); //erase the previous second hand
    Display.gfx_Line(cX + 1, cY + 1, preMinutesX + cX + 1, preMinutesY + cY + 1, BLACK); //erase the previous second hand
  }

  preMinutesX = (int)MinutesX; //save the previous coordinates
  preMinutesY = (int)MinutesY; //save the previous coordinates

  Display.gfx_Line(cX, cY, (int)MinutesX + cX, (int)MinutesY + cY, colourvalue); //draw the new seconds hand
  Display.gfx_Line(cX + 1, cY + 1, (int)MinutesX + cX + 1, (int)MinutesY + cY + 1, colourvalue); //draw the new seconds hand

}

void getTouch() {
  touch_X = Display.touch_Get(TOUCH_GETX);
  touch_Y = Display.touch_Get(TOUCH_GETY);
  // Torkun asetus
  if (menu_id == 6 && touch_X >= 113 && touch_Y >= 88 && touch_X <= 217 && touch_Y <= 192 && cnt == 25) {

    seconds_alarm_total += 900;

    cnt = 0;

    menu_id = 1;
    play_alarm = false;
    redraw_menu = true;
    Display.snd_Stop();



  }
  // Hälytyksen sammutus
  if (menu_id == 6 && touch_X >= 100 && touch_Y >= 210 && touch_X <= 220 && touch_Y <= 240 && cnt == 25) {
    cnt = 0;
    alarm = false;
    menu_id = 1;
    play_alarm = false;
    redraw_menu = true;
    Display.snd_Stop();


  }
  // Valikon vaihtaminen
  if (!is_pressed) {
    for (int menuCounter = 0; menuCounter < 5; menuCounter++)
    {
      if (touch_X >= buttonCoord[menuCounter][0] && touch_X <= buttonCoord[menuCounter][1]
        && touch_Y >= button_y1 - 1 && touch_Y <= button_y2 + 16 && menu_id != 6) {
        menu_id = menuCounter + 1;
        redraw_menu = true;
      }
    }
    //Kappaleen valitseminen herätykselle
    if (menu_id == 5) {
      for (int a = 0; a <= 1; a++) {
        for (int i = 1; i <= 4; i++) {



          if (touch_X >= 22 + (a * 140) && touch_Y >= 53 + ((i - 1) * 26) && touch_X <= 155 + (a * 140) && touch_Y <= 71 + ((i - 1) * 26) && cnt == 25) {
            cnt = 0;

            /*Display.gfx_RectangleFilled(22 + (a * 140), 53 + (i * 26), 155 + (a * 140), 71 + (i * 26), BLACK);
            Display.gfx_Rectangle(22 + (a * 140), 53 + (i * 26), 155 + (a * 140), 71 + (i * 26), colourvalue);
            Display.gfx_MoveTo(28 + (a * 140), 56 + (i * 26));
            sprintf(tempstring, audio_library[((a + 1)*(i + 1) - 1)], colourvalue);
            Display.putstr(tempstring);*/

            if (selected_alarm != ((a * 4) + i) - 1) {
              selected_alarm = ((a * 4) + i) - 1;
              Display.gfx_RectangleFilled(22 + (a * 140), 53 + ((i - 1) * 26), 155 + (a * 140), 71 + ((i - 1) * 26), colourvalue);
              Display.txt_Height(1);
              Display.txt_Width(1);
              Display.gfx_MoveTo(28 + (a * 140), 56 + ((i - 1) * 26));
              Display.txt_BGcolour(colourvalue);
              Display.txt_FGcolour(BLACK);
              sprintf(tempstring, audio_library[((a * 4) + i) - 1], BLACK);
              Display.putstr(tempstring);
              Display.txt_BGcolour(BLACK);
              Display.txt_FGcolour(colourvalue);
              Display.gfx_RectangleFilled(22 + (saved_X * 140), 53 + (saved_Y * 26), 155 + (saved_X * 140), 71 + (saved_Y * 26), BLACK);
              Display.gfx_Rectangle(22 + (saved_X * 140), 53 + (saved_Y * 26), 155 + (saved_X * 140), 71 + (saved_Y * 26), colourvalue);
              Display.gfx_MoveTo(28 + (saved_X * 140), 56 + (saved_Y * 26));
              sprintf(tempstring, audio_library[(saved_X * 4) + saved_Y], colourvalue);
              Display.putstr(tempstring);
              saved_X = a;
              saved_Y = i - 1;

            }

          }
        }
      }
    }



    // PREVIEW napin äänen toisto ja painallus
    if (menu_id == 5 && touch_X >= 100 && touch_Y >= 210 && touch_X <= 220 && touch_Y <= 240 && cnt == 25) {

      preview_pressed = !preview_pressed;
      cnt = 0;


      if (preview_pressed) {

        Display.txt_Height(2);
        Display.txt_Width(2);
        Display.txt_BGcolour(colourvalue);
        Display.txt_FGcolour(BLACK);
        Display.gfx_MoveTo(110, 210);
        sprintf(tempstring, "PREVIEW", BLACK);
        Display.putstr(tempstring);
        Display.txt_BGcolour(BLACK);
        Display.txt_FGcolour(colourvalue);

        if (Display.snd_Playing() == 0) {

          Display.file_PlayWAV(audio_library[selected_alarm]);
        }

      }
      if (!preview_pressed) {

        Display.txt_Height(2);
        Display.txt_Width(2);
        Display.txt_BGcolour(BLACK);
        Display.txt_FGcolour(colourvalue);
        Display.gfx_MoveTo(110, 210);
        sprintf(tempstring, "PREVIEW", colourvalue);
        Display.putstr(tempstring);
        Display.snd_Stop();
      }

    }


    if (menu_id == 2 && cnt == 25) {
      if (touch_X >= 103 && touch_Y >= 186 && touch_X <= 217 && touch_Y <= 226) {
        alarm = !alarm;
        cnt = 0;
        //Hältytysnappi
        Display.txt_Height(3);
        Display.txt_Width(3);
        if (alarm == true) {
          Display.txt_BGcolour(colourvalue);
          Display.txt_FGcolour(BLACK);
          Display.gfx_MoveTo(115, 193);
          sprintf(tempstring, " ON ", colourvalue);
          Display.putstr(tempstring);
          Display.txt_BGcolour(BLACK);
          Display.txt_FGcolour(colourvalue);
        }
        if (alarm == false) {
          Display.txt_BGcolour(BLACK);
          Display.txt_FGcolour(colourvalue);
          Display.gfx_MoveTo(105, 193);
          sprintf(tempstring, " OFF ", colourvalue);
          Display.putstr(tempstring);
        }
      }
    }

    // Kellonajan asettaminen (SET nappi)
    if (menu_id == 3 && touch_X >= 97 && touch_Y >= 186 && touch_X <= 217 && touch_Y <= 226 && cnt == 25) {
      cnt = 0;
      hour = new_hour;
      minute = new_minute;
      second = new_second;
      day = new_day;
      time_changed = true;
      Display.txt_Height(3);
      Display.txt_Width(3);
      Display.txt_BGcolour(colourvalue);
      Display.txt_FGcolour(BLACK);
      Display.gfx_MoveTo(110, 193);
      sprintf(tempstring, " SET ", colourvalue);
      Display.putstr(tempstring);
      Display.txt_BGcolour(BLACK);
      Display.txt_FGcolour(colourvalue);
    }

    // Värin asettaminen (SET nappi)
    if (menu_id == 4 && touch_X >= 97 && touch_Y >= 186 && touch_X <= 217 && touch_Y <= 226 && cnt == 25) {
      cnt = 0;
      r_value = r_pos;
      g_value = g_pos;
      b_value = b_pos;
      rgb_value = RGB2COLOR(r_value, g_value, b_value);
      colourvalue = rgb_value;
      Display.gfx_Set(OBJECT_COLOUR, colourvalue);
      colour_changed = true;
      Display.txt_Height(3);
      Display.txt_Width(3);
      Display.txt_BGcolour(BLACK);
      Display.txt_FGcolour(colourvalue);
      Display.gfx_MoveTo(110, 193);
      sprintf(tempstring, " SET ", colourvalue);
      Display.putstr(tempstring);
    }

    if (menu_id == 4 && touch_Y >= 40 && touch_Y <= 77 && cnt == 25) {
      if (touch_X >= 28 && touch_X <= 68) {
        r_pos = 255, g_pos = 0, b_pos = 0;
        preset_pressed = true;
      }

      if (touch_X >= 69 && touch_X <= 110) {
        r_pos = 0, g_pos = 0, b_pos = 255;
        preset_pressed = true;
      }

      if (touch_X >= 111 && touch_X <= 152) {
        r_pos = 0, g_pos = 255, b_pos = 0;
        preset_pressed = true;
      }

      if (touch_X >= 153 && touch_X <= 194) {
        r_pos = 255, g_pos = 255, b_pos = 0;
        preset_pressed = true;
      }

      if (touch_X >= 195 && touch_X <= 236) {
        r_pos = 128, g_pos = 0, b_pos = 128;
        preset_pressed = true;
      }

      if (touch_X >= 237 && touch_X <= 278) {
        r_pos = 255, g_pos = 128, b_pos = 0;
        preset_pressed = true;
      }

      Display.gfx_Slider(0, 25, 90, 280, 110, BLACK, 255, r_pos);
      Display.gfx_Slider(0, 25, 120, 280, 140, BLACK, 255, g_pos);
      Display.gfx_Slider(0, 25, 150, 280, 170, BLACK, 255, b_pos);
    }
  }

  //Herätyksen asetuksen tuntien, minuuttien ja sekuntien koordinaatit
  if (menu_id == 2 && touch_X >= 66 && touch_Y >= 120 && touch_X <= 121 && touch_Y <= 160 && !is_pressed && !alarm) {
    old_touch_Y = touch_Y;
    is_pressed = true;
    alarm_hour_pressed = true;
  }

  if (menu_id == 2 && touch_X >= 121 && touch_Y >= 120 && touch_X <= 176 && touch_Y <= 160 && !is_pressed && !alarm) {
    old_touch_Y = touch_Y;
    is_pressed = true;
    alarm_minute_pressed = true;
  }

  //Ajan muutos
  if (menu_id == 3 && touch_X >= 66 && touch_Y >= 120 && touch_X <= 121 && touch_Y <= 160 && !is_pressed) {
    old_touch_Y = touch_Y;
    is_pressed = true;
    change_hour_pressed = true;
  }

  if (menu_id == 3 && touch_X >= 122 && touch_Y >= 120 && touch_X <= 176 && touch_Y <= 160 && !is_pressed) {
    old_touch_Y = touch_Y;
    is_pressed = true;
    change_minute_pressed = true;
  }

  if (menu_id == 3 && touch_X >= 177 && touch_Y >= 120 && touch_X <= 246 && touch_Y <= 160 && !is_pressed) {
    old_touch_Y = touch_Y;
    is_pressed = true;
    change_second_pressed = true;
  }

  if (menu_id == 3 && touch_X >= 31 && touch_Y >= 67 && touch_X <= 269 && touch_Y <= 110 && !is_pressed) {
    old_touch_Y = touch_Y;
    is_pressed = true;
    change_day_pressed = true;
  }

  if (menu_id == 4 && touch_X >= 25 && touch_Y >= 86 && touch_X <= 280 && touch_Y <= 115 && !is_pressed) {
    is_pressed = true;
    change_r_pressed = true;
  }

  if (menu_id == 4 && touch_X >= 25 && touch_Y >= 116 && touch_X <= 280 && touch_Y <= 145 && !is_pressed) {
    is_pressed = true;
    change_g_pressed = true;
  }

  if (menu_id == 4 && touch_X >= 25 && touch_Y >= 146 && touch_X <= 280 && touch_Y <= 175 && !is_pressed) {
    is_pressed = true;
    change_b_pressed = true;
  }
  //sliderin piirtäminen 
  if (menu_id == 5 && touch_X >= 34 && touch_Y >= 173 && touch_X <= 283 && touch_Y <= 189 && !is_pressed) {

    is_pressed = true;
    volume_pressed = true;
  }
}

/// Tuntiviisarien piirtäminen
void drawHoursHand(float hours)
{
  if (hours > 12) {
    hours -= 12;
  }

  //float tempmin = minute/12;
  hours = ((hours / 12) * 60) + (float)minute / 12;

  hours = hours - 15;

  float HoursX, HoursY;

  HoursX = (35) * cos((hours * 6)*DegreesToRadians); //calculate the position of the X coordinate
  HoursY = (35) * sin((hours * 6)*DegreesToRadians); //calculate the position of the Y coordinate

                             // Vanhan viivan päälle mustaa
  if ((int)HoursX != preHoursX || (int)HoursY != preHoursY) {
    Display.gfx_Line(cX, cY, preHoursX + cX, preHoursY + cY, BLACK); //erase the previous second hand
    Display.gfx_Line(cX + 1, cY + 1, preHoursX + cX + 1, preHoursY + cY + 1, BLACK); //erase the previous second hand
  }

  preHoursX = (int)HoursX; //save the previous coordinates
  preHoursY = (int)HoursY; //save the previous coordinates

  Display.gfx_Line(cX, cY, (int)HoursX + cX, (int)HoursY + cY, colourvalue); //draw the new seconds hand
  Display.gfx_Line(cX + 1, cY + 1, (int)HoursX + cX + 1, (int)HoursY + cY + 1, colourvalue); //draw the new seconds hand

}

// SETUP
void setup() {
  // RGB PWM PINNIT
  pinMode(3, OUTPUT); // R
  pinMode(5, OUTPUT); // G
  pinMode(6, OUTPUT); // B

            ///Ajastimen määritykset
  noInterrupts();           // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;

  timer1_counter = 3036;
  TCNT1 = timer1_counter;   // preload timer
  TCCR1B |= (1 << CS12);    // 256 prescaler 
  TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt
  interrupts();             // enable all interrupts

                /// Näytön määritykset
  pinMode(4, OUTPUT);  // Set D4 on Arduino to Output (4D Arduino Adaptor V2 - Display Reset)
  digitalWrite(4, 1);  // Reset the Display via D4
  delay(100);
  digitalWrite(4, 0);  // unReset the Display via D4  

  delay(3500); //let the display start up

  DisplaySerial.begin(9600); // Using default 9600 baud rate
  Display.TimeLimit4D = 5000; // 5 second timeout on all commands  
  Display.gfx_BGcolour(BLACK); // Make it text on white background
  Display.txt_BGcolour(BLACK);
  Display.gfx_Set(OBJECT_COLOUR, colourvalue);
  Display.gfx_ScreenMode(4); // Landscape, microSD Card Facing Up  
  Display.gfx_Cls(); // Clear Screen

             // SD Kortin mounttaus ja volumen asettaminen
  Display.file_Mount();
  Display.snd_Volume(65);
  //Display.snd_Pitch();
  //Display.snd_BufSize(1);

  Display.touch_Set(TOUCH_ENABLE);
  Display.touch_Set(TOUCH_REGIONDEFAULT);

  // Piirretään menut
  draw_Menu();
  draw_AnalogBack();
}

// AJASTIN
/// interrupt service routine 
ISR(TIMER1_OVF_vect)
{
  TCNT1 = timer1_counter;   // preload timer

  second++;

  //Sekunnit
  if (second == 60) {
    second = 0;
    minute++;
  }
  //Minuutit
  if (minute == 60) {
    minute = 0;
    hour++;
  }
  //Tunnit
  if (hour == 24) {
    hour = 0;
    minute = 0;
    second = 0;
    day++;
  }
  //Päivät
  if (day == 7) {
    day = 0;
  }

  // Draw trueksi
  draw = true;
  canpress++;
  if (canpress > 2) { canpress = 2; }
}

/// *********************** ///
//      PÄÄLOOPPI         //
/// ********************** ///
void loop() {

  // hälytyksen tapahtuessa toistettavat asitat
  if (seconds_alarm_total == seconds_total && alarm) {
    menu_id = 6;
    redraw_menu = true;
    play_alarm = true;
  }
  if (play_alarm == true) {

    if (Display.snd_Playing() == 0 && cnt == 25) {
      cnt = 0;
      Display.file_PlayWAV(audio_library[selected_alarm]);
    }
  }
  if (preview_pressed && menu_id != currentMenu) {
    Display.snd_Stop();
  }

  //sliderin piirto
  if (volume_pressed) {

    slider_touch = (touch_X - 34) / 1.97;
    Display.gfx_Slider(0, 34, 173, 284, 189, BLACK, 127, slider_touch);
  }
  //PREVIEW napin palauttaminen normaaliksi toiston jälkeen
  if (menu_id == 5 && Display.snd_Playing() == 0 && preview_pressed && cnt == 25) {

    preview_pressed = false;
    Display.txt_Height(2);
    Display.txt_Width(2);
    Display.txt_BGcolour(BLACK);
    Display.txt_FGcolour(colourvalue);
    Display.gfx_MoveTo(110, 210);
    sprintf(tempstring, "PREVIEW", colourvalue);
    Display.putstr(tempstring);

  }

  //Herätys ajan asetus
  if (alarm_hour_pressed) {
    if ((old_touch_Y - touch_Y) < (-4)) {
      alarm_hour++;
      old_touch_Y = touch_Y;
    }
    if ((old_touch_Y - touch_Y) > 4) {
      alarm_hour--;
      old_touch_Y = touch_Y;
    }
    if (alarm_hour < 0) {
      alarm_hour = 23;
    }
    if (alarm_hour == 23) {
      draw_Time2(0, 2, 73, 97);
    }
    else {
      draw_Time2(alarm_hour + 1, 2, 73, 97);
    }
    if (alarm_hour > 23 || alarm_hour <= 0) {
      alarm_hour = 0;
      draw_Time2(23, 2, 73, 155);
    }
    else {
      draw_Time2(alarm_hour - 1, 2, 73, 155);
    }
    draw_Time(alarm_hour, alarm_minute, 0, 3, 66, 120);
  }

  if (alarm_minute_pressed) {
    if ((old_touch_Y - touch_Y) < (-4)) {
      alarm_minute++;
      old_touch_Y = touch_Y;
    }
    if ((old_touch_Y - touch_Y) > 4) {
      alarm_minute--;
      old_touch_Y = touch_Y;
    }
    if (alarm_minute < 0) {
      alarm_minute = 59;
    }
    if (alarm_minute == 59) {
      draw_Time2(0, 2, 142, 97);
    }
    else {
      draw_Time2(alarm_minute + 1, 2, 142, 97);
    }
    if (alarm_minute > 59 || alarm_minute <= 0) {
      alarm_minute = 0;
      draw_Time2(59, 2, 142, 155);
    }
    else {
      draw_Time2(alarm_minute - 1, 2, 142, 155);
    }
    draw_Time(alarm_hour, alarm_minute, 0, 3, 66, 120);
  }

  // AJAN ASETUS

  /// Tuntien muuttaminen
  if (change_hour_pressed) {
    if ((old_touch_Y - touch_Y) < (-4)) {
      new_hour++;
      old_touch_Y = touch_Y;
    }
    if ((old_touch_Y - touch_Y) > 4) {
      new_hour--;
      old_touch_Y = touch_Y;
    }
    if (new_hour < 0) {
      new_hour = 23;
    }
    if (new_hour == 23) {
      draw_Time2(0, 2, 73, 97);
    }
    else {
      draw_Time2(new_hour + 1, 2, 73, 97);
    }
    if (new_hour > 23 || new_hour <= 0) {
      new_hour = 0;
      draw_Time2(23, 2, 73, 155);
    }
    else {
      draw_Time2(new_hour - 1, 2, 73, 155);
    }
    draw_Time(new_hour, new_minute, new_second, 3, 66, 120);
  }

  /// Minuuttien muuttaminen
  if (change_minute_pressed) {
    if ((old_touch_Y - touch_Y) < (-4)) {
      new_minute++;
      old_touch_Y = touch_Y;
    }
    if ((old_touch_Y - touch_Y) > 4) {
      new_minute--;
      old_touch_Y = touch_Y;
    }
    if (new_minute < 0) {
      new_minute = 59;
    }
    if (new_minute == 59) {
      draw_Time2(0, 2, 142, 97);
    }
    else {
      draw_Time2(new_minute + 1, 2, 142, 97);
    }
    if (new_minute > 59 || new_minute <= 0) {
      new_minute = 0;
      draw_Time2(59, 2, 142, 155);
    }
    else {
      draw_Time2(new_minute - 1, 2, 142, 155);
    }
    draw_Time(new_hour, new_minute, new_second, 3, 66, 120);
  }

  /// Sekunttien muuttaminen
  if (change_second_pressed) {
    if ((old_touch_Y - touch_Y) < (-4)) {
      new_second++;
      old_touch_Y = touch_Y;
    }
    if ((old_touch_Y - touch_Y) > 4) {
      new_second--;
      old_touch_Y = touch_Y;
    }
    if (new_second < 0) {
      new_second = 59;
    }
    if (new_second == 59) {
      draw_Time2(0, 2, 211, 97);
    }
    else {
      draw_Time2(new_second + 1, 2, 211, 97);
    }
    if (new_second > 59 || new_second <= 0) {
      new_second = 0;
      draw_Time2(59, 2, 211, 155);
    }
    else {
      draw_Time2(new_second - 1, 2, 211, 155);
    }
    draw_Time(new_hour, new_minute, new_second, 3, 66, 120);
  }

  /// Päivien muuttaminen
  if (change_day_pressed) {
    if ((old_touch_Y - touch_Y) < (-9)) {
      new_day++;
      old_touch_Y = touch_Y;
    }
    if ((old_touch_Y - touch_Y) > 9) {
      new_day--;
      old_touch_Y = touch_Y;
    }
    if (new_day < 0) {
      new_day = 6;
    }
    if (new_day > 6) {
      new_day = 0;
    }
    if (new_day == 0) {
      draw_Day(6, 1, 103, 94);
    }
    else {
      draw_Day(new_day - 1, 1, 103, 94);
    }
    if (new_day == 6) {
      draw_Day(0, 1, 103, 55);
    }
    else {
      draw_Day(new_day + 1, 1, 103, 55);
    }
    draw_Day(new_day, 2, 31, 67);
  }

  if (change_r_pressed) {
    if ((r_pos + g_pos + b_pos) < 100) {
      if ((touch_X - 25) > r_pos) {
        r_pos = touch_X - 25;
      }
    }
    else {
      r_pos = touch_X - 25;
    }
    if (r_pos < 0) r_pos = 0;
    if (r_pos > 255) r_pos = 255;
    Display.gfx_Slider(0, 25, 90, 280, 110, BLACK, 255, r_pos);
  }

  if (change_g_pressed) {
    if ((r_pos + g_pos + b_pos) < 100) {
      if ((touch_X - 25) > g_pos) {
        g_pos = touch_X - 25;
      }
    }
    else {
      g_pos = touch_X - 25;
    }
    if (g_pos < 0) g_pos = 0;
    if (g_pos > 255) g_pos = 255;
    Display.gfx_Slider(0, 25, 120, 280, 140, BLACK, 255, g_pos);
  }

  if (change_b_pressed) {
    if ((r_pos + g_pos + b_pos) < 100) {
      if ((touch_X - 25) > b_pos) {
        b_pos = touch_X - 25;
      }
    }
    else {
      b_pos = touch_X - 25;
    }
    if (b_pos < 0) b_pos = 0;
    if (b_pos > 255) b_pos = 255;
    Display.gfx_Slider(0, 25, 150, 280, 170, BLACK, 255, b_pos);
  }

  if (change_r_pressed || change_g_pressed || change_b_pressed || preset_pressed) {
    Display.txt_Height(3);
    Display.txt_Width(3);
    Display.gfx_MoveTo(110, 193);
    rgb_value = RGB2COLOR(r_pos, g_pos, b_pos);
    Display.txt_BGcolour(rgb_value);
    Display.txt_FGcolour(BLACK);
    sprintf(tempstring, " SET ", BLACK);
    Display.putstr(tempstring);
    Display.txt_BGcolour(BLACK);
    Display.txt_FGcolour(colourvalue);
  }

  // Menujen piirtäminen
  if ((redraw_menu == true && menu_id != currentMenu) || colour_changed) {
    Display.gfx_RectangleFilled(0, 25, 320, 240, BLACK);
    currentMenu = menu_id;
    redraw_menu = false;

    if (colour_changed) {
      draw_Menu();
      colour_changed = false;
    }

    switch (menu_id) {
    case 1: /// MAIN
      draw_AnalogBack();
      break;
    case 2: /// ALARM
      draw_Time(alarm_hour, alarm_minute, 0, 3, 66, 120);
      /*
      Display.txt_Height(2);
      Display.txt_Width(2);
      Display.txt_FGcolour(colourvalue);
      Display.gfx_MoveTo(120, 97);
      sprintf(tempstring, "Alarm", colourvalue);
      Display.putstr(tempstring);
      */
      //Kellonaika
      draw_Time(hour, minute, second, 3, 66, 50);
      draw_Day(day, 2, 31, 26);
      //Hältytysnappi
      Display.txt_Height(3);
      Display.txt_Width(3);
      if (alarm == true) {
        Display.txt_BGcolour(colourvalue);
        Display.txt_FGcolour(BLACK);
        Display.gfx_MoveTo(115, 193);
        sprintf(tempstring, " ON ", colourvalue);
        Display.putstr(tempstring);
        Display.txt_BGcolour(BLACK);
        Display.txt_FGcolour(colourvalue);
      }
      if (alarm == false) {
        Display.txt_BGcolour(BLACK);
        Display.txt_FGcolour(colourvalue);
        Display.gfx_MoveTo(110, 193);
        sprintf(tempstring, " OFF ", colourvalue);
        Display.putstr(tempstring);
      }
      break;
    case 3: /// TIME
      new_hour = hour;
      new_minute = minute;
      new_second = second;
      new_day = day;
      draw_Time(new_hour, new_minute, new_second, 3, 66, 120);
      draw_Day(new_day, 2, 31, 67);
      Display.txt_Height(3);
      Display.txt_Width(3);
      Display.gfx_MoveTo(110, 193);
      sprintf(tempstring, " SET ", colourvalue);
      Display.putstr(tempstring);
      break;
    case 4: /// LED
      Display.gfx_RectangleFilled(32, 40, 64, 72, 0xF800); //255    0   0 RED
      Display.gfx_RectangleFilled(74, 40, 106, 72, 0x001F); //0   0   255 BLUE
      Display.gfx_RectangleFilled(116, 40, 148, 72, 0x07E0); //0    255   0 GREEN
      Display.gfx_RectangleFilled(158, 40, 190, 72, 0xFFE0); //255  255   0 YELLOW
      Display.gfx_RectangleFilled(200, 40, 232, 72, 0x8010); //128  0   128 PURPLE
      Display.gfx_RectangleFilled(242, 40, 274, 72, 0xFC00); //255  128   0 ORANGE
      Display.gfx_RectangleFilled(24, 89, 281, 111, colourvalue);
      Display.gfx_RectangleFilled(24, 119, 281, 141, colourvalue);
      Display.gfx_RectangleFilled(24, 149, 281, 171, colourvalue);
      Display.gfx_Slider(0, 25, 90, 280, 110, BLACK, 255, r_value);
      Display.gfx_Slider(0, 25, 120, 280, 140, BLACK, 255, g_value);
      Display.gfx_Slider(0, 25, 150, 280, 170, BLACK, 255, b_value);
      Display.txt_Height(2);
      Display.txt_Width(2);
      Display.gfx_MoveTo(290, 89);
      sprintf(tempstring, "R", colourvalue);
      Display.putstr(tempstring);
      Display.gfx_MoveTo(290, 119);
      sprintf(tempstring, "G", colourvalue);
      Display.putstr(tempstring);
      Display.gfx_MoveTo(290, 149);
      sprintf(tempstring, "B", colourvalue);
      Display.putstr(tempstring);
      Display.txt_Height(3);
      Display.txt_Width(3);
      Display.gfx_MoveTo(110, 193);
      rgb_value = RGB2COLOR(r_value, g_value, b_value);
      Display.txt_BGcolour(rgb_value);
      Display.txt_FGcolour(BLACK);
      sprintf(tempstring, " SET ", BLACK);
      Display.putstr(tempstring);
      Display.txt_FGcolour(colourvalue);
      Display.txt_BGcolour(BLACK);
      break;
    case 5: /// AUDIO
      Display.txt_Height(2);
      Display.txt_Width(2);
      Display.gfx_MoveTo(110, 210);
      sprintf(tempstring, "PREVIEW", colourvalue);
      Display.putstr(tempstring);
      Display.txt_Height(1);
      Display.txt_Width(1);
      Display.gfx_MoveTo(120, 160);
      sprintf(tempstring, "  VOLUME", colourvalue);
      Display.putstr(tempstring);


      Display.txt_Height(1);
      Display.txt_Width(1);
      Display.gfx_MoveTo(108, 35);
      sprintf(tempstring, "AUDIO LIBRARY", colourvalue);
      Display.putstr(tempstring);
      Display.gfx_Slider(0, 34, 173, 284, 189, BLACK, 127, slider_touch);
      Display.gfx_Rectangle(33, 172, 285, 190, colourvalue);

      if (menu_id == 5) {
        for (int a = 0; a <= 1; a++) {
          for (int i = 1; i <= 4; i++) {


            Display.txt_Height(1);
            Display.txt_Width(1);
            Display.gfx_MoveTo(28 + (a * 140), 56 + ((i - 1) * 26));
            Display.txt_BGcolour(BLACK);
            Display.txt_FGcolour(colourvalue);
            Display.gfx_Rectangle(22 + (a * 140), 53 + ((i - 1) * 26), 155 + (a * 140), 71 + ((i - 1) * 26), colourvalue);
            sprintf(tempstring, audio_library[((a * 4) + i) - 1], colourvalue);
            Display.putstr(tempstring);
            if (saved_X <= 6 && saved_Y <= 6) {
              Display.txt_Height(1);
              Display.txt_Width(1);
              Display.txt_BGcolour(colourvalue);
              Display.txt_FGcolour(BLACK);
              Display.gfx_RectangleFilled(22 + (saved_X * 140), 53 + (saved_Y * 26), 155 + (saved_X * 140), 71 + (saved_Y * 26), colourvalue);

              Display.gfx_MoveTo(28 + (saved_X * 140), 56 + (saved_Y * 26));
              sprintf(tempstring, audio_library[saved_X * 4 + saved_Y], colourvalue);
              Display.putstr(tempstring);
              Display.txt_BGcolour(BLACK);
              Display.txt_FGcolour(colourvalue);
            }
          }
        }
      }

      break;
    case 6: /// HIDDEN ALARM
      Display.gfx_Circle(165, 140, 52, colourvalue);
      //Display.gfx_Rectangle(78, 105, 235, 185, colourvalue);
      Display.txt_Height(2);
      Display.txt_Width(2);
      Display.gfx_MoveTo(93, 205);
      sprintf(tempstring, "ALARM OFF", colourvalue);
      Display.putstr(tempstring);
      Display.txt_Height(1);
      Display.txt_Width(1);
      Display.gfx_MoveTo(117, 125);
      sprintf(tempstring, "   SNOOZE", colourvalue);
      Display.putstr(tempstring);
      Display.txt_Height(1);
      Display.txt_Width(1);
      Display.gfx_MoveTo(117, 145);
      sprintf(tempstring, "   15 MIN", colourvalue);
      Display.putstr(tempstring);
      break;
    default:
      break;
    }
  }

  // Tämä piirto tapahtuu joka sekuntti!
  if (draw == true) {

    // Herätysajan sekuntien päivitys ja kellon sekuntien päivitys
    seconds_alarm_total = (alarm_hour * 3600) + (alarm_minute * 60);
    seconds_total = (hour * 3600) + (minute * 60) + second;

    //PWM kasvatus
    if ((alarm == true && (seconds_alarm_total - seconds_total) >= 0 && (seconds_alarm_total - seconds_total) < 1800) || play_alarm) {
      //if ((int)pwm_current[0] < r_value) {
      //pwm_r = ((seconds_total - (seconds_alarm_total - 1800)) / 1800)*r_value;
      int pwm_temp = 1800 - (seconds_alarm_total - seconds_total);
      pwm_r = ((float)pwm_temp / 1800)*(float)r_value;
      //pwm_r = 255;
      //if (pwm_current[0] >= r_value) { pwm_current[0] = r_value; }
      //if ((int)pwm_current[1] < g_value) {
      //pwm_g = ((seconds_total - (seconds_alarm_total - 1800)) / 1800)*g_value;
      pwm_g = ((float)pwm_temp / 1800)*(float)g_value;
      //pwm_g = 255;
      //if (pwm_current[1] >= g_value) { pwm_current[1] = g_value; }
      //if ((int)pwm_current[2] < b_value) {
      //pwm_b = ((seconds_total - (seconds_alarm_total - 1800)) / 1800)*b_value;
      pwm_b = ((float)pwm_temp / 1800)*(float)b_value;
      //pwm_b = 0;
      //if (pwm_current[2] >= b_value) { pwm_current[2] = b_value; }
      Display.txt_Height(1);
      Display.txt_Width(1);
      Display.txt_BGcolour(BLACK);
      Display.txt_FGcolour(colourvalue);
      Display.gfx_MoveTo(10, 160);
      sprintf(tempstring, "PWM_R: %i PWM_G: %i PWM_B: %i", (int)pwm_r, (int)pwm_g, (int)pwm_b);
      Display.putstr(tempstring);
      Display.gfx_MoveTo(10, 180);
      sprintf(tempstring, "R: %i G: %i B: %i", r_value, g_value, b_value);
      Display.putstr(tempstring);
      Display.gfx_MoveTo(10, 200);
      sprintf(tempstring, "seconds_total: %i\nseconds_alarm_total: %i", seconds_total, seconds_alarm_total);
      Display.putstr(tempstring);
    }

    if (alarm == false && menu_id != 4) {
      analogWrite(3, 0); // R
      analogWrite(5, 0); // G
      analogWrite(6, 0); // B
    }
    else {
      analogWrite(3, (int)pwm_r); // R
      analogWrite(5, (int)pwm_g); // G
      analogWrite(6, (int)pwm_b); // B
    }


    switch (menu_id) {
    case 1: /// MAIN
        /// Päänäkymän kellonaika
      draw_Time(hour, minute, second, 3, 66, 50);
      draw_Day(day, 2, 31, 26);
      /// Päänäkymän viisarit
      drawSecondsHand(second);
      drawMinutesHand(minute);
      drawHoursHand(hour);
      break;
    case 2: /// ALARM
      draw_Time(hour, minute, second, 3, 66, 50);
      draw_Day(day, 2, 31, 26);
      break;
    case 3: /// TIME
        //draw_Time(new_hour, new_minute, new_second, 3, 66, 120);
        //draw_Day(new_day, 31, 97);
      break;
    case 4: /// LED
      pwm_r = r_value;
      pwm_g = g_value;
      pwm_b = b_value;
      break;
    case 5: /// AUDIO
      break;
    case 6: /// HIDDEN ALARM
      draw_Time(hour, minute, second, 3, 66, 50);
      draw_Day(day, 2, 31, 26);
      break;
    default: break;
    }
    draw = false;

  }

  // Touching päivitys
  if (Display.touch_Get(TOUCH_STATUS) != NOTOUCH) {
    getTouch();
  }

  // Kosketuksen päivitys caset
  else if (Display.touch_Get(TOUCH_STATUS) == NOTOUCH) {
    if (is_pressed) {
      is_pressed = false;
    }
    switch (menu_id) {
    case 1: /// MAIN
      break;
    case 2: /// ALARM
      break;
    case 3: /// TIME
      break;
    case 4: /// LED
      break;
    case 5: /// AUDIO
      break;
    case 6: /// HIDDEN ALARM
      break;
    default:
      break;
    }

    // Hälytyksen tuntien piirtohomma
    if (alarm_hour_pressed || alarm_minute_pressed) {
      alarm_hour_pressed = false;
      alarm_minute_pressed = false;
      Display.gfx_RectangleFilled(0, 97, 320, 120, BLACK);
      Display.gfx_RectangleFilled(0, 150, 320, 180, BLACK);
    }

    if (change_hour_pressed || change_minute_pressed || change_second_pressed || change_day_pressed) {
      change_hour_pressed = false;
      change_minute_pressed = false;
      change_second_pressed = false;
      change_day_pressed = false;
      Display.gfx_RectangleFilled(0, 94, 320, 120, BLACK);
      Display.gfx_RectangleFilled(0, 150, 320, 180, BLACK);
      Display.gfx_RectangleFilled(0, 55, 320, 68, BLACK);
    }

    if (time_changed) {
      time_changed = false;
      if (menu_id == 3) {
        Display.txt_Height(3);
        Display.txt_Width(3);
        Display.txt_BGcolour(BLACK);
        Display.txt_FGcolour(colourvalue);
        Display.gfx_MoveTo(110, 193);
        sprintf(tempstring, " SET ", 0xFFE0);
        Display.putstr(tempstring);
      }
    }


    if (change_r_pressed) {
      change_r_pressed = false;
    }

    if (change_g_pressed) {
      change_g_pressed = false;
    }

    if (change_b_pressed) {
      change_b_pressed = false;
    }

    if (preset_pressed) {
      preset_pressed = false;
    }

    if (volume_pressed) {
      volume_pressed = false;
      Display.snd_Volume(slider_touch);
    }
  }

  // Program countteri joka nollaantuu 25
  cnt = cnt + 1;
  if (cnt > 25) {
    cnt = 25;

  }

}
