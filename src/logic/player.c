#include "player.h"

Player* (init_player)(){
  Player* ship = (Player*) malloc (sizeof(Player));
  /*
  width = 40px;
  height = 40px;
  */
  ship->x_min = 400;
  ship->x_max = 440;
  ship->y_min = 540;
  ship->y_max = 580;
  ship->lives = 6;
  ship->score = 0;
  ship->shots_no = 0;

  return ship;
}

void (move_player)(Player* ship, enum direction dir) {
  if (can_player_move(ship, dir)) {
    ship->x_min += dir == left ? -10 : 10;
    ship->x_max += dir == left ? -10 : 10;
  }
}

bool (can_player_move)(Player* ship, enum direction dir) {
  switch (dir) {
    case left: if (ship->x_min <= 1) return false; break;
    case right: if (ship->x_max >= 798) return false; break;
    default: break;
  }
  return true;
}

void (fire)(Player* ship) {
  Shot* array = (Shot*) malloc (sizeof(Shot) * (ship->shots_no + 1));
  for (int i = 0; i < ship->shots_no; i++) array[i] = ship->shots[i];
  Shot new_shot = init_shot(ship->x_min + 10, ship->y_min - 60, player);
  ship->shots_no++;
  array[ship->shots_no - 1] = new_shot;
  ship->shots = array;
}

void (loose_life)(Player* ship) {
  ship->lives--;
}

void (delete_player_shot)(Player* ship, int i) {
  Shot* s = &(ship->shots[i]);
  for (int x = i; x < ship->shots_no - 1; x++) ship->shots[x] = ship->shots[x + 1];
  free(s);
  ship->shots_no--;
}

void (draw_player)(Player* ship) {
  video_draw_xpm(ship->x_min, ship->y_min, "rocket");
  for (int i = 0; i < ship->shots_no; i++) draw_shot(&(ship->shots[i]));
}

void (increment_score)(Player* ship, int id) {
  int increment;
  if(id >= 0 && id <= 9) increment = 5;
  else if(id >= 10 && id <= 19) increment = 4;
  else if(id >= 20 && id <= 29) increment = 3;
  else if(id >= 30 && id <= 39) increment = 2;
  else increment = 1;
  ship->score = ship->score + increment;
}

void (draw_score)(Player* ship){
  int score = ship->score;
  video_draw_xpm(10, 10, "score");
  draw_score_aux(112, 10, score);
}

void (draw_lives)(int lives){
  switch(lives){
    case 6:
      video_draw_xpm(751, 10, "heart");
      video_draw_xpm(702, 10, "heart");
      video_draw_xpm(653, 10, "heart");
      break;
    case 5:
      video_draw_xpm(751, 10, "heart");
      video_draw_xpm(702, 10, "heart");
      video_draw_xpm(653, 10, "heart_bad");
      break;
    case 4:
      video_draw_xpm(751, 10, "heart");
      video_draw_xpm(702, 10, "heart");
      break;
    case 3:
      video_draw_xpm(751, 10, "heart");
      video_draw_xpm(702, 10, "heart_bad");
      break;    
    case 2:
      video_draw_xpm(751, 10, "heart");
      break;
    case 1:
      video_draw_xpm(751, 10, "heart_bad");
      break;
    case 0:
      break;
    default:
      video_draw_xpm(751, 10, "heart");
      video_draw_xpm(702, 10, "heart");
      video_draw_xpm(653, 10, "heart");
      break;
  }
}

void (draw_score_aux)(uint16_t x, uint16_t y, int score){
  if(score < 10){
    loop_draw_score_aux(x, y, score);
  }
  else if(score < 100){
    int second_half = score % 10;
    int first_half = score / 10;
    loop_draw_score_aux(x, y, first_half);
    loop_draw_score_aux(x + 17, y, second_half);    
  }
  else if(score < 1000){
    int third_half = score % 10;
    int second_half = (score / 10) % 10;
    int first_half = (score / 100) % 10;
    loop_draw_score_aux(x, y, first_half);
    loop_draw_score_aux(x + 17, y, second_half);
    loop_draw_score_aux(x + 34, y, third_half);
  }
}

void (loop_draw_score_aux)(uint16_t x, uint16_t y, int score) {
  switch(score){
    case 0:
      video_draw_xpm(x, y, "zero");
      break;
    case 1:
      video_draw_xpm(x, y, "one");
      break;
    case 2:
      video_draw_xpm(x, y, "two");
      break;
    case 3:
      video_draw_xpm(x, y, "three");
      break;
    case 4:
      video_draw_xpm(x, y, "four");
      break;            
    case 5:
      video_draw_xpm(x, y, "five");
      break;    
    case 6:
      video_draw_xpm(x, y, "six");
      break;
    case 7:
      video_draw_xpm(x, y, "seven");
      break;        
    case 8:
      video_draw_xpm(x, y, "eight");
      break;    
    case 9:
      video_draw_xpm(x, y, "nine");
      break;
    default: break;    
  } 
}
