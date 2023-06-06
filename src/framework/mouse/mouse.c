#include "mouse.h"

uint8_t counter_byte = 0;


// Coordenadas Iniciais do Mouse
int x_mouse = 400;
int y_mouse = 300;

// Coordenadas que muda no x
int x_mouse_delta = 0;


bool mouse_read = false;
struct packet mouse_packet;

int (mouse_interrupt_handler)() {
  if (read_status_byte() != 0) return 1; // DEU ERRO
  if (read_packet() != 0) return 1; // DEU ERRO
  if (update_mouse() != 0) return 1; // DEU ERRO
  return 0;
}

int (read_status_byte)() {
  uint8_t status = 0;
  ut_sys_inb(STATUS_REGISTER, &status);

  if ((status & BIT(5)) == 0) // NÃO É RATO, É TECLADO
    return 1;

  if ((status & BIT(6)) != 0) // TIMEOUT
    return 1;

  if ((status & BIT(7)) != 0) // ERRO DE PARIDADE
    return 1;

  if ((status & BIT(0)) == 0) // BUFFER DE SAÍDA VAZIO
    return 1;

  return 0; // NÃO DEU ERRO 
}

bool (left_click)() {
  return mouse_packet.lb;
}

int (read_packet)() {
  uint8_t data;

  if (ut_sys_inb(OUTPUT_BUFFER, &data) != 0)
    return 1;

  mouse_read = false;

  if (counter_byte % 3 == 0){
    if ((data & BIT(3)) == 0)
      return 1;

    // LER O BIT DE CONTROLO DO PACOTE
    mouse_packet.lb = data & MOUSE_LB;
    mouse_packet.rb = data & MOUSE_RB;
    mouse_packet.mb = data & MOUSE_MB;
    mouse_packet.delta_x = data & MOUSE_X_SIGN;
    mouse_packet.delta_y = data & MOUSE_Y_SIGN;
    mouse_packet.x_ov = data & MOUSE_X_OV;
    mouse_packet.y_ov = data & MOUSE_Y_OV;
  }
  else if (counter_byte % 3 == 1){
    mouse_packet.delta_x = (mouse_packet.delta_x) ? (0xFF00 | data) : data;
  }
  else if (counter_byte % 3 == 2){
    mouse_packet.delta_y = (mouse_packet.delta_y) ? (0xFF00 | data) : data;
    mouse_read = true;
  }
  
  counter_byte = (counter_byte + 1) % 3;
  return 0;
}

int hook_id_mouse;

int (subscribe_mouse_int)(uint8_t* bit_no) {
  hook_id_mouse = *bit_no;
  return sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_mouse);
}

int (unsubscribe_mouse_int)() {
  return sys_irqrmpolicy(&hook_id_mouse);
}

int (update_mouse)(){
  if (mouse_read == true){
    // dar update às coordenadas do rato
    x_mouse += mouse_packet.delta_x / 2;
    y_mouse -= mouse_packet.delta_y / 2;
    x_mouse_delta = mouse_packet.delta_x;

    // verificar se o rato está dentro dos limites do ecrã
    if (x_mouse < 0)
      x_mouse = 0;
    else if (x_mouse > 800 - 32)
      x_mouse = 800 - 32;

    if (y_mouse < 0)
      y_mouse = 0;
    else if (y_mouse > 600 - 32)
      y_mouse = 600 - 32;
    
    mouse_read = false;
    return 0;
  }
  return 1;
}
