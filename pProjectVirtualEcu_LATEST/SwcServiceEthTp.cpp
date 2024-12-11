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
#include "interface_Persistency.hpp"

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
typedef struct{
   uint32                   u32Domain;
   uint32                   u32SocketType;
   uint32                   u32Protocol;
}Type_CfgMcalEth_stSocket;

typedef struct{
   uint32                   u32Level;
   uint32                   u32OptionName;
   uint32                   u32Option;
   socklen_t                tSizeOption;
}Type_CfgMcalEth_stOption;

typedef struct{
   Type_CfgMcalEth_stSocket stSocket;
   Type_CfgMcalEth_stOption stOption;
   socklen_t                tSizeAddress;
   struct sockaddr_in       stAddress;
   uint32                   u32BacklogListen;
   uint32                   u32FlagsSend;
}Type_CfgMcalEth;

typedef struct{
   sint32 s32FileDescriptorSocket;
   sint32 s32Socket;
}Type_McalEth_stChannel;

class Type_SwcServiceEthTp:
      public infClientSwcServiceEthTp
{
   private:
      Type_McalEth_stChannel stChannel;
      Type_CfgMcalEth        stCfg;

   public:
      void vInitFunction   (void);
      void vRead           (      uint8* pu8Buffer,  uint32 u32LengthBuffer);
      void vWrite          (const uint8* pcu8Buffer, uint32 u32LengthBuffer);
      void vMainFunction   (void);
      void vDeInitFunction (void);
};

/******************************************************************************/
/* CONSTS                                                                     */
/******************************************************************************/

/******************************************************************************/
/* PARAMS                                                                     */
/******************************************************************************/

/******************************************************************************/
/* OBJECTS                                                                    */
/******************************************************************************/
Type_SwcServiceEthTp            SwcServiceEthTp;
infClientSwcServiceEthTp* const cpstinfClientSwcServiceEthTp = &SwcServiceEthTp;

/******************************************************************************/
/* FUNCTIONS                                                                  */
/******************************************************************************/
void Type_SwcServiceEthTp::vInitFunction(void){
   pPersistency->vGetCfg(
         (uint64*)&this->stCfg
      ,  "/home/nagarajahuliyapuradamata/Workspace_Ubuntu/repo_projects/pProjectVirtualEcu/main_LATEST/source/SwcServiceEthTp/CfgMcalEth.hex"
   );

   if(
         0
      == (
            this->stChannel.s32FileDescriptorSocket = socket(
                  this->stCfg.stSocket.u32Domain
               ,  this->stCfg.stSocket.u32SocketType
               ,  this->stCfg.stSocket.u32Protocol
            )
         )
   ){
      std::cerr << std::endl << "ERROR: socket creation failed" << std::endl;
      exit(EXIT_FAILURE);
   }

   if(
      setsockopt(
                           this->stChannel.s32FileDescriptorSocket
         ,                 this->stCfg.stOption.u32Level
         ,                 this->stCfg.stOption.u32OptionName
         ,  (const void*) &this->stCfg.stOption.u32Option
         ,                 this->stCfg.stOption.tSizeOption
      )
   ){
      std::cerr << std::endl << "ERROR: setsockopt" << std::endl;
      exit(EXIT_FAILURE);
   }

   if(
         0
      >  bind(
                                   this->stChannel.s32FileDescriptorSocket
            ,  (struct sockaddr*) &this->stCfg.stAddress
            ,                      this->stCfg.tSizeAddress
         )
   ){
      std::cerr << std::endl << "ERROR: bind failed" << std::endl;
      exit(EXIT_FAILURE);
   }

   if(
         0
      >  listen(
               this->stChannel.s32FileDescriptorSocket
            ,  this->stCfg.u32BacklogListen
         )
   ){
      std::cerr << std::endl << "ERROR: listen" << std::endl;
      exit(EXIT_FAILURE);
   }

   if(
         0
      >  (
            this->stChannel.s32Socket = accept(
                                      this->stChannel.s32FileDescriptorSocket
               ,  (struct sockaddr*) &this->stCfg.stAddress
               ,  (socklen_t*)       &this->stCfg.tSizeAddress
            )
         )
   ){
      std::cerr << std::endl << "ERROR: accept" << std::endl;
      exit(EXIT_FAILURE);
   }
}

