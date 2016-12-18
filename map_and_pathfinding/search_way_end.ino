int row_h;
int row_v;
int shelf_1 = -1;
int shelf_2 = -2;
int shelf_3 = -3;
int start = -11;
int finish = -99;
int robot_x = 0; //стартовая координата "х" для робота
int robot_y = 4; //стартовая координата "у" для робота
int shelf_x = 2; //координата "х" нужной полки
int shelf_y = 0; //координата "у" нужной полки
int shelf = -2; //номер нужной полки
int n = 0; //переменные, необходимые для выполнения последующих условий и циклов
int m = 1;
int h_up;
int h_down;
int v_left;
int v_right;
int need;
int need_1 = 0;
int need_2 = 0;
int x;
int y;
int x_1 = 0;
int y_1 = 0;
int x_2 = 0;
int y_2 = 0;
int stock[5][5];
int way;
int lengtharray;
int i;
int hint;
int direction_r = 4;


void setup() {
  Serial.begin(9600);
  for (row_h = 0; row_h < 5; ++row_h) {
    for (row_v = 0; row_v < 5; ++row_v) {
      stock[row_h][row_v] = 0;

    }
  }
  stock[0][1] = shelf_1;
  stock[2][0] = shelf_2;
  stock[4][4] = shelf_3;
  stock[0][4] = start;
  stock[4][0] = finish;
}


