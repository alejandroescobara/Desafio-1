#include "lib.hpp"
#include "utilities.hpp"

#include <ctime>

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

void print_bit_token(unsigned char token) {
  switch(token) {
    case 0b0:
      std::cout << RED BOLD << ' ' << "000" << ' ' << RESET;
      break;
    case 0b1:
      std::cout << GREEN BOLD << ' ' << "001" << ' ' << RESET;
      break;
    case 0b10:
      std::cout << YELLOW BOLD << ' ' << "010" << ' ' << RESET;
      break;
    case 0b11:
      std::cout << BLUE BOLD << ' ' << "011" << ' ' << RESET;
      break;
    case 0b100:
      std::cout << MAGENTA BOLD << ' ' << "100" << ' ' << RESET;
      break;
    case 0b101:
      std::cout << CYAN BOLD << ' ' << "101" << ' ' << RESET;
      break;
    default:
      std::cout << " E ";
      break;
  }
}

void print_board(const size_t cols, const size_t used_tokens, const unsigned char* board) {

  unsigned char bit_iterator = 0, token = 0;
  size_t char_index = 0, token_iterator = 0;
  std::cout << "\n ";
  for (size_t i = 0; i < cols; ++i) {
    std::cout << ' '<< i;
    if (i < 10) std::cout << ' '; 
  }
  std::cout << " X\n\n ";
  
  size_t row_counter = 0; 
  while (token_iterator < used_tokens) {
    if (bit_iterator >= 8) {
      ++char_index;
      bit_iterator -= 8;
      if (bit_iterator > 0) {
        token ^= (board[char_index] & (and_mask << (token_size-bit_iterator))) >> (8-bit_iterator);
        print_token(token);
        token = 0;
        ++token_iterator;
        if (token_iterator % cols == 0) {
          std::cout << ' ' << row_counter << "\n\n ";
          ++row_counter;
        }
      }
    }
    if (token_iterator < used_tokens) {
      token = board[char_index] & (and_mask >> bit_iterator);
      token = ((5-bit_iterator < 0)? token << (bit_iterator-5) : token >> (5-bit_iterator));
      if (bit_iterator <= 5) {
        print_token(token);
        token = 0;
        ++token_iterator;
        if (token_iterator % cols == 0) {
          std::cout << ' ' << row_counter << "\n\n ";
          ++row_counter;
        }
      }
    }
    bit_iterator += 3;
  }
}


