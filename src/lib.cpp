#include "lib.hpp"
#include "utilities.hpp"

//#include <bitset>
//std::cout << std::bitset<8>(token) << std::endl;


void create_board(const size_t char_capacity, const size_t used_tokens, unsigned char* board) {
  
  std::uniform_int_distribution<size_t> dist(0, 5);

  //se llenan todos los bytes de tokens invalidos
  for (size_t index = 0; index < char_capacity; ++index) {
    board[index] = 0xFF;
  }

  //inicializacion de variables de control necesarias
  unsigned char bit_iterator = 0, xor_mask;
  size_t char_index = 0, token_iterator = 0;

  while (token_iterator < used_tokens) {
    
    //nos excedimos mas alla del byte actual?
    if (bit_iterator >= 8) {
      ++char_index; //pasamos al siguiente byte
      bit_iterator -= 8; //queda solo el desfase
                         
      if (bit_iterator > 0) { //si el token quedo incompleto, se termina de formar en el otro byte
        board[char_index] &= ~(and_mask << (token_size-bit_iterator)); 
        board[char_index] ^= (xor_mask << (token_size-bit_iterator));
        ++token_iterator;
      }
    } 
    //todavia faltan tokens?
    if (token_iterator < used_tokens) {
      board[char_index] &= ~(and_mask >> bit_iterator);
      xor_mask = token_masks[dist(gen)];
      board[char_index] ^= (xor_mask >> bit_iterator);

      bit_iterator += 3; //se agregan los bits sensados
      if (bit_iterator <= 8) { //si el token se escribio completo, se cuenta
        ++token_iterator;
      }
    }
  }
}

void print_token(unsigned char token) {
  switch(token) {
    case 0b0:
      std::cout << RED BOLD << ' ' << token1 << ' ' << RESET;
      break;
    case 0b1:
      std::cout << GREEN BOLD << ' ' << token2 << ' ' << RESET;
      break;
    case 0b10:
      std::cout << YELLOW BOLD << ' ' << token3 << ' ' << RESET;
      break;
    case 0b11:
      std::cout << BLUE BOLD << ' ' << token4 << ' ' << RESET;
      break;
    case 0b100:
      std::cout << MAGENTA BOLD << ' ' << token5 << ' ' << RESET;
      break;
    case 0b101:
      std::cout << CYAN BOLD << ' ' << token6 << ' ' << RESET;
      break;
    default:
      std::cout << " E ";
      break;
  }
}

void print_board(const size_t cols, const size_t char_capacity, const size_t used_tokens, const unsigned char* board) {

  unsigned char bit_iterator = 0, token = 0;
  size_t char_index = 0, token_iterator = 0;
  std::cout << "\n";
  while (token_iterator < used_tokens) {
    if (bit_iterator >= 8) {
      ++char_index;
      bit_iterator -= 8;
      if (bit_iterator > 0) {
        token ^= (board[char_index] & (and_mask << (token_size-bit_iterator))) >> (8-bit_iterator);
        print_token(token);
        token = 0;
        ++token_iterator;
        if (token_iterator % cols == 0) std::cout << "\n\n";
      }
    }
    if (token_iterator < used_tokens) {
      token = board[char_index] & (and_mask >> bit_iterator);
      token = ((5-bit_iterator < 0)? token << (bit_iterator-5) : token >> (5-bit_iterator));
      if (bit_iterator <= 5) {
        print_token(token);
        token = 0;
        ++token_iterator;
        if (token_iterator % cols == 0) std::cout << "\n\n";
      }
    }
    bit_iterator += 3;
  }
}

