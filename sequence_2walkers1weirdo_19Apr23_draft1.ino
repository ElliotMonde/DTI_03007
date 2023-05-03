/*
   Scripted by Elliot Phua 2023, for DTI 03.007
   https://github.com/ElliotMonde
*/
#include "FastLED.h"

#define NUM_LEDS 96
#define LED_PIN 7
#define COLOR_ORDER RGB
CRGB leds[NUM_LEDS];
//
//  if (Serial.available() > 0) {
//    char c = Serial.read();
//    if (c == 'A') {
//      digitalWrite(13, HIGH);
//    }
//    else if (c == 'B') {
//      digitalWrite(13, LOW);
//    }
//  }


int colMatrix [12][8] = {0};
//////////////////////////////////LED series to 2d array matrix///////////////////////////////////////////
// assign each led series' element id to its index position in a 2d array matrix
void toMatrix(int totalLED, int newArr[][8])
{
  for (int i = 0; i < totalLED; i++)
  { // for each Leds..
    int r = i / 8;
    if (r % 2)
    { // if odd row
      newArr[r][i - r * 8] = i; // col starts from left if even row
    }
    else
    {
      newArr[r][8 - (i - r * 8) - 1] = i; // col starts from right if odd row
    }
  }
}

// for debugging; prints 2d matrix with each element's corresponding led id
void printMatrix(int newArr[12][8])
{
  for (int i = 0; i < 12; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      int id = newArr[i][j];
      Serial.print("[");
      Serial.print(id);
      Serial.print("]");
    }
    Serial.println();
  }
}
void printMatrix(bool newArr[12][8])
{
  for (int i = 0; i < 12; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      int id = newArr[i][j];
      Serial.print("[");
      Serial.print(id);
      Serial.print("]");
    }
    Serial.println();
  }
}

int totalLED = NUM_LEDS;
int ledArr[12][8] = {0};
//////////////////////declare struct user class////////////////////////////////////////
int idCounter = 1; // starts from 1 as lightUpLeds function uses userId and is boolean
// user class containing properties of each user
struct user {
  int row;
  int col;
  bool userMatrix[12][8];
  int color[3];
  int userId;
};
///////////////////////////////Declare functions////////////////////////////////////////////
// create diamond pattern -> userMatrix element to true
void makeDiamond(int userRow, int userCol, bool userMatrix[12][8]) {
  int max_r = 11, max_c = 7;

  // reset userMatrix;
  for (int i = 0; i <= max_r; i++) {
    for (int j = 0; j <= max_c; j++) {
      userMatrix[i][j] = false;
    }
  }
  int counter = 0;
  int k = 0;
  for (int i = userRow - 3; i <= userRow + 3; i++) {
    if (i >= 0 && i <= max_r) {
      for (int j = userCol - counter; j <= userCol + counter; j++) {
        if (j >= 0 && j <= max_c) {
          userMatrix[i][j] = true;
        }
      }
    }
    if (k < 3) {
      k++;
      counter += 1;
    } else {
      k++;
      counter -= 1 ;
    }
  }
}



// to update colMatrix, which contains boolean of overlap and whether the LED at index should light up
void updateColMatrix(bool userMatrix[12][8], int colMatrix[12][8], int userId) {
  for (int i = 0; i < 12; i++) { // for each row of userMatrix
    for (int j = 0; j < 8; j++) { // for each col of userMatrix
      if (userMatrix[i][j]) {
        if (colMatrix[i][j]) {

          if (colMatrix[i][j] + userId == 3) {
            colMatrix[i][j] = 30;
          }
          if (colMatrix[i][j] + userId == 4) {
            colMatrix[i][j] = 40;
          }
          if (colMatrix[i][j] + userId == 5) {
            colMatrix[i][j] = 50;
          }
        } else {
          colMatrix[i][j] += userId;
        }

      }
    }
  }
}

// put each userMatrix thru the updateColMatrix  function
void allUsersToColMatrix(struct user* userArr[5], int colMatrix[12][8]) {
  for (int i = 0; i <= 3; i++) { // limit of i is 3 since there are only 2 actual users
    updateColMatrix((*userArr[i]).userMatrix, colMatrix, (*userArr[i]).userId);
  }
}

