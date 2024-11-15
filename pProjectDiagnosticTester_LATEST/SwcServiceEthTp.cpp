/******************************************************************************/
/* File   : SwcServiceEthTp.cpp                                               */
/* Author : Nagaraja HULIYAPURADA-MATA                                        */
/* Date   : 01.02.1982                                                        */
/******************************************************************************/

/******************************************************************************/
/* #INCLUDES                                                                  */
/******************************************************************************/
#include "TypesStd.hpp"

#include "infClientSwcServiceEthTp.hpp"

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

/******************************************************************************/
/* #DEFINES                                                                   */
/******************************************************************************/

/******************************************************************************/
/* MACROS                                                                     */
/******************************************************************************/

/******************************************************************************/
/* TYPEDEFS                                                                   */
/******************************************************************************/

/******************************************************************************/
/* CONSTS                                                                     */
/******************************************************************************/

/******************************************************************************/
/* PARAMS                                                                     */
/******************************************************************************/
typedef struct{
   uint32 u32Domain;
   uint32 u32SocketType;
   uint32 u32Protocol;
}Type_CfgMcalEth_stSocket;

/*
typedef struct{
   uint32    u32Level;
   uint32    u32OptionName;
   uint32    u32Option;
   socklen_t tSizeOption;
}Type_CfgMcalEth_stOption;
*/

typedef struct{
   Type_CfgMcalEth_stSocket stSocket;
/*
   Type_CfgMcalEth_stOption stOption;
   socklen_t                tSizeAddress;
   struct sockaddr_in       stAddress;
*/
}Type_CfgMcalEth;

volatile const Type_CfgMcalEth CfgMcalEth = {
      {
            AF_INET
         ,  SOCK_STREAM
         ,  0
      }
/*
   ,  {
            SOL_SOCKET
         ,  (SO_REUSEADDR | SO_REUSEPORT)
         ,  1
         ,  sizeof(uint32)
      }
   ,  sizeof(struct sockaddr_in)
   ,  {
            AF_INET
         ,  htons(8080) // Port number
         ,  {INADDR_ANY}
      }
*/
};

/******************************************************************************/
/* OBJECTS                                                                    */
/******************************************************************************/
Type_SwcServiceEthTp SwcServiceEthTp;

/******************************************************************************/
/* FUNCTIONS                                                                  */
/******************************************************************************/
struct sockaddr_in serv_addr;

void Type_SwcServiceEthTp::vInitFunction(void){
   if(
         0
      >  (
            this->stChannel.u32FileDescriptorSocket = socket(
                  CfgMcalEth.stSocket.u32Domain
               ,  CfgMcalEth.stSocket.u32SocketType
               ,  CfgMcalEth.stSocket.u32Protocol
            )
         )
   ){
      std::cerr << "socket creation failed" << std::endl;
      exit(EXIT_FAILURE);
   }

   serv_addr.sin_family = AF_INET;
   serv_addr.sin_port   = htons(8080);

   // Convert IPv4 and IPv6 addresses from text to binary form
   if(
         0
      >= inet_pton(
               AF_INET
            ,  "127.0.0.1"
            ,  &serv_addr.sin_addr
         )
   ){
      std::cerr << "Invalid address / Address not supported" << std::endl;
      exit(EXIT_FAILURE);
   }

   // Connect to the server
   if(
         0
      >  connect(
               this->stChannel.u32FileDescriptorSocket
            ,  (struct sockaddr*) &serv_addr
            ,  sizeof(serv_addr)
         )
   ){
      std::cerr << "Connection failed" << std::endl;
      exit(EXIT_FAILURE);
   }
}

void Type_SwcServiceEthTp::vDeInitFunction(void){
   close(this->stChannel.u32FileDescriptorSocket);
}

void Type_SwcServiceEthTp::vRead(
      sint8* ps8Buffer
   ,  uint32 u32LengthBuffer
){
   read(
         this->stChannel.u32FileDescriptorSocket
      ,  ps8Buffer
      ,  u32LengthBuffer
   );
}

void Type_SwcServiceEthTp::vWrite(
      const sint8* pcs8Buffer
   ,        uint32 u32LengthBuffer
){
   send(
         this->stChannel.u32FileDescriptorSocket
      ,  pcs8Buffer
      ,  u32LengthBuffer
      ,  0
   );
}

void Type_SwcServiceEthTp::vMainFunction(void){
}

/******************************************************************************/
/* EOF                                                                        */
/******************************************************************************/