//debugging pendiente [FALTA VALIDACION DE TAMAÑO PARA EMPEQUEÑECER, PARAMETRO DE CAPACIDAD]
size_t delete_row(unsigned char* board, size_t& rows, const size_t cols, const unsigned char selected_row) {
  
  const size_t token_limit = (rows*cols) - (selected_row*cols);
  size_t token_counter = 0;
  
  --rows;
  
  size_t  start_bit  = selected_row*cols*token_size,
          start_char = start_bit/8,
          end_bit    = (selected_row+1)*cols*token_size, 
          end_char   = end_bit/8;

  unsigned char start_bit_iterator = start_bit%8,
                end_bit_iterator   = end_bit%8,

                end_token = 0;

  while (token_counter < token_limit) {

    end_token = (board[end_char] & (and_mask >> end_bit_iterator)) << end_bit_iterator;
    
    end_bit_iterator += 3;
    if (end_bit_iterator >= 8) {
      ++end_char;
      end_bit_iterator -= 8;
      if (end_bit_iterator > 0) {
        end_token ^= (board[end_char] & (and_mask << (token_size-end_bit_iterator))) >> (token_size-end_bit_iterator);
      }
    } 

    board[start_char] &= ~(and_mask >> start_bit_iterator);
    board[start_char] ^= (end_token >> start_bit_iterator);

    start_bit_iterator += 3;

    if (start_bit_iterator >= 8) {
      ++start_char;
      start_bit_iterator -= 8;
      if (start_bit_iterator > 0) {
        board[start_char] &= ~(and_mask << (token_size-start_bit_iterator));
        board[start_char] ^= end_token << (token_size-start_bit_iterator);
        ++token_counter;
      }
      else ++token_counter;
    }
    else ++token_counter;
  }

  return rows*cols;
}

//size_t delete_column(unsigned char* board, size_t& cols, const size_t rows, size_t used_to, const unsigned char selected_col) {}

//############################ PABLO ##########################################################

#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

//estas funciones arman el tablero

int calcularbytesnecesarios(int filas, int columnas){
    int bitstotales = filas * columnas * 3;
    int bytesnecesarios = bitstotales / 8;
    if (bitstotales % 8 != 0) {
        bytesnecesarios = bytesnecesarios + 1;   // redondeamos hacia arriba si sobran bits
    }
    return bytesnecesarios;
}

unsigned char* creartablero(int filas, int columnas){
    int bytes = calcularbytesnecesarios(filas, columnas); //obtener los bytes necesarios para luego reservar memoria
    unsigned char* tablero = new unsigned char[bytes];

    for (int i = 0;i < bytes; i++){
        tablero[i] = 0; //cada byte es igual a 00000000 ya que lo inicializamos en 0 (vacio por asi decirlo)
    }

    return tablero;
}

char simbolo_apartirdebits(unsigned char codebits) //codebits es cada una de las ficha en su forma de bits
{
    switch (codebits)
    {
    case 0: return 'Y';
    case 1: return 'A';
    case 2: return 'K';
    case 3: return 'M';
    case 4: return 'Q';
    case 5: return 'R';
    case 6: return '-';
    case 7: return '*';
    }
}



unsigned char llenarAleatorio(){
    return rand() % 5;
}



unsigned char ver_ficha(unsigned char* tablero, int fila, int columna, int columnas){

    int index = fila * columnas + columna; //el index me dice que ficha es 0,1,2,3,4,5
    int firstbit = index * 3; //me indica la posicion lineal del bit en el que inicia la ficha
    int Byte = firstbit / 8;  //este me dice el byte en el que esta
    int posicionenelbyte = firstbit % 8; //este me dice la posicion en el byte en el que esta la ficha

    if (posicionenelbyte <= 5) //cuando la ficha esta en un solo byte
    {
        int desplazamiento = 8 - posicionenelbyte - 3; //8 es la cantidad de bits en un byte, cuando hago (8 - posicionenelbyte) obtengo el bit menos significativo y cuando resto 3 se obtiene el mas, (todo esto de derecha a izquierda)
        return (tablero[Byte] >> desplazamiento) & 7; //aqui solo basta con mover el bit más a la derecha de la ficha al bit menos significativo de el byte
    }

    else //esta se ocupa para cuando una ficha esta de 2 bytes
    {
        int desplazamiento = 16 - posicionenelbyte - 3; //16 es el numero de bits en 2 bytes,
        unsigned short entre2bytes = (tablero[Byte] << 8) | (tablero[Byte+1]); //conectamos los 2 bytes para que queden de una forma lineal
        return (entre2bytes >> desplazamiento) & 7; //el >> desplazamiento mueve la ficha hasta el bit menos significativo de los 16 bits ya sabemos que la ficha esta ahi entonces aplicamos un and 7 (00000111 en 2 bytes)
    }
}




/* Aqui estan ubicadas las funciones de eliminacion de filas y columna, en el futuro tambien estaran las de agregar filas y columnas*/

