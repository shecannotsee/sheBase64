//
// Created by shecannotsee on 2022/12/5.
//

#include "sheBase64.h"

unsigned char sheBase64::pos_of_char(const unsigned char chr) {
  if      (chr >= 'A' && chr <= 'Z') return chr - 'A';
  else if (chr >= 'a' && chr <= 'z') return chr - 'a' + ('Z' - 'A')               + 1;
  else if (chr >= '0' && chr <= '9') return chr - '0' + ('Z' - 'A') + ('z' - 'a') + 2;
  else if (chr == '+' || chr == '-') return 62;
  else if (chr == '/' || chr == '_') return 63;
  else
    throw std::runtime_error("Input is not valid base64-encoded data.");
};

std::string sheBase64::encode(std::string data) {
  int _size = data.size()%3;
  unsigned char zero = 0x00;
  if ( _size == 1 ) {
    data += zero;
    data += zero;
  } else if ( _size == 2 ) {
    data += zero;
  }

  std::string ret;
  for (int i=0; i<data.size(); i=i+3 ) {
    unsigned char a;
    a = data[i]>>2;
    unsigned char b;
    b = data[i]<<6;
    b = b>>2;
    b += data[i+1]>>4;
    unsigned char c;
    c = data[i+1]<<4;
    c = c>>2;
    c += data[i+2]>>6;
    unsigned char d;
    d = data[i+2]<<2;
    d = d>>2;
    ret += base64_table[0][a];
    ret += base64_table[0][b];
    ret += base64_table[0][c];
    ret += base64_table[0][d];
  }

  if ( _size == 1 ) {
    ret[ret.size()-1]='=';
    ret[ret.size()-2]='=';
  } else if ( _size == 2 ) {
    ret[ret.size()-1]='=';
  }

  return ret;
};

std::string sheBase64::decode(std::string data_base64) {
  int wait = 0;
  for (const char& e : data_base64) {
    if (e == '=') wait++;
  }
  int _size = (data_base64.size()/4 )* 3 - wait ;
  std::string ret;
  unsigned char temp = 0x00;
  int index = 0;
  for (int i=0; i<data_base64.size(); i++) {
    if (ret.size()==_size) break;
    if ( index == 0) {
      temp += pos_of_char(data_base64[i])<<2;
      index = 6;
    } else if ( index == 6 ) {
      temp += pos_of_char(data_base64[i])>>4;
      ret += temp;
      temp = 0x00;
      temp = pos_of_char(data_base64[i])<<4;
      index = 4;
    } else if (index==4) {
      temp += pos_of_char(data_base64[i])>>2;
      ret += temp;
      temp = 0x00;
      temp = pos_of_char(data_base64[i])<<6;
      index = 2;
    } else if ( index == 2) {
      temp += pos_of_char(data_base64[i]);
      ret += temp;
      temp = 0x00;
      index = 0;
    }
  };

  return ret;
};

