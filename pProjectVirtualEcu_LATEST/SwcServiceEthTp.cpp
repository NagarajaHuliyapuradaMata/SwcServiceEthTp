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

typedef struct{
   uint32    u32Level;
   uint32    u32OptionName;
   uint32    u32Option;
   socklen_t tSizeOption;
}Type_CfgMcalEth_stOption;

typedef struct{
   Type_CfgMcalEth_stSocket stSocket;
   Type_CfgMcalEth_stOption stOption;
   socklen_t                tSizeAddress;
   struct sockaddr_in       stAddress;
}Type_CfgMcalEth;

volatile const Type_CfgMcalEth CfgMcalEth = {
      {
            AF_INET
         ,  SOCK_STREAM
         ,  0
      }
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
};

/******************************************************************************/
/* OBJECTS                                                                    */
/******************************************************************************/
Type_SwcServiceEthTp SwcServiceEthTp;

/******************************************************************************/
/* FUNCTIONS                                                                  */
/******************************************************************************/
void Type_SwcServiceEthTp::vInitFunction(void){
   if(
         0
      == (
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

   if(
      setsockopt(
            this->stChannel.u32FileDescriptorSocket
         ,                 CfgMcalEth.stOption.u32Level
         ,                 CfgMcalEth.stOption.u32OptionName
         ,  (const void*) &CfgMcalEth.stOption.u32Option
         ,                 CfgMcalEth.stOption.tSizeOption
      )
   ){
      std::cerr << "setsockopt" << std::endl;
      exit(EXIT_FAILURE);
   }

   if(
         0
      >  bind(
               this->stChannel.u32FileDescriptorSocket
            ,  (struct sockaddr*) &CfgMcalEth.stAddress
            ,  CfgMcalEth.tSizeAddress
         )
   ){
      std::cerr << "bind failed" << std::endl;
      exit(EXIT_FAILURE);
   }
    
   // Start listening for incoming connections
   if(
         0
      >  listen(
               this->stChannel.u32FileDescriptorSocket
            ,  3
         )
   ){
      std::cerr << "listen" << std::endl;
      exit(EXIT_FAILURE);
   }

   // Accept an incoming connection
   if(
         0
      >  (
            this->stChannel.u32Socket = accept(
                  this->stChannel.u32FileDescriptorSocket
               ,  (struct sockaddr*) &CfgMcalEth.stAddress
               ,  (socklen_t*)       &CfgMcalEth.tSizeAddress
            )
         )
   ){
      std::cerr << "accept" << std::endl;
      exit(EXIT_FAILURE);
   }
}

void Type_SwcServiceEthTp::vDeInitFunction(void){
   close(this->stChannel.u32Socket);
   close(this->stChannel.u32FileDescriptorSocket);
}

void Type_SwcServiceEthTp::vRead(
      sint8* ps8Buffer
   ,  uint32 u32LengthBuffer
){
   read(
         this->stChannel.u32Socket
      ,  ps8Buffer
      ,  u32LengthBuffer
   );
}

void Type_SwcServiceEthTp::vWrite(
      const sint8* pcs8Buffer
   ,        uint32 u32LengthBuffer
){
   send(
         this->stChannel.u32Socket
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