void colocarfichaindividual(unsigned char* tablero, int fila, int columna, int columnas, unsigned short valor){

    int index = fila * columnas + columna; //el index me dice que ficha es 0,1,2,3,4,5
    int firstbit = index * 3; //me indica la posicion lineal del bit en el que inicia la ficha
    int Byte = firstbit / 8;  //este me dice el byte en el que esta
    int posicionenelbyte = firstbit % 8; //este me dice la posicion en el byte en el que esta la ficha

    if (posicionenelbyte <= 5) //cuando la ficha esta en un solo byte
    {
        int desplazamiento = (8 - posicionenelbyte - 3);//8 es la cantidad de bits en un byte, cuando hago (8 - posicionenelbyte) obtengo el bit menos significativo y cuando resto 3 se obtiene el mas, (todo esto de derecha a izquierda)
        unsigned char mascara = 7 << desplazamiento; // coloco la mascara justo en los bits a cambiar
        tablero[Byte] =(tablero[Byte] & ~mascara) | (valor << desplazamiento); //aqui solo basta con mover el bit más a la derecha de la ficha al bit menos significativo de el byte
    }

    else //esta se ocupa para cuando una ficha esta de 2 bytes
    {
        int desplazamiento = 16 - posicionenelbyte - 3; //16 es el numero de bits en 2 bytes,
        unsigned short entre2bytes = (tablero[Byte] << 8) | (tablero[Byte+1]); //conectamos los 2 bytes para que queden de una forma lineal
        unsigned short mascara = 7 << desplazamiento; // creo la mascara para apagar todos los bits de la posicion donde ira la ficha
        entre2bytes = (entre2bytes & ~mascara);//(valor << desplazamiento) mueve el valor a cambiar al bit más significativo de la ficha a reemplazar
        valor = valor << desplazamiento; //movemos la ficha a la posicion de bit más significativo de la ficha que vamos a cambiar
        entre2bytes = entre2bytes | valor; //prendemos los valores de la ficha mediante un or
        tablero[Byte] = (entre2bytes>>8);
        tablero[Byte+1] = entre2bytes;
    }

}

void eliminarfila(unsigned char*& tablero, int& filas, int columnas, int fila_eliminar, int& bytesreservados){
    int newrows = filas - 1;
    int filadestino = 0;

    for (int fila = 0; fila < filas; fila++){
        if (fila == fila_eliminar){
            continue;
        }
        for (int columna = 0; columna < columnas; columna++){
            unsigned char ficha_No_eliminada = ver_ficha(tablero, fila, columna, columnas);
            colocarfichaindividual(tablero, filadestino, columna, columnas, ficha_No_eliminada);

        }
        filadestino ++;
    }

    for (int columna = 0; columna < columnas; columna++){
        colocarfichaindividual(tablero, filas - 1, columna, columnas, 6);
    }

    filas = newrows;

    int bytesnecesariosnewtable = calcularbytesnecesarios(newrows, columnas);
    double porcentajeactual = bytesnecesariosnewtable/bytesreservados;

    if (porcentajeactual < 0.65){
        unsigned char* tableronuevo = creartablero(filas, columnas);

        for (int fila = 0; fila < filas; fila++){
            for(int columna = 0; columna < columnas; columna++){
                unsigned char fichaoldtable = ver_ficha(tablero, fila, columna, columnas);
                colocarfichaindividual(tableronuevo, fila, columna, columnas, fichaoldtable);
            }
        }
        delete[] tablero;
        tablero = tableronuevo;
        bytesreservados = bytesnecesariosnewtable;
    }
}


void eliminarcolumna(unsigned char*& tablero, int filas, int& columnas, int columna_eliminar, int& bytesreservados){
    int newcolumns = columnas - 1;


    for (int fila = 0; fila < filas; fila++){
        int columnaDestino = 0; // se escribe toda la columna en forma de lineas C0, C1, C2, C3
        for (int columna = 0; columna < columnas; columna++){
            if (columna == columna_eliminar){
                continue;
            }
            unsigned char ficha_No_eliminada = ver_ficha(tablero, fila, columna, columnas);
            colocarfichaindividual(tablero, fila, columnaDestino, newcolumns, ficha_No_eliminada);
            columnaDestino++;
        }

        colocarfichaindividual(tablero, fila, columnas - 1, columnas, 6);
    }

    columnas = newcolumns;

    int bytesnecesariosnewtable = calcularbytesnecesarios(filas, newcolumns);
    double porcentajeactual = bytesnecesariosnewtable / bytesreservados;

    if (porcentajeactual < 0.65){
        unsigned char* tableronuevo = creartablero(filas, columnas);

        for (int fila = 0; fila < filas; fila++){
            for (int columna = 0; columna < columnas; columna++){
                unsigned char fichaoldtable = ver_ficha(tablero, fila, columna, columnas);
                colocarfichaindividual(tableronuevo, fila, columna, columnas, fichaoldtable);
            }
        }
        delete[] tablero;
        tablero = tableronuevo;
        bytesreservados = bytesnecesariosnewtable;
    }
}