// iterate thru each colMatrix's elements to light up each LED pixel that is true
void lightUpLeds(int colMatrix[12][8], struct user* userArr[5]) {
  int r, g, b;
  for (int i = 0; i < 12; i++) {
    for (int j = 0; j < 8; j++) {
      switch (colMatrix[i][j]) { // think of a faster way than switch-case, maybe a nested if-else..?
        case 0:
          leds[ledArr[i][j]] = CRGB::Black; // unlit pixel if colMatrix element is false
          break;
        case 1:
          leds[ledArr[i][j]] = CRGB((*userArr[0]).color[0], (*userArr[0]).color[1], (*userArr[0]).color[2]);
          break;
        case 2:
          leds[ledArr[i][j]] = CRGB((*userArr[1]).color[0], (*userArr[1]).color[1], (*userArr[1]).color[2]);
          break;
        case 3:
          leds[ledArr[i][j]] = CRGB((*userArr[2]).color[0], (*userArr[2]).color[1], (*userArr[2]).color[2]);
        case 30:
          //          r = ((*userArr[0]).color[0] + (*userArr[1]).color[0]) / 3;
          //          g = ((*userArr[0]).color[1] + (*userArr[1]).color[1]) / 2;
          //          b = ((*userArr[0]).color[2] + (*userArr[1]).color[2]) / 2;
          r = 0;
          g = 255;
          b = 0;
          leds[ledArr[i][j]] = CRGB(r, g, b);
          break;
        case 40:
          //          r = ((*userArr[0]).color[0] + (*userArr[2]).color[0]) / 2;
          //          g = ((*userArr[0]).color[1] + (*userArr[2]).color[1]) / 3;
          //          b = ((*userArr[0]).color[2] + (*userArr[2]).color[2]) / 2;
          r = 255;
          g = 0;
          b = 0;
          leds[ledArr[i][j]] = CRGB(r, g, b);
          break;
        case 50:
          //          r = ((*userArr[1]).color[0] + (*userArr[2]).color[0]) / 3;
          //          g = ((*userArr[1]).color[1] + (*userArr[2]).color[1]) / 3;
          //          b = ((*userArr[1]).color[2] + (*userArr[2]).color[2]) / 2;
          r = 0;
          g = 0;
          b = 255;
          leds[ledArr[i][j]] = CRGB(r, g, b);
          break;
        default:
          leds[ledArr[i][j]] = CRGB(255, 0, 0); // yellow color for any cases above 3
          break;
      }
    }
    FastLED.show();
  }
}

int userCounterUpdate(int userCounter) {
  if (userCounter < 12) { // increment sequence index, change the counter max if the number of items in the sequence changes
    userCounter += 1;
    return userCounter;
  }
  userCounter = 0;
  return userCounter; // reset sequence
}
///////////////////////// array to hold all users; instantiate users//////////////////////////


// first user initialisation : amy
int amySeqCounter = 0;
/*
    change the sequence by including elements with {row, column} components,
    change the sequence array size if sequence has more than 12 items,
    remember to also change the sequence counter max in userCounterUpdate function
*/

/*
   {{0, 2}, {1, 2}, {2, 2}, {3, 2}, {4, 2}, {5, 2}, {6, 2}, {7, 2}, {8, 2}, {9, 2}, {10, 2}, {11, 2}}
   {{0, 3}, {1, 3}, {2, 3}, {3, 3}, {4, 3}, {5, 3}, {6, 3}, {7, 3}, {8, 3}, {9, 3}, {10, 3}, {11, 3}}
   {{0, 4}, {1, 4}, {2, 4}, {3, 4}, {4, 4}, {5, 4}, {6, 4}, {7, 4}, {8, 4}, {9, 4}, {10, 4}, {11, 4}}
   {{11, 6}, {10, 6}, {9, 6}, {8, 6}, {7, 6}, {6, 6}, {5, 6}, {4, 6}, {3, 6}, {2, 6}, {1, 6}, {0, 6}}
   {{11, 5}, {10, 5}, {9, 5}, {8, 5}, {7, 5}, {6, 5}, {5, 5}, {4, 5}, {3, 5}, {2, 5}, {1, 5}, {0, 5}}
   {{11, 7}, {10, 7}, {9, 7}, {8, 7}, {7, 7}, {6, 7}, {5, 7}, {4, 7}, {3, 7}, {2, 7}, {1, 7}, {0, 7}};
*/
int amySeq[12][2] = {{0, 2}, {1, 2}, {2, 2}, {3, 2}, {4, 2}, {5, 2}, {6, 2}, {7, 2}, {8, 2}, {9, 2}, {10, 2}, {11, 2}};
struct user amy = {
  amySeq[amySeqCounter][0], // amy's curr row
  amySeq[amySeqCounter][1], // amy's curr col
  {0}, // amy's curr diamond
  {255, 102, 255}, // amy's color is pink
  idCounter++, // assign userId, then increment idCounter by 1 after
};
// secound user initialisation: coco
int cocoSeqCounter = 0;
int cocoSeq[12][2] = {{11, 5}, {10, 5}, {9, 5}, {8, 5}, {7, 5}, {6, 5}, {5, 5}, {4, 5}, {3, 5}, {2, 5}, {1, 5}, {0, 5}};
struct user coco = {
  cocoSeq[cocoSeqCounter][0], // coco's curr row
  cocoSeq[cocoSeqCounter][1], // coco's curr col
  {0}, // coco's initial diamond (haven't initialise row & col yet so cannot initialise diamond)
  {0, 153, 255}, // coco's color is blue
  idCounter++,
};

