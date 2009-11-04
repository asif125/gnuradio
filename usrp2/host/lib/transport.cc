/* -*- c++ -*- */
/*
 * Copyright 2009 Free Software Foundation, Inc.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "transport.h"
#include <stdexcept>

static void nop_cb(void *buff, size_t len){
    //NOP
}

usrp2::transport::transport(const std::string &type_str){
    d_cb = nop_cb;
    d_type_str = type_str;
    d_running = false;
}

usrp2::transport::~transport(){
    if (d_running) stop();
}

void usrp2::transport::init(){
    //NOP
}

void usrp2::transport::start(){
    if (d_running){
        throw std::runtime_error("usrp2::transport for" + d_type_str + " already started\n");
    }
    d_thread = new boost::thread(boost::bind(&usrp2::transport::run, this));
}

void usrp2::transport::stop(){
    if (not d_running){
        throw std::runtime_error("usrp2::transport for" + d_type_str + " already stopped\n");
    }
    d_running = false;
    d_thread->join();
}

void usrp2::transport::run(){
    init();
    d_running = true;
    void *buff;
    while (d_running){
        try{
            // call recv to get a pointer into memory
            // pass the buffer into the callback
            int len = recv(&buff);
            if (len > 0) d_cb(buff, len);
        //catch thread interrupts from join, sleep, etc
        //the running condition will be re-checked
        }catch(boost::thread_interrupted const &){}
    }
}

int usrp2::transport::send(const void *buff, int len){
    return -1; //NOP
}

int usrp2::transport::recv(void **buff){
    *buff = NULL;
    return -1; //NOP
}