void agregarfila(unsigned char*& tablero,int &filas,int columnas,int posicionfilaadd, int& bytesreservados){
    int newrows = filas + 1;
    unsigned char* tableronuevo = creartablero(newrows, columnas);

    for(int fila = 0; fila < newrows; fila++){
        if (fila < posicionfilaadd){
            for(int columna = 0; columna < columnas; columna++){
                unsigned char fichavieja = ver_ficha(tablero, fila, columna, columnas);
                colocarfichaindividual(tableronuevo,fila,columna,columnas,fichavieja);

            }
        }

        else if (fila == posicionfilaadd){
            for(int columna = 0; columna < columnas;columna++)//esto ya que las columnas todavia tienen la misma dimension
            {
                unsigned char ficha_new_row = llenarAleatorio();
                colocarfichaindividual(tableronuevo,fila,columna,columnas,ficha_new_row);

            }
        }
        else{
            for (int columna = 0; columna < columnas; columna++){
                unsigned char ficha = ver_ficha(tablero, fila - 1, columna, columnas);
                colocarfichaindividual(tableronuevo, fila, columna, columnas, ficha);
            }
        }

    }
    delete[] tablero;
    tablero = tableronuevo;
    filas = newrows;
    bytesreservados = calcularbytesnecesarios(newrows, columnas);

}

//#############################################################################################

//debugging pendiente
size_t upper_replace(unsigned char* board, const size_t cols, size_t initial_bit) {
  size_t initial_byte = initial_bit/8,
         upper_bit    = initial_bit - (cols*token_size),
         next_bit     = upper_bit/8,
         upper_byte   = upper_bit/8;

  initial_bit %= 8;
  upper_bit %= 8;

  unsigned char upper_token = (board[upper_byte] & (and_mask >> upper_bit)) << upper_bit;
  if (upper_bit > 5) {
    ++upper_byte;
    upper_bit -= 5;
    upper_token ^= (board[upper_byte] & (and_mask << (token_size-upper_bit) )) >> (token_size-upper_bit);
  }

  board[initial_byte] &= ~(and_mask >> initial_bit);
  board[initial_byte] ^= upper_token >> initial_bit;

  if (initial_bit > 5) {
    ++initial_byte;
    initial_bit -= 5;
    board[initial_byte] &= ~(and_mask << (token_size-initial_bit));
    board[initial_byte] ^= upper_token << (token_size-initial_bit);
  }
  return next_bit;
}


//debugging pendiente
void cascaded_fall(unsigned char* board, const size_t cols, size_t bit_index) {

  std::uniform_int_distribution<size_t> dist(0, 5);

  size_t first_row_bits = (cols*token_size);
  
  while (bit_index > first_row_bits) {
    bit_index = upper_replace(board, cols, bit_index);
  }

  size_t last_byte = bit_index/8;
  bit_index %= 8;

  unsigned char last_token = token_masks[dist(gen)];

  board[last_byte] &= ~(and_mask >> bit_index);
  board[last_byte] ^= ( last_token >> bit_index);

  if (bit_index > 5) {
    ++last_byte;
    bit_index -= 5;
    board[last_byte] &= ~(and_mask << (token_size-bit_index));
    board[last_byte] ^= last_token << (token_size-bit_index);
  } 
}

  //para combo horizontal:
  //                       estrategia 2.
  //                se recorre hasta la izquierda hasta la ficha mas lejana, horizontal_start pasa a ser el indice global de 
  //                esa ficha.
  //                       
  //                despues, se cuenta cuantas fichas a partir de ella
  //                son de igual valor consecutivamente, el ciclo termina cuando la siguiente no sea del mismo valor o 
  //                termine la fila
  //                si el combo_size es menor a 3, se devuelve 0, si no, se devuelve combo size