void loop() {
  delay (3000);
  h_up = robot_x - 1;
  h_down = robot_x + 1;
  v_left = robot_y - 1;
  v_right = robot_y + 1;

  if (h_up >= 0) {
    if (stock[h_up][robot_y] == shelf) {
      n = 1;
    }
  }
  if (h_down <= 4) {
    if (stock[h_down][robot_y] == shelf) {
      n = 1;
    }
  }
  if (v_left >= 0) {
    if (stock[robot_x][v_left] == shelf) {
      n = 1;
    }
  }
  if (v_right <= 4) {
    if (stock[robot_x][v_right] == shelf) {
      n = 1;
    }
  }

  if (n == 0) {
    if (h_up >= 0 && stock[h_up][robot_y] == 0) {
      stock[h_up][robot_y] = m;
    }
    if (h_down <= 4 && stock[h_down][robot_y] == 0) {
      stock[h_down][robot_y] = m;

    }
    if (v_left >= 0 && stock[robot_x][v_left] == 0) {
      stock[robot_x][v_left] = m;
    }
    if (v_right <= 4 && stock[robot_x][v_right] == 0) {
      stock[robot_x][v_right] = m;
    }
  }
  while (n == 0) {
    for (row_h = 0; row_h < 5; ++row_h) {
      for (row_v = 0; row_v < 5; ++row_v) {
        if (stock[row_h][row_v] == m) {
          robot_x = row_h;
          robot_y = row_v;
          h_up = robot_x - 1;
          h_down = robot_x + 1;
          v_left = robot_y - 1;
          v_right = robot_y + 1;
          if (h_up >= 0) {
            if (stock[h_up][robot_y] == shelf) {
              n = 1;
            }
          }
          if (h_down <= 4) {
            if (stock[h_down][robot_y] == shelf) {
              n = 1;
            }
          }
          if (v_left >= 0) {
            if (stock[robot_x][v_left] == shelf) {
              n = 1;
            }
          }
          if (v_right <= 4) {
            if (stock[robot_x][v_right] == shelf) {
              n = 1;
            }
          }
          if (n == 0) {

            if (h_up >= 0 && stock[h_up][robot_y] == 0) {
              stock[h_up][robot_y] = m + 1;
            }
            if (h_down <= 4 && stock[h_down][robot_y] == 0) {
              stock[h_down][robot_y] = m + 1;
            }
            if (v_left >= 0 && stock[robot_x][v_left] == 0) {
              stock[robot_x][v_left] = m + 1;
            }
            if (v_right <= 4 && stock[robot_x][v_right] == 0) {
              stock[robot_x][v_right] = m + 1;
            }
          }
        }
      }
    }
    m = m + 1;
  }
  
   //for (row_h = 0; row_h < 5; ++row_h) {
   //for (row_v = 0; row_v < 5; ++row_v) {
      //Serial.print(stock[row_h][row_v]);
      //Serial.print(" ");
  //}
      //Serial.println(" "); 
  //}
  
  h_up = shelf_x - 1;
  h_down = shelf_x + 1;
  v_left = shelf_y - 1;
  v_right = shelf_y + 1;
  if (v_left >= 0 && v_right <= 4) {
    if (stock[shelf_x][v_left] != 0 && stock[shelf_x][v_right] != 0) {
      if (stock[shelf_x][v_right] < stock[shelf_x][v_left]) {
        need_1 = stock[shelf_x][v_right];
        x_1 = shelf_x;
        y_1 = v_right;
      }
      else {
        need_1 = stock[shelf_x][v_left];
        x_1 = shelf_x;
        y_1 = v_left;
      }
    }

    else {
      if (stock[shelf_x][v_left] == 0) {
        if (stock[shelf_x][v_right] != 0) {
          need_1 = stock[shelf_x][v_right];
          x_1 = shelf_x;
          y_1 = v_right;
        }
      }
      else {
        need_1 = stock[shelf_x][v_left];
        x_1 = shelf_x;
        y_1 = v_left;
      }
    }
  }
  else if (v_left < 0 && v_right <= 4) {
    need_1 = stock[shelf_x][v_right];
    x_1 = shelf_x;
    y_1 = v_right;
  }
  else if (v_left >= 0 && v_right > 4) {
    need_1 = stock[shelf_x][v_left];
    x_1 = shelf_x;
    y_1 = v_left;
  }
  
  if (h_up >= 0 && h_down <= 4) {
    if (stock[h_up][shelf_y] != 0 && stock[h_down][shelf_y] != 0) {
      if (stock[h_up][shelf_y] < stock[h_down][shelf_y]) {
        need_2 = stock[h_up][shelf_y];
        x_2 = h_up;
        y_2 = shelf_y;
      }
      else {
        need_2 = stock[h_down][shelf_y];
        x_2 = h_down;
        y_2 = shelf_y;
      }
    }

    else {
      if (stock[h_up][shelf_y] == 0) {
        if (stock[h_down][shelf_y] != 0) {
          need_1 = stock[h_down][shelf_y];
          x_2 = h_down;
          y_2 = shelf_y;
        }
      }
      else {
        need_2 = stock[h_up][shelf_y];
        x_2 = h_up;
        y_2 = shelf_y;
      }
    }
  }
  else if (h_up < 0 && h_down <= 4) {
    need_2 = stock[h_down][shelf_y];
    x_2 = h_down;
    y_2 = shelf_y;
  }
  else if (h_up >= 0 && h_down > 4) {
    need_2 = stock[h_up][shelf_y];
    x_2 = h_up;
    y_2 = shelf_y;
  }
  if (need_1 > 0 && need_2 > 0) {
    if (need_1 < need_2) {
      need = need_1;
      x = x_1;
      y = y_1;
    }
    else {
      need = need_2;
      x = x_2;
      y = y_2;
    }
  }
  if (need_1 == 0) {
    need = need_2;
    x = x_2;
    y = y_2;
  }
  if (need_2 == 0) {
    need = need_1;
    x = x_1;
    y = y_1;
  }
  lengtharray = need + 1;
  
  int way[lengtharray]; 

  if (x < shelf_x) {
    way[lengtharray - 1] = 1;
  }
  if (x > shelf_x) {
    way[lengtharray - 1] = 2;
  }
  if (y < shelf_y) {
    way[lengtharray - 1] = 3;
  }
  if (y > shelf_y) {
    way[lengtharray - 1] = 4;
  }
  lengtharray = lengtharray - 1;
  m = need - 1;

  hint = m;
 
    for (i = 0; i < hint; ++i) {

    h_up = x - 1;
    h_down = x + 1;
    v_left = y - 1;
    v_right = y + 1;
  
    if (h_up >= 0) {

      if (stock[h_up][y] == m) {  
        way[lengtharray - 1] = 1;
        x = h_up;
        y = y;
      }
    }
    if (h_down <= 4) {
      if (stock[h_down][y] == m) {
        way[lengtharray - 1] = 2;
        x = h_down;
        y = y;
      }
    }
    if (v_left >= 0) {
      if (stock[x][v_left] == m) {
        way[lengtharray - 1] = 3;
        x = x;
        y = v_left;
      }
    }
    if (v_right <= 4) {
      if (stock[x][v_right] == m) {

        way[lengtharray - 1] = 4;
        x = x;
        y = v_right;
      }
    }
    m = m - 1;
    lengtharray = lengtharray - 1;
  }
  h_up = x - 1;
  h_down = x + 1;
  v_left = y - 1;
  v_right = y + 1;

  if (h_up >= 0) {
    if (stock[h_up][y] == stock[0][4]) {
      way[lengtharray-1] = 1;
    }
  }
  if (h_down <= 4) {
    if (stock[h_down][y] == stock[0][4]) {
      way[lengtharray-1] = 2;
    }
  }
  if (v_left >= 0) {
    if (stock[x][v_left] == stock[0][4]) {
      way[lengtharray-1] = 3;
    }
  }
  if (v_right <= 4) {
    if (stock[x][v_right] == stock[0][4]) {
      way[lengtharray-1] = 4;
    }
  }
  lengtharray = need + 1;

  //for (i = 0; i < lengtharray; ++i) {
  //  Serial.println(way[i]);
  //} 

if(direction_r == 1){
  if(way[0]==1){
    forward();
    
  }
  if(way[0]==2){
    right();
    right();
    forward();
  }
  if(way[0]==3){
    left();
    forward();
  }
  if(way[0]==4){
    right();
    forward();
}
}
if(direction_r == 2){
  if(way[0]==1){
    left();
    left();
    forward();
}
  if(way[0]==2){
    forward();
}
  if(way[0]==3){
    right();
    forward();
}
  if(way[0]==4){
    left();
    forward();
}
}
if(direction_r == 3){
  if(way[0]==1){
    right();
    forward();
}
  if(way[0]==2){
    left();
    forward();
}
  if(way[0]==3){
    forward();
  }
  if(way[0]==4){
    left();
    left();
    forward();
}
}
if(direction_r == 4){
  if(way[0]==1){
    left();
    forward();
}
  if(way[0]==2){
    right();
    forward();
}
  if(way[0]==3){
    left();
    left();
    forward();
}
  if(way[0]==4){
    forward();
}
}


for (i=1; i<lengtharray; ++i){
  if (way[i-1] == 1){
    if(way[i]==1){
      forward();
    }
    if(way[i]==2){
      left();
      left();
      forward();
    }
    if(way[i]==3){
      left();
      forward();
    }
    if(way[i]==4){
      right();
      forward();
    }
}

  if (way[i-1] == 2){
    if(way[i]==1){
      left();
      left();
      forward();
    }
    if(way[i]==2){
      forward();
    }
    if(way[i]==3){
      right();
      forward();
    }
    if(way[i]==4){
      left();
      forward();
    }
}

  if (way[i-1] == 3){
    if(way[i]==1){
      right();
      forward();
    }
    if(way[i]==2){
      left();
      forward();
    }
    if(way[i]==3){
      forward();
    }
    if(way[i]==4){
      left();
      left();
      forward();
    }
}

  if (way[i-1] ==4){
    if(way[i]==1){
      left();
      forward();
    }
    if(way[i]==2){
      right();
      forward();
    }
    if(way[i]==3){
      left();
      left();
      forward();
    }
    if(way[i]==4){
      forward();
    }
}
}

  
  while (true) {};
}