void Type_SwcServiceEthTp::vDeInitFunction(void){
   close(this->stChannel.s32Socket);
   close(this->stChannel.s32FileDescriptorSocket);
}

uint8 u8Ascii2Hex(const uint8 lcu8Ascii){
   return(
      (('0' <= lcu8Ascii) && ('9' >= lcu8Ascii))
      ?  lcu8Ascii - '0'
      :  (
            (('A' <= lcu8Ascii) && ('F' >= lcu8Ascii))
            ?  lcu8Ascii - 0x37
            :  (
                  (('a' <= lcu8Ascii) && ('f' >= lcu8Ascii))
                  ?  lcu8Ascii - 0x57
                  :  0
               )
         )
   );
}

uint8 u8Hex2Ascii(const uint8 lcu8Hex){
   return(
      ((0 <= lcu8Hex) && (9 >= lcu8Hex))
      ?  lcu8Hex + '0'
      :  (
            ((0x0A <= lcu8Hex) && (0x0F >= lcu8Hex))
            ?  lcu8Hex - 0x0A + 'A'
            :  0
         )
   );
}

void vBcd2Hex(
            uint8* lpu8Hex
   ,  const uint8* lpcu8Bcd
){
   *lpu8Hex = u8Ascii2Hex(lpcu8Bcd[0])*0x10 + u8Ascii2Hex(lpcu8Bcd[1]);
}

void vHex2Bcd(
            uint8* lpu8Bcd
   ,  const uint8* lpcu8Hex
){
   lpu8Bcd[0] = u8Hex2Ascii((*lpcu8Hex & 0xF0)>>4);
   lpu8Bcd[1] = u8Hex2Ascii( *lpcu8Hex & 0x0F);
}

void Type_SwcServiceEthTp::vRead(
      uint8* lpu8BufferHex
   ,  uint32 lu32LengthBufferHex
){
   uint8 lau8BufferBcd[2048];
   read(
         this->stChannel.s32Socket
      ,  &lau8BufferBcd[0]
      ,  2048
   );
   std::cout << "client\t: " << lau8BufferBcd << std::endl;

   for(
      uint8 lu8IndexBufferHex =  0;
            lu8IndexBufferHex <= lpu8BufferHex[0];
            lu8IndexBufferHex ++
   ){
      vBcd2Hex(
            &lpu8BufferHex[  lu8IndexBufferHex]
         ,  &lau8BufferBcd[2*lu8IndexBufferHex]
      );
   }
}

uint8 gau8BufferBcd[2048];
void Type_SwcServiceEthTp::vWrite(
      const uint8* lpcu8BufferHex
   ,        uint32 lu32LengthBufferHex
){
   uint8 lu8IndexBufferHex;
   for(
      lu8IndexBufferHex =  0;
      lu8IndexBufferHex <= lpcu8BufferHex[0];
      lu8IndexBufferHex ++
   ){
      vHex2Bcd(
            &gau8BufferBcd [2*lu8IndexBufferHex]
         ,  &lpcu8BufferHex[  lu8IndexBufferHex]
      );
   }
   gau8BufferBcd[2*lu8IndexBufferHex] = '\0';

   std::cout << "server\t: " << gau8BufferBcd << std::endl << std::endl;
   send(
         this->stChannel.s32Socket
      ,  &gau8BufferBcd[0]
      ,  2048
      ,  this->stCfg.u32FlagsSend
   );
}

void Type_SwcServiceEthTp::vMainFunction(void){
}

/******************************************************************************/
/* EOF                                                                        */
/******************************************************************************/