size_t horizontal_scanner(const unsigned char* board,
                          const size_t cols,
                          const size_t rows,
                          size_t& combo_count,
                          size_t h_start,
                          const unsigned char original_token) {
//-------

  const size_t limit = 0; //CAMBIAR
  size_t aux_index = h_start;
  size_t bit_iterator = h_start%8;

  

  return h_start;
}

//en proceso
size_t vertical_scanner(const unsigned char* board,
                        const size_t cols,
                        const size_t rows,
                        size_t& combo_count,
                        size_t v_start,
                        const unsigned char original_token) {
//-------

  return v_start;
}

//en proceso
void combo_scanner(unsigned char* board, const size_t cols, const size_t rows, const size_t c_bit_index) {

  //unsigned int center_byte = central_bit_index /8;
  //unsigned int center_bit = central_bit_index %8;

  size_t h_start          = c_bit_index,
         v_start          = c_bit_index,
         v_combo_count    = 0,
         h_combo_count    = 0;

  unsigned char c_token = get_token(board, c_bit_index);

  v_start = vertical_scanner(cols, rows, v_start, v_combo_count, c_token);

  h_start = horizontal_scanner(cols, rows, h_start, h_combo_count, c_token);
  
  if (!(h_combo_count || v_combo_cout)) return;

  if (h_combo_count) {
    if (v_combo_count) --v_combo_count;
    size_t token_counter = 0;
    size_t bit_index = horizontal_start;
    while (token_counter < h_combo_count) {
      cascaded_fall(board, cols, bit_index);
      bit_index += 3;
      ++token_counter;
    }
  }

  size_t token_counter = 0;
  while (token_counter < vertical_combo_size) {
    cascaded_fall(board, cols, vertical_start);
    ++token_counter;
  }

  const size_t scan_limit  = (v_start >= (h_start + (h_combo_count*token_size)))? v_start : 
                                                                                  h_start + (h_combo_count*token_size);

  for (size_t bit_index = 0; bit_index < scan_limit ; bit_index += 3) {
    combo_scanner(board, cols, rows, bit_index);
  }
}

  
  //para combo horizontal:
  //                       estrategia 2.
  //                se recorre hasta la izquierda hasta la ficha mas lejana, horizontal_start pasa a ser el indice global de 
  //                esa ficha.
  //                       
  //                despues, se cuenta cuantas fichas a partir de ella
  //                son de igual valor consecutivamente, el ciclo termina cuando la siguiente no sea del mismo valor o 
  //                termine la fila
  //                si el combo_size es menor a 3, se devuelve 0, si no, se devuelve combo size
  //
  //para combo vertical:
  //                       estrategia 2. 
  //                  se hace vertical_scanner al principio de todo.
  //
  //                  desde la posicion original, se va bajando hasta encontrar la ultima ficha del mismo valor, con cuidado
  //                  de no pasarse mas alla del tablero, el indice de esa ficha pasa a ser vertical_start, a partir de esa posicion
  //                  se va subiendo a la siguiente fila, contando cuantas fichas con consecutivamente iguales a la original
  //
  //                  si el combo_size es menor a 3, se devuelve 0, si no, se devuelve combo size
  //
  //despues:               despues, se empieza a hacer combo_scanner recursivamente
  //
  //para recursive combo_scanner:
  //  
  //                       estrategia 1.
  //                       se detecta que columnas salieron afectadas, y la maxima profundidad de afectacion, se aplica combo_scanner
  //                       de arriba hacia abajo en todas las fichas afectadas en un patron de [derecha, abajo, derecha, abajo]
  //                       se deja de escanear una columna cuando la ficha afectada en ella ya es la mas profunda
  //
  //                       estrategia 2. [mas simple]
  //                  si hubo vertical, se hace combo_scanner hasta ese indice global,
  //                  si no, se hace hasta horizontal_start + (horizontal_size*token_size)
  //


//pendiente
void create_valid_board() {

}

//a la creacion inicial del tablero, se tiene que crear y despues modificar hasta que ya no quede ningun combo de fichas, esa primera funcion de sensado debe de estar
//incorporada en una funcion grande que contenga combo_scanner y create_board hasta que quede uno valido