void print_bit_board(const size_t cols, const size_t used_tokens, const unsigned char* board) {

  unsigned char bit_iterator = 0, token = 0;
  size_t char_index = 0, token_iterator = 0;
  std::cout << "\n ";
  for (size_t i = 0; i < cols; ++i) std::cout << ' '<< i << "   ";
  std::cout << " X\n\n ";
  
  size_t row_counter = 0; 
  while (token_iterator < used_tokens) {
    if (bit_iterator >= 8) {
      ++char_index;
      bit_iterator -= 8;
      if (bit_iterator > 0) {
        token ^= (board[char_index] & (and_mask << (token_size-bit_iterator))) >> (8-bit_iterator);
        print_bit_token(token);
        token = 0;
        ++token_iterator;
        if (token_iterator % cols == 0) {
          std::cout << ' ' << row_counter << "\n\n\n ";
          ++row_counter;
        }
      }
    }

    if (token_iterator < used_tokens) {
      token = board[char_index] & (and_mask >> bit_iterator);
      token = ((5-bit_iterator < 0)? token << (bit_iterator-5) : token >> (5-bit_iterator));
      if (bit_iterator <= 5) {
        print_bit_token(token);
        token = 0;
        ++token_iterator;
        if (token_iterator % cols == 0) {
          std::cout << ' ' << row_counter << "\n\n\n ";
          ++row_counter;
        }
      }
    }
    bit_iterator += 3;
  }
}
//debugging pendiente [FALTA VALIDACION DE TAMAÑO PARA EMPEQUEÑECER, PARAMETRO DE CAPACIDAD]
size_t delete_row(unsigned char*& board, size_t& rows, const size_t cols, const unsigned char selected_row, size_t& byte_capacity) {
  
  //  El flujo del algoritmo es: 
  //  Se encuentra la distancia entre una fila y la otra, a partir de esa distancia, nos ubicamos en dos posiciones diferentes:
  //  start_bit & start_char corresponden al punto que va a ser sobreescribido
  //  end_bit & end_char corresponden al punto que va a ser leido y pegado en la posicion inicial
  //  
  //  el algoritmo termina en el momento en el que la posicion board[end_char] + end_bit corresponda al indice de la ultima ficha 
  //  del tablero

  const size_t token_limit = (rows*cols) - (selected_row*cols);
  size_t token_counter = 0;
  
  --rows;
  size_t new_used_tokens = rows*cols;

  size_t  start_bit  = selected_row*cols*token_size,
          start_char = start_bit/8,
          end_bit    = (selected_row+1)*cols*token_size, 
          end_char   = end_bit/8;

  unsigned char start_bit_iterator = start_bit%8,
                end_bit_iterator   = end_bit%8,

                end_token = 0;

  while (token_counter < token_limit) {

    //proceso de extraccion del token final

    end_token = (board[end_char] & (and_mask >> end_bit_iterator)) << end_bit_iterator;
    
    end_bit_iterator += 3;
    if (end_bit_iterator >= 8) {
      ++end_char;
      end_bit_iterator -= 8;
      if (end_bit_iterator > 0) {
        end_token ^= (board[end_char] & (and_mask << (token_size-end_bit_iterator))) >> (token_size-end_bit_iterator);
      }
    } 

    //proceso de sobreescritura del token inicial

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
  
  size_t new_byte_capacity = (( (rows*cols*token_size) + 7)/8);

  if (new_used_tokens <= byte_capacity*0.65) { 
    byte_capacity = new_byte_capacity;
    resize_cchain(board, new_byte_capacity, byte_capacity);
  }

  return new_used_tokens;
}

size_t calcularbytesnecesarios(size_t filas, size_t columnas){
    size_t bitstotales = filas * columnas * 3;
    size_t bytesnecesarios = bitstotales / 8;
    if (bitstotales % 8 != 0) {
        bytesnecesarios = bytesnecesarios + 1;   // redondeamos hacia arriba si sobran bits
    }
    return bytesnecesarios;
}


unsigned char ver_ficha(unsigned char* tablero, size_t fila, size_t columna, size_t columnas){

    size_t index = fila * columnas + columna; //el index me dice que ficha es 0,1,2,3,4,5
    size_t firstbit = index * 3; //me indica la posicion lineal del bit en el que inicia la ficha
    size_t Byte = firstbit / 8;  //este me dice el byte en el que esta
    size_t posicionenelbyte = firstbit % 8; //este me dice la posicion en el byte en el que esta la ficha

    if (posicionenelbyte <= 5) //cuando la ficha esta en un solo byte
    {
        size_t desplazamiento = 8 - posicionenelbyte - 3; //8 es la cantidad de bits en un byte, cuando hago (8 - posicionenelbyte) obtengo el bit menos significativo y cuando resto 3 se obtiene el mas, (todo esto de derecha a izquierda)
        return (tablero[Byte] >> desplazamiento) & 7; //aqui solo basta con mover el bit más a la derecha de la ficha al bit menos significativo de el byte
    }

    else //esta se ocupa para cuando una ficha esta de 2 bytes
    {
        size_t desplazamiento = 16 - posicionenelbyte - 3; //16 es el numero de bits en 2 bytes,
        unsigned short entre2bytes = (tablero[Byte] << 8) | (tablero[Byte+1]); //conectamos los 2 bytes para que queden de una forma lineal
        return (entre2bytes >> desplazamiento) & 7; //el >> desplazamiento mueve la ficha hasta el bit menos significativo de los 16 bits ya sabemos que la ficha esta ahi entonces aplicamos un and 7 (00000111 en 2 bytes)
    }
}


void colocarfichaindividual(unsigned char* tablero, size_t fila, size_t columna, size_t columnas, unsigned short valor){

    size_t index = fila * columnas + columna; //el index me dice que ficha es 0,1,2,3,4,5
    size_t firstbit = index * 3; //me indica la posicion lineal del bit en el que inicia la ficha
    size_t Byte = firstbit / 8;  //este me dice el byte en el que esta
    size_t posicionenelbyte = firstbit % 8; //este me dice la posicion en el byte en el que esta la ficha

    if (posicionenelbyte <= 5) //cuando la ficha esta en un solo byte
    {
        size_t desplazamiento = (8 - posicionenelbyte - 3);//8 es la cantidad de bits en un byte, cuando hago (8 - posicionenelbyte) obtengo el bit menos significativo y cuando resto 3 se obtiene el mas, (todo esto de derecha a izquierda)
        unsigned char mascara = 7 << desplazamiento; // coloco la mascara justo en los bits a cambiar
        tablero[Byte] = (tablero[Byte] & ~mascara) | (valor << desplazamiento); //aqui solo basta con mover el bit más a la derecha de la ficha al bit menos significativo de el byte
    }

    else //esta se ocupa para cuando una ficha esta de 2 bytes
    {
        size_t desplazamiento = 16 - posicionenelbyte - 3; //16 es el numero de bits en 2 bytes,
        unsigned short entre2bytes = (tablero[Byte] << 8) | (tablero[Byte+1]); //conectamos los 2 bytes para que queden de una forma lineal
        unsigned short mascara = 7 << desplazamiento; // creo la mascara para apagar todos los bits de la posicion donde ira la ficha
        entre2bytes = (entre2bytes & ~mascara);//(valor << desplazamiento) mueve el valor a cambiar al bit más significativo de la ficha a reemplazar
        valor = valor << desplazamiento; //movemos la ficha a la posicion de bit más significativo de la ficha que vamos a cambiar
        entre2bytes = entre2bytes | valor; //prendemos los valores de la ficha mediante un or
        tablero[Byte] = (entre2bytes >> 8);
        tablero[Byte+1] = entre2bytes;
    }
}

unsigned char ver_ficha(unsigned char* tablero, size_t c_bit_index){

    size_t Byte = c_bit_index / 8;
    size_t posicionenelbyte = c_bit_index % 8;

    if (posicionenelbyte <= 5) // cuando la ficha esta en un solo byte
    {
        size_t desplazamiento = 8 - posicionenelbyte - 3;

        return (tablero[Byte] >> desplazamiento) & 7;
    }

    else // cuando la ficha esta en 2 bytes
    {
        size_t desplazamiento = 16 - posicionenelbyte - 3;

        unsigned short entre2bytes =
            (tablero[Byte] << 8) | tablero[Byte + 1];

        return (entre2bytes >> desplazamiento) & 7;
    }
}

void eliminarcolumna(unsigned char*& tablero, size_t filas, size_t& columnas, size_t columna_eliminar, size_t& bytesreservados){
    size_t newcolumns = columnas - 1;

    for (size_t fila = 0; fila < filas; fila++){
        size_t columnaDestino = 0;
        for (size_t columna = 0; columna < columnas; columna++){
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

    size_t bytesnecesariosnewtable = calcularbytesnecesarios(filas, newcolumns);
    double porcentajeactual = static_cast<double>(bytesnecesariosnewtable) / bytesreservados;

    if (porcentajeactual < 0.65){
        unsigned char* tableronuevo = new unsigned char[bytesnecesariosnewtable];
        create_board(bytesnecesariosnewtable, filas * newcolumns, tableronuevo);

        for (size_t fila = 0; fila < filas; fila++){
            for (size_t columna = 0; columna < newcolumns; columna++){
                unsigned char fichaoldtable = ver_ficha(tablero, fila, columna, columnas);
                colocarfichaindividual(tableronuevo, fila, columna, columnas, fichaoldtable);
            }
        }
        delete[] tablero;
        tablero = tableronuevo;
        bytesreservados = bytesnecesariosnewtable;
    }
}

size_t upper_replace(unsigned char* board, const size_t cols, size_t initial_bit) {
  size_t initial_byte = initial_bit/8,
         upper_bit    = initial_bit - (cols*token_size),
         next_bit     = upper_bit,
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

void cascaded_fall(unsigned char* board, const size_t cols, size_t bit_index) {

  std::uniform_int_distribution<size_t> dist(0, 5);

  size_t first_row_bits = (cols*token_size);
  
  while (bit_index >= first_row_bits) {
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

unsigned char* creartablero(int filas, int columnas){
    int bytes = calcularbytesnecesarios(filas, columnas); //obtener los bytes necesarios para luego reservar memoria
    unsigned char* tablero = new unsigned char[bytes];

    for (int i = 0;i < bytes; i++){
        tablero[i] = 0; //cada byte es igual a 00000000 ya que lo inicializamos en 0 (vacio por asi decirlo)
    }

    return tablero;
}

unsigned char llenarAleatorio(){
    return rand() % 6;
}

void agregarfila(unsigned char*& tablero, size_t &filas, size_t columnas, unsigned short posicionfilaadd, size_t& bytesreservados){
    ++filas;
    bytesreservados = (( (filas*columnas*3) + 7)/8);

    unsigned char* tableronuevo = new unsigned char[bytesreservados];

    for (size_t i = 0;i < bytesreservados; i++){
        tableronuevo[i] = 0; //cada byte es igual a 00000000 ya que lo inicializamos en 0 (vacio por asi decirlo)
    }

    for(size_t fila = 0; fila < filas; fila++){
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
}

void agregarcolumna(unsigned char*& tablero, size_t filas, size_t& columnas, size_t posicion_columnanew, size_t& bytesreservados) {

    size_t newcolumns = columnas + 1;
    unsigned char* tableronuevo = creartablero(filas, newcolumns);

    for (size_t fila = 0; fila < filas; fila++) {
        for (size_t columna = 0; columna < newcolumns; columna++) {
            if (columna < posicion_columnanew) {
                unsigned char ficha = ver_ficha(tablero, fila, columna, columnas);
                colocarfichaindividual(tableronuevo, fila, columna, newcolumns, ficha);
            }
            else if (columna == posicion_columnanew) {
                unsigned char fichaaleatoria = llenarAleatorio();
                colocarfichaindividual(tableronuevo, fila, columna, newcolumns, fichaaleatoria);
            }
            else {
                unsigned char ficha = ver_ficha(tablero, fila, columna - 1, columnas);
                colocarfichaindividual(tableronuevo, fila, columna, newcolumns, ficha);
            }
        }
    }
    delete[] tablero;
    tablero = tableronuevo;
    columnas = newcolumns;
    bytesreservados = calcularbytesnecesarios(filas, newcolumns);
}

//########################## COMBO SCANNER |



bool horizontal_scanner(const unsigned char* board,
                        const size_t cols,
                        const size_t rows,
                        int& h_start,
                        size_t& h_combo_count,
                        int& v_start,
                        size_t& v_combo_count,
                        const unsigned char original_token,
                        bool recursive) {
//-------

  const size_t row_bits = cols*token_size;

  int aux_index = h_start;
  h_start -= 3;

  const int lower_limit = (aux_index/row_bits)*row_bits,
            upper_limit = lower_limit + row_bits;

  bool same_token = true;
 
  char bit_counter = h_start%8;
  int byte_index  = h_start/8;
 
  unsigned char aux_token = 0;

  //parte que va lo mas a la izquierda <- posible
  while (h_start >= lower_limit && same_token) {

    aux_token = (board[byte_index] & (and_mask >> bit_counter)) << bit_counter;

    if (bit_counter + token_size > 8) {
      aux_token ^= ((board[byte_index+1] & (and_mask << (8-bit_counter))) >> (8-bit_counter));
    }
    
    if (aux_token == original_token) {
      ++h_combo_count;
      h_start -= token_size;
      bit_counter -= token_size;
      if (bit_counter < 0) {
        bit_counter += 8;
        --byte_index;
      }
    }
    else same_token = false;
  }
  
  h_start += token_size;

  aux_index += token_size;
  bit_counter = aux_index%8;
  byte_index  = aux_index/8; 
  same_token  = true;

  //parte que va lo mas a la derecha posible
  while (aux_index < upper_limit && same_token) {
    aux_token = (board[byte_index] & (and_mask >> bit_counter )) << bit_counter;
    bit_counter += token_size; 
    aux_index += token_size;
    if (bit_counter >= 8) {
      ++byte_index;
      bit_counter -= 8;
      if (bit_counter > 0) {
        aux_token ^= (board[byte_index] & (and_mask << (token_size-bit_counter))) >> (token_size-bit_counter);
      }
    }
    if (aux_token == original_token) ++h_combo_count;
    else same_token = false;
    aux_token = 0;
  }

  if (h_combo_count < 3) {
    h_combo_count = 0;
    h_start = 0;
    return 0;
  }
  if (recursive) {
    for (unsigned int h_combo_element = 0; h_combo_element < h_combo_count; ++h_combo_element) {
      v_start = h_start + (h_combo_element*token_size);
      v_combo_count = 1;
      if (vertical_scanner(board, cols, rows, h_start, h_combo_count, v_start, v_combo_count, original_token, 0)) {
        break;
      }
    }
  }

  return 1;
}

bool vertical_scanner(const unsigned char* board,
                      const size_t cols,
                      const size_t rows,
                      int& h_start,
                      size_t& h_combo_count,
                      int& v_start,
                      size_t& v_combo_count,
                      const unsigned char original_token,
                      bool recursive) {

  const size_t row_bits = cols*token_size,
               upper_limit = row_bits * rows;

  int aux_index = v_start;
  v_start += row_bits;

  bool same_token = true;
 
  char bit_counter = v_start % 8;
  int byte_index = v_start / 8;

  unsigned char aux_token = 0;

  // parte que va lo mas abajo posible
  while (v_start < upper_limit && same_token) {

    aux_token = (board[byte_index] & (and_mask >> bit_counter)) << bit_counter;

    if (bit_counter + token_size > 8) {
      aux_token ^= ((board[byte_index + 1] &
                     (and_mask << (8 - bit_counter))) >>
                    (8 - bit_counter));
    }

    if (aux_token == original_token) {
      ++v_combo_count;

      v_start += row_bits;
      bit_counter = v_start % 8;
      byte_index = v_start / 8;
    }
    else {
      same_token = false;
    }
    aux_token = 0;
  }

  v_start = aux_index - row_bits;

  bit_counter = v_start % 8;
  byte_index = v_start / 8;
  same_token = true;

  // parte que va lo mas arriba posible
  while (v_start >= 0 && same_token) {

    aux_token = (board[byte_index] & (and_mask >> bit_counter)) << bit_counter;

    if (bit_counter + token_size > 8) {
      aux_token ^= ((board[byte_index + 1] &
                     (and_mask << (8 - bit_counter))) >>
                    (8 - bit_counter));
    }

    if (aux_token == original_token) {
      ++v_combo_count;

      v_start -= row_bits;
      if (v_start >= 0) {
        bit_counter = v_start % 8;
        byte_index = v_start / 8;
      }
    }
    else {
      same_token = false;
    }

    aux_token = 0;
  }

  v_start += row_bits;
  
  if (v_combo_count < 3) {
    v_combo_count = 0;
    v_start = 0;
    return 0;
  }

  if (recursive) {
    for (int v_combo_element = 0; v_combo_element < v_combo_count; ++v_combo_element) {
      h_start = v_start + (cols*token_size*v_combo_element);
      h_combo_count = 1;
      if (horizontal_scanner(board, cols, rows, h_start, h_combo_count, v_start, v_combo_count, original_token, 0)) {
        break;
      }
    }
  }
  return 1;
}

bool combo_scanner(unsigned char* board, const size_t cols, const size_t rows, const size_t c_bit_index) {

int      h_start          = c_bit_index,
         v_start          = c_bit_index;
size_t   v_combo_count    = 1,
         h_combo_count    = 1;

  unsigned char c_token = get_token(board, c_bit_index);

  if (!(vertical_scanner(board, cols, rows, h_start, h_combo_count, v_start, v_combo_count, c_token, 1))) {
    horizontal_scanner(board, cols, rows, h_start, h_combo_count, v_start, v_combo_count, c_token, 1);
  }

  if (h_combo_count == 0 && v_combo_count == 0) return 0;

  const unsigned char delete_token = 0b11000000;

  //fichas horizontales -> 0b110

  size_t token_counter = 0;
  size_t bit_index = h_start;

  while (token_counter < h_combo_count) {

    size_t byte_index = bit_index / 8;
    unsigned char bit_counter = bit_index % 8;

    board[byte_index] &= ~(and_mask >> bit_counter);
    board[byte_index] ^= (delete_token >> bit_counter);

    bit_counter += token_size;

    if (bit_counter >= 8) {
      ++byte_index;
      bit_counter -= 8;

      if (bit_counter > 0) {
        board[byte_index] &= ~(and_mask << (token_size - bit_counter));
        board[byte_index] ^= delete_token << (token_size - bit_counter);
      }
    }

    bit_index += token_size;
    ++token_counter;
  }


  //fichas verticales -> 0b110

  token_counter = 0; 

  bit_index = v_start;

  while (token_counter < v_combo_count) {

    size_t byte_index = bit_index / 8;
    unsigned char bit_counter = bit_index % 8;

    board[byte_index] &= ~(and_mask >> bit_counter);
    board[byte_index] ^= (delete_token >> bit_counter);

    bit_counter += token_size;

    if (bit_counter >= 8) {
      ++byte_index;
      bit_counter -= 8;

      if (bit_counter > 0) {
        board[byte_index] &= ~(and_mask << (token_size - bit_counter));
        board[byte_index] ^= delete_token << (token_size - bit_counter);
      }
    }

    bit_index += (cols * token_size);
    ++token_counter;
  }


  const size_t h_limit = h_start + (h_combo_count * token_size);
  
  const size_t v_limit = (v_combo_count == 0)? v_start : v_start + ((v_combo_count - 1) * cols * token_size) + token_size;

  const size_t scan_start = (v_start < h_start) ? v_start : h_start;

  const size_t scan_limit = (v_limit > h_limit) ? v_limit : h_limit;


  //buscar cada 0b110 y hacer cascaded_fall

  bit_index = scan_start;

  while (bit_index < scan_limit) {

    if (get_token(board, bit_index) == delete_token) {
      cascaded_fall(board, cols, bit_index);
    }

    bit_index += token_size;
  }

  return 1;
}