int blaireSeqCounter = 0;
int blaireSeq[12][12] = {{6, 4}, {6, 4}, {6, 4}, {6, 4}, {6, 4}, {6, 4}, {6, 4}, {6, 4}, {6, 4}, {6, 4}, {6, 4}, {6, 4}};
struct user blaire = {
  blaireSeq[blaireSeqCounter][0], // blaire's curr row
  blaireSeq[blaireSeqCounter][1], // blaire's curr col
  {0}, // blaire's initial diamond matrix, haven't init
  {255, 150, 0}, // orange color (G,R, B)
  idCounter++,
};

struct user* amy_ptr = &amy;
struct user* coco_ptr = &coco;
struct user* blaire_ptr = &blaire;
struct user* userArr[5] = {amy_ptr, coco_ptr, blaire_ptr};

//////////////////////////detect keypress function//////////////////////////

void onKeyUpdateCoordinates(struct user person) {
  // switch-case for 3 persons
  int up, down, left, right;

  switch (person.userId) {
    case 0:
      up = 'w' ;
      down = 's';
      left = 'a';
      right = 'd';
      break;
    case 1:
      up = 'i';
      down = 'k';
      left = 'j';
      right = 'l';
      break;
    default:
      up = '8' ;
      down = '5';
      left = '4';
      right = '6';
  }
  if (Serial.available() > 0) {
    if (Serial.read() == up) {
      person.row = (person.row + 1 <= 11) ? person.row + 1 : 0;
    } else if (Serial.read() == down) {
      person.row = (person.row - 1 >= 0) ? person.row - 1 : 11;
    } else if (Serial.read() == left) {
      person.col = (person.col - 1 >= 0) ? person.col - 1 : 7;
    } else if (Serial.read() == right) {
      person.col = (person.col + 1 <= 7) ? person.col + 1 : 0;
    }
  }
}

//////////////////////////main & loop function//////////////////////////////

void setup() {
  // input and output set up
  Serial.begin(9600);
  FastLED.addLeds<WS2812, LED_PIN, RGB>(leds, NUM_LEDS);
  FastLED.setBrightness(100);
  pinMode(2, INPUT);
  pinMode(7, OUTPUT);
  toMatrix(totalLED, ledArr);

  for (int i = 0; i < 96; i++) {
    leds[i] = CRGB(102, 255, 102);
    FastLED.show();
  };
  for (int j = 95; j >= 0; j--) {
    leds[j] = CRGB::Black;
    FastLED.show();
  }
  // get users' Matrices
  makeDiamond(amy.row, amy.col, amy.userMatrix);
  makeDiamond(coco.row, coco.col, coco.userMatrix);
  // add users' to userArr
}

void loop() {
  //update all users' current position at start of each iteration
  // update amy's position with each iteration of loop
  amy.row = amySeq[amySeqCounter][0]; // amy's curr row
  amy.col = amySeq[amySeqCounter][1]; // amy's curr col

  blaire.row = blaireSeq[blaireSeqCounter][0];
  blaire.col = blaireSeq[blaireSeqCounter][1];

  coco.row = cocoSeq[cocoSeqCounter][0];
  coco.col = cocoSeq[cocoSeqCounter][1];

  makeDiamond(amy.row, amy.col, amy.userMatrix);
  makeDiamond(coco.row, coco.col, coco.userMatrix);
  makeDiamond(blaire.row, blaire.col, blaire.userMatrix);

  for (int i = 0; i < 12; i++) {
    for (int j = 0; j < 8; j++) {
      colMatrix[i][j] = 0;
    }
  }

  if (Serial.available() > 0) {
    char c = Serial.read();
    if (c == 'a') {
      Serial.println(c);
    }
    else if (c == 'b') {
      leds[0] = CRGB(13, 0, 255); // turn on the LED
      FastLED.show();
    }
  }

  allUsersToColMatrix(userArr, colMatrix);
  lightUpLeds(colMatrix, userArr);
  //update all users' sequence counter
  amySeqCounter = userCounterUpdate(amySeqCounter);
  cocoSeqCounter = userCounterUpdate(cocoSeqCounter);
  blaireSeqCounter = userCounterUpdate(blaireSeqCounter);
  // small pause between each iteration
  delay(1200);
}
