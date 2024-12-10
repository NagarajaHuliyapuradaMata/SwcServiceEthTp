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

void Type_SwcServiceEthTp::vRead(
      uint8* pu8Buffer
   ,  uint32 u32LengthBuffer
){
   read(
         this->stChannel.s32Socket
      ,  pu8Buffer
      ,  u32LengthBuffer
   );
}

void Type_SwcServiceEthTp::vWrite(
      const uint8* pcu8Buffer
   ,        uint32 u32LengthBuffer
){
   send(
         this->stChannel.s32Socket
      ,  pcu8Buffer
      ,  u32LengthBuffer
      ,  this->stCfg.u32FlagsSend
   );
}

void Type_SwcServiceEthTp::vMainFunction(void){
}

/******************************************************************************/
/* EOF                                                                        */
/******************************************************************************/
