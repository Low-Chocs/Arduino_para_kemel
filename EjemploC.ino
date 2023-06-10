#include "LedControl.h"

#define DIN    7
#define CLK    6
#define LOAD   5

LedControl matriz = LedControl(DIN,CLK,LOAD,1);

void setup() {
    Serial.begin(9600); 
    randomSeed(analogRead(0)); 

    matriz.shutdown(0,false); 
    matriz.setIntensity(0,8); 
    for(int i = 14; i <= 21; i++)
        pinMode(i, OUTPUT);
    for(int i = 22; i <= 29; i++)
        pinMode(i, OUTPUT);
    for(int i = 14; i <= 21; i++)
        digitalWrite(i, HIGH);
    for(int i = 22; i <= 29; i++)
        digitalWrite(i, LOW);
    
}

void pintarLED(int x, int y) {
    digitalWrite(14  + y, LOW);
    digitalWrite(22 + x, HIGH);
    delayMicroseconds(1100);
    digitalWrite(14  + y, HIGH);
    digitalWrite(22 + x, LOW);
}


byte buffer[8][16] = { { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, \
                       { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, \
                       { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, \
                       { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, \
                       { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, \
                       { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, \
                       { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, \
                       { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } };

int xAvion = 0;
int yAvion = 0;
void pintarAvion() {
//    buffer[yAvion][xAvion+2]   = 1;
//    buffer[yAvion+1][xAvion+2] = 1;
//    buffer[yAvion+1][xAvion+1] = 1;
//    buffer[yAvion+1][xAvion]   = 1;
//
    buffer[yAvion][xAvion]     = 1;
    buffer[yAvion+1][xAvion]   = 1;
    buffer[yAvion+1][xAvion+1] = 1;
    buffer[yAvion+1][xAvion+2] = 1;
}

void borrarAvion() {
//    buffer[yAvion][xAvion+2]   = 0;
//    buffer[yAvion+1][xAvion+2] = 0;
//    buffer[yAvion+1][xAvion+1] = 0;
//    buffer[yAvion+1][xAvion]   = 0;
    buffer[yAvion][xAvion]     = 0;
    buffer[yAvion+1][xAvion]   = 0;
    buffer[yAvion+1][xAvion+1] = 0;
    buffer[yAvion+1][xAvion+2] = 0;
}

void mostrarMatriz() {
    // Con driver
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            matriz.setLed(0, i, j, buffer[i][j] == 1 ? true : false);
    // Sin driver
    for (int k = 0; k < 3; k++)
        for (int i = 0; i < 8; i++)
            for (int j = 8; j < 16; j++)
                if (buffer[i][j] == 1)
                    pintarLED(j - 8, i);
                else
                    delayMicroseconds(50);
}

void color_floor(short actual_pos, short quantity) {
    for (int i = 0; i < quantity; i++) {
        buffer[7][actual_pos + i] = 1;
    }

}

void show_matrix() {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 16; j++) {

            if (j == 15) {

            }
        }
    }

}

void destroy_buildings(short pos_y) {
    //Evitamos que agarre un punto inexistente
    if (buffer[7][pos_y] == 0) {
        return;
    }
    //Verificamos ambos extremos para hacer la verificación de un solo lado
    bool verification = true;
    short counter = 0;

    //Para destruir los edificios que estan ubicados en la primera posición
    if (pos_y == 0) {
        while (verification) {
            for (short i = 4; i < 8; i++) {
                buffer[i][pos_y] = 0;
            }
            pos_y++;
            if (buffer[7][pos_y] == 0) {
                verification = false;
                return;
            }
        }   
    }
    //Para destruir los edificios que estan ubicados en la ultima posición
    if (pos_y == 15) {
        while (verification) {
            for (short i = 4; i < 8; i++) {
                buffer[i][pos_y] = 0;
            }
            pos_y--;
            if (buffer[7][pos_y] == 0) {
                verification = false;
                return;
            }
        }
    }
    //Cuando no este en una esquina, comenzamos viendo el lado izquierdo
    short pos_y_left_temp = pos_y;
    while (verification) {
        for (short i = 4; i < 8; i++) {
            buffer[i][pos_y_left_temp] = 0;
        }
        pos_y_left_temp--;
        if (pos_y_left_temp == 0 && buffer[7][pos_y_left_temp] == 1) {
            for (short i = 4; i < 8; i++) {
                buffer[i][pos_y_left_temp] = 0;
            }
            verification = false;
            break;
        }
        if (buffer[7][pos_y_left_temp] == 0) {
            verification = false;
            break;
        }
    }
    //Cuando no este en una esquina, continuamos con lado derecho
    verification = true;
    while (verification) {
        pos_y++;
        if (pos_y_left_temp == 15 && buffer[7][pos_y] == 1) {
            for (short i = 4; i < 8; i++) {
                buffer[i][pos_y] = 0;
            }
            verification = false;
            break;
        }
        if (buffer[7][pos_y] == 0) {
            verification = false;
            break;
        }
        for (short i = 4; i < 8; i++) {
            buffer[i][pos_y] = 0;
        }
    }


}

void check_matrix() {
    short minimo_altura = 2;
    short maximo_altura = 5;
    short numeroAleatorio = 0;

    for (int j = 0; j < 16; j++) {
        if (buffer[7][j] == 1) {
            numeroAleatorio = random(minimo_altura, maximo_altura); 
            for (int aleatorio = 0; aleatorio < numeroAleatorio; aleatorio++) {
                buffer[7 - aleatorio][j] = 1;
            }
        }
    }
}

//Genera los edificios correspondientes
void new_game() {
  for(int i = 0; i < 8; i++){
    for(int j = 0; j < 16; j++){
      buffer[i][j] = 0;
    }
  }

    short minimo_base = 1;
    short maximo_base = 4;
    short espacio_inicial = 0;
    short minimo_espacio = 1;
    short maximo_espacio = 3;
    short numeroAleatorio = 0;
    short max_space = 15;


    bool complete = true;
    short counter = 0;
  
    /* Por si se necesita testear la columna 15
    for (short hola = 0; hola < 4; hola++) {
        spaces[7 - hola][15] = true;
        spaces[7 - hola][14] = true;
    }   
    */
    do {

        //Primer espacio
        if (counter == 0) {
            //Looking for first space
            numeroAleatorio = random(espacio_inicial, maximo_espacio); 
            counter += numeroAleatorio;
        }
        else {
            numeroAleatorio = random(minimo_espacio, maximo_espacio); 
            counter += numeroAleatorio;
        }

        if ((counter + maximo_base) > 16) {
            complete = false;
            break;
        }
        //Looking for fist base space
        numeroAleatorio = random( minimo_base, maximo_base); 
        counter += numeroAleatorio;
        color_floor(counter - numeroAleatorio, numeroAleatorio);
    } while (complete);

    check_matrix();

}


//COMIENZA AQUI
//Este es el metodo que se usa para crear un nuevo nivel de juego
void new_buildings(int level){
   short max_space = 16;
   short min_space = 0;
   short base_randomizer = 0;
   short height_randomizer = 0;
   short min_height = 1;
   short max_height = 5;
   for(short i = 0; i < level; i++){
    bool check_space = true;
    do{ 
      base_randomizer = random(min_space, max_space);
      if(buffer[7][base_randomizer] == 0){
        buffer[7][base_randomizer] = 1;
        check_space = false;
        height_randomizer = random(min_height, max_height);
        new_height(base_randomizer, height_randomizer);
        break;
      }
    }while(check_space);
    
   }
}
//Para eliminar edificio
void erradicate_building(short next_pos_x){
  for(short i = 0; i < 4; i++){
    buffer[7 - i][next_pos_x] = 0;
  }
}
//Para ponerle altura al edificio
void new_height(short next_pos_x, short height){
  for(short i = 0; i < height; i++){
    buffer[7 - i][next_pos_x] = 1;
  }
}
//Para terminar una partida
bool check_game_status(){
  for(int i = 0; i < 16; i++){
    if(buffer[7][i] == 1){
      return false;
    }
  }
  return true;
}

long int t0 = 0;
long int t1 = 0;
int prueba = 0;
int nuevo = 0;
short prueba2 = 1;
void loop() {
    // borrarAvion();
    // t1 = millis();
    // if ((t1 - t0) >= 150) {
    //     t0 = millis();
    //     xAvion++;
    // }
    // pintarAvion();


   if(check_game_status()){
      new_buildings(prueba2);
      prueba2++;
      prueba = 0;
      nuevo = 0;
   }
    prueba++;
    if(prueba == 100){
      prueba = 0;
      nuevo = 0;
    }
    if(prueba % 6 == 0){
      erradicate_building(nuevo);
      //destroy_buildings(nuevo);
      nuevo++;
    }
    mostrarMatriz();
     
}